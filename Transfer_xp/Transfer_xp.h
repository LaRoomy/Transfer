#pragma once
#pragma comment(lib,"comctl32.lib")

#include<CommCtrl.h>

// Enable Visual Styles
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include"SafeRelease.h"

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if(!(b)){ OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif //( DEBUG ) || ( _DEBUG )
#endif

#include "resource.h"

#define		APPLICATION_CLASS_NAME		L"TRANSFERCLASS4876"

// create a font with default values (height, weight, fontname)
#define		_CREATE_FONT(h,w,f)			(HFONT)(CreateFont(((int)(h)), 0, 0, 0, ((int)(w)), FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, ((LPCTSTR)(f))))

#define		_DESTROYMAINWINDOW			(BOOL)SNDMSG((	\
														FindWindow(APPLICATION_CLASS_NAME,nullptr)),(UINT)WM_EXDESTROY,(WPARAM)0,(LPARAM)0)

#define		ERROR_APPINITALIZATION_FAILURE		208

typedef __int64 Int64;

#if defined(_WIN64)
typedef DWORD64 uintX;
typedef __int64 intX;
#else
typedef DWORD32 uintX;
typedef int intX;
#endif

constexpr auto APPLICATION_PRIMARY_FONT = L"Calibri";

HFONT CreateCFont(int fontHeight, int fontWeight, LPCTSTR fontFamily);
HRESULT GenerateWorkingDir();
LPCWSTR GetGlobalAppPath();
LPCRITICAL_SECTION GetCriticalSection();
LONG WINAPI lpTopLevelExceptionFilter(_EXCEPTION_POINTERS * exceptionInfo);



