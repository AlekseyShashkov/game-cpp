#include "PanelTop.h"

PanelTop::PanelTop(const wchar_t *_NameBMP)
    : m_NAMEBMP{ _NameBMP }, 
    m_CountEmerald{ 0 }, m_CountSapphire{ 0 },
    m_CountRuby{ 0 }, m_CountDiamond{ 0 }, 
    m_CountEmeraldChar{ NULL }, m_CountSapphireChar{ NULL } ,
    m_CountRubyChar{ NULL }, m_CountDiamondChar{ NULL }
{

}

PanelTop::~PanelTop() noexcept
{
    ::DeleteDC(m_HandlePanelTop);
    ::DeleteDC(m_HandlePanelTopMask);
    ::DeleteObject(m_MemoryPanelTop);
    ::DeleteObject(m_MemoryPanelTopMask);
    ::DeleteObject(m_PixelFont);
}

void PanelTop::InitializeObjectGUI() noexcept
{
    m_MemoryPanelTop = (HBITMAP)::LoadImage(NULL, m_NAMEBMP,
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    m_MemoryPanelTopMask = CreateBitmapMask((HBITMAP)m_MemoryPanelTop,
        RGB(0, 0, 0));

    m_HandlePanelTop     = ::CreateCompatibleDC(nullptr);
    m_HandlePanelTopMask = ::CreateCompatibleDC(nullptr);

    ::SelectObject(m_HandlePanelTop, m_MemoryPanelTop);
    ::SelectObject(m_HandlePanelTopMask, m_MemoryPanelTopMask);

    m_PixelFont = ::CreateFont(8, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
        DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, TEXT("Pixel"));
}

void PanelTop::Draw(const HDC &_DisplayGUI) noexcept
{
    ::BitBlt(_DisplayGUI,
             0, 0,
             1008, 180,
             m_HandlePanelTopMask,
             0, 0,
             SRCAND);

    ::BitBlt(_DisplayGUI,
             0, 0,
             1008, 180,
             m_HandlePanelTop,
             0, 0,
             SRCPAINT);

    ::SelectObject(_DisplayGUI, m_PixelFont);
    ::SetBkMode(_DisplayGUI, TRANSPARENT);
    
    ::TextOut(_DisplayGUI, 91, 9, m_CountEmeraldChar,
        wcslen(m_CountEmeraldChar));
    ::TextOut(_DisplayGUI, 77, 19, m_CountSapphireChar,
        wcslen(m_CountSapphireChar));
    ::TextOut(_DisplayGUI, 68, 29, m_CountRubyChar,
        wcslen(m_CountRubyChar));
    ::TextOut(_DisplayGUI, 68, 39, m_CountDiamondChar,
        wcslen(m_CountDiamondChar));
}

void PanelTop::SetCountGems(const std::string _GemName) noexcept
{
    if (std::strcmp(_GemName.c_str(), "emerald") == 0) {
        ++m_CountEmerald;
        wsprintf(m_CountEmeraldChar, L"%hu", m_CountEmerald);
    }
    if (std::strcmp(_GemName.c_str(), "sapphire") == 0) {
        ++m_CountSapphire;
        wsprintf(m_CountSapphireChar, L"%hu", m_CountSapphire);
    }
    if (std::strcmp(_GemName.c_str(), "ruby") == 0) {
        ++m_CountRuby;
        wsprintf(m_CountRubyChar, L"%hu", m_CountRuby);
    }
    if (std::strcmp(_GemName.c_str(), "diamond") == 0) {
        ++m_CountDiamond;
        wsprintf(m_CountDiamondChar, L"%hu", m_CountDiamond);
    }
}
