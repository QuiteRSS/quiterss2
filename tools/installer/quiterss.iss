; -- QuiteRSS.iss --

#define _AppName "QuiteRSS"
#define _AppVersion GetStringFileInfo("..\..\build\bin\QuiteRSS.exe", FILE_VERSION)
#define _AppVerName GetStringFileInfo("..\..\build\bin\QuiteRSS.exe", PRODUCT_VERSION)
#define _AppPublisher "QuiteRSS Team"

[Setup]
AppId={{7CAE3D97-5801-4E37-921F-58F93C083EE9}
AppName={#_AppName}
AppVersion={#_AppVerName}
AppPublisher={#_AppPublisher}
VersionInfoVersion={#_AppVersion}
DefaultDirName={pf}\{#_AppName}
DefaultGroupName={#_AppName}
Compression=lzma/Max
InternalCompressLevel=Max
SolidCompression=true
VersionInfoCompany={#_AppPublisher}
VersionInfoDescription={#_AppName} {#_AppVerName} setup
MinVersion=5.0.2195
AppPublisherURL=https://quiterss.org
WizardImageFile=image.bmp
WizardSmallImageFile=smallimage.bmp
WizardImageStretch=false
WizardImageBackColor=clWhite
OutputDir=..\..\build
OutputBaseFilename={#_AppName}-{#_AppVerName}-setup
RestartIfNeededByRun=false
ShowTasksTreeLines=true
SetupIconFile=setup.ico
LanguageDetectionMethod=locale
PrivilegesRequired=none

[Files]
Source: "{#_AppName}.exe"; DestDir: "{app}"; Flags: skipifsourcedoesntexist
Source: "..\..\build\bin\*"; DestDir: "{app}"; Flags: recursesubdirs

[Icons]
Name: {group}\{#_AppName}; Filename: {app}\{#_AppName}.exe; WorkingDir: {app}
Name: {userdesktop}\{#_AppName}; Filename: {app}\{#_AppName}.exe; WorkingDir: {app}
Name: {group}\{cm:UninstallProgram, {#_AppName}}; Filename: {uninstallexe}

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "cs"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "nl"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "el"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "hu"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "it"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "pl"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "pt"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "pt_BR"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "sr"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "uk"; MessagesFile: "compiler:Languages\Ukrainian.isl"
Name: "fi"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "sl"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "tr"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"

[INI]
Filename: {app}\{#_AppName}.url; Section: InternetShortcut; Key: URL; String: https://quiterss.org

[UninstallDelete]
Type: files; Name: {app}\{#_AppName}.url

[Run]
Filename: "{app}\{#_AppName}.exe"; Description: "{cm:LaunchProgram,{#StringChange(_AppName, "&", "&&")}}"; Flags: nowait postinstall skipifsilent

