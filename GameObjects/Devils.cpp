#include "Devils.h"

Devils::Devils(const wchar_t *_NameXML)
{
    m_PlaygroundPtr->XMLParser(_NameXML, m_ElementGUI);

    m_Collider = { 10l, 0l, 
        m_ElementGUI.gridWidth - 10l, m_ElementGUI.gridHeight };
}

Devils::~Devils() noexcept
{
    ::DeleteDC(m_ElementGUI.handleTileSet);
    ::DeleteDC(m_ElementGUI.handleTileSetMask);
    ::DeleteObject(m_ElementGUI.memoryTileSet);
    ::DeleteObject(m_ElementGUI.memoryTileSetMask);
    m_ElementGUI.names.clear();
    m_ElementGUI.coordsX.clear();
    m_ElementGUI.coordsY.clear();
    m_ElementGUI.dimensionsLeft.clear();
    m_ElementGUI.dimensionsTop.clear();
    m_ElementGUI.dimensionsRight.clear();
    m_ElementGUI.dimensionsBottom.clear();

    m_NewCoordsX.clear();
    m_NewCoordsY.clear();
    m_IsVisibles.clear();
    m_IsActives.clear();
    m_IsAlives.clear();
    m_RespawnTimes.clear();
}

Devils::ObjectAction Devils::Action()
{
    return ObjectAction::NOTHING;
}

Devils::ObjectType Devils::GetType() const noexcept
{
    return ObjectType::ENEMY;
}

float Devils::GetRespawnTime(const std::string _ElementName) const noexcept
{
    return 0.0f;
}

void Devils::SetIsActive(bool _IsActive, size_t _ElementNo) noexcept
{
    if (_IsActive) {
        m_IsActives[_ElementNo] = _IsActive;
        m_ElementGUI.dimensionsTop[_ElementNo] = 1
            * m_ElementGUI.gridWidth;
        m_ElementGUI.dimensionsBottom[_ElementNo] = 2
            * m_ElementGUI.gridWidth;
    } else {
        m_IsActives[_ElementNo] = _IsActive;
        m_ElementGUI.dimensionsTop[_ElementNo] = 0
            * m_ElementGUI.gridWidth;
        m_ElementGUI.dimensionsBottom[_ElementNo] = 1
            * m_ElementGUI.gridWidth;
    }
}
