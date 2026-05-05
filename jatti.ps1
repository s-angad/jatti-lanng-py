param(
  [Parameter(ValueFromRemainingArguments = $true)]
  [string[]] $Args
)

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$exe = Join-Path $root 'c\bin\jatti.exe'

if (-not (Test-Path $exe)) {
  Write-Error "jatti: missing '$exe'. Build it with: .\\c\\build_msvc.ps1 -Config Release"
  exit 1
}

& $exe @Args
exit $LASTEXITCODE
