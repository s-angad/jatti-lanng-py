Param(
  [ValidateSet('Debug','Release')] [string]$Config = 'Release'
)

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$cRoot = $PSScriptRoot
$src = Join-Path $cRoot 'src'
$bin = Join-Path $cRoot 'bin'
$obj = Join-Path $bin 'obj'

New-Item -ItemType Directory -Force -Path $bin | Out-Null
New-Item -ItemType Directory -Force -Path $obj | Out-Null

$vswhere = "${Env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

$vsDevCmd = $null

if (Test-Path $vswhere) {
  $installPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath | Select-Object -First 1
  if ($installPath) {
    $candidate = Join-Path $installPath 'Common7\Tools\VsDevCmd.bat'
    if (Test-Path $candidate) {
      $vsDevCmd = $candidate
    }
  }
}

if (-not $vsDevCmd) {
  # Fallback: try to locate VsDevCmd.bat without vswhere.
  $root = "${Env:ProgramFiles(x86)}\Microsoft Visual Studio"
  if (Test-Path $root) {
    $found = Get-ChildItem -Path $root -Recurse -Force -ErrorAction SilentlyContinue -Filter 'VsDevCmd.bat' |
      Select-Object -First 1
    if ($found) { $vsDevCmd = $found.FullName }
  }
}

if (-not $vsDevCmd) {
  throw "MSVC build tools not found. Install 'Build Tools for Visual Studio' with 'Desktop development with C++' workload."
}

# Import the VS dev environment into this PowerShell session.
# Note: PowerShell escapes embedded quotes using the backtick.
$envDump = cmd /s /c "call `"$vsDevCmd`" -arch=x64 -host_arch=x64 >nul & set"
foreach ($line in $envDump) {
  $p = $line.IndexOf('=')
  if ($p -gt 0) {
    $name = $line.Substring(0, $p)
    $value = $line.Substring($p + 1)
    Set-Item -Path ("Env:" + $name) -Value $value
  }
}

$opt = @()
if ($Config -eq 'Release') {
  $opt += '/O2'
} else {
  $opt += '/Od'
  $opt += '/Zi'
}

$sources = @(
  'main.c',
  'arena.c',
  'value.c',
  'object.c',
  'table.c',
  'chunk.c',
  'vm.c',
  'jatti_source.c',
  'expr_scan.c',
  'expr_parse.c',
  'compiler.c'
) | ForEach-Object { Join-Path $src $_ }

$exe = Join-Path $bin 'jatti.exe'

Write-Host "Building $exe ($Config)" -ForegroundColor Cyan

& cl /nologo /W4 /I $src ("/Fo$($obj)\") /Fe:$exe @opt $sources

Write-Host "Done." -ForegroundColor Green
