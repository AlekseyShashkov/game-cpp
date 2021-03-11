#include "MainWindow.h"

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
//
// PARAMETERS: [in]  hWind       - window handle;
//             [in]  uMsg        - message for window;
//             [in]  wParam      - additional information for message;
//             [in]  lParam      - additional information for message.
//
// RETURN VALUE: 0    - in all cases.
//
// PURPOSE: message processing function.
//
// COMMENTS: no.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static auto gameStart = std::chrono::steady_clock::now();

    static auto display     = std::make_shared<DisplayGUI>();
    static auto playground  = std::make_shared<Playground>(AREA_XML);
    static auto panelTop    = std::make_shared<PanelTop>(PANELTOP_BMP);
    static auto panelBottom = std::make_shared<PanelBottom>(PANELBOTTOM_BMP);

    static auto list = std::make_unique<ObjectList>(playground, panelTop, 
        std::move(gameStart));

    HDC hDC;
    PAINTSTRUCT ps;

    switch (uMsg) {
    case WM_CREATE:
        display->AddObject(playground);
        display->AddObject(panelTop);
        display->AddObject(panelBottom);

        list->AddObject(std::make_unique<BadRockHorizontal>(BRHORIZTOP_XML));
        list->AddObject(std::make_unique<BadRockVertical>(BRVERTICLEFT_XML));
        list->AddObject(std::make_unique<BadRockVertical>(BRVERTICRIGHT_XML));
        list->AddObject(std::make_unique<BadRockHorizontal>(BRHORIZBOT_XML));
        list->AddObject(std::make_unique<Devils>(DEVILS_XML));
        list->AddObject(std::make_unique<Gems>(GEMS_XML));
        list->AddObject(std::make_unique<Player>(PLAYER_XML));

        if (::AddFontResourceEx(FONT_TTF, FR_NOT_ENUM, nullptr)) {
            ::SendNotifyMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
        }

        ::SetTimer(hWnd, ID_TIMER, 40, nullptr);
        
        return 0;

    case WM_PAINT:
        hDC = ::BeginPaint(hWnd, &ps);
    
        list->UpdateGame();
        display->DisplayGame(hDC);

        ::EndPaint(hWnd, &ps);
        return 0;

    case WM_TIMER:
        ::InvalidateRect(hWnd, NULL, FALSE);
        ::UpdateWindow(hWnd);
        return 0;

    case WM_DESTROY:
        ::RemoveFontResourceEx(FONT_TTF, FR_NOT_ENUM, nullptr);
        ::KillTimer(hWnd, ID_TIMER);
        ::PostQuitMessage(NULL);
        return 0;

    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}