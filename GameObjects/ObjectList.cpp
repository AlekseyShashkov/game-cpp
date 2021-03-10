#include "ObjectList.h"

ObjectList::ObjectList(const PlaygroundPtr &_PlaygroundPtr, 
                       const PanelTopPtr &_PanelTopPtr, 
                       const TimePoint _TimePoint) noexcept
    : m_PlaygroundPtr{ _PlaygroundPtr }, m_GameStart{ _TimePoint }, 
    m_PanelTopPtr{ _PanelTopPtr }
{

}

ObjectList::~ObjectList()
{
    m_ListGameObjects.clear();
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void AddObject(GameObjectPtr)
//
// RETURN VALUE: no.
//
// PURPOSE: adding game objects to list.
//
// COMMENTS: also initialize separate components.
//
void ObjectList::AddObject(GameObjectPtr _GameObjectPtr) noexcept
{
    m_ListGameObjects.emplace_back(std::move(_GameObjectPtr));
    m_ListGameObjects.back()->InitializeElementGUI();
    m_ListGameObjects.back()->TakeSizeLocationArea();
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void UpdateGames()
//
// RETURN VALUE: no.
//
// PURPOSE: interaction of game objects with each other.
//
// COMMENTS: implementation of game logic.
//
void ObjectList::UpdateGame() noexcept
{
    HRGN lv_RangeRegion = NULL;
    HRGN lv_MeleeRegion = NULL;

    auto lv_ThisMoment = std::chrono::steady_clock::now();
    float lv_TimeIntervalFromStart = std::chrono::duration<float, std::milli> 
        (lv_ThisMoment - m_GameStart).count();

    GameObject::ObjectAction lv_PlayerAction;

    m_PlaygroundPtr->CleanScreen();

    if (!m_ListGameObjects.empty()) {
        for (auto &i : m_ListGameObjects) {
            bool lv_IsVisibleGroup = CheckIsVisibleGroup(i);

            if (lv_IsVisibleGroup) {
                CheckIsVisibleElements(i);

                switch (i->GetType()) {
                case GameObject::ObjectType::ENEMY :
                    CheckRespawn(i, lv_TimeIntervalFromStart);
                    i->Action();
                    break;
                
                case GameObject::ObjectType::RESOURCE :
                    CheckRespawn(i, lv_TimeIntervalFromStart);
                    break;

                case GameObject::ObjectType::PLAYER :
                    //CheckRespawn(i, lv_TimeInterval);
                    lv_PlayerAction = i->Action();

                    InitializeRegion(i->GetElementGUI(), 160, lv_RangeRegion);
                    InitializeRegion(i->GetElementGUI(), 16, lv_MeleeRegion);
                    break;

                case GameObject::ObjectType::BARRIER :
                    break;
                }
            }
        }

        for (auto &i : m_ListGameObjects) {
            bool lv_IsVisibleGroup = CheckIsVisibleGroup(i);

            if (lv_IsVisibleGroup) {
                switch (i->GetType()) {
                case GameObject::ObjectType::ENEMY :
                    CheckCollision(i);

                    i->SetNewCoords();
                    i->TakeSizeLocationArea();

                    CheckRegion(i, lv_RangeRegion);
                    break;

                case GameObject::ObjectType::RESOURCE :
                    CheckRegion(i, lv_MeleeRegion);

                    if (lv_PlayerAction 
                        == GameObject::ObjectAction::PRESSED_E) {
                        CollectElement(i, lv_TimeIntervalFromStart);
                    }
                    break;

                case GameObject::ObjectType::PLAYER :
                    CheckCollision(i);

                    i->SetNewCoords();
                    i->TakeSizeLocationArea();

                    m_PlaygroundPtr->Camera(
                        i->GetNewCoordsX()[0], i->GetNewCoordsY()[0]);
                    break;

                case GameObject::ObjectType::BARRIER :
                    break;
                }
            }

            if (lv_PlayerAction == GameObject::ObjectAction::PRESSED_C) {
                if (m_PlaygroundPtr->GetColliderIsVisible()) {
                    m_PlaygroundPtr->SetColliderIsVisible(false);
                } else {
                    m_PlaygroundPtr->SetColliderIsVisible(true);
                }
            }

            m_PlaygroundPtr->DrawElementGUI(i->GetElementGUI(),
                                            i->GetCollider(),
                                            i->GetIsVisibles(),
                                            i->GetIsAlives());
        }
    }

    ::DeleteObject(lv_RangeRegion);
    ::DeleteObject(lv_MeleeRegion);
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: bool CheckIsVisibleGroup(const GameObjectPtr &)
//
// RETURN VALUE: true    - group in sight.
//               false   - group out sight.
//
// PURPOSE: checking visibility group of elements.
//
// COMMENTS: each object is a group located in a previously known territory,
// whose visibility we check (getting into camera area).
//
bool ObjectList::CheckIsVisibleGroup(
                                const GameObjectPtr &_GameObjectPtr) noexcept
{
    RECT lv_BufferRect    = { 0 };
    RECT lv_CompareRect = { 0 };

    ::CopyRect(&lv_CompareRect, &_GameObjectPtr->GetLocationArea());

    BOOL lv_InVisibleArea = ::IntersectRect(&lv_BufferRect,
        &m_PlaygroundPtr->GetCamera(), &lv_CompareRect);

    if (lv_InVisibleArea) {
        return true;
    } else {
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void CheckIsVisibleElements(const GameObjectPtr &)
//
// RETURN VALUE: no.
//
// PURPOSE: checking visibility of separate elements.
//
// COMMENTS: check happen only if group is visible.
//
void ObjectList::CheckIsVisibleElements(
                                const GameObjectPtr &_GameObjectPtr) noexcept
{
    RECT lv_BufferRect = { 0 };
    RECT lv_CompareRect = { 0 };

    size_t lv_AmountElements = _GameObjectPtr->GetElementGUI().names.size();

    for (size_t i = 0; i < lv_AmountElements; ++i) {
        bool lv_IsAlive = _GameObjectPtr->GetIsAlives()[i];

        if (lv_IsAlive) {
            ::CopyRect(&lv_CompareRect,
                &_GameObjectPtr->GetCollider());
            ::OffsetRect(&lv_CompareRect,
                _GameObjectPtr->GetNewCoordsX()[i],
                _GameObjectPtr->GetNewCoordsY()[i]);

            BOOL lv_IsVisible = ::IntersectRect(&lv_BufferRect,
                &m_PlaygroundPtr->GetCamera(), &lv_CompareRect);

            if (lv_IsVisible) {
                _GameObjectPtr->SetIsVisible(true, i);
            } else {
                _GameObjectPtr->SetIsVisible(false, i);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void CheckCollision(const GameObjectPtr &)
//
// RETURN VALUE: нет.
//
// PURPOSE: checking collision elements.
//
// COMMENTS: active objects (ENEMY, PLAYER) are checked against all other 
// objects (also with other active objects, but inside the object (group), 
// there are no check between elements.
//
void ObjectList::CheckCollision(
                        const GameObjectPtr &_CurrentGameObjectPtr) noexcept
{
    RECT lv_BufferRect  = { 0 };
    RECT lv_CurrentRect = { 0 };
    RECT lv_CompareRect = { 0 };

    size_t lv_AmountCurrentElements = 
        _CurrentGameObjectPtr->GetElementGUI().names.size();

    for (size_t i = 0; i < lv_AmountCurrentElements; ++i) {
        bool lv_IsVisible = _CurrentGameObjectPtr->GetIsVisibles()[i];
        bool lv_IsAlive   = _CurrentGameObjectPtr->GetIsAlives()[i];

        if (lv_IsVisible && lv_IsAlive) {
            ::CopyRect(&lv_CurrentRect,
                &_CurrentGameObjectPtr->GetCollider());
            ::OffsetRect(&lv_CurrentRect,
                _CurrentGameObjectPtr->GetNewCoordsX()[i],
                _CurrentGameObjectPtr->GetNewCoordsY()[i]);

            for (auto &lv_CompareGameObjectPtr : m_ListGameObjects) {
                bool lv_IsNotEqual     = _CurrentGameObjectPtr
                    != lv_CompareGameObjectPtr;
                bool lv_IsVisibleGroup = CheckIsVisibleGroup(
                    lv_CompareGameObjectPtr);

                if (lv_IsVisibleGroup && lv_IsNotEqual) {
                    size_t lv_AmountCompareElements =
                        lv_CompareGameObjectPtr->GetElementGUI().names.size();

                    for (size_t j = 0; j < lv_AmountCompareElements; ++j) {
                        bool lv_IsVisible = 
                            lv_CompareGameObjectPtr->GetIsVisibles()[j];
                        bool lv_IsAlive   = 
                            lv_CompareGameObjectPtr->GetIsAlives()[j];

                        if (lv_IsVisible && lv_IsAlive) {
                            ::CopyRect(&lv_CompareRect,
                                &lv_CompareGameObjectPtr->GetCollider());
                            ::OffsetRect(&lv_CompareRect,
                                lv_CompareGameObjectPtr->GetNewCoordsX()[j],
                                lv_CompareGameObjectPtr->GetNewCoordsY()[j]);

                            BOOL lv_IsCollision = ::IntersectRect(
                                                            &lv_BufferRect,
                                                            &lv_CurrentRect,
                                                            &lv_CompareRect);

                            if (lv_IsCollision) {
                                _CurrentGameObjectPtr->SetCoord(i);
                            }
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void InitializeRegion(const ElementGUI &, const size_t, HRGN &)
//
// PARAMETERS: [in]  _ElementGUI - structure with objects graphic information;
//             [in]  _Radius     - radius of region; 
//             [out] _Region     - region.                 
//
// RETURN VALUE: no.
//
// PURPOSE: creating region.
//
// COMMENTS: region - area around PLAYER, elements that getting into 
// become active.
//
void ObjectList::InitializeRegion(const ElementGUI &_ElementGUI, 
                                  const size_t _Radius,
                                  HRGN &_Region) noexcept
{
    _Region = ::CreateEllipticRgn(
        _ElementGUI.coordsX[0] - _Radius,
        _ElementGUI.coordsY[0] - _Radius,
        _ElementGUI.coordsX[0] + _ElementGUI.gridWidth + _Radius,
        _ElementGUI.coordsY[0] + _ElementGUI.gridHeight + _Radius);
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void CheckRegion(const GameObjectPtr &, const HRGN &)            
//
// RETURN VALUE: no.
//
// PURPOSE: checking element getting into region.
//
// COMMENTS: region - area around PLAYER, elements that getting into 
// become active.
//
void ObjectList::CheckRegion(const GameObjectPtr &_GameObjectPtr,
                             const HRGN &_Region) noexcept
{
    RECT lv_CompareRect = { 0 };

    size_t lv_AmountElements =
        _GameObjectPtr->GetElementGUI().names.size();

    for (size_t i = 0; i < lv_AmountElements; ++i) {
        bool lv_IsVisible = _GameObjectPtr->GetIsVisibles()[i];
        bool lv_IsAlive   = _GameObjectPtr->GetIsAlives()[i];

        if (lv_IsVisible && lv_IsAlive) {
            ::CopyRect(&lv_CompareRect, 
                &_GameObjectPtr->GetCollider());
            ::OffsetRect(&lv_CompareRect,
                _GameObjectPtr->GetElementGUI().coordsX[i],
                _GameObjectPtr->GetElementGUI().coordsY[i]);

            BOOL lv_InRegion = ::RectInRegion(_Region, &lv_CompareRect);

            if (lv_InRegion) {
                _GameObjectPtr->SetIsActive(true, i);
            }
            if (!lv_InRegion) {
                _GameObjectPtr->SetIsActive(false, i);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void CollectElement(const GameObjectPtr &, const float)            
//
// RETURN VALUE: no.
//
// PURPOSE: collecting element.
//
// COMMENTS: if all conditions are met - the item is deleted to respawn, 
// counter of collected items on the top panel increases.
//
void ObjectList::CollectElement(const GameObjectPtr &_GameObjectPtr,
                                const float _TimeIntervalFromStart) noexcept
{
    size_t lv_AmountElements =
        _GameObjectPtr->GetElementGUI().names.size();

    for (size_t i = 0; i < lv_AmountElements; ++i) {
        bool lv_IsActive = _GameObjectPtr->GetIsActives()[i];

        if (lv_IsActive) {
            float lv_RespawnMoment = _GameObjectPtr->GetRespawnTime(
                _GameObjectPtr->GetElementGUI().names[i]);
            float lv_RespawnTime   = _TimeIntervalFromStart + lv_RespawnMoment;

            _GameObjectPtr->SetRespawnTime(lv_RespawnTime, i);
            _GameObjectPtr->SetIsActive(false, i);
            _GameObjectPtr->SetIsAlive(false, i);

            m_PanelTopPtr->SetCountGems(
                _GameObjectPtr->GetElementGUI().names[i]);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: void CheckRespawn(const GameObjectPtr &, const float)            
//
// RETURN VALUE: no.
//
// PURPOSE: checking a groups element for possibility of respawn.
//
// COMMENTS: no.
//
void ObjectList::CheckRespawn(const GameObjectPtr &_GameObjectPtr, 
                              const float _TimeIntervalFromStart) noexcept
{
    size_t lv_AmountElements = _GameObjectPtr->GetElementGUI().names.size();

    for (size_t i = 0; i < lv_AmountElements; ++i) {
        bool lv_IsVisible = _GameObjectPtr->GetIsVisibles()[i];
        bool lv_IsAlive   = _GameObjectPtr->GetIsAlives()[i];
        bool lv_IsRespawn = _TimeIntervalFromStart 
            > _GameObjectPtr->GetRespawnTimes()[i];

        if (lv_IsVisible && !lv_IsAlive && lv_IsRespawn) {
            _GameObjectPtr->SetIsAlive(true, i);
            _GameObjectPtr->SetRespawnTime(0.0f, i);
        }
    }
}
