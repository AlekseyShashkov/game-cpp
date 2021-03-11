#ifndef PANELTOP_H
#define PANELTOP_H

#include "GUI.h"

class PanelTop : public GUI
{
 public:
    PanelTop(const wchar_t *_NameBMP) noexcept;

    ~PanelTop() noexcept override final;

    void InitializeObjectGUI() noexcept;
    void Draw(const HDC &_DisplayGUI) noexcept override final;

    void SetCountGems(const std::string _GemName) noexcept;

 private:
    const wchar_t *m_NAMEBMP;
    HDC m_HandlePanelTop;
    HDC m_HandlePanelTopMask;
    HBITMAP m_MemoryPanelTop;
    HBITMAP m_MemoryPanelTopMask;
    HFONT m_PixelFont;
    size_t m_CountEmerald;
    size_t m_CountSapphire;
    size_t m_CountRuby;
    size_t m_CountDiamond;
    wchar_t m_CountEmeraldChar[MAX_PATH];
    wchar_t m_CountSapphireChar[MAX_PATH];
    wchar_t m_CountRubyChar[MAX_PATH];
    wchar_t m_CountDiamondChar[MAX_PATH];
};

#endif  // PANELTOP_H
