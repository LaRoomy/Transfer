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
!define REDISTRIBUTABLE_FILEPATH "redist_x86\vc_redist.x86.exe"     ;MARK x86/x64
!define REDISTRIBUTABLE_FILENAME "vc_redist.x86.exe"                ;MARK x86/x64

Name "Transfer ${VERSION}"

; change location for different types of target-operating system
OutFile "installer_output\winXP\Transfer x86 XP ${VERSION}.exe"              ;MARK: x86/x64

InstallDir "$PROGRAMFILES\Transfer"                                    ;MARK: x86/x64 ($PROGRAMFILES64 for x64)

RequestExecutionLevel admin

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

; TODO: define the ui-strings for the pages and make it language-dependent !!

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

;TODO's:
; - Write Registry, don't forget to write the version to hklm to check it in installer
;
; - Check for previous version and remove it or tell the user that it is already installed


; INSTALLER SECTION **********************************************;
Section "Installer Section"

    ;select current user
    SetShellVarContext current

    ;SetOverwrite on (the default is 'on' so this is not necessary, but maybe set it to 'ifnewer' ?)
    SetOverwrite ifnewer

    ;Create the temporary installer for the redistributable
    SetOutPath "$LOCALAPPDATA"
    File "${REDISTRIBUTABLE_FILEPATH}"
    
    ; -> execute the external installer
    ExecWait '"$LOCALAPPDATA\${REDISTRIBUTABLE_FILENAME}"  "/quiet" "/norestart"' ;"/quiet" "/install" "/silent" "/norestart" ;/install /passive /silent /norestart
    ;IfErrors redistributable_error

    ;Create files and directories
    SetOutPath "$INSTDIR"

    File "bin\winXP\Transfer.exe"                            ;MARK: x86/x64
    ;File "Transfer.VisualElementsManifest.xml"

    SetOutPath "$INSTDIR\img"  ;create folder

    File "img\transfer_img_small.ico"

    ; ----------------------------------------

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ;Create startmenu-entry
    CreateShortCut "$SMPROGRAMS\Transfer.lnk" "$INSTDIR\Transfer.exe"

    ; the following section is disabled because in this installer we don't want to create a folder in the start-menu
    ; old: CreateDirectory "$SMPROGRAMS\Transfer"
    ; old: CreateShortCut "$SMPROGRAMS\Transfer\Transfer.lnk" "$INSTDIR\Transfer.exe"

    ;Create desktop shortcut
    CreateShortCut "$DESKTOP\Transfer.lnk" "$INSTDIR\Transfer.exe"

    ;Write registry
    ;------------------------------------------------------------------------------------------->
    ;Register Uninstaller:
    ${If} ${RunningX64}
       SetRegView 64
    ${EndIf}

    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayName" "Transfer"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayIcon" "$INSTDIR\img\transfer_img_small.ico"   
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "DisplayVersion" "${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "Publisher" "LaroomySoft"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Transfer" "URLInfoAbout" "http://www.laroomy.de"    
    
    ;Set Application Registry entry
    WriteRegStr HKLM "SOFTWARE\Transfer\CurrentVersion" "" "1.1.0"

    SetRegView 32

    ;goto finalize

;redistributable_error:
    ;MessageBox MB_OK|MB_ICONSTOP "Error installing the required components for Transfer.$\nInstallation will be canceled."
    ;Delete "$LOCALAPPDATA\${REDISTRIBUTABLE_FILENAME}"
    ;Quit

;finalize:
    ;clean up
    Delete "$LOCALAPPDATA\${REDISTRIBUTABLE_FILENAME}"

SectionEnd
; end INSTALLER SECTION ********************************************;
; ******************************************************************;
; UNINSTALLER SECTION **********************************************;
Section "un.Application" uninst_app

    ;Delete files and folders
    ;----------------------------------------------------------------------->
    Delete "$INSTDIR\img\transfer_img_small.ico"

    RMDir "$INSTDIR\img"

    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\Transfer.exe"
    ;Delete "$INSTDIR\Transfer.VisualElementsManifest.xml"

    RMDir "$INSTDIR"

    ;Delete startmenu-entries
    Delete "$SMPROGRAMS\Transfer.lnk"
    ;Delete "$SMPROGRAMS\Transfer\Transfer.lnk"
    ;RMDir "$SMPROGRAMS\Transfer"

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