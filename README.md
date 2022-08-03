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
- NOTE: When the executable is built to run on Windows XP, the **COMPILE_FOR_WIN_XP** preprocessor definition in the header-file *framework.h* must be uncommented, otherwise when built for Windows 7 or higher this definition **must not** be set(commented). When built in Release mode, the output location of the executables is set to the appropriate Installer/buildOutput Directory (depending on the target platform), this location is the source location for the Installer generation.

