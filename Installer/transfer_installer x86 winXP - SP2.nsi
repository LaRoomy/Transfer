; include_region--------------------------------------------------;
Unicode true
!include "LogicLib.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
; end include_region----------------------------------------------;
; ----------------------------------------------------------------;
; ----------------------------------------------------------------;
; general_region--------------------------------------------------;
!define VERSION "1.1.0"

Name "Transfer ${VERSION}"

; change location for different types of target-operating system
OutFile "installer_output\winXP_sp2\Transfer x86 XP_SP2 ${VERSION}.exe"              ;MARK: x86/x64

InstallDir "C:\Transfer"                                    ;MARK: x86/x64 ($PROGRAMFILES64 for x64)

RequestExecutionLevel user

VIProductVersion 1.0.3.0
VIAddVersionKey /LANG=0 "ProductName" "Transfer for XP Installer"
VIAddVersionKey /LANG=0 "Comments" "Windows XP"
VIAddVersionKey /LANG=0 "CompanyName" "LaroomySoft"
VIAddVersionKey /LANG=0 "FileDescription" "This file installs Transfer for XP on your Computer"
VIAddVersionKey /LANG=0 "LegalCopyright" "(C) LaroomySoft"
VIAddVersionKey /LANG=0 "ProductVersion" "1.1.0"
VIAddVersionKey /LANG=0 "FileVersion" "1.1.0"

;Var finishbuttontext

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

; Installer:
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "resources\license.rtf"
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller:
!insertmacro MUI_UNPAGE_WELCOME
;!insertmacro MUI_UNPAGE_CONFIRM
;!insertmacro MUI_UNPAGE_COMPONENTS     ;let the user select if he wants to delete the created userfolder (appdata + projects)
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

    ;no variables yet!

; end variable region --------------------------------------------;

; INSTALLER SECTION **********************************************;
Section "Installer Section"

    ;select current user
    SetShellVarContext current

    ;SetOverwrite on (the default is 'on' so this is not necessary, but maybe set it to 'ifnewer' ?)
    SetOverwrite ifnewer

    ;Create files and directories
    SetOutPath "$INSTDIR"

    File "resources\winXP_sp2\dll\api-ms-win-core-console-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-console-l1-2-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-datetime-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-debug-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-errorhandling-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-fibers-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-file-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-file-l1-2-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-file-l2-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-handle-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-heap-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-interlocked-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-libraryloader-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-localization-l1-2-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-memory-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-namedpipe-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-processenvironment-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-processthreads-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-processthreads-l1-1-1.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-profile-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-rtlsupport-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-string-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-synch-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-synch-l1-2-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-sysinfo-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-timezone-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-core-util-l1-1-0.dll"
    File "resources\winXP_sp2\dll\API-MS-Win-core-xstate-l2-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-conio-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-convert-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-environment-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-filesystem-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-heap-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-locale-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-math-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-multibyte-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-private-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-process-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-runtime-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-stdio-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-string-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-time-l1-1-0.dll"
    File "resources\winXP_sp2\dll\api-ms-win-crt-utility-l1-1-0.dll"
    File "resources\winXP_sp2\dll\ucrtbase.dll"
    File "resources\winXP_sp2\dll\concrt140.dll"
    File "resources\winXP_sp2\dll\msvcp140.dll"
    File "resources\winXP_sp2\dll\msvcp140_1.dll"
    File "resources\winXP_sp2\dll\msvcp140_2.dll"
    File "resources\winXP_sp2\dll\msvcp140_codecvt_ids.dll"
    File "resources\winXP_sp2\dll\msvcp140_atomic_wait.dll"
    File "resources\winXP_sp2\dll\vccorlib140.dll"
    File "resources\winXP_sp2\dll\vcruntime140.dll"

    File "buildOutput\x86\Transfer.exe"                            ;MARK: x86/x64

    SetOutPath "$INSTDIR\img"  ;create folder

    File "img\transfer_img_small.ico"

    ; ----------------------------------------

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ;Create startmenu-entry
    CreateShortCut "$SMPROGRAMS\Transfer.lnk" "$INSTDIR\Transfer.exe"

    ;Create desktop shortcut
    CreateShortCut "$DESKTOP\Transfer.lnk" "$INSTDIR\Transfer.exe"

    ;Write registry
    ;------------------------------------------------------------------------------------------->
    ${If} ${RunningX64}
       SetRegView 64
    ${EndIf}

    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayName" "Transfer"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayIcon" "$INSTDIR\img\transfer_img_small.ico"   
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayVersion" "${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "Publisher" "Hans Philipp Zimmermann"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "URLInfoAbout" "https://epl2-datatransmission.blogspot.com/2020/08/epl2-datenubertragung.html"    
    
    ;Set Application Registry entry
    WriteRegStr HKLM "SOFTWARE\Transfer\CurrentVersion" "" "1.1.0"

    SetRegView 32

SectionEnd
; end INSTALLER SECTION ********************************************;
; ******************************************************************;
; UNINSTALLER SECTION **********************************************;
Section "un.Application" uninst_app

    ;Delete files and folders
    ;----------------------------------------------------------------------->
    Delete "$INSTDIR\img\transfer_img_small.ico"

    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\Transfer.exe"

    Delete "$INSTDIR\api-ms-win-core-console-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-console-l1-2-0.dll"
    Delete "$INSTDIR\api-ms-win-core-datetime-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-debug-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-errorhandling-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-fibers-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-file-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-file-l1-2-0.dll"
    Delete "$INSTDIR\api-ms-win-core-file-l2-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-handle-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-heap-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-interlocked-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-libraryloader-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-localization-l1-2-0.dll"
    Delete "$INSTDIR\api-ms-win-core-memory-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-namedpipe-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-processenvironment-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-processthreads-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-processthreads-l1-1-1.dll"
    Delete "$INSTDIR\api-ms-win-core-profile-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-rtlsupport-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-string-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-synch-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-synch-l1-2-0.dll"
    Delete "$INSTDIR\api-ms-win-core-sysinfo-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-timezone-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-core-util-l1-1-0.dll"
    Delete "$INSTDIR\API-MS-Win-core-xstate-l2-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-conio-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-convert-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-environment-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-filesystem-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-heap-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-locale-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-math-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-multibyte-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-private-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-process-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-runtime-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-stdio-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-string-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-time-l1-1-0.dll"
    Delete "$INSTDIR\api-ms-win-crt-utility-l1-1-0.dll"
    Delete "$INSTDIR\ucrtbase.dll"
    Delete "$INSTDIR\concrt140.dll"
    Delete "$INSTDIR\msvcp140.dll"
    Delete "$INSTDIR\msvcp140_1.dll"
    Delete "$INSTDIR\msvcp140_2.dll"
    Delete "$INSTDIR\msvcp140_codecvt_ids.dll"
    Delete "$INSTDIR\msvcp140_atomic_wait.dll"
    Delete "$INSTDIR\vccorlib140.dll"
    Delete "$INSTDIR\vcruntime140.dll"

    RMDir "$INSTDIR\img"

    RMDir "$INSTDIR"

    ;Delete startmenu-entries
    Delete "$SMPROGRAMS\Transfer.lnk"

    ;Delete desktop-shortcut
    Delete "$DESKTOP\Transfer.lnk"

    ;Delete settings-folder and content

    ;For Win-XP this path is hardcoded:
    Delete "C:\Transfer 121\comset.dat"
    Delete "C:\Transfer 121\temp_content.dat"
    RMDir  "C:\Transfer 121"

    ;For all other windows-versions, this is the documents-user folder:
    ;Delete "$DOCUMENTS\Transfer 121\comset.dat"
    ;Delete "$DOCUMENTS\Transfer 121\temp_content.dat"
    ;RMDir  "$DOCUMENTS\Transfer 121"

    ;Remove Registry Entries
    ;------------------------------------------------------------------------->
    ${If} ${RunningX64}
       SetRegView 64
    ${EndIf}

    ;Delete uninstaller key
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer"
    DeleteRegKey HKLM "SOFTWARE\Transfer\CurrentVersion"

    SetRegView 32
SectionEnd
; end UNINSTALLER SECTION *************************************************************************************;