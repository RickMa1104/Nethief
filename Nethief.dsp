# Microsoft Developer Studio Project File - Name="Nethief" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Nethief - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Nethief.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Nethief.mak" CFG="Nethief - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Nethief - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Nethief - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Nethief - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Nethief - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Nethief - Win32 Release"
# Name "Nethief - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Community.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EasterEggDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpUpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IPToLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Md5.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\Nethief.cpp
# End Source File
# Begin Source File

SOURCE=.\Nethief.rc
# End Source File
# Begin Source File

SOURCE=.\NethiefDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RemarkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectFileOperationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectIPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TipOfTheDayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarBuildServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarFindFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarFindFileResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarPropertyDiskdriveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarPropertyMultiDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarPropertySingleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarRunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarRunRemoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage1.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage2.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage3.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage31.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage4.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage5.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPage6.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\Community.h
# End Source File
# Begin Source File

SOURCE=.\DownloadDlg.h
# End Source File
# Begin Source File

SOURCE=.\EasterEggDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileSocket.h
# End Source File
# Begin Source File

SOURCE=.\HelpUpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\IPToLocal.h
# End Source File
# Begin Source File

SOURCE=.\LinkStatic.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Md5.h
# End Source File
# Begin Source File

SOURCE=.\MyFormView.h
# End Source File
# Begin Source File

SOURCE=.\MyListView.h
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\MyTreeView.h
# End Source File
# Begin Source File

SOURCE=.\Nethief.h
# End Source File
# Begin Source File

SOURCE=.\NethiefDoc.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\RemarkDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectFileOperationDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectIPDlg.h
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Public\STRUCTURE_DEFINE.h
# End Source File
# Begin Source File

SOURCE=.\TipOfTheDayDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarBuildServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarFindFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarFindFileResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarPropertyDiskdriveDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarPropertyMultiDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarPropertySingleDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarRunDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarRunRemoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage1.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage2.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage3.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage31.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage4.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage5.h
# End Source File
# Begin Source File

SOURCE=.\WizardPage6.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\adapter.ico
# End Source File
# Begin Source File

SOURCE=.\res\busy.ico
# End Source File
# Begin Source File

SOURCE=.\res\ConsoleToolBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ConsoleToolBar16M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ConsoleToolBarHot16M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cutline.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cutline1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drive_cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\res\drive_fixed.ico
# End Source File
# Begin Source File

SOURCE=.\res\drive_remote.ico
# End Source File
# Begin Source File

SOURCE=.\res\drive_removable.ico
# End Source File
# Begin Source File

SOURCE=.\res\drive_zip.ico
# End Source File
# Begin Source File

SOURCE=.\res\export.ico
# End Source File
# Begin Source File

SOURCE=.\res\filemanager.ico
# End Source File
# Begin Source File

SOURCE=.\res\filename.ico
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBar16M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarHot16M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FindFile.ico
# End Source File
# Begin Source File

SOURCE=.\res\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\folder_open.ico
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\keyname.ico
# End Source File
# Begin Source File

SOURCE=.\res\large_drive_cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\res\large_drive_fixed.ico
# End Source File
# Begin Source File

SOURCE=.\res\large_drive_remote.ico
# End Source File
# Begin Source File

SOURCE=.\res\large_drive_removable.ico
# End Source File
# Begin Source File

SOURCE=.\res\large_drive_zip.ico
# End Source File
# Begin Source File

SOURCE=.\res\Light.ico
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mode.ico
# End Source File
# Begin Source File

SOURCE=.\res\mycomputer.ico
# End Source File
# Begin Source File

SOURCE=.\res\Nethief.ico
# End Source File
# Begin Source File

SOURCE=.\res\Nethief.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NetToolBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NetToolBar16M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NetToolBarHot16M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\note.ico
# End Source File
# Begin Source File

SOURCE=.\res\offline.ico
# End Source File
# Begin Source File

SOURCE=.\res\Parameter.ico
# End Source File
# Begin Source File

SOURCE=.\res\Program.ico
# End Source File
# Begin Source File

SOURCE=.\res\property.ico
# End Source File
# Begin Source File

SOURCE=.\res\remotehost.ico
# End Source File
# Begin Source File

SOURCE=.\res\remotehost_disconnect.ico
# End Source File
# Begin Source File

SOURCE=.\res\remotehost_httptunnel.ico
# End Source File
# Begin Source File

SOURCE=.\res\remotehost_httptunnel_disconnect.ico
# End Source File
# Begin Source File

SOURCE=.\res\selectfileoperation.ico
# End Source File
# Begin Source File

SOURCE=.\res\selectip.ico
# End Source File
# Begin Source File

SOURCE=.\res\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\systray.ico
# End Source File
# Begin Source File

SOURCE=.\res\systray1.ico
# End Source File
# Begin Source File

SOURCE=.\res\unknownfile.ico
# End Source File
# Begin Source File

SOURCE=.\res\url.ico
# End Source File
# Begin Source File

SOURCE=.\res\view.ico
# End Source File
# Begin Source File

SOURCE=.\res\Wizard.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\global.wav
# End Source File
# Begin Source File

SOURCE=.\res\online.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\system.wav
# End Source File
# End Target
# End Project
