# Microsoft Developer Studio Project File - Name="Krystal Drop Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Krystal Drop Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Krystal Drop Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Krystal Drop Client.mak" CFG="Krystal Drop Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Krystal Drop Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Krystal Drop Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Krystal Drop Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Release"
# PROP Intermediate_Dir "..\..\bin\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sdl.lib sdlmain.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Krystal Drop Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\Debug"
# PROP Intermediate_Dir "..\..\bin\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sdl.lib sdlmain.lib SDL_image.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Krystal Drop Client - Win32 Release"
# Name "Krystal Drop Client - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Interf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\interf\Application.cpp
# End Source File
# Begin Source File

SOURCE=..\..\interf\Application.h
# End Source File
# Begin Source File

SOURCE=..\..\interf\Controller.cpp
# End Source File
# Begin Source File

SOURCE=..\..\interf\Controller.h
# End Source File
# Begin Source File

SOURCE=..\..\interf\DuelController.cpp
# End Source File
# Begin Source File

SOURCE=..\..\interf\DuelController.h
# End Source File
# Begin Source File

SOURCE=..\..\interf\StartController.cpp
# End Source File
# Begin Source File

SOURCE=..\..\interf\StartController.h
# End Source File
# Begin Source File

SOURCE=..\..\interf\SurvivalController.cpp
# End Source File
# Begin Source File

SOURCE=..\..\interf\SurvivalController.h
# End Source File
# Begin Source File

SOURCE=..\..\interf\TitleController.cpp
# End Source File
# Begin Source File

SOURCE=..\..\interf\TitleController.h
# End Source File
# End Group
# Begin Group "Video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\video\Display.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\Display.h
# End Source File
# Begin Source File

SOURCE=..\..\video\font.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\font.h
# End Source File
# Begin Source File

SOURCE=..\..\video\gem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\gem.h
# End Source File
# Begin Source File

SOURCE=..\..\video\image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\image.h
# End Source File
# Begin Source File

SOURCE=..\..\video\imagemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\imagemanager.h
# End Source File
# Begin Source File

SOURCE=..\..\video\SDL_rotozoom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\SDL_rotozoom.h
# End Source File
# Begin Source File

SOURCE=..\..\video\sprite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\sprite.h
# End Source File
# Begin Source File

SOURCE=..\..\video\spriteinstance.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\spriteinstance.h
# End Source File
# End Group
# Begin Group "game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\game\anim_row.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\anim_row.h
# End Source File
# Begin Source File

SOURCE=..\..\game\hand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\hand.h
# End Source File
# Begin Source File

SOURCE=..\..\game\memo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\memo.h
# End Source File
# Begin Source File

SOURCE=..\..\game\parameter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\parameter.h
# End Source File
# Begin Source File

SOURCE=..\..\game\row.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\row.h
# End Source File
# Begin Source File

SOURCE=..\..\game\rowmacros.h
# End Source File
# Begin Source File

SOURCE=..\..\game\set.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\set.h
# End Source File
# Begin Source File

SOURCE=..\..\game\table.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game\table.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\util\direct.cpp
# End Source File
# Begin Source File

SOURCE=..\..\util\direct.h
# End Source File
# Begin Source File

SOURCE=..\..\util\logfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\util\logfile.h
# End Source File
# Begin Source File

SOURCE=..\..\util\textfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\util\textfile.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Main.cpp
# End Source File
# End Target
# End Project
