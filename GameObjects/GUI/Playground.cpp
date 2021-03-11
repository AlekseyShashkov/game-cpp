#include "Playground.h"
#include <algorithm>

Playground::Playground(const wchar_t *_NameXML)
    : m_NAMEXML(_NameXML), m_SizeGameSpace{ 0 },
    m_Camera{ 0, 0, 1008, 480 },
    m_HalfSizeGameWindow{ 504, 240 },
    m_IsVisibleCollider{ false }
{
    XMLParser(m_NAMEXML, m_Floor);

    m_SizeGameSpace.x = *std::max_element(m_Floor.coordsX.begin(),
        m_Floor.coordsX.end()) + m_Floor.gridWidth;
    m_SizeGameSpace.y = *std::max_element(m_Floor.coordsY.begin(),
        m_Floor.coordsY.end()) + m_Floor.gridHeight;
}

Playground::~Playground() noexcept
{
    ::DeleteDC(m_HandleGameSpace);
    ::DeleteObject(m_MemoryGameSpace);

    ::DeleteDC(m_Floor.handleTileSetMask);
    ::DeleteObject(m_Floor.memoryTileSetMask);
}

void Playground::InitializeObjectGUI() noexcept
{
    BITMAPINFO lv_Bmi;

    memset(&lv_Bmi, 0, sizeof(BITMAPINFOHEADER));
    lv_Bmi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
    lv_Bmi.bmiHeader.biWidth        = m_SizeGameSpace.x;
    lv_Bmi.bmiHeader.biHeight       = m_SizeGameSpace.y;
    lv_Bmi.bmiHeader.biPlanes       = 1;
    lv_Bmi.bmiHeader.biBitCount     = 8 << 2;
    lv_Bmi.bmiHeader.biCompression  = BI_RGB;
    lv_Bmi.bmiHeader.biSizeImage    = lv_Bmi.bmiHeader.biWidth 
                                    * lv_Bmi.bmiHeader.biHeight * (1 << 2);

    m_MemoryGameSpace = ::CreateDIBSection(nullptr, &lv_Bmi, 
                                        DIB_RGB_COLORS, nullptr, nullptr, 0);
    m_Floor.memoryTileSetMask = ::CreateDIBSection(nullptr, &lv_Bmi, 
                                        DIB_RGB_COLORS, nullptr, nullptr, 0);

    m_HandleGameSpace         = ::CreateCompatibleDC(nullptr);
    m_Floor.handleTileSet     = ::CreateCompatibleDC(nullptr);
    m_Floor.handleTileSetMask = ::CreateCompatibleDC(nullptr);

    ::SelectObject(m_HandleGameSpace, m_MemoryGameSpace);
    ::SelectObject(m_Floor.handleTileSet, m_Floor.memoryTileSet);
    ::SelectObject(m_Floor.handleTileSetMask, m_Floor.memoryTileSetMask);

    // We form our substrate ("floor") and store it in the "mask" bitmap,
    // clean the rest of the ElementGUI m_Floor data as unnecessary.
    for (size_t i = 0; i < m_Floor.names.size(); ++i) {
        ::BitBlt(m_Floor.handleTileSetMask,
                 m_Floor.coordsX[i],
                 m_Floor.coordsY[i],
                 m_Floor.dimensionsRight[i], 
                 m_Floor.dimensionsBottom[i],
                 m_Floor.handleTileSet,
                 m_Floor.dimensionsLeft[i], 
                 m_Floor.dimensionsTop[i],
                 SRCCOPY);        
    }

    ::BitBlt(m_HandleGameSpace,
             0, 0,
             m_SizeGameSpace.x, m_SizeGameSpace.y,
             m_Floor.handleTileSetMask,
             0, 0,
             SRCCOPY);

    ::DeleteDC(m_Floor.handleTileSet);
    ::DeleteObject(m_Floor.memoryTileSet);

    m_Floor.names.clear();
    m_Floor.coordsX.clear();
    m_Floor.coordsY.clear();
    m_Floor.dimensionsLeft.clear();
    m_Floor.dimensionsTop.clear();
    m_Floor.dimensionsRight.clear();
    m_Floor.dimensionsBottom.clear();
}

void Playground::CleanScreen() noexcept
{
    ::BitBlt(m_HandleGameSpace,
        m_Camera.left, m_Camera.top,
        1008, 480,
        m_Floor.handleTileSetMask,
        m_Camera.left, m_Camera.top,
        SRCCOPY);
}

void Playground::Draw(const HDC &_DisplayGUI) noexcept
{
    ::BitBlt(_DisplayGUI,
             0, 60, 
             1008, 480,
             m_HandleGameSpace,
             m_Camera.left, m_Camera.top,
             SRCCOPY);
}

void Playground::DrawElementGUI(const ElementGUI &_ElementGUI, 
                                const RECT &_ColliderElementGUI,
                                const Bools &_IsVisibles,
                                const Bools &_IsAlives) noexcept
{
    for (size_t i = 0; i < _ElementGUI.names.size(); ++i) {
        if (_IsVisibles[i] && _IsAlives[i]) {
            ::BitBlt(m_HandleGameSpace,
                _ElementGUI.coordsX[i], _ElementGUI.coordsY[i],
                _ElementGUI.dimensionsRight[i],
                _ElementGUI.dimensionsBottom[i],
                _ElementGUI.handleTileSetMask,
                _ElementGUI.dimensionsLeft[i],
                _ElementGUI.dimensionsTop[i],
                SRCAND);

            ::BitBlt(m_HandleGameSpace,
                _ElementGUI.coordsX[i], _ElementGUI.coordsY[i],
                _ElementGUI.dimensionsRight[i],
                _ElementGUI.dimensionsBottom[i],
                _ElementGUI.handleTileSet,
                _ElementGUI.dimensionsLeft[i],
                _ElementGUI.dimensionsTop[i],
                SRCPAINT);

            if (m_IsVisibleCollider) {
                DrawCollider(_ElementGUI, _ColliderElementGUI, i);
            }
        }
    }
}

void Playground::DrawCollider(const ElementGUI &_ElementGUI, 
                             const RECT &_ColliderElementGUI,
                             const size_t _ElementNo) noexcept
{
    RECT lv_Rect = { 0 };
    HBRUSH lv_Brush = NULL;    

    lv_Brush = ::CreateSolidBrush(RGB(255, 255, 255));

    ::SetRect(&lv_Rect,
        _ElementGUI.coordsX[_ElementNo] + _ColliderElementGUI.left,
        _ElementGUI.coordsY[_ElementNo] + _ColliderElementGUI.top,
        _ElementGUI.coordsX[_ElementNo] + _ColliderElementGUI.right,
        _ElementGUI.coordsY[_ElementNo] + _ColliderElementGUI.bottom);
            
    ::FrameRect(m_HandleGameSpace, &lv_Rect, lv_Brush);

    ::DeleteObject(lv_Brush);
}

void Playground::Camera(const int _PlayerPositionX, 
                        const int _PlayerPositionY) noexcept
{
    bool lv_IsLessMaximumX = _PlayerPositionX 
                            + m_HalfSizeGameWindow.x <= m_SizeGameSpace.x;
    bool lv_IsMoreMinimumX = _PlayerPositionX - m_HalfSizeGameWindow.x >= 0;

    if (lv_IsLessMaximumX && lv_IsMoreMinimumX) {
        m_Camera.left  = _PlayerPositionX - m_HalfSizeGameWindow.x;
        m_Camera.right = _PlayerPositionX + m_HalfSizeGameWindow.x;
    }    

    bool lv_IsLessMaximumY = _PlayerPositionY 
                            + m_HalfSizeGameWindow.y <= m_SizeGameSpace.y;
    bool lv_IsMoreMinimumY = _PlayerPositionY - m_HalfSizeGameWindow.y >= 0;

    if (lv_IsLessMaximumY && lv_IsMoreMinimumY) {
        m_Camera.top    = _PlayerPositionY - m_HalfSizeGameWindow.y;
        m_Camera.bottom = _PlayerPositionY + m_HalfSizeGameWindow.y;
    }
}

bool Playground::GetColliderIsVisible() const noexcept
{
    return m_IsVisibleCollider;
}

RECT Playground::GetCamera() const noexcept
{
    return m_Camera;
}

void Playground::SetColliderIsVisible(bool _IsVisible) noexcept
{
    m_IsVisibleCollider = _IsVisible;
}
