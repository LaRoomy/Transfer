# Transfer
Transfer is a simple Win32 Desktop Application to enable serial data transmission with older EPL2 based machines on newer Windows systems. Supported OS-Versions are XP/7/10/11. This project includes the C++ source for the executable and NSI Scripts to generate an Installer with NSIS.

![Transfer App User Interface](Resources/Screenshot_b.png)

## How to build the executable:

- Download and Install [Visual Studio 2022 (Community)](https://visualstudio.microsoft.com/de/downloads/) and make sure to install the C++ Desktop Development Package
- Open Visual Studio
- Click on "Clone Repository" on the right side
- Copy the Repository path from GitHub.com and paste it
- Click on Clone button
- When cloning is finished, open the project
- Select the desired build configuration (Release/Debug and x86/x64) in the toolbar
- Click on the Build-Button.
- NOTE: When the executable is built to run on Windows XP, the **COMPILE_FOR_WIN_XP** preprocessor definition in the header-file *framework.h* must be uncommented, otherwise when built for Windows 7 or higher this definition **must not** be set(commented). When built in Release mode, the output location of the executables is set to the appropriate *($ProjectDirectory)/Installer/buildOutput* Directory (depending on the target platform), this location is the source location for the Installer generation.

## How to build the installer:

- Download and install the [Nullsoft Scriptable Install System (NSIS)](https://nsis.sourceforge.io/Download)
- The scripts are located in the Installer directory, the names identify the target platform
- Open NSIS and click on *Compile NSI scripts* 
- Open the appropriate script for the desired target platform x86/x64, the installer will be built automatically and the results are displayed in the textbox
- The default output location of the generated installer is *($ProjectDirectory)/Installer/installer_output/*
- Optional: To edit the scripts I would recommend to download [Visual Studio Code](https://code.visualstudio.com/) and when done, install the NSIS language support extension from the Extension Marketplace inside VS Code.

