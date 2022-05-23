 //WindowsProject2.cpp : Определяет точку входа для приложения.


#include "framework.h"
#include "WindowsProject2.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <math.h>
//#include "basewin.h"
#pragma comment(lib, "d2d1")

#define MAX_LOADSTRING 100
#define NUM    1000
#define TWOPI  (2 * 3.14159)

D2D1_POINT_2F	apt[NUM];
D2D1_POINT_2F	apt1[2];
int mode = ID_SIN;

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

template <class T>
void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case ID_SIN:
            {
                mode = ID_SIN;
                ID2D1Factory* pD2DFactory = NULL;
                HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
                if (SUCCEEDED(hr))
                {
                    RECT rc;
                    GetClientRect(hWnd, &rc);

                    ID2D1HwndRenderTarget* pRT = NULL;
                    hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pRT);

                    if (SUCCEEDED(hr))
                    {
                        ID2D1SolidColorBrush* pBrush;
                        const D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0);
                        hr = pRT->CreateSolidColorBrush(color, &pBrush);
                        if (SUCCEEDED(hr))
                        {
                            pRT->BeginDraw();
                            pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

                            apt1[0].x = 0; apt1[0].y = (rc.bottom) / 2;
                            apt1[1].x = (rc.right); apt1[1].y = (rc.bottom) / 2;
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);
                            apt1[0].x = (rc.right) / 2; apt1[0].y = 0;
                            apt1[1].x = (rc.right) / 2; apt1[1].y = (rc.bottom);
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);

                            //Sin
                            apt[0].x = 0 * rc.right / NUM;
                            apt[0].y = (int)(rc.bottom / 2 * (1 - sin(TWOPI * 0 / NUM)));
                            for (int i = 1; i < NUM; i++)
                            {
                                apt[i].x = i * rc.right / NUM;
                                apt[i].y = (int)(rc.bottom / 2 * (1 - sin(TWOPI * i / NUM)));
                                pRT->DrawLine(apt[i - 1], apt[i], pBrush);
                            }

                            pRT->EndDraw();
                        }
                        SafeRelease(&pBrush);
                    }
                    SafeRelease(&pRT);
                }
                SafeRelease(&pD2DFactory);
            }
            break;
            case ID_COS:
            {
                mode = ID_COS;
                ID2D1Factory* pD2DFactory = NULL;
                HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
                if (SUCCEEDED(hr))
                {
                    RECT rc;
                    GetClientRect(hWnd, &rc);

                    ID2D1HwndRenderTarget* pRT = NULL;
                    hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pRT);

                    if (SUCCEEDED(hr))
                    {
                        ID2D1SolidColorBrush* pBrush;
                        const D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0);
                        hr = pRT->CreateSolidColorBrush(color, &pBrush);
                        if (SUCCEEDED(hr))
                        {
                            pRT->BeginDraw();
                            pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

                            apt1[0].x = 0; apt1[0].y = (rc.bottom) / 2;
                            apt1[1].x = (rc.right); apt1[1].y = (rc.bottom) / 2;
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);
                            apt1[0].x = (rc.right) / 2; apt1[0].y = 0;
                            apt1[1].x = (rc.right) / 2; apt1[1].y = (rc.bottom);
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);

                            //Cos
                            apt[0].x = 0 * rc.right / NUM;
                            apt[0].y = rc.bottom - (int)(rc.bottom / 2 * (1 - sin(TWOPI * 0 / NUM)));
                            for (int i = 1; i < NUM; i++)
                            {
                            	apt[i].x = i * rc.right / NUM;
                            	apt[i].y = rc.bottom - (int)(rc.bottom / 2 * (1 - sin(TWOPI * i / NUM)));
                                pRT->DrawLine(apt[i - 1], apt[i], pBrush);
                            }

                            pRT->EndDraw();
                        }
                        SafeRelease(&pBrush);
                    }
                    SafeRelease(&pRT);
                }
                SafeRelease(&pD2DFactory);
            }

            break;
            case ID_SQRT:
            {
                mode = ID_SQRT;
                ID2D1Factory* pD2DFactory = NULL;
                HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
                if (SUCCEEDED(hr))
                {
                    RECT rc;
                    GetClientRect(hWnd, &rc);
                    ID2D1HwndRenderTarget* pRT = NULL;
                    hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pRT);

                    if (SUCCEEDED(hr))
                    {
                        ID2D1SolidColorBrush* pBrush;
                        const D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0);
                        hr = pRT->CreateSolidColorBrush(color, &pBrush);
                        if (SUCCEEDED(hr))
                        {
                            pRT->BeginDraw();
                            pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

                            apt1[0].x = 0; apt1[0].y = (rc.bottom) / 2;
                            apt1[1].x = (rc.right); apt1[1].y = (rc.bottom) / 2;
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);
                            apt1[0].x = (rc.right) / 2; apt1[0].y = 0;
                            apt1[1].x = (rc.right) / 2; apt1[1].y = (rc.bottom);
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);

                            apt[0].x = rc.right / 2;
                            apt[0].y = rc.bottom / 2;
                            for (int i = 1; i < NUM; i++)
                            {
                            	apt[i].x = rc.right / 2 + i * rc.right / NUM;
                            	apt[i].y = rc.bottom / 2 - sqrt(i * rc.right / NUM) * 10;
                            	pRT->DrawLine(apt[i - 1], apt[i], pBrush);
                            }

                            pRT->EndDraw();
                        }
                        SafeRelease(&pBrush);
                    }
                    SafeRelease(&pRT);
                }
                SafeRelease(&pD2DFactory);
            }
            break;
            case ID_PAR:
            {
                mode = ID_PAR;
                ID2D1Factory* pD2DFactory = NULL;
                HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
                if (SUCCEEDED(hr))
                {
                    RECT rc;
                    GetClientRect(hWnd, &rc);

                    ID2D1HwndRenderTarget* pRT = NULL;
                    hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pRT);

                    if (SUCCEEDED(hr))
                    {
                        ID2D1SolidColorBrush* pBrush;
                        const D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0);
                        hr = pRT->CreateSolidColorBrush(color, &pBrush);
                        if (SUCCEEDED(hr))
                        {
                            pRT->BeginDraw();
                            pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

                            apt1[0].x = 0; apt1[0].y = (rc.bottom) / 2;
                            apt1[1].x = (rc.right); apt1[1].y = (rc.bottom) / 2;
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);
                            apt1[0].x = (rc.right) / 2; apt1[0].y = 0;
                            apt1[1].x = (rc.right) / 2; apt1[1].y = (rc.bottom);
                            pRT->DrawLine(apt1[0], apt1[1], pBrush);

                            //Parabola
                            apt[0].x = rc.right / 2;
                            apt[0].y = rc.bottom / 2;
                            for (int i = 1; i < NUM; i++)
                            {
                                apt[i].x = rc.right / 2 + (i * rc.right / NUM);
                                apt[i].y = (int)(rc.bottom / 2 - (i * rc.bottom / NUM) * (i * rc.bottom / NUM) / 30);
                                pRT ->DrawLine(apt[i - 1], apt[i], pBrush);
                            }
                            for (int i = 1; i < NUM; i++)
                            {
                                apt[i].x = rc.right / 2 - (i * rc.right / NUM);
                                apt[i].y = (int)(rc.bottom / 2 - (i * rc.bottom / NUM) * (i * rc.bottom / NUM) / 30);
                                pRT->DrawLine(apt[i - 1], apt[i], pBrush);
                            }

                            pRT->EndDraw();
                        }
                        SafeRelease(&pBrush);
                    }
                    SafeRelease(&pRT);
                }
                SafeRelease(&pD2DFactory);
            }
            break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_SIZE:
        WndProc(hWnd, WM_COMMAND, mode, lParam);   
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    //case WM_PAINT:
    //    UpdateWindow(hWnd);
    //    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}