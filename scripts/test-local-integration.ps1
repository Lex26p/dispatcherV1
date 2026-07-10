[CmdletBinding()]
param(
    [string]$BackendBaseUrl = "http://127.0.0.1:8080",
    [string]$FrontendOrigin = "http://localhost:5030"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Assert-Condition {
    param(
        [Parameter(Mandatory = $true)]
        [bool]$Condition,

        [Parameter(Mandatory = $true)]
        [string]$Message
    )

    if (-not $Condition) {
        throw "FAILED: $Message"
    }

    Write-Host "PASS: $Message"
}

function Get-HeaderValue {
    param(
        [Parameter(Mandatory = $true)]
        [object]$Response,

        [Parameter(Mandatory = $true)]
        [string]$Name
    )

    $value = $Response.Headers[$Name]

    if ($null -eq $value) {
        return ""
    }

    $values = @(
        $value | ForEach-Object {
            [string]$_
        }
    )

    return $values -join ", "
}

$healthUrl = "$BackendBaseUrl/api/system/health"
$modulesUrl = "$BackendBaseUrl/api/system/modules"

try {
    Write-Host ""
    Write-Host "Dispatcher local integration smoke test"
    Write-Host "Backend: $BackendBaseUrl"
    Write-Host "Frontend origin: $FrontendOrigin"
    Write-Host ""

    Write-Host "Checking system health..."
    $health = Invoke-RestMethod -Method Get -Uri $healthUrl

    Assert-Condition `
        ([string]::Equals(
            [string]$health.status,
            "ok",
            [System.StringComparison]::OrdinalIgnoreCase
        )) `
        "Health status is ok"

    Assert-Condition `
        ([string]::Equals(
            [string]$health.product,
            "Dispatcher",
            [System.StringComparison]::OrdinalIgnoreCase
        )) `
        "Health product is Dispatcher"

    Assert-Condition `
        ([string]::Equals(
            [string]$health.api,
            "available",
            [System.StringComparison]::OrdinalIgnoreCase
        )) `
        "Backend API is available"

    Assert-Condition `
        ([string]::Equals(
            [string]$health.transport,
            "http",
            [System.StringComparison]::OrdinalIgnoreCase
        )) `
        "Backend transport is HTTP"

    Write-Host ""
    Write-Host "Checking system modules..."
    $modulesResponse = Invoke-RestMethod -Method Get -Uri $modulesUrl
    $modules = @($modulesResponse.modules)

    Assert-Condition `
        ($modules.Count -gt 0) `
        "Backend returned registered modules"

    Assert-Condition `
        ([int]$modulesResponse.count -eq $modules.Count) `
        "Module count matches module collection"

    $invalidModules = @(
        $modules | Where-Object {
            [string]::IsNullOrWhiteSpace([string]$_.code) -or
            [string]::IsNullOrWhiteSpace([string]$_.name) -or
            [string]::IsNullOrWhiteSpace([string]$_.status)
        }
    )

    Assert-Condition `
        ($invalidModules.Count -eq 0) `
        "Every module has code, name, and status"

    Write-Host ""
    Write-Host "Checking CORS response..."
    $corsResponse = Invoke-WebRequest `
        -Method Get `
        -Uri $healthUrl `
        -Headers @{
            Origin = $FrontendOrigin
        } `
        -UseBasicParsing

    $corsOrigin = Get-HeaderValue `
        -Response $corsResponse `
        -Name "Access-Control-Allow-Origin"

    $corsMethods = Get-HeaderValue `
        -Response $corsResponse `
        -Name "Access-Control-Allow-Methods"

    Assert-Condition `
        ($corsOrigin -eq $FrontendOrigin) `
        "CORS allows configured frontend origin"

    Assert-Condition `
        ($corsMethods -match "(^|,\s*)GET($|,)") `
        "CORS allows GET"

    Assert-Condition `
        ($corsMethods -match "(^|,\s*)OPTIONS($|,)") `
        "CORS allows OPTIONS"

    Write-Host ""
    Write-Host "Checking CORS preflight..."
    $preflightResponse = Invoke-WebRequest `
        -Method Options `
        -Uri $healthUrl `
        -Headers @{
            Origin = $FrontendOrigin
            "Access-Control-Request-Method" = "GET"
            "Access-Control-Request-Headers" = "Content-Type"
        } `
        -UseBasicParsing

    $preflightOrigin = Get-HeaderValue `
        -Response $preflightResponse `
        -Name "Access-Control-Allow-Origin"

    $preflightMethods = Get-HeaderValue `
        -Response $preflightResponse `
        -Name "Access-Control-Allow-Methods"

    $preflightHeaders = Get-HeaderValue `
        -Response $preflightResponse `
        -Name "Access-Control-Allow-Headers"

    Assert-Condition `
        (@(200, 204) -contains [int]$preflightResponse.StatusCode) `
        "Preflight status is 200 or 204"

    Assert-Condition `
        ($preflightOrigin -eq $FrontendOrigin) `
        "Preflight allows configured frontend origin"

    Assert-Condition `
        ($preflightMethods -match "(^|,\s*)GET($|,)") `
        "Preflight allows GET"

    Assert-Condition `
        ($preflightMethods -match "(^|,\s*)OPTIONS($|,)") `
        "Preflight allows OPTIONS"

    Assert-Condition `
        ($preflightHeaders -match "(^|,\s*)Content-Type($|,)") `
        "Preflight allows Content-Type"

    Write-Host ""
    Write-Host "Checking rejected CORS origin..."
    $rejectedOriginResponse = Invoke-WebRequest `
        -Method Get `
        -Uri $healthUrl `
        -Headers @{
            Origin = "http://invalid-origin.example"
        } `
        -UseBasicParsing

    $rejectedOriginHeader = Get-HeaderValue `
        -Response $rejectedOriginResponse `
        -Name "Access-Control-Allow-Origin"

    Assert-Condition `
        ([string]::IsNullOrWhiteSpace($rejectedOriginHeader)) `
        "Unknown origin does not receive CORS permission"

    Write-Host ""
    Write-Host "SMOKE TEST PASSED"
    exit 0
}
catch {
    Write-Host ""
    Write-Error $_
    Write-Host "SMOKE TEST FAILED"
    exit 1
}
