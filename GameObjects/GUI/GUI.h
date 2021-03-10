#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include <vector>

struct ElementGUI
{
    std::vector<std::string> names;
    std::vector<size_t> dimensionsLeft;
    std::vector<size_t> dimensionsTop;
    std::vector<size_t> dimensionsRight;
    std::vector<size_t> dimensionsBottom;
    std::vector<size_t> coordsX;
    std::vector<size_t> coordsY;

    HDC handleTileSet     = nullptr;
    HDC handleTileSetMask = nullptr;
    HBITMAP memoryTileSet     = NULL;
    HBITMAP memoryTileSetMask = NULL;

    int gridWidth  = 0;
    int gridHeight = 0;
};

class GUI
{
 public:
    GUI() = default;

    virtual ~GUI() = default;

    void XMLParser(const wchar_t *_XML, ElementGUI &_ElementGUI);
    HBITMAP CreateBitmapMask(const HBITMAP _TileSet, 
                            const COLORREF _TransparentColor)  noexcept;

    virtual void InitializeObjectGUI() = 0;
    virtual void Draw(const HDC &_DisplayGUI) = 0;

 private:
    std::string BstrToStdString(const BSTR _Bstr, std::string &_Result);
};

#endif  // GUI_H
