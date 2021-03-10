#include "DisplayGUI.h"

DisplayGUI::DisplayGUI()
{
    InitializeDisplay();
}

DisplayGUI::~DisplayGUI() noexcept
{
    ::DeleteDC(m_HandleDisplayGUI);
    ::DeleteObject(m_MemoryDisplayGUI);

    m_ListGUIObjects.clear();
}

void DisplayGUI::InitializeDisplay() noexcept
{
    BITMAPINFO lv_Bmi;

    memset(&lv_Bmi, 0, sizeof(BITMAPINFOHEADER));
    lv_Bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    lv_Bmi.bmiHeader.biWidth       = 1008;
    lv_Bmi.bmiHeader.biHeight      = 629;
    lv_Bmi.bmiHeader.biPlanes      = 1;
    lv_Bmi.bmiHeader.biBitCount    = 8 << 2;
    lv_Bmi.bmiHeader.biCompression = BI_RGB;
    lv_Bmi.bmiHeader.biSizeImage   = lv_Bmi.bmiHeader.biWidth
        * lv_Bmi.bmiHeader.biHeight * (1 << 2);

    m_MemoryDisplayGUI = ::CreateDIBSection(nullptr, &lv_Bmi, DIB_RGB_COLORS,
        nullptr, nullptr, 0);

    m_HandleDisplayGUI = ::CreateCompatibleDC(nullptr);

    ::SelectObject(m_HandleDisplayGUI, m_MemoryDisplayGUI);
}

void DisplayGUI::AddObject(const GUIObjectPtr &_GUIObjectPtr) noexcept
{
    m_ListGUIObjects.emplace_back(_GUIObjectPtr);
    m_ListGUIObjects.back()->InitializeObjectGUI();
}

void DisplayGUI::DisplayGame(const HDC &_ScreenHdc) noexcept
{
    if (!m_ListGUIObjects.empty()) {
        for (auto &i : m_ListGUIObjects) {
            i->Draw(m_HandleDisplayGUI);
        }
    }

    ::BitBlt(_ScreenHdc,
             0, 0,
             1008, 629,
             m_HandleDisplayGUI,
             0, 0,
             SRCCOPY);
}

HDC DisplayGUI::GetHandleDisplayGUI() const noexcept
{
    return m_HandleDisplayGUI;
}
