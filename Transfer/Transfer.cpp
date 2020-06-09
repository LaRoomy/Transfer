// Transfer_xp.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"
#include "Transfer.h"
#include "config.h"

#define MAX_LOADSTRING 100

// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Der Klassenname des Hauptfensters.
WCHAR* pAppWorkingPath = nullptr;               // Der Arbeitspfad der Anwendung
ConfigProperty* cProperty = nullptr;            // Hauptanwendungsklasse
CRITICAL_SECTION CriticalSection;               // Thread-Zugriffskontrolle für geteilte Resourcen

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                mRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, ConfigProperty*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    msg.wParam = ERROR_APPINITALIZATION_FAILURE;

    InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
    SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);

    cProperty = new ConfigProperty();
    if (cProperty != nullptr)
    {
        // Arbeitspfad erstellen
        if (SUCCEEDED(GenerateWorkingDir()))
        {
            // Globale Zeichenfolgen initialisieren
            LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
            LoadStringW(hInstance, IDC_TRANSFERXP, szWindowClass, MAX_LOADSTRING);
            mRegisterClass(hInstance);

            // Anwendungsinitialisierung ausführen:
            if (InitInstance(hInstance, nCmdShow, cProperty))
            {
                HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRANSFERXP));

                // Hauptnachrichtenschleife:
                while (GetMessage(&msg, nullptr, 0, 0))
                {
                    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
            }
        }
        cProperty->Release();
    }
    DeleteCriticalSection(&CriticalSection);

    return (int) msg.wParam;
}

//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM mRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = sizeof(LONG_PTR);
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRANSFERXP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRANSFERXP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, ConfigProperty* cp)
{
    auto cx = GetSystemMetrics(SM_CXSCREEN);
    auto cy = GetSystemMetrics(SM_CYSCREEN);

    auto width = cx / 2;
    if (width < 700)
        width = 700;
    auto height = cy / 2;
    if (height < 700)
        height = 700;

    hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

    HWND hWnd =
        CreateWindowW(
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            cx / 2 - width / 2,
            cy / 2 - height / 2,
            width, height,
            nullptr, nullptr, hInstance,
            reinterpret_cast<LPVOID>(cProperty)
        );

    if (!hWnd)
    {
        return FALSE;
    }
    else {
        if (SUCCEEDED(cp->CreateConfigWindow(hInstance, hWnd)))
        {
            if (SUCCEEDED(cp->CreateEditBox()))
            {
                if (SUCCEEDED(cp->CreateSubControls()))
                {
                    // ...
                }
                else
                    return FALSE;
            }
            else
                return FALSE;
        }
        else
            return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK: Verarbeitet Meldungen für das Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ConfigProperty* pProp = nullptr;
    if (message == WM_CREATE)
    {
        auto pcr = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pProp = reinterpret_cast<ConfigProperty*>(pcr->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pProp));

        return 1;
    }
    else
    {
        pProp = reinterpret_cast<ConfigProperty*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (pProp != nullptr)
        {
            switch (message)
            {
            case WM_COMMAND:
                return pProp->onMainWndCommand(hWnd, wParam, lParam);
            case WM_PAINT:
                return pProp->onMainWndPaint(hWnd);
            case WM_SIZE:
                return pProp->onResizeWindow(hWnd);
            case WM_CLOSE:
                return pProp->onClose();
            case WM_EXDESTROY:
                return pProp->onExternalDestroy(hWnd);
            case WM_DESTROY:
                return pProp->onDestroy();
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


HFONT CreateCFont(int fontHeight, int fontWeight, LPCTSTR fontFamily)
{
    return CreateFont(
        fontHeight,
        0, 0, 0,
        fontWeight,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FF_DONTCARE,
        fontFamily
    );
}

HRESULT GenerateWorkingDir()
{
    HRESULT hr;

    auto bfpo = CreateBasicFPO();

    hr =
        (bfpo != nullptr)
        ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        WCHAR* path = nullptr;

        hr =
            bfpo->GetKnownFolderPath(&path, FOLDERID_Documents) ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            hr =
                AppendStringToString(path, L"\\Transfer 121", &pAppWorkingPath) ? S_OK : E_FAIL;

            if (SUCCEEDED(hr))
            {
                auto result =
                    CreateDirectory(pAppWorkingPath, nullptr);

                if (!result)
                {
                    hr = (GetLastError() == ERROR_ALREADY_EXISTS) ? S_OK : E_FAIL;
                }
            }
        }
    }
    return hr;
}

LPCWSTR GetGlobalAppPath()
{
    return pAppWorkingPath;
}

LPCRITICAL_SECTION GetCriticalSection()
{
    return &CriticalSection;
}

LONG WINAPI lpTopLevelExceptionFilter(_EXCEPTION_POINTERS* exceptionInfo)
{
    if (exceptionInfo != nullptr)
    {
        if (exceptionInfo->ExceptionRecord != nullptr)
        {
            iString message(L"Unbehandelte Ausnahme aufgetreten:\n\nInformationen:");
            message += L"\nExceptionCode: ";
            message += iString::fromHex(exceptionInfo->ExceptionRecord->ExceptionCode);
            message += L"\nExceptionAddress:  ";
            message += iString::fromHex(reinterpret_cast<DWORD>(exceptionInfo->ExceptionRecord->ExceptionAddress));

            MessageBox(nullptr, message.GetData(), L"Unhandled Exception", MB_OK | MB_ICONERROR);
        }
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

