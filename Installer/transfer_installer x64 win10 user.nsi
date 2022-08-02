; include_region--------------------------------------------------;
Unicode true
!include "LogicLib.nsh"
!include "MUI2.nsh"
!include "FileFunc.nsh"
!include "x64.nsh"
; end include_region----------------------------------------------;
; ----------------------------------------------------------------;
; ----------------------------------------------------------------;
; general_region--------------------------------------------------;
!define VERSION "1.1.0"

Name "Transfer ${VERSION}"
OutFile "installer_output\win10\Transfer x64 user ${VERSION}.exe"        ;MARK: x64

InstallDir "$LOCALAPPDATA\Transfer"                                      ;MARK: x64

VIProductVersion 1.3.8.0
VIAddVersionKey /LANG=0 "ProductName" "Transfer Installer x64"           ;MARK: x64
VIAddVersionKey /LANG=0 "Comments" "Windows 10"
VIAddVersionKey /LANG=0 "CompanyName" "LaroomySoft"
VIAddVersionKey /LANG=0 "FileDescription" "This file installs Transfer on your Computer"
VIAddVersionKey /LANG=0 "LegalCopyright" "(C) LaroomySoft"
VIAddVersionKey /LANG=0 "ProductVersion" "1.1.0"
VIAddVersionKey /LANG=0 "FileVersion" "1.1.0"

RequestExecutionLevel user

; end general_region----------------------------------------------;
; ----------------------------------------------------------------;
; ----------------------------------------------------------------;
; interface_configuration_region ---------------------------------;
BrandingText "LaroomySoft"

!define MUI_ICON "img\orange-install.ico"
!define MUI_UNICON "img\orange-uninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "img\headerImageLeft.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "img\headerImageLeft.bmp"
!define MUI_BGCOLOR 0xD2D2D2
!define MUI_ABORTWARNING

!define MUI_WELCOMEFINISHPAGE_BITMAP "img\welcomeImg.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "img\uninstallImg.bmp"    ;recommended size 164 x 314

; end interface_configuration_region -----------------------------;
; ----------------------------------------------------------------;
; ----------------------------------------------------------------;
; pages region ---------------------------------------------------;

; TODO: define the ui-strings for the pages and make it language-dependent !!


;!insertmacro ${GetTime} "" "L" $day $month $year $weekday $hours $minute $seconds

; Installer:
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "resources\license.rtf"
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller:
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
; end pages region -----------------------------------------------;
; ----------------------------------------------------------------;
; ----------------------------------------------------------------;
; language region ------------------------------------------------;

!insertmacro MUI_LANGUAGE "English"

; end language region --------------------------------------------;
; ----------------------------------------------------------------;
; ----------------------------------------------------------------;
; variable region ------------------------------------------------;

    Var day
    Var month
    Var year
    Var weekday
    Var hours
    Var minute
    Var seconds

; end variable region --------------------------------------------;


; INSTALLER SECTION **********************************************;
Section "Installer Section"

    ;select current user
    SetShellVarContext current

    ;SetOverwrite on (the default is 'on' so this is not necessary, but maybe set it to 'ifnewer' ?)
    SetOverwrite ifnewer

    ;Create files and directories
    SetOutPath "$INSTDIR"
    SetOutPath "$INSTDIR\bin"

    ;    Program files
    File "resources\Transfer.VisualElementsManifest.xml"
    File "buildOutput\out_x64\Transfer.exe"                             ; MARK x64
    
    ; NOTE: Write the dependency dll's direct in the installation directory
    ; ************************************************************************************** ;

    ;Visual Studio distributed dll's - Path: C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Redist\MSVC\14.24.28127\x64
    File "DLLs\Microsoft.VC143.CRT.x64\concrt140.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\msvcp140.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\msvcp140_1.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\msvcp140_2.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\msvcp140_codecvt_ids.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\msvcp140_atomic_wait.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\vccorlib140.dll"
    File "DLLs\Microsoft.VC143.CRT.x64\vcruntime140.dll"

    ;Other dependency files - not all are necessary but it's easier to copy the hole folder
    ;Path: C:\Program Files (x86)\Windows Kits\10\Redist\10.0.18362.0\ucrt\DLLs\...
    File "DLLs\x64\api-ms-win-core-console-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-console-l1-2-0.dll"
    File "DLLs\x64\api-ms-win-core-datetime-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-debug-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-errorhandling-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-fibers-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-file-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-file-l1-2-0.dll"
    File "DLLs\x64\api-ms-win-core-file-l2-1-0.dll"
    File "DLLs\x64\api-ms-win-core-handle-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-heap-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-interlocked-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-libraryloader-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-localization-l1-2-0.dll"
    File "DLLs\x64\api-ms-win-core-memory-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-namedpipe-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-processenvironment-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-processthreads-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-processthreads-l1-1-1.dll"
    File "DLLs\x64\api-ms-win-core-profile-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-rtlsupport-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-string-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-synch-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-synch-l1-2-0.dll"
    File "DLLs\x64\api-ms-win-core-sysinfo-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-timezone-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-core-util-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-conio-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-convert-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-environment-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-filesystem-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-heap-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-locale-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-math-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-multibyte-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-private-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-process-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-runtime-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-stdio-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-string-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-time-l1-1-0.dll"
    File "DLLs\x64\api-ms-win-crt-utility-l1-1-0.dll"
    File "DLLs\x64\ucrtbase.dll"

    ;Write the assets-folder
    SetOutPath "$INSTDIR\bin\assets"
    File "resources\logo_sq70.png"
    File "resources\logo_sq150.png"

    ;Write the image folder
    SetOutPath "$INSTDIR\image"
    File "img\transfer_img_small.ico"

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\bin\Uninstall.exe"

    ;Create startmenu-entry
    CreateShortCut "$STARTMENU\Transfer.lnk" "$INSTDIR\bin\Transfer.exe" ;NOTE: The $STARTMENU variable depends on the SetShellVarContext setting
  
    ;Create desktop shortcut
    CreateShortCut "$DESKTOP\Transfer.lnk" "$INSTDIR\bin\Transfer.exe"

    ;Write registry (Admin level required - only necessary for Admin Installation (deprecated))
    ;------------------------------------------------------------------------------------------->
    ${GetTime} "" "L" $day $month $year $weekday $hours $minute $seconds

    ${If} ${RunningX64}
       SetRegView 64
    ${EndIf}

    ;Write registry (Only for current user (user-installation))
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayName" "Transfer"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayIcon" "$INSTDIR\image\transfer_img_small.ico"   
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "UninstallString" "$INSTDIR\bin\Uninstall.exe"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayVersion" "${VERSION}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "Publisher" "Hans Philipp Zimmermann"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "URLInfoAbout" "https://epl2-datatransmission.blogspot.com/2020/08/epl2-datenubertragung.html"    
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "URLUpdateInfo" "https://epl2-datatransmission.blogspot.com/2020/08/epl2-datenubertragung.html"    
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "InstallLocation" "$INSTDIR"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "InstallDate" "$hours:$minute - $day/$month/$year"

    ;Set Application Registry entry (not necessary - only for use by installer and transfer.exe)
    WriteRegStr HKCU "SOFTWARE\Transfer\CurrentVersion" "" "${VERSION}"

    SetRegView 32

SectionEnd
; end INSTALLER SECTION ********************************************;
; ******************************************************************;
; UNINSTALLER SECTION **********************************************;
Section "un.Application" uninst_app

    ;select the current user
    SetShellVarContext current

    ;Delete files and folders
    ;----------------------------------------------------------------------->

    ; Remove image-folder (and content):
    RMDir /r /REBOOTOK "$LOCALAPPDATA\Transfer\image"
    ; Remove bin-folder (and content)
    RMDir /r /REBOOTOK "$LOCALAPPDATA\Transfer\bin"

    ; Remove the Installation-Directory (DO NOT USE /r !!! - See: https://nsis.sourceforge.io/Reference/RMDir for more info)
    ;RMDir "$INSTDIR" ; this does not work - why?!
    RMDir "$LOCALAPPDATA\Transfer"

    ;Delete startmenu-entry
    Delete "$STARTMENU\Transfer.lnk"

    ;Delete desktop-shortcut
    Delete "$DESKTOP\Transfer.lnk"

    ;Delete the app data
    ;For Win-XP this path is hardcoded ("C:\Transfer 121")
    ;For all other windows-versions, this is the documents-user folder:
    Delete "$DOCUMENTS\Transfer 121\comset.dat"
    Delete "$DOCUMENTS\Transfer 121\temp_content.dat"
    RMDir  "$DOCUMENTS\Transfer 121"

    ;Remove Registry Entries
    ;------------------------------------------------------------------------->
    ${If} ${RunningX64}
       SetRegView 64
    ${EndIf}

    ;Delete uninstaller key
    DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer"
    DeleteRegKey HKCU "SOFTWARE\Transfer"

    SetRegView 32
SectionEnd

; end UNINSTALLER SECTION *************************************************************************************;