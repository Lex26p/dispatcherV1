$ErrorActionPreference = "Stop"

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$strictUtf8 = New-Object System.Text.UTF8Encoding($false, $true)

$binaryExtensions = @(
    ".png",
    ".jpg",
    ".jpeg",
    ".gif",
    ".ico",
    ".pdf",
    ".zip",
    ".7z",
    ".dll",
    ".exe",
    ".pdb",
    ".lib",
    ".obj",
    ".bin"
)

$trackedFiles = git ls-files

foreach ($relativePath in $trackedFiles) {
    if ([string]::IsNullOrWhiteSpace($relativePath)) {
        continue
    }

    if (-not (Test-Path $relativePath)) {
        continue
    }

    $extension = [System.IO.Path]::GetExtension($relativePath).ToLowerInvariant()

    if ($binaryExtensions -contains $extension) {
        Write-Host "Skip binary extension: $relativePath"
        continue
    }

    $bytes = [System.IO.File]::ReadAllBytes($relativePath)

    if ($bytes.Length -eq 0) {
        [System.IO.File]::WriteAllText($relativePath, "", $utf8NoBom)
        Write-Host "Normalize empty file: $relativePath"
        continue
    }

    if ($bytes -contains 0) {
        Write-Host "Skip binary-like file: $relativePath"
        continue
    }

    try {
        $text = $strictUtf8.GetString($bytes)
    }
    catch {
        Write-Host "Skip non-UTF8 text file: $relativePath"
        continue
    }

    if ($text.Length -gt 0 -and $text[0] -eq [char]0xFEFF) {
        $text = $text.Substring(1)
    }

    $text = $text -replace "`r`n", "`n"
    $text = $text -replace "`r", "`n"

    if (-not $text.EndsWith("`n")) {
        $text += "`n"
    }

    [System.IO.File]::WriteAllText($relativePath, $text, $utf8NoBom)

    Write-Host "Normalized LF: $relativePath"
}

Write-Host "Repository text line endings normalized to LF."
