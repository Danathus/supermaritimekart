; Script generated by the HM NIS Edit Script Wizard.
; ...and subsequently edited by Eric R. Heine

; Uses environment variables to find external files
; DELTA_ROOT           - root of the delta3d folder

!define /date NOW "%m-%d-%Y_%H%M%S"

!ifndef VERSION
  !define VERSION ${NOW}
!endif

!ifdef OUTFILE
  OutFile "${OUTFILE}"
!else
  OutFile DamageVisualizer--${VERSION}--setup.exe
!endif

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME             "Damage Visualizer"
!define PRODUCT_VERSION          ${VERSION}
!define PRODUCT_PUBLISHER        "MOVES"
!define PRODUCT_WEB_SITE         "http://delta3d.org"
!define PRODUCT_DIR_REGKEY       "Software\DamageVisualizer"
!define PRODUCT_UNINST_KEY       "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY  "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON   "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; paths to assets and binaries for Damage Visualizer App
!define DELTA_ASSET_PATH        "$%DELTA_ROOT%\data"
!define APPLICATION_ASSET_PATH  "..\data"
!define APPLICATION_UI_PATH     "..\ui"
!define BIN_PATH                "..\build\Release"

; Welcome page
!insertmacro MUI_PAGE_WELCOME

; License page
;!insertmacro MUI_PAGE_LICENSE "c:\path\to\licence\YourSoftwareLicence.txt"

; Components selection page
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!insertmacro MUI_PAGE_DIRECTORY

; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Damage Visualizer"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

!define USE_CUSTOM_OSG false

; Dependencies
!define XERCES "xerces-c_3_1.dll"
!define QTCORE4 "QtCore4.dll"
!define ODE_SINGLE "ode_single.dll"
!define QTGUI4 "QtGui4.dll"
!define QTOPENGL4 "QtOpenGL4.dll"
!define OSG "osg65-osg.dll"
!define OSGDB "osg65-osgDB.dll"
!define OSGGA "osg65-osgGA.dll"
!define OSGPARTICLE "osg65-osgParticle.dll"
!define OSGTEXT "osg65-osgText.dll"
!define OSGUTIL "osg65-osgUtil.dll"
!define OSGVIEWER "osg65-osgViewer.dll"
!define OPENTHREADS  "ot11-OpenThreads.dll"
!define OSGDB_FREETYPE "osgdb_freetype.dll"
!define OSGDB_JPEG "osgdb_jpeg.dll"
!define DTABC "dtABC.dll"
!define DTCORE "dtCore.dll"
!define DTDAL "dtDAL.dll"
!define DTQT "dtQt.dll"
!define DTUTIL "dtUtil.dll"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
InstallDir "c:\Training\Damage Visualizer"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show


Section "-Delta3D Dependencies" DeltaDepsSection
   SetOverwrite ifnewer
   SetOutPath "$INSTDIR\bin"

   ; Delta3D dependencies
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${ODE_SINGLE}"
   File "$%DELTA_ROOT%\ext\bin\${XERCES}"

   ; try getting Qt from QTDIR
   File /nonfatal "$%QTDIR%\bin\${QTCORE4}"
   File /nonfatal "$%QTDIR%\bin\${QTGUI4}"
   File /nonfatal "$%QTDIR%\bin\${QTOPENGL4}"
   
   SetOutPath "$INSTDIR\bin"

!if USE_CUSTOM_OSG == true
   ; try an OSG_DIR-based osg installation first
   File /nonfatal "$%OSG_DIR%\build\bin\${OSG}"
   File /nonfatal "$%OSG_DIR%\build\bin\${OSGDB}"
   File /nonfatal "$%OSG_DIR%\build\bin\${OSGGA}"
   File /nonfatal "$%OSG_DIR%\build\bin\${OSGPARTICLE}"
   File /nonfatal "$%OSG_DIR%\build\bin\${OSGTEXT}"
   File /nonfatal "$%OSG_DIR%\build\bin\${OSGUTIL"
   File /nonfatal "$%OSG_DIR%\build\bin\${OSGVIEWER}"
   File /nonfatal "$%OSG_DIR%\build\bin\${OPENTHREADS}"
   File /nonfatal "$%OSG_DIR%\build\bin\osgPlugins-2.8.1\${OSGDB_FREETYPE}"
   File /nonfatal "$%OSG_DIR%\build\bin\osgPlugins-2.8.1\${OSGDB_JPEG}"

   ; try an OSG_ROOT-based osg installation next
   File /nonfatal "$%OSG_ROOT%\bin\${OSG}"
   File /nonfatal "$%OSG_ROOT%\bin\${OSGDB}"
   File /nonfatal "$%OSG_ROOT%\bin\${OSGGA}"
   File /nonfatal "$%OSG_ROOT%\bin\${OSGPARTICLE}"
   File /nonfatal "$%OSG_ROOT%\bin\${OSGTEXT}"
   File /nonfatal "$%OSG_ROOT%\bin\${OSGUTIL}"
   File /nonfatal "$%OSG_ROOT%\bin\${OSGVIEWER}"
   File /nonfatal "$%OSG_ROOT%\bin\${OPENTHREADS}"
   File /nonfatal "$%OSG_ROOT%\bin\osgPlugins-2.8.1\${OSGDB_FREETYPE}"
   File /nonfatal "$%OSG_ROOT%\bin\osgPlugins-2.8.1\${OSGDB_JPEG}"
!else
   ; try an DELTA_ROOT-based osg installation next
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSG}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGDB}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGGA}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGPARTICLE}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGTEXT}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGUTIL}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGVIEWER}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OPENTHREADS}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGDB_FREETYPE}"
   File /nonfatal "$%DELTA_ROOT%\ext\bin\${OSGDB_JPEG}"
!endif

SectionEnd

Section "-Delta3D Engine" DeltaSection
   SetOverwrite ifnewer

   ; non-debug Delta3D dll's
   SetOutPath "$INSTDIR\bin"
     File "$%DELTA_ROOT%\build\bin\${DTABC}"
     File "$%DELTA_ROOT%\build\bin\${DTCORE}"
     File "$%DELTA_ROOT%\build\bin\${DTDAL}"
     File "$%DELTA_ROOT%\build\bin\${DTQT}"
     File "$%DELTA_ROOT%\build\bin\${DTUTIL}"
SectionEnd

Section "-DamageVisualizer Application" AppSection
   SetOverwrite ifnewer
   SetOutPath "$INSTDIR"

   ; batch files
   File "Damage Visualizer.bat"

   ; DamageVisualizer application binaries
   SetOutPath "$INSTDIR\bin"
	 File "${BIN_PATH}\DEVOApp.exe"

SectionEnd

Section "-DamageVisualizer Data" DataSection
   SetOverwrite ifnewer

   SetOutPath "$INSTDIR"
     File "${APPLICATION_ASSET_PATH}\object.xsd"

   ; assets for DamageVisualizer application
   SetOutPath "$INSTDIR\bin"
     File "${APPLICATION_ASSET_PATH}\config.xml"
     
   SetOutPath "$INSTDIR\data\images"
   
   SetOutPath "$INSTDIR\data\objects"
     File "${APPLICATION_ASSET_PATH}\objects\example.xml"
     File "${APPLICATION_ASSET_PATH}\objects\JHSV Structure.xml"
     

SectionEnd

Section "Shortcuts" ShortcutsSection
  SetOverwrite ifnewer

  ; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath "$INSTDIR"
      CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
      CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Damage Visualizer.lnk" "$INSTDIR\Damage Visualizer.bat"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section /o "Install VCRedist Package" VCRedistSection
   SetOutPath $INSTDIR
   File "vcredist_x86.exe"
   ExecWait "$INSTDIR\vcredist_x86.exe"
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" $INSTDIR
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\DEVOApp.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
   !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
   Delete "$INSTDIR\bin\*.*"
   RMDIR /r "$INSTDIR\bin"

   Delete "$INSTDIR\data\*.*"
   RMDIR /r $INSTDIR\data"

   Delete "$INSTDIR\*.*"
   RMDIR /r $INSTDIR

   Delete "$SMPROGRAMS\$ICONS_GROUP\*.*"
   RMDir /r "$SMPROGRAMS\$ICONS_GROUP"

   DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
   DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
   SetAutoClose true
SectionEnd