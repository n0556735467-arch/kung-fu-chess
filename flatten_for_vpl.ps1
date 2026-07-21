$srcDirs = @("model", "io", "input", "engine", "realtime", "rules", "texttests")
$destDir = "flat_upload"

if (Test-Path $destDir) { Remove-Item $destDir -Recurse -Force }
New-Item -ItemType Directory -Path $destDir | Out-Null

foreach ($dir in $srcDirs) {
    Get-ChildItem -Path $dir -Recurse -Include *.hpp, *.cpp | ForEach-Object {
        $content = Get-Content $_.FullName -Raw
        $content = $content -replace '#include\s+"[^"]*[\\/]([\w]+\.(hpp|cpp))"', '#include "$1"'
        Set-Content -Path (Join-Path $destDir $_.Name) -Value $content -NoNewline
    }
}

$mainContent = Get-Content "main.cpp" -Raw
$mainContent = $mainContent -replace '#include\s+"[^"]*[\\/]([\w]+\.(hpp|cpp))"', '#include "$1"'
Set-Content -Path (Join-Path $destDir "main.cpp") -Value $mainContent -NoNewline

Write-Host "Done. Flattened files are in: $destDir"
Write-Host "Upload the files from $destDir to VPL individually (not as ZIP)."