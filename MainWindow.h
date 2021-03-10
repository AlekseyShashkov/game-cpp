#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameObjects\Player.h"
#include "GameObjects\Devils.h"
#include "GameObjects\Gems.h"
#include "GameObjects\BadRockHorizontal.h"
#include "GameObjects\BadRockVertical.h"
#include "GameObjects\ObjectList.h"
#include "GameObjects\GUI\PanelTop.h"
#include "GameObjects\GUI\PanelBottom.h"
#include "GameObjects\GUI\Playground.h"
#include "GameObjects\GUI\DisplayGUI.h"

#include <chrono>
#include <numeric>
#include <CommCtrl.h>

#define WINDOW_MAINWINDOW_WIDTH  1024
#define WINDOW_MAINWINDOW_HEIGHT 690

#define ID_TIMER        1
#define ID_STATUSWINDOW 100

#define AREA_XML          TEXT("Area.xml")
#define PANELTOP_BMP      TEXT("Images\\GUI\\PanelTop.bmp")
#define PANELBOTTOM_BMP   TEXT("Images\\GUI\\PanelBottom.bmp")

#define PLAYER_XML        TEXT("Player.xml")
#define DEVILS_XML        TEXT("Devils.xml")
#define GEMS_XML          TEXT("Gems.xml")
#define BRVERTICLEFT_XML  TEXT("BadRockVerticalLeft.xml")
#define BRVERTICRIGHT_XML TEXT("BadRockVerticalRight.xml")
#define BRHORIZTOP_XML    TEXT("BadRockHorizontalTop.xml")
#define BRHORIZBOT_XML    TEXT("BadRockHorizontalBottom.xml")

#define FONT_TTF          TEXT("Pixel.ttf")

ATOM RegClassEx(HINSTANCE hInstance, const WCHAR *szWindowClass);
HWND CreateWnd(HINSTANCE hInstance, const WCHAR *szWindowClass, 
               const WCHAR *szWindowTitle);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //MAINWINDOW_H