#include "PanelBottom.h"

PanelBottom::PanelBottom(const wchar_t *_NameBMP)
    : m_NAMEBMP(_NameBMP)
{

}

PanelBottom::~PanelBottom() noexcept
{
    ::DeleteDC(m_HandlePanelBottom);
    ::DeleteObject(m_MemoryPanelBottom);
}

void PanelBottom::InitializeObjectGUI() noexcept
{
    m_MemoryPanelBottom = (HBITMAP)::LoadImage(NULL, m_NAMEBMP,
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    m_HandlePanelBottom = ::CreateCompatibleDC(nullptr);

    ::SelectObject(m_HandlePanelBottom, m_MemoryPanelBottom);
}

void PanelBottom::Draw(const HDC &_DisplayGUI) noexcept
{
    ::BitBlt(_DisplayGUI,
             0, 539,
             1008, 90,
             m_HandlePanelBottom,
             0, 0,
             SRCCOPY);
}
