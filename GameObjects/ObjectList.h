#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "GameObject.h"
#include "GUI\PanelTop.h"
#include <windows.h>
#include <list>
#include <memory>
#include <chrono>

using GameObjectPtr   = std::unique_ptr<GameObject>;
using ListGameObjects = std::list<GameObjectPtr>;
using Strings         = std::vector<std::string>;
using PanelTopPtr     = std::shared_ptr<PanelTop>;
using TimePoint       = std::chrono::time_point<std::chrono::steady_clock>;

class ObjectList
{
 public:
    ObjectList(const PlaygroundPtr &_PlaygroundPtr,
               const PanelTopPtr &_PanelTopPtr,
               const TimePoint _TimePoint) noexcept;

    ~ObjectList() noexcept;

    void AddObject(GameObjectPtr _GameObjectPtr) noexcept;
    void UpdateGame() noexcept;    

 private:
    bool CheckIsVisibleGroup (const GameObjectPtr &_GameObjectPtr) noexcept;
    void CheckIsVisibleElements(const GameObjectPtr &_GameObjectPtr) noexcept;
    void CheckCollision(const GameObjectPtr &_CurrentGameObjectPtr) noexcept;
    void CheckRegion(const GameObjectPtr &_GameObjectPtr, 
                     const HRGN &_Region) noexcept;
    void CheckRespawn(const GameObjectPtr & _GameObjectPtr, 
                      const float _TimeIntervalFromStart) noexcept;
    void InitializeRegion(const ElementGUI &_ElementGUI, const size_t _Radius, 
                          HRGN &_Region) noexcept;
    void CollectElement(const GameObjectPtr &_GameObjectPtr, 
                        const float _TimeIntervalFromStart) noexcept;

    ListGameObjects m_ListGameObjects;
    PlaygroundPtr m_PlaygroundPtr;
    PanelTopPtr m_PanelTopPtr;
    TimePoint m_GameStart;
};

#endif  // OBJECTLIST_H
