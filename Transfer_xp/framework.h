// header.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen

//#pragma comment(lib,"Wininet.lib")
//#pragma comment(lib, "Comdlg32.lib")
//#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Setupapi.lib")
//

//#pragma comment( lib, "Kernel32.lib" )
//#pragma comment( lib, "User32.lib" )
#pragma comment(lib,"Comdlg32.lib")
#pragma comment( lib, "Shlwapi.lib")



// Windows-Headerdateien
#include <windows.h>
#include <WindowsX.h>

#include <commdlg.h>

#include <tchar.h>
#include <strsafe.h>
#include <Richedit.h>
//
#include<Shlwapi.h>
#include<shellapi.h>
#include<ShlObj.h>
#include<ShObjIdl.h>

#include <SetupAPI.h>


// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include "cObject.h"
#include "Transfer_xp.h"
#include "Colors.h"
#include "BasicFPO.h"
#include "HelperF.h"
