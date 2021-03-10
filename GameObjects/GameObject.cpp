#include "GameObject.h"
#include <algorithm>

void GameObject::InitializeElementGUI() noexcept
{
    m_ElementGUI.memoryTileSetMask = m_PlaygroundPtr->CreateBitmapMask(
        (HBITMAP)m_ElementGUI.memoryTileSet, RGB(0, 0, 0));

    m_ElementGUI.handleTileSet     = ::CreateCompatibleDC(nullptr);
    m_ElementGUI.handleTileSetMask = ::CreateCompatibleDC(nullptr);

    ::SelectObject(m_ElementGUI.handleTileSet, 
                m_ElementGUI.memoryTileSet);
    ::SelectObject(m_ElementGUI.handleTileSetMask, 
                m_ElementGUI.memoryTileSetMask);

    m_NewCoordsX = m_ElementGUI.coordsX;
    m_NewCoordsY = m_ElementGUI.coordsY;

    for (auto &i: m_ElementGUI.names) {
        m_IsVisibles.emplace_back(false);
        m_IsActives.emplace_back(false);
        m_IsAlives.emplace_back(true);
        m_RespawnTimes.emplace_back(0.0f);
    }
}

void GameObject::TakeSizeLocationArea() noexcept
{
    auto lv_MinMaxX = std::minmax_element(m_ElementGUI.coordsX.begin(),
                                          m_ElementGUI.coordsX.end());
    auto lv_MinMaxY = std::minmax_element(m_ElementGUI.coordsY.begin(),
                                          m_ElementGUI.coordsY.end());

    m_LocationArea.left   = *lv_MinMaxX.first;
    m_LocationArea.right  = *lv_MinMaxX.second + m_ElementGUI.gridWidth;
    m_LocationArea.top    = *lv_MinMaxY.first;
    m_LocationArea.bottom = *lv_MinMaxY.second + m_ElementGUI.gridHeight;
}

RECT GameObject::GetCollider() const noexcept
{
    return m_Collider;
}

Bools GameObject::GetIsAlives() const noexcept
{
    return m_IsAlives;
}

Bools GameObject::GetIsActives() const noexcept
{
    return m_IsActives;
}

Bools GameObject::GetIsVisibles() const noexcept
{
    return m_IsVisibles;
}

RECT GameObject::GetLocationArea() const noexcept
{
    return m_LocationArea;
}

Floats GameObject::GetRespawnTimes() const noexcept
{
    return m_RespawnTimes;
}

ElementGUI GameObject::GetElementGUI() const noexcept
{
    return m_ElementGUI;
}

UnsignedInts GameObject::GetNewCoordsX() const noexcept
{
    return m_NewCoordsX;
}

UnsignedInts GameObject::GetNewCoordsY() const noexcept
{
    return m_NewCoordsY;
}

void GameObject::SetNewCoords() noexcept
{
    for (size_t i = 0; i < m_ElementGUI.names.size(); ++i) {
        m_ElementGUI.coordsX[i] = m_NewCoordsX[i];
        m_ElementGUI.coordsY[i] = m_NewCoordsY[i];
    }
}

void GameObject::SetCoord(size_t _ElementNo) noexcept
{
    m_NewCoordsX[_ElementNo] = m_ElementGUI.coordsX[_ElementNo];
    m_NewCoordsY[_ElementNo] = m_ElementGUI.coordsY[_ElementNo];
}

void GameObject::SetIsAlive(bool _IsAlive, size_t _ElementNo) noexcept
{
    m_IsAlives[_ElementNo] = _IsAlive;
}

void GameObject::SetRespawnTime(float _Time, size_t _ElementNo) noexcept
{
    m_RespawnTimes[_ElementNo] = _Time;
}

void GameObject::SetIsVisible(bool _IsVisible, size_t _ElementNo) noexcept
{
    m_IsVisibles[_ElementNo] = _IsVisible;
}
