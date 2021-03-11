#ifndef PANELBOTTOM_H
#define PANELBOTTOM_H

#include "GUI.h"

class PanelBottom : public GUI
{
 public:
    PanelBottom(const wchar_t *_NameBMP) noexcept;

    ~PanelBottom() noexcept override final;

    void InitializeObjectGUI() noexcept;
    void Draw(const HDC &_DisplayGUI) noexcept override final;

 private:
    const wchar_t *m_NAMEBMP;
    HDC m_HandlePanelBottom;
    HBITMAP m_MemoryPanelBottom;
};

#endif  // PANELBOTTOM_H
