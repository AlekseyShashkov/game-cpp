#include "Gems.h"

Gems::Gems(const wchar_t *_NameXML)
{
    m_PlaygroundPtr->XMLParser(_NameXML, m_ElementGUI);

    m_Collider = { 5l, 5l, 
        m_ElementGUI.gridWidth - 5l, m_ElementGUI.gridHeight - 5l };
}

Gems::~Gems() noexcept
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

Gems::ObjectAction Gems::Action() noexcept
{
    return ObjectAction::NOTHING;
}

Gems::ObjectType Gems::GetType() const noexcept
{
    return ObjectType::RESOURCE;
}

float Gems::GetRespawnTime(const std::string _ElementName) const noexcept
{
    if (std::strcmp(_ElementName.c_str(), "diamond") == 0) {
        return 5000.0f;
    }
    if (std::strcmp(_ElementName.c_str(), "sapphire") == 0) {
        return 2000.0f;
    }
    if (std::strcmp(_ElementName.c_str(), "emerald") == 0) {
        return 4000.0f;
    }
    if (std::strcmp(_ElementName.c_str(), "ruby") == 0) {
        return 5000.0f;
    }

    return 0.0f;
}

void Gems::SetIsActive(bool _IsActive, size_t _ElementNo) noexcept
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
