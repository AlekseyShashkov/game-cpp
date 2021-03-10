#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include "GUI.h"

using Bools = std::vector<bool>;

class Playground : public GUI
{
 public:
    Playground(const wchar_t *_NameXML);

    ~Playground() noexcept override final;

    void InitializeObjectGUI() noexcept;
    void CleanScreen() noexcept;
    void Draw(const HDC &_DisplayGUI) noexcept override final;
    void DrawElementGUI(const ElementGUI &_ElementGUI,
                        const RECT &_ColliderElementGUI,
                        const Bools &_IsVisibles,
                        const Bools &_IsAlives) noexcept;
    void Camera(const int _PlayerPositionX, 
                const int _PlayerPositionY) noexcept;

    RECT GetCamera() const noexcept;

    void SetColliderIsVisible(bool _IsVisible) noexcept;
    bool GetColliderIsVisible() const noexcept;

 private:
    void DrawCollider(const ElementGUI &_ElementGUI,
                      const RECT &_ColliderElementGUI,
                      const size_t _ElementNo) noexcept;

    const wchar_t *m_NAMEXML;
    HDC m_HandleGameSpace;
    HBITMAP m_MemoryGameSpace;
    RECT m_Camera;
    POINT m_SizeGameSpace;
    POINT m_HalfSizeGameWindow;
    ElementGUI m_Floor;
    bool m_IsVisibleCollider;
};

#endif  // PLAYGROUND_H

