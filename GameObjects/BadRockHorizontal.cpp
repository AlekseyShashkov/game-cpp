#include "BadRockHorizontal.h"

BadRockHorizontal::BadRockHorizontal(const wchar_t *_NameXML)
{
    m_PlaygroundPtr->XMLParser(_NameXML, m_ElementGUI);

    m_Collider = { 0l, 10l, 
        m_ElementGUI.gridWidth, m_ElementGUI.gridHeight - 10l };
}

BadRockHorizontal::~BadRockHorizontal() noexcept
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

BadRockHorizontal::ObjectAction BadRockHorizontal::Action()
{
    return ObjectAction::NOTHING;
}

BadRockHorizontal::ObjectType BadRockHorizontal::GetType() const noexcept
{
    return ObjectType::BARRIER;
}

float BadRockHorizontal::GetRespawnTime(
    const std::string _ElementName) const noexcept
{
    return 0.0f;
}

void BadRockHorizontal::SetIsActive(bool _IsActive, size_t _ElementNo) noexcept
{

}
