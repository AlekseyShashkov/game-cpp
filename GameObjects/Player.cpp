#include "Player.h"

Player::Player(const wchar_t *_NameXML)
    : m_Speed{ 6u }
{
    m_PlaygroundPtr->XMLParser(_NameXML, m_ElementGUI);

    m_Collider = { 0l, 0l, 
        m_ElementGUI.gridWidth, m_ElementGUI.gridHeight };
}

Player::~Player() noexcept
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

Player::ObjectAction Player::Action() noexcept
{
    BYTE lv_KeyState[256] = { 0 };
    ::GetKeyboardState(lv_KeyState);

    if ((lv_KeyState['W'] & 0xF0) != 0) {
        m_NewCoordsY[0] -= m_Speed;
        m_ElementGUI.dimensionsLeft[0] = 0 * m_ElementGUI.gridWidth;
    }
    if ((lv_KeyState['S'] & 0xF0) != 0) {
        m_NewCoordsY[0] += m_Speed;
        m_ElementGUI.dimensionsLeft[0] = 1 * m_ElementGUI.gridWidth;
    }
    if ((lv_KeyState['D'] & 0xF0) != 0) {
        m_NewCoordsX[0] += m_Speed;
        m_ElementGUI.dimensionsLeft[0] = 2 * m_ElementGUI.gridWidth;
    }
    if ((lv_KeyState['A'] & 0xF0) != 0) {
        m_NewCoordsX[0] -= m_Speed;
        m_ElementGUI.dimensionsLeft[0] = 3 * m_ElementGUI.gridWidth;
    }

    bool lv_IsWD = (lv_KeyState['W'] & lv_KeyState['D'] & 0xF0) != 0;
    bool lv_IsWA = (lv_KeyState['W'] & lv_KeyState['A'] & 0xF0) != 0;
    bool lv_IsSD = (lv_KeyState['S'] & lv_KeyState['D'] & 0xF0) != 0;
    bool lv_IsSA = (lv_KeyState['S'] & lv_KeyState['A'] & 0xF0) != 0;

    if (lv_IsWD) {
        m_ElementGUI.dimensionsLeft[0] = 4 * m_ElementGUI.gridWidth;
    }
    if (lv_IsWA) {
        m_ElementGUI.dimensionsLeft[0] = 5 * m_ElementGUI.gridWidth;
    }
    if (lv_IsSD) {
        m_ElementGUI.dimensionsLeft[0] = 6 * m_ElementGUI.gridWidth;
    }
    if (lv_IsSA) {
        m_ElementGUI.dimensionsLeft[0] = 7 * m_ElementGUI.gridWidth;
    }

    if ((::GetAsyncKeyState('C') & 0x0001) != 0) {
        return ObjectAction::PRESSED_C;
    }
    if ((::GetAsyncKeyState('E') & 0x0001) != 0) {
        return ObjectAction::PRESSED_E;
    }

    return ObjectAction::NOTHING;
}

Player::ObjectType Player::GetType() const noexcept
{
    return ObjectType::PLAYER;
}

float Player::GetRespawnTime(const std::string _ElementName) const noexcept
{
    return 0.0f;
}

void Player::SetIsActive(bool _IsActive, size_t _ElementNo) noexcept
{
    m_IsActives[_ElementNo] = _IsActive;
}
