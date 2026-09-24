; SurrounDead Bridge launcher installer.
; Built with Inno Setup (https://jrsoftware.org/isinfo.php).
;
; Publish the self-contained build first:
;   dotnet publish -c Release -r win-x64 --self-contained true ^
;       -p:PublishSingleFile=false -p:PublishTrimmed=false -o publish
; then compile this script:
;   "%LocalAppData%\Programs\Inno Setup 6\ISCC.exe" installer\setup.iss

#define MyAppName "SurrounDead Bridge"
#define MyAppVersion "1.0.0"
#define MyAppExeName "SDBLauncher.exe"

[Setup]
AppId={{B6C3B6C0-6B0E-4E7A-9C7C-6C8B7E9A9E7C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher=RiStL
DefaultDirName={autopf}\{#MyAppName}
; No admin rights required and no UAC prompt: installs per-user under
; %LocalAppData%\Programs when the user isn't an admin (autopf resolves
; accordingly), same friction level as VS Code / Chrome's default installer.
PrivilegesRequired=lowest
DisableProgramGroupPage=yes
OutputDir=Output
OutputBaseFilename=SDBLauncher-Setup
Compression=lzma2/max
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
WizardStyle=modern
UninstallDisplayIcon={app}\{#MyAppExeName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop shortcut"; GroupDescription: "Additional shortcuts:"

[Files]
Source: "..\publish\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch {#MyAppName}"; Flags: nowait postinstall skipifsilent
