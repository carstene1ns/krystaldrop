; Turn off old selected section
; 10 02 2003: Tom van Gemert
; Template voor het genereren van een installer.
; speciaal voor het genereren van EasyPlayer installers.
; Trimedia Interactive Projects


; -------------------------------
; Start


  !define MUI_PRODUCT "Krystal Drop"
  !define MUI_VERSION "0.7"
  !define MUI_FILE "KrystalDrop"
  !define MUI_BRANDINGTEXT "Krystal Drop"
  CRCCheck On

  ; Bij deze moeten we waarschijnlijk een absoluut pad gaan gebruiken
  ; dit moet effe uitgetest worden.
  !include "${NSISDIR}\Contrib\Modern UI\System.nsh"


;--------------------------------
;General

  OutFile "krystal_drop_0.7.exe"
  ShowInstDetails "nevershow"
  ShowUninstDetails "nevershow"
  ;SetCompressor "bzip2"
 
  !define MUI_ICON "..\..\art\kdropinstall.ico"
  !define MUI_UNICON "..\..\art\kdropinstall.ico"
;  !define MUI_SPECIALBITMAP "Bitmap.bmp"


;--------------------------------
;Folder selection page

  InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"


;--------------------------------
;Modern UI Configuration

  !define MUI_WELCOMEPAGE  
  !define MUI_LICENSEPAGE
  !define MUI_DIRECTORYPAGE
  !define MUI_ABORTWARNING
  !define MUI_UNINSTALLER
  !define MUI_UNCONFIRMPAGE
  !define MUI_FINISHPAGE  


;--------------------------------
;Language

  !insertmacro MUI_LANGUAGE "English"

  
  
;--------------------------------
;Data

  LicenseData "..\..\COPYING"


;-------------------------------- 
;Installer Sections     
Section "install" Installation info

;Add files
  SetOutPath "$INSTDIR"
  
  file "..\..\COPYING"
  file "..\..\README"
  file "..\..\KD++.dll"
  file "..\..\SDL.dll"
  file "..\..\jpeg.dll"
  file "..\..\libpng1.dll"
  file "..\..\SDL_ttf.dll"
  file "..\..\SDL_mixer.dll"
  file "..\..\smpeg.dll"
  file "..\..\libxml2.dll"
  file "..\..\vorbisfile.dll"
  file "..\..\ogg.dll"
  file "..\..\SDL_gfx.dll"
  file "..\..\sdl_sound.dll"
  file "..\..\zlib.dll"
  file "..\..\SDL_image.dll"
  file "..\..\msvcr70.dll"
  file "..\..\msvcp70.dll"
  file "..\..\iconv.dll"
  file "..\..\${MUI_FILE}.exe"
  file "..\..\kdrop.xml"
  SetOutPath "$INSTDIR\art"
  file "..\..\art\table.txt"
  file "..\..\art\tableDuel.txt"
  file "..\..\art\survival.sco"
  file "..\..\art\Slapstick.txt"
  file "..\..\art\Slapstick.png"
  file "..\..\art\kdrop.ico"
  file "..\..\art\*.ogg"
  SetOutPath "$INSTDIR\art\UI"
  file /r "..\..\art\UI\*.*"
  SetOutPath "$INSTDIR\art\sound"
  file "..\..\art\sound\sound.txt"
  file "..\..\art\sound\*.wav"
  SetOutPath "$INSTDIR\art\title"
  file "..\..\art\title\*.*"
  SetOutPath "$INSTDIR\art\star"
  file "..\..\art\star\*.*"
  SetOutPath "$INSTDIR\art\line"
  file "..\..\art\line\*.*"
  SetOutPath "$INSTDIR\art\cup"
  file "..\..\art\cup\*.*"
  SetOutPath "$INSTDIR\art\menu"
  file "..\..\art\menu\*.*"
  SetOutPath "$INSTDIR\art\gems"
  file "..\..\art\gems\gems.txt"
  file "..\..\art\gems\nb*"
  file "..\..\art\gems\ng*"
  file "..\..\art\gems\nr*"
  file "..\..\art\gems\ny*"
  SetOutPath "$INSTDIR\art\survival"
  file "..\..\art\survival\*"
  SetOutPath "$INSTDIR\art\characters"
  file /r "..\..\art\characters\*"
  SetOutPath "$INSTDIR"

;create desktop shortcut
  CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" "" "$INSTDIR\art\kdrop.ico" 0
 
;create start-menu items
  CreateDirectory "$SMPROGRAMS\${MUI_PRODUCT}"
  CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" "" "$INSTDIR\art\kdrop.ico" 0
 
;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
 
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd


;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
   
;Delete Files
  Delete "$INSTDIR\BUGLIST"
  Delete "$INSTDIR\COPYING"
  Delete "$INSTDIR\README"
  Delete "$INSTDIR\KD++.dll"
  Delete "$INSTDIR\SDL.dll"
  Delete "$INSTDIR\jpeg.dll"
  Delete "$INSTDIR\libpng1.dll"
  Delete "$INSTDIR\SDL_ttf.dll"
  Delete "$INSTDIR\SDL_mixer.dll"
  Delete "$INSTDIR\smpeg.dll"
  Delete "$INSTDIR\libxml2.dll"
  Delete "$INSTDIR\vorbisfile.dll"
  Delete "$INSTDIR\ogg.dll"
  Delete "$INSTDIR\SDL_gfx.dll"
  Delete "$INSTDIR\sdl_sound.dll"
  Delete "$INSTDIR\zlib.dll"
  Delete "$INSTDIR\SDL_image.dll"
  Delete "$INSTDIR\msvcr70.dll"
  Delete "$INSTDIR\msvcp70.dll"
  Delete "$INSTDIR\${MUI_FILE}.exe"
  Delete "$INSTDIR\kdrop.xml"

  RMDir /r "$INSTDIR\art\*.*"
  RMDir "$INSTDIR\art"
 
;Remove the installation directory
  RMDir "$INSTDIR"
  
;Delete Start Menu Shortcuts
  Delete "$DESKTOP\${MUI_PRODUCT}.lnk"
  Delete "$SMPROGRAMS\${MUI_PRODUCT}\*.*"
  RmDir  "$SMPROGRAMS\${MUI_PRODUCT}"
  
;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${MUI_PRODUCT}"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"  
  
SectionEnd


;--------------------------------    
;MessageBox Section


;Function that calls a messagebox when installation finished correctly
Function .onInstSuccess
  MessageBox MB_OK "You have successfully installed ${MUI_PRODUCT}. Use the desktop icon to start the program."
FunctionEnd


Function un.onUninstSuccess
  MessageBox MB_OK "You have successfully uninstalled ${MUI_PRODUCT}."
FunctionEnd


;eof
