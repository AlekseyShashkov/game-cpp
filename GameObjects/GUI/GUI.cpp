#include "GUI.h"        
#include <comutil.h>
#include <atlbase.h>

#pragma comment( lib, "comsuppw.lib")

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: vois XMLParser(const wchar_t *, ElementGUI &)
//
// PARAMETERS: [in]  _XML           - name of XML document;
//             [out] _ElementGUI    - into that structure we write data 
//                                    received from the XML document.
//
// RETURN VALUE: no.
//
// PURPOSE: loading XML document and getting values of nodes attributes.
//
// COMMENTS: no.
//
void GUI::XMLParser(const wchar_t *_XML, ElementGUI &_ElementGUI)
{
    CComPtr<IXMLDOMDocument>  lv_XMLDocument = nullptr;
    CComQIPtr<IXMLDOMElement> lv_XMLElement = nullptr;

    IXMLDOMNodeList *lv_NodeList = nullptr;
    IXMLDOMNode     *lv_Node = nullptr;

    HRESULT    lv_Hr;
    _variant_t lv_VarValue;

    // Initialize COM.
    CoInitialize(NULL);

    try {
        // Create parser instance.
        lv_Hr = lv_XMLDocument.CoCreateInstance(__uuidof(DOMDocument));
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;

        // Loading XML document.
        VARIANT_BOOL lv_Success = false;
        lv_Hr = lv_XMLDocument->load(CComVariant(_XML), &lv_Success);
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;

        // Connecting to head note of tileset.
        lv_Hr = lv_XMLDocument->selectSingleNode(CComBSTR(TEXT("tileset")),
                                            &lv_Node);
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;

        // Getting access to node attributes of tileset.
        lv_XMLElement = lv_Node;
        if (!lv_XMLElement) {
            throw TEXT("Error of get access to node attributes of tileset!");
        }
        // Getting pass to image (tileset).
        lv_Hr = lv_XMLElement->getAttribute(CComBSTR(TEXT("source")),
                                            &lv_VarValue);
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;

        // Loading tileset along the resulting path..
        _ElementGUI.memoryTileSet = (HBITMAP)::LoadImage(NULL,
                                _bstr_t(lv_VarValue), IMAGE_BITMAP,
                                0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        if (!_ElementGUI.memoryTileSet) {
            throw TEXT("Failed to load image!");
        }

        // Get elements width of tileset grid.
        lv_Hr = lv_XMLElement->getAttribute(CComBSTR(TEXT("gridWidth")),
                                            &lv_VarValue);
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
        _ElementGUI.gridWidth = atoi(_bstr_t(lv_VarValue));

        // Get elements height of tileset grid.
        lv_Hr = lv_XMLElement->getAttribute(CComBSTR(TEXT("gridHeight")),
                                            &lv_VarValue);
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
        _ElementGUI.gridHeight = atoi(_bstr_t(lv_VarValue));

        // Searching all tile elements.   
        lv_Hr = lv_XMLDocument->getElementsByTagName(CComBSTR(TEXT("tile")),
                                                &lv_NodeList);
        SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;

        // Getting length of list returned by the previous search.  
        long lv_NodeListLenght = 0;
        lv_Hr = lv_NodeList->get_length(&lv_NodeListLenght);

        if (SUCCEEDED(lv_Hr)) {
            lv_NodeList->reset();
            // Loop over all found tile elements  
            // and getting values of their attributes.
            for (auto i = 0; i < lv_NodeListLenght; ++i) {
                lv_Hr = lv_NodeList->get_item(i, &lv_Node);
                SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;

                if (lv_Node) {
                    // Getting accessing to node attributes of tile.
                    lv_XMLElement = lv_Node;
                    if (!lv_XMLElement) {
                        throw TEXT(
                            "Error getting access to node attributes of tile!");
                    }

                    // Getting tile name.
                    lv_Hr = lv_XMLElement->getAttribute(
                                        CComBSTR(TEXT("name")), &lv_VarValue);
                    SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
                    std::string lv_Name;
                    _ElementGUI.names.emplace_back(
                        BstrToStdString(lv_VarValue.bstrVal, lv_Name));

                    // Getting left and right tile dimensions on tileset.
                    lv_Hr = lv_XMLElement->getAttribute(
                                        CComBSTR(TEXT("tileX")), &lv_VarValue);
                    SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
                    _ElementGUI.dimensionsLeft.emplace_back(
                        atoi(_bstr_t(lv_VarValue)) * _ElementGUI.gridWidth);
                    _ElementGUI.dimensionsRight.emplace_back(
                        _ElementGUI.gridWidth);

                    // Getting top and bottom tile dimensions on tileset.
                    lv_Hr = lv_XMLElement->getAttribute(
                                        CComBSTR(TEXT("tileY")), &lv_VarValue);
                    SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
                    _ElementGUI.dimensionsTop.emplace_back(
                        atoi(_bstr_t(lv_VarValue)) * _ElementGUI.gridHeight);
                    _ElementGUI.dimensionsBottom.emplace_back(
                        (atoi(_bstr_t(lv_VarValue)) + 1) 
                        * _ElementGUI.gridHeight);

                    // Getting tile coordinate X of drawing point.    
                    lv_Hr = lv_XMLElement->getAttribute(
                                        CComBSTR(TEXT("x")), &lv_VarValue);
                    SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
                    _ElementGUI.coordsX.emplace_back(
                        atoi(_bstr_t(lv_VarValue)));

                    // Getting tile coordinate Y of drawing point.    
                    lv_Hr = lv_XMLElement->getAttribute(
                                        CComBSTR(TEXT("y")), &lv_VarValue);
                    SUCCEEDED(lv_Hr) ? 0 : throw lv_Hr;
                    _ElementGUI.coordsY.emplace_back(
                        atoi(_bstr_t(lv_VarValue)));

                    lv_Node->Release();
                    lv_Node = nullptr;
                }
            }
            lv_NodeList->Release();
            lv_NodeList = nullptr;
        }

        lv_XMLDocument.Release();
        lv_XMLDocument = nullptr;
        lv_XMLElement.Release();
        lv_XMLElement = nullptr;
        VariantClear(&lv_VarValue); 
    } catch (WCHAR *wchErr) {
        MessageBox(NULL,
            wchErr,
            TEXT("Parser error!"),
            MB_OK | MB_ICONEXCLAMATION);

        if (lv_XMLDocument) {
            lv_XMLDocument.Release();
        }
        if (lv_XMLElement) {
            lv_XMLElement.Release();
        }
        if (lv_NodeList) {
            lv_NodeList->Release();
        }
        if (lv_Node) {
            lv_Node->Release();
        }
        if (lv_VarValue) {
            VariantClear(&lv_VarValue);
        }
    } catch (...) {
        MessageBox(NULL,
            TEXT("Failed to load XML file data"),
            TEXT("Parser error!"),
            MB_OK | MB_ICONEXCLAMATION);

        if (lv_XMLDocument) {
            lv_XMLDocument.Release();
        }
        if (lv_XMLElement) {
            lv_XMLElement.Release();
        }
        if (lv_NodeList) {
            lv_NodeList->Release();
        }
        if (lv_Node) {
            lv_Node->Release();
        }
        if (lv_VarValue) {
            VariantClear(&lv_VarValue);
        }
    }

    // Close COM.
    CoUninitialize();
}

std::string GUI::BstrToStdString(const BSTR _Bstr, std::string &_Result)
{
	int lv_Size = ::WideCharToMultiByte(CP_UTF8, 0, _Bstr,
		-1, NULL, 0, NULL, NULL);

	if (lv_Size > 0) {
		_Result.resize(lv_Size);

		::WideCharToMultiByte(CP_UTF8, 0, _Bstr,
			-1, &_Result[0], lv_Size, NULL, NULL);
	}
	else {
		_Result.clear();
	}

	return _Result;
}

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: HBITMAP CreateBitmapMask(HBITMAP, COLORREF)
//
// PARAMETERS: [in]  _TileSet            - original image;
//             [in]  _TransparentColor   - transparent color.
//
// RETURN VALUE: lv_MemoryMask    - image mask.
//
// PURPOSE: creating image mask.
//
// COMMENTS: no.
//
HBITMAP GUI::CreateBitmapMask(const HBITMAP _TileSet, 
                              const COLORREF _TransparentColor) noexcept
{
    HDC     lv_HandleTileSet;
    HDC     lv_HandleMask;
    HBITMAP lv_MemoryMask;
    BITMAP  lv_Bm;

    // Create monochrome (1 bit) mask bitmap.
    ::GetObject(_TileSet, sizeof(BITMAP), &lv_Bm);
    lv_MemoryMask = ::CreateBitmap(lv_Bm.bmWidth, lv_Bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver.
    lv_HandleTileSet = ::CreateCompatibleDC(0);
    lv_HandleMask    = ::CreateCompatibleDC(0);

    ::SelectObject(lv_HandleTileSet, _TileSet);
    ::SelectObject(lv_HandleMask, lv_MemoryMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    ::SetBkColor(lv_HandleTileSet, _TransparentColor);

    // Copy bits from the colour image to the B+W mask. Everything
    // with the background colour ends up white while everythig else 
    // ends up black.
    ::BitBlt(lv_HandleMask,
             0, 0, 
             lv_Bm.bmWidth, lv_Bm.bmHeight,
             lv_HandleTileSet,
             0, 0, 
             SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    ::BitBlt(lv_HandleTileSet,
             0, 0, 
             lv_Bm.bmWidth, lv_Bm.bmHeight,
             lv_HandleMask,
             0, 0, 
             SRCINVERT);

    ::DeleteDC(lv_HandleTileSet);
    ::DeleteDC(lv_HandleMask);

    return lv_MemoryMask;
}
