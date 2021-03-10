#ifndef DISPLAYGUI_H
#define DISPLAYGUI_H

#include "GUI.h"
#include <windows.h>
#include <list>
#include <memory>

using GUIObjectPtr   = std::shared_ptr<GUI>;
using ListGUIObjects = std::list<GUIObjectPtr>;

class DisplayGUI
{
 public:
    DisplayGUI();

    ~DisplayGUI() noexcept;
    
    void AddObject(const GUIObjectPtr &_GUIObjectPtr) noexcept;
    void DisplayGame(const HDC &_ScreenHdc) noexcept;

    HDC GetHandleDisplayGUI() const noexcept;

 private:
    void InitializeDisplay() noexcept;

    HDC m_HandleDisplayGUI;
    HBITMAP m_MemoryDisplayGUI;
    ListGUIObjects m_ListGUIObjects;
};

#endif  // DISPLAYGUI_H
