[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot

Push-Location $repoRoot

try {
    $trackedFiles = @(
        git ls-files --eol
    )

    if ($LASTEXITCODE -ne 0) {
        throw "git ls-files --eol failed."
    }

    $invalidFiles = @(
        $trackedFiles | Where-Object {
            $_ -match "\bw/(crlf|mixed)\b"
        }
    )

    if ($invalidFiles.Count -gt 0) {
        Write-Host ""
        Write-Host "Tracked files with invalid working-tree line endings:"
        $invalidFiles | ForEach-Object {
            Write-Host $_
        }

        throw "Repository contains CRLF or mixed tracked files."
    }

    Write-Host ""
    Write-Host "LINE ENDINGS CHECK PASSED"
    Write-Host "Tracked text files use LF working-tree endings."
}
finally {
    Pop-Location
}
