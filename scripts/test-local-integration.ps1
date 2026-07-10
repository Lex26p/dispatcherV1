[CmdletBinding()]
param(
    [string]$BackendBaseUrl = "http://127.0.0.1:8080",
    [string]$FrontendOrigin = "http://localhost:5030"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$BackendBaseUrl = $BackendBaseUrl.TrimEnd('/')

function Assert-True {
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

function Assert-Equal {
    param(
        [Parameter(Mandatory = $true)]
        [AllowNull()]
        [object]$Actual,

        [Parameter(Mandatory = $true)]
        [AllowNull()]
        [object]$Expected,

        [Parameter(Mandatory = $true)]
        [string]$Message
    )

    if ($Actual -ne $Expected) {
        throw "FAILED: $Message. Expected '$Expected', actual '$Actual'."
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

    if ($value -is [System.Array]) {
        return (($value | ForEach-Object { [string]$_ }) -join ", ")
    }

    return [string]$value
}

function Invoke-ExpectedApiError {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Uri,

        [Parameter(Mandatory = $true)]
        [string]$CorrelationId
    )

    $request = @{
        Method = "Get"
        Uri = $Uri
        Headers = @{
            "X-Correlation-Id" = $CorrelationId
        }
        UseBasicParsing = $true
        ErrorAction = "Stop"
    }

    try {
        $null = Invoke-WebRequest @request
    }
    catch {
        $response = $_.Exception.Response

        if ($null -eq $response) {
            throw
        }

        $bodyText = [string]$_.ErrorDetails.Message

        if ([string]::IsNullOrWhiteSpace($bodyText)) {
            try {
                $stream = $response.GetResponseStream()
                $reader = New-Object System.IO.StreamReader($stream)

                try {
                    $bodyText = $reader.ReadToEnd()
                }
                finally {
                    $reader.Dispose()
                }
            }
            catch {
                $bodyText = ""
            }
        }

        if ([string]::IsNullOrWhiteSpace($bodyText)) {
            throw "FAILED: API error response body is empty for $Uri"
        }

        $body = $bodyText | ConvertFrom-Json

        return [pscustomobject]@{
            StatusCode = [int]$response.StatusCode
            HeaderCorrelationId = [string]$response.Headers["X-Correlation-Id"]
            Body = $body
        }
    }

    throw "FAILED: Expected an API error from $Uri"
}

$healthUrl = "$BackendBaseUrl/api/system/health"
$modulesUrl = "$BackendBaseUrl/api/system/modules"
$objectsUrl = "$BackendBaseUrl/api/objects"
$objectTreeUrl = "$BackendBaseUrl/api/objects/tree"
$devicesUrl = "$BackendBaseUrl/api/devices"
$tagsUrl = "$BackendBaseUrl/api/tags"

try {
    Write-Host ""
    Write-Host "Dispatcher local integration smoke test"
    Write-Host "Backend: $BackendBaseUrl"
    Write-Host "Frontend origin: $FrontendOrigin"
    Write-Host ""

    Write-Host "Checking System health..."

    $health = Invoke-RestMethod -Method Get -Uri $healthUrl

    Assert-Equal ([string]$health.status) "ok" "Health status is ok"
    Assert-Equal ([string]$health.product) "Dispatcher" "Health product is Dispatcher"
    Assert-Equal ([string]$health.api) "available" "Backend API is available"
    Assert-Equal ([string]$health.transport) "http" "Backend transport is HTTP"

    Write-Host ""
    Write-Host "Checking System modules..."

    $modulesResponse = Invoke-RestMethod -Method Get -Uri $modulesUrl
    $modules = @($modulesResponse.modules)

    Assert-True ($modules.Count -gt 0) "Backend returned registered modules"
    Assert-Equal ([int]$modulesResponse.count) ([int]$modules.Count) "Module count matches module collection"

    $invalidModules = @(
        $modules | Where-Object {
            [string]::IsNullOrWhiteSpace([string]$_.code) -or
            [string]::IsNullOrWhiteSpace([string]$_.name) -or
            [string]::IsNullOrWhiteSpace([string]$_.status)
        }
    )

    Assert-Equal ([int]$invalidModules.Count) 0 "Every module has code, name, and status"

    Write-Host ""
    Write-Host "Checking Object collection..."

    $objectCorrelationId = "smoke-object-116"

    $objectRequest = @{
        Method = "Get"
        Uri = $objectsUrl
        Headers = @{
            "X-Correlation-Id" = $objectCorrelationId
        }
        UseBasicParsing = $true
    }

    $objectsWebResponse = Invoke-WebRequest @objectRequest
    $objects = $objectsWebResponse.Content | ConvertFrom-Json
    $objectItems = @($objects.objects)

    Assert-Equal ([int]$objectsWebResponse.StatusCode) 200 "Object collection returns HTTP 200"
    Assert-Equal (Get-HeaderValue -Response $objectsWebResponse -Name "X-Correlation-Id") $objectCorrelationId "Object collection preserves correlation ID"
    Assert-Equal ([int]$objectItems.Count) 5 "Development snapshot exposes five objects"
    Assert-Equal ([int]$objects.count) ([int]$objectItems.Count) "Object count matches current page"
    Assert-Equal ([int]$objects.total) 5 "Object total is five"
    Assert-Equal ([int]$objects.offset) 0 "Object default offset is zero"
    Assert-Equal ([int]$objects.limit) 1000 "Object default limit is 1000"
    Assert-True (-not [bool]$objects.hasMore) "Object default page has no additional items"

    $invalidObjects = @(
        $objectItems | Where-Object {
            [string]::IsNullOrWhiteSpace([string]$_.id) -or
            [string]::IsNullOrWhiteSpace([string]$_.type) -or
            [string]::IsNullOrWhiteSpace([string]$_.code) -or
            [string]::IsNullOrWhiteSpace([string]$_.name)
        }
    )

    Assert-Equal ([int]$invalidObjects.Count) 0 "Every Object item has stable identity fields"

    Write-Host ""
    Write-Host "Checking Object filters and pagination..."

    $objectById = Invoke-RestMethod -Method Get -Uri "${objectsUrl}?id=room-control"
    $objectByIdItems = @($objectById.objects)

    Assert-Equal ([int]$objectById.total) 1 "Object id filter returns one object"
    Assert-Equal ([string]$objectByIdItems[0].id) "room-control" "Object id filter returns room-control"

    $objectPage = Invoke-RestMethod -Method Get -Uri "${objectsUrl}?limit=2&offset=1"

    Assert-Equal ([int]$objectPage.count) 2 "Object pagination returns two items"
    Assert-Equal ([int]$objectPage.total) 5 "Object pagination preserves total"
    Assert-Equal ([int]$objectPage.offset) 1 "Object pagination preserves offset"
    Assert-Equal ([int]$objectPage.limit) 2 "Object pagination preserves limit"
    Assert-True ([bool]$objectPage.hasMore) "Object pagination reports additional items"

    Write-Host ""
    Write-Host "Checking Object tree..."

    $objectTree = Invoke-RestMethod -Method Get -Uri $objectTreeUrl
    $objectTreeRoots = @($objectTree.roots)

    Assert-Equal ([int]$objectTree.count) 5 "Object tree contains five nodes"
    Assert-Equal ([int]$objectTreeRoots.Count) 2 "Object tree contains two roots"

    $siteTree = Invoke-RestMethod -Method Get -Uri "${objectTreeUrl}?id=site-main"
    $siteTreeRoots = @($siteTree.roots)

    Assert-Equal ([int]$siteTree.count) 3 "Object subtree contains three nodes"
    Assert-Equal ([int]$siteTreeRoots.Count) 1 "Object subtree contains one root"
    Assert-Equal ([string]$siteTreeRoots[0].id) "site-main" "Object subtree root is site-main"

    Write-Host ""
    Write-Host "Checking Device collection..."

    $devices = Invoke-RestMethod -Method Get -Uri $devicesUrl
    $deviceItems = @($devices.devices)

    Assert-Equal ([int]$deviceItems.Count) 3 "Default Device collection hides disabled configuration"
    Assert-Equal ([int]$devices.total) 3 "Default Device total is three"

    $invalidDevices = @(
        $deviceItems | Where-Object {
            [string]::IsNullOrWhiteSpace([string]$_.id) -or
            [string]::IsNullOrWhiteSpace([string]$_.code) -or
            [string]::IsNullOrWhiteSpace([string]$_.protocol) -or
            [string]::IsNullOrWhiteSpace([string]$_.state) -or
            $null -eq $_.connection
        }
    )

    Assert-Equal ([int]$invalidDevices.Count) 0 "Every Device item has stable configuration fields"

    $allDevices = Invoke-RestMethod -Method Get -Uri "${devicesUrl}?includeDisabled=true"
    $allDeviceItems = @($allDevices.devices)

    Assert-Equal ([int]$allDevices.total) 4 "Device includeDisabled exposes four devices"

    $disabledDevices = @(
        $allDeviceItems | Where-Object {
            [string]$_.state -eq "Disabled"
        }
    )

    Assert-Equal ([int]$disabledDevices.Count) 1 "Device snapshot contains one disabled device"

    $maintenanceDevices = Invoke-RestMethod -Method Get -Uri "${devicesUrl}?state=Maintenance"
    $maintenanceDeviceItems = @($maintenanceDevices.devices)

    Assert-Equal ([int]$maintenanceDevices.total) 1 "Device state filter returns one maintenance device"
    Assert-Equal ([string]$maintenanceDeviceItems[0].id) "device-snmp-1" "Maintenance Device is device-snmp-1"

    $roomDevices = Invoke-RestMethod -Method Get -Uri "${devicesUrl}?objectId=room-control"
    $roomDeviceItems = @($roomDevices.devices)

    Assert-Equal ([int]$roomDevices.total) 1 "Device objectId filter returns one device"
    Assert-Equal ([string]$roomDeviceItems[0].id) "device-modbus-1" "room-control Device is device-modbus-1"

    $allDevicesJson = $allDevices | ConvertTo-Json -Depth 12 -Compress

    $forbiddenDeviceFields = @(
        "password",
        "token",
        "community",
        "secret",
        "secretReference",
        "credential",
        "credentials"
    )

    foreach ($forbiddenField in $forbiddenDeviceFields) {
        $fieldPattern = '"' + [regex]::Escape($forbiddenField) + '"\s*:'
        Assert-True ($allDevicesJson -notmatch $fieldPattern) "Device API does not expose $forbiddenField"
    }

    Write-Host ""
    Write-Host "Checking Tag collection..."

    $tags = Invoke-RestMethod -Method Get -Uri $tagsUrl
    $tagItems = @($tags.tags)

    Assert-Equal ([int]$tagItems.Count) 5 "Default Tag collection hides disabled configuration"
    Assert-Equal ([int]$tags.total) 5 "Default Tag total is five"

    $invalidTags = @(
        $tagItems | Where-Object {
            [string]::IsNullOrWhiteSpace([string]$_.id) -or
            [string]::IsNullOrWhiteSpace([string]$_.code) -or
            [string]::IsNullOrWhiteSpace([string]$_.type) -or
            [string]::IsNullOrWhiteSpace([string]$_.valueType) -or
            [string]::IsNullOrWhiteSpace([string]$_.archivePolicy)
        }
    )

    Assert-Equal ([int]$invalidTags.Count) 0 "Every Tag item has stable configuration fields"

    $allTags = Invoke-RestMethod -Method Get -Uri "${tagsUrl}?includeDisabled=true"
    $allTagItems = @($allTags.tags)

    Assert-Equal ([int]$allTags.total) 6 "Tag includeDisabled exposes six tags"

    $disabledTags = @(
        $allTagItems | Where-Object {
            -not [bool]$_.enabled
        }
    )

    Assert-Equal ([int]$disabledTags.Count) 1 "Tag snapshot contains one disabled tag"

    $roomTags = Invoke-RestMethod -Method Get -Uri "${tagsUrl}?objectId=room-control"

    Assert-Equal ([int]$roomTags.total) 2 "Tag objectId filter returns two tags"

    $deviceTags = Invoke-RestMethod -Method Get -Uri "${tagsUrl}?deviceId=device-disabled-1"

    Assert-Equal ([int]$deviceTags.total) 1 "Default Tag deviceId filter hides disabled tag"

    $allDeviceTagsUrl = "${tagsUrl}?deviceId=device-disabled-1&includeDisabled=true"
    $allDeviceTags = Invoke-RestMethod -Method Get -Uri $allDeviceTagsUrl

    Assert-Equal ([int]$allDeviceTags.total) 2 "Tag deviceId filter includes disabled tag on request"

    Write-Host ""
    Write-Host "Checking invalid Object query..."

    $invalidObjectQuery = Invoke-ExpectedApiError -Uri "${objectsUrl}?limit=0" -CorrelationId "smoke-invalid-object-116"

    Assert-Equal ([int]$invalidObjectQuery.StatusCode) 400 "Invalid Object query returns HTTP 400"
    Assert-Equal ([string]$invalidObjectQuery.Body.error.code) "invalid_query" "Invalid Object query uses invalid_query code"
    Assert-Equal ([string]$invalidObjectQuery.Body.error.details.field) "limit" "Invalid Object query identifies limit field"
    Assert-Equal ([string]$invalidObjectQuery.Body.error.details.reason) "must_be_positive" "Invalid Object query identifies reason"
    Assert-Equal ([string]$invalidObjectQuery.Body.error.correlationId) "smoke-invalid-object-116" "Invalid Object body preserves correlation ID"
    Assert-Equal ([string]$invalidObjectQuery.HeaderCorrelationId) "smoke-invalid-object-116" "Invalid Object header preserves correlation ID"

    Write-Host ""
    Write-Host "Checking unsupported Tag query..."

    $unsupportedTagQuery = Invoke-ExpectedApiError -Uri "${tagsUrl}?valueType=Double" -CorrelationId "smoke-invalid-tag-116"

    Assert-Equal ([int]$unsupportedTagQuery.StatusCode) 400 "Unsupported Tag query returns HTTP 400"
    Assert-Equal ([string]$unsupportedTagQuery.Body.error.code) "invalid_query" "Unsupported Tag query uses invalid_query code"
    Assert-Equal ([string]$unsupportedTagQuery.Body.error.details.field) "valueType" "Unsupported Tag query identifies parameter"
    Assert-Equal ([string]$unsupportedTagQuery.Body.error.details.reason) "unsupported_parameter" "Unsupported Tag query identifies reason"
    Assert-Equal ([string]$unsupportedTagQuery.Body.error.correlationId) "smoke-invalid-tag-116" "Unsupported Tag query preserves correlation ID"

    Write-Host ""
    Write-Host "Checking CORS response..."

    $corsRequest = @{
        Method = "Get"
        Uri = $objectsUrl
        Headers = @{
            Origin = $FrontendOrigin
        }
        UseBasicParsing = $true
    }

    $corsResponse = Invoke-WebRequest @corsRequest
    $corsOrigin = Get-HeaderValue -Response $corsResponse -Name "Access-Control-Allow-Origin"
    $corsMethods = Get-HeaderValue -Response $corsResponse -Name "Access-Control-Allow-Methods"

    Assert-Equal $corsOrigin $FrontendOrigin "CORS allows configured frontend origin"
    Assert-True ($corsMethods -match "(^|,\s*)GET($|,)") "CORS allows GET"
    Assert-True ($corsMethods -match "(^|,\s*)OPTIONS($|,)") "CORS allows OPTIONS"

    Write-Host ""
    Write-Host "Checking CORS preflight..."

    $preflightRequest = @{
        Method = "Options"
        Uri = $objectsUrl
        Headers = @{
            Origin = $FrontendOrigin
            "Access-Control-Request-Method" = "GET"
            "Access-Control-Request-Headers" = "Content-Type, X-Correlation-Id"
        }
        UseBasicParsing = $true
    }

    $preflightResponse = Invoke-WebRequest @preflightRequest
    $preflightOrigin = Get-HeaderValue -Response $preflightResponse -Name "Access-Control-Allow-Origin"
    $preflightMethods = Get-HeaderValue -Response $preflightResponse -Name "Access-Control-Allow-Methods"
    $preflightHeaders = Get-HeaderValue -Response $preflightResponse -Name "Access-Control-Allow-Headers"

    Assert-True (@(200, 204) -contains [int]$preflightResponse.StatusCode) "Preflight status is 200 or 204"
    Assert-Equal $preflightOrigin $FrontendOrigin "Preflight allows configured frontend origin"
    Assert-True ($preflightMethods -match "(^|,\s*)GET($|,)") "Preflight allows GET"
    Assert-True ($preflightMethods -match "(^|,\s*)OPTIONS($|,)") "Preflight allows OPTIONS"
    Assert-True ($preflightHeaders -match "(^|,\s*)Content-Type($|,)") "Preflight allows Content-Type"
    Assert-True ($preflightHeaders -match "(^|,\s*)X-Correlation-Id($|,)") "Preflight allows X-Correlation-Id"

    Write-Host ""
    Write-Host "Checking rejected CORS origin..."

    $rejectedOriginRequest = @{
        Method = "Get"
        Uri = $objectsUrl
        Headers = @{
            Origin = "http://invalid-origin.example"
        }
        UseBasicParsing = $true
    }

    $rejectedOriginResponse = Invoke-WebRequest @rejectedOriginRequest
    $rejectedOriginHeader = Get-HeaderValue -Response $rejectedOriginResponse -Name "Access-Control-Allow-Origin"

    Assert-True ([string]::IsNullOrWhiteSpace($rejectedOriginHeader)) "Unknown origin does not receive CORS permission"

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
