#include "BadRockVertical.h"

BadRockVertical::BadRockVertical(const wchar_t *_NameXML)
{
    m_PlaygroundPtr->XMLParser(_NameXML, m_ElementGUI);

    m_Collider = { 10l, 0l,
        m_ElementGUI.gridWidth - 10l, m_ElementGUI.gridHeight };
}

BadRockVertical::~BadRockVertical() noexcept
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

BadRockVertical::ObjectAction BadRockVertical::Action()
{
    return ObjectAction::NOTHING;
}

BadRockVertical::ObjectType BadRockVertical::GetType() const noexcept
{
    return ObjectType::BARRIER;
}

float BadRockVertical::GetRespawnTime(const std::string elementName) const noexcept
{
    return 0.0f;
}

void BadRockVertical::SetIsActive(bool _IsActive, size_t _ElementNo) noexcept
{

}
