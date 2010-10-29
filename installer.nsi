;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

!define /date NOW "%m-%d-%Y_%H%M%S"

!ifndef VERSION
  !define VERSION ${NOW}
!endif

!ifndef DELTA_BUILD_DIR
  !define DELTA_BUILD_DIR 'build'
!endif

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Water Sprint"
!define PRODUCT_VERSION ${VERSION}
!define PRODUCT_PUBLISHER "Delta3D"
!define PRODUCT_WEB_SITE "http://www.delta3d.org"
!define PRODUCT_DIR_REGKEY "Software\${PRODUCT_NAME}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

;--------------------------------
;General

  ;Name and file
  Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  OutFile "WaterSprintDemo_${PRODUCT_VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${PRODUCT_NAME}_${PRODUCT_VERSION}"

  ;Get installation folder from registry if available, this will overwrite InstallDir if key is set
  InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user



Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  ;!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  ;!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

    ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}_${PRODUCT_VERSION}"
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
 !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
 
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Main Section" SecMain

  ;Root folder
  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
  File "WaterSprint.exe"
  
  File /r /x *.svn "ProjectContext"
  
  File "config.xml"
  File "WaterSprint.bat"

  ;Schemas
  File "$%DELTA_DATA%\map.xsd"
  File "$%DELTA_DATA%\application.xsd"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

;Dependencies
Section "External Dependencies" SecExternals
  SetOutPath "$INSTDIR"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtABC.dll"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtActors.dll"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtAudio.dll"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtCore.dll"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtDAL.dll"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtGame.dll"
  File "$%DELTA_ROOT%\${DELTA_BUILD_DIR}\bin\dtUtil.dll"
  

  File "$%DELTA_ROOT%\ext\bin\alut.dll"
  File "$%DELTA_ROOT%\ext\bin\ode_single.dll"
  File "$%DELTA_ROOT%\ext\bin\OpenAL32.dll"
  File "$%DELTA_ROOT%\ext\bin\wrap_oal.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osg.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgdb.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgfx.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgga.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgparticle.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgsim.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgtext.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgutil.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgTerrain.dll"
  File "$%DELTA_ROOT%\ext\bin\osg65-osgviewer.dll"
  File "$%DELTA_ROOT%\ext\bin\osgdb_ive.dll"
  File "$%DELTA_ROOT%\ext\bin\osgdb_osg.dll"
  File "$%DELTA_ROOT%\ext\bin\osgdb_osgparticle.dll"
  File "$%DELTA_ROOT%\ext\bin\osgdb_png.dll"
  File "$%DELTA_ROOT%\ext\bin\osgdb_tga.dll"
  File "$%DELTA_ROOT%\ext\bin\libpng13.dll"
  File "$%DELTA_ROOT%\ext\bin\zlib1.dll"
  File "$%DELTA_ROOT%\ext\bin\ot11-openthreads.dll"
  File "$%DELTA_ROOT%\ext\bin\xerces-c_3_1.dll"

  File "$%DELTA_SHARED_CODE%\${DELTA_BUILD_DIR}\bin\BoatActors.dll"

  File "$%DTOCEAN_ROOT%\${DELTA_BUILD_DIR}\bin\dtOcean.dll"
  File "$%OSGOCEAN_DIR%\bin\osgOcean.dll"

SectionEnd


Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  ;Store installation folder
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" $INSTDIR
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


;--------------------------------
;Descriptions

  ;Language strings
;  LangString DESC_SecMain ${LANG_ENGLISH} "A test section."

  ;Assign language strings to sections
;  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
;  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\WaterSprint.exe"
  Delete "$INSTDIR\config.xml"
  Delete "$INSTDIR\WaterSprint.bat"
  Delete "$INSTDIR\*.xsd"

  Delete "$INSTDIR\*.dll"

  RMDir /r "$INSTDIR\ProjectContext"
  RMDir "$INSTDIR"
  
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
 
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd