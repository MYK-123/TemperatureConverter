// TemperatureConverter.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "resource.h"

#include <string>
#include <CommCtrl.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hFrom, hComboFrom, hTo, hComboTo;

#define ITEM_CLECIUS     0
#define ITEM_Fahrenheit  1
#define ITEM_Kelvin      2

#define IDC_From 32
#define IDC_To 33
#define IDC_COMBOFROM 34
#define IDC_COMBOTO 35
#define ID_CONVERT 36
#define ID_CLEAR 37


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

float CelciusToKelvin(float c) {
    return c + 273.15f;
}
float KelvinToCelcius(float k) {
    return k - 273.15f;
}

float CelciusToFahrenheit(float c) {
    return 1.8 * c + 32; // 1.8 = 9/5
}

float FahrenheitToCelcius(float f) {
    return (f - 32) * 0.55555556f; // 0.5555555 = 5/9
}

float FahrenheitToKelvin(float f) {
    return CelciusToKelvin(FahrenheitToCelcius(f));
}

float KelvinToFahrenheit(float k) {
    return CelciusToFahrenheit(KelvinToCelcius(k));
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEMPERATURECONVERTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEMPERATURECONVERTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      100, 100, 320, 160, nullptr, nullptr, hInstance, nullptr);

   CreateWindowEx(WS_EX_RIGHT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_STATIC, L"From : ", SS_RIGHT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       23, 20, 52, 20, hWnd, nullptr, hInstance, nullptr);
   
   hFrom = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_RIGHT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_EDIT, L"", ES_AUTOHSCROLL | WS_CHILD | ES_RIGHT | WS_OVERLAPPED | WS_VISIBLE,
       83, 20, 72, 23, hWnd, (HMENU) IDC_From, hInstance, nullptr);

   hComboFrom = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       160, 20, 100, 170, hWnd, (HMENU)IDC_COMBOFROM, hInstance, nullptr);

   CreateWindowEx(WS_EX_RIGHT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_STATIC, L"To : ", SS_RIGHT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       23, 46, 52, 20, hWnd, nullptr, hInstance, nullptr);
   
   hTo = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_RIGHT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_EDIT, L"", ES_AUTOHSCROLL | WS_CHILD | ES_RIGHT | WS_OVERLAPPED | WS_VISIBLE,
       83, 46, 72, 23, hWnd, (HMENU) IDC_To, hInstance, nullptr);

   hComboTo = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       160, 46, 100, 170, hWnd, (HMENU)IDC_COMBOTO, hInstance, nullptr);

   CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_BUTTON, L"Convert", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       77, 81, 70, 30, hWnd, (HMENU)ID_CONVERT, hInstance, nullptr);

   CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
       WC_BUTTON, L"Clear", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       159, 81, 70, 30, hWnd, (HMENU)ID_CLEAR, hInstance, nullptr);

   TCHAR items[3][12] = {
       L"Celcius", L"Fahrenheit", L"Kelvin"
   };

   for (int i = 0; i < 3; i++)
   {
       TCHAR* A = items[i];
       SendMessage(hComboFrom, CB_ADDSTRING, 0, (LPARAM)A);
       SendMessage(hComboTo, CB_ADDSTRING, 0, (LPARAM)A);
   }
   SendMessage(hComboFrom, CB_SETCURSEL, 0, 0);
   SendMessage(hComboTo, CB_SETCURSEL, 0, 0);

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
    case WM_CTLCOLORSTATIC:
        return (LRESULT)(HBRUSH)(COLOR_WINDOW + 1);
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case ID_CLEAR:
                SendMessage(hFrom, WM_SETTEXT, 0, (LPARAM)L"");
                SendMessage(hTo, WM_SETTEXT, 0, (LPARAM)L"");
                break;
            case ID_CONVERT:
            {
                int len1 = SendMessage(hFrom, WM_GETTEXTLENGTH, 0, 0) + 1;
                TCHAR* from = (TCHAR*)malloc(len1);
                SendMessage(hFrom, WM_GETTEXT, len1, (LPARAM)from);

                std::wstring strFrom;
                strFrom.append(from);

                if (strFrom.length() == 0)
                {
                    return 0;
                }
                

                float nFrom = std::stof(strFrom);

                int SelectionFrom = SendMessage(hComboFrom, CB_GETCURSEL, 0, 0);
                int SelectionTo = SendMessage(hComboTo, CB_GETCURSEL, 0, 0);
                
                float nTo = 0.0f;
                switch (SelectionFrom)
                {
                case ITEM_CLECIUS:
                    nTo = SelectionTo == ITEM_Fahrenheit ? CelciusToFahrenheit(nFrom) 
                        : (SelectionTo == ITEM_Kelvin ? CelciusToKelvin(nFrom) : nFrom);
                    break;
                case ITEM_Fahrenheit:
                    nTo = SelectionTo == ITEM_CLECIUS ? FahrenheitToCelcius(nFrom)
                        : (SelectionTo == ITEM_Kelvin ? FahrenheitToKelvin(nFrom) : nFrom);
                    break;
                case ITEM_Kelvin:
                    nTo = SelectionTo == ITEM_Fahrenheit ? KelvinToFahrenheit(nFrom)
                        : (SelectionTo == ITEM_CLECIUS ? KelvinToCelcius(nFrom) : nFrom);
                    break;

                default:
                    nTo = 0.0f;
                    break;
                }
                SendMessage(hTo, WM_SETTEXT, 0, (LPARAM) std::to_wstring(nTo).c_str());
            }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

