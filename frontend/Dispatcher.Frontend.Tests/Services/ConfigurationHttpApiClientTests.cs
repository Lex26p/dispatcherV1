using System.Net;
using System.Text;
using Dispatcher.Frontend.Services;

namespace Dispatcher.Frontend.Tests.Services;

public sealed class ConfigurationHttpApiClientTests
{
    [Fact]
    public async Task GetObjectsAsync_Query_BuildsPathAndMapsResponse()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        """
                        {
                          "objects": [
                            {
                              "id": "site main/1",
                              "parentId": null,
                              "type": "Site",
                              "code": "SITE_MAIN",
                              "name": "Main site",
                              "description": "",
                              "isRoot": true
                            }
                          ],
                          "count": 1,
                          "total": 1,
                          "offset": 1,
                          "limit": 2,
                          "hasMore": false
                        }
                        """
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var result =
            await client.GetObjectsAsync(
                new ObjectReadRequest
                {
                    Id = "site main/1",
                    ParentId = "root 1",
                    IncludeChildren = true,
                    Limit = 2,
                    Offset = 1
                }
            );

        Assert.Equal(1, result.Count);
        Assert.Equal("site main/1", result.Items[0].Id);

        Assert.Equal(
            "/api/objects?" +
            "id=site%20main%2F1&" +
            "parentId=root%201&" +
            "includeChildren=true&" +
            "limit=2&offset=1",
            handler.LastRequestUri?.PathAndQuery
        );
    }

    [Fact]
    public async Task GetObjectTreeAsync_Query_MapsNestedTree()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        """
                        {
                          "roots": [
                            {
                              "id": "site-main",
                              "parentId": null,
                              "type": "Site",
                              "code": "SITE_MAIN",
                              "name": "Main site",
                              "description": "",
                              "children": [
                                {
                                  "id": "building-admin",
                                  "parentId": "site-main",
                                  "type": "Building",
                                  "code": "BUILDING_ADMIN",
                                  "name": "Administration building",
                                  "description": "",
                                  "children": []
                                }
                              ]
                            }
                          ],
                          "count": 2
                        }
                        """
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var result =
            await client.GetObjectTreeAsync(
                new ObjectTreeReadRequest
                {
                    Id = "site-main"
                }
            );

        Assert.Equal(2, result.Count);
        Assert.Single(result.Roots);
        Assert.True(result.Roots[0].HasChildren);

        Assert.Equal(
            "/api/objects/tree?id=site-main",
            handler.LastRequestUri?.PathAndQuery
        );
    }

    [Fact]
    public async Task GetDevicesAsync_Query_BuildsCompletePath()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        """
                        {
                          "devices": [
                            {
                              "id": "device-snmp-1",
                              "objectId": "building-admin",
                              "code": "UPS_MAIN",
                              "name": "Main UPS",
                              "description": "",
                              "protocol": "Snmp",
                              "state": "Maintenance",
                              "runtimeEnabled": false,
                              "connection": {
                                "host": "2001:db8::10",
                                "port": 161,
                                "ipMode": "IPv6",
                                "networkInterface": "eth0"
                              }
                            }
                          ],
                          "count": 1,
                          "total": 1,
                          "offset": 0,
                          "limit": 25,
                          "hasMore": false
                        }
                        """
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var result =
            await client.GetDevicesAsync(
                new DeviceReadRequest
                {
                    ObjectId = "building-admin",
                    State = "Maintenance",
                    IncludeDisabled = true,
                    Limit = 25,
                    Offset = 0
                }
            );

        var device = Assert.Single(result.Items);

        Assert.Equal("device-snmp-1", device.Id);
        Assert.Equal("2001:db8::10", device.Connection.Host);

        Assert.Equal(
            "/api/devices?" +
            "objectId=building-admin&" +
            "state=Maintenance&" +
            "includeDisabled=true&" +
            "limit=25&offset=0",
            handler.LastRequestUri?.PathAndQuery
        );
    }

    [Fact]
    public async Task GetTagsAsync_Query_MapsEngineeringFields()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        """
                        {
                          "tags": [
                            {
                              "id": "tag-temperature",
                              "objectId": "room-control",
                              "deviceId": "device-modbus-1",
                              "type": "DeviceTag",
                              "valueType": "Double",
                              "archivePolicy": "OnChange",
                              "code": "TEMPERATURE",
                              "name": "Room temperature",
                              "description": "",
                              "engineeringUnit": "degC",
                              "scale": 0.1,
                              "offset": 0.0,
                              "address": "40001",
                              "enabled": true
                            }
                          ],
                          "count": 1,
                          "total": 1,
                          "offset": 0,
                          "limit": 1000,
                          "hasMore": false
                        }
                        """
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var result =
            await client.GetTagsAsync(
                new TagReadRequest
                {
                    DeviceId = "device-modbus-1",
                    IncludeDisabled = false
                }
            );

        var tag = Assert.Single(result.Items);

        Assert.Equal("tag-temperature", tag.Id);
        Assert.Equal("degC", tag.EngineeringUnit);
        Assert.Equal("40001", tag.Address);

        Assert.Equal(
            "/api/tags?" +
            "deviceId=device-modbus-1&" +
            "includeDisabled=false",
            handler.LastRequestUri?.PathAndQuery
        );
    }

    [Fact]
    public async Task GetObjectsAsync_ErrorEnvelope_MapsBackendError()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        """
                        {
                          "error": {
                            "code": "invalid_query",
                            "message": "The query string is invalid.",
                            "correlationId": "configuration-error-113",
                            "details": {
                              "field": "limit",
                              "reason": "must_be_positive"
                            }
                          }
                        }
                        """,
                        HttpStatusCode.BadRequest,
                        "header-error-113"
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetObjectsAsync(
                    new ObjectReadRequest
                    {
                        Limit = 0
                    }
                )
            );

        Assert.Equal(
            DispatcherApiErrorKind.HttpStatus,
            exception.ErrorKind
        );

        Assert.Equal(
            HttpStatusCode.BadRequest,
            exception.StatusCode
        );

        Assert.Equal(
            "invalid_query",
            exception.ApiErrorCode
        );

        Assert.Equal(
            "configuration-error-113",
            exception.CorrelationId
        );

        Assert.Equal(
            "The query string is invalid.",
            exception.Message
        );
    }

    [Fact]
    public async Task GetDevicesAsync_InvalidJson_ThrowsInvalidResponse()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        "{invalid-json",
                        correlationId:
                            "invalid-configuration-json-113"
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetDevicesAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.InvalidResponse,
            exception.ErrorKind
        );

        Assert.Equal(
            "invalid-configuration-json-113",
            exception.CorrelationId
        );
    }

    [Fact]
    public async Task GetTagsAsync_NullJson_ThrowsInvalidResponse()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse(
                        "null",
                        correlationId:
                            "null-configuration-json-113"
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetTagsAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.InvalidResponse,
            exception.ErrorKind
        );

        Assert.Equal(
            "null-configuration-json-113",
            exception.CorrelationId
        );

        Assert.Contains(
            "пустой ответ",
            exception.Message,
            StringComparison.OrdinalIgnoreCase
        );
    }

    [Fact]
    public async Task GetObjectsAsync_ConnectionFailure_ThrowsConnectionError()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromException<HttpResponseMessage>(
                    new HttpRequestException(
                        "Connection failed."
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var client =
            CreateClient(httpClient);

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetObjectsAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.Connection,
            exception.ErrorKind
        );

        Assert.True(exception.IsConnectionFailure);
        Assert.False(exception.IsTimeout);
    }

    [Fact]
    public async Task GetObjectTreeAsync_InternalCancellation_ThrowsTimeout()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromException<HttpResponseMessage>(
                    new TaskCanceledException(
                        "Request timeout."
                    )
                )
            );

        using var httpClient =
            CreateHttpClient(handler);

        var options =
            new DispatcherApiClientOptions
            {
                RequestTimeoutSeconds = 7
            };

        var client =
            new ConfigurationHttpApiClient(
                httpClient,
                options
            );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetObjectTreeAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.Timeout,
            exception.ErrorKind
        );

        Assert.True(exception.IsTimeout);

        Assert.Contains(
            "7",
            exception.Message,
            StringComparison.Ordinal
        );
    }

    [Fact]
    public async Task GetTagsAsync_CallerCancellation_IsNotConverted()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, cancellationToken) =>
                    Task.FromCanceled<HttpResponseMessage>(
                        cancellationToken
                    )
            );

        using var httpClient =
            CreateHttpClient(handler);

        using var cancellation =
            new CancellationTokenSource();

        cancellation.Cancel();

        var client =
            CreateClient(httpClient);

        await Assert.ThrowsAnyAsync<OperationCanceledException>(
            () => client.GetTagsAsync(
                cancellationToken:
                    cancellation.Token
            )
        );
    }

    [Fact]
    public async Task GetObjectsAsync_MissingBaseAddress_ThrowsConfigurationError()
    {
        var handler =
            new StubHttpMessageHandler(
                (_, _) => Task.FromResult(
                    CreateJsonResponse("{}")
                )
            );

        using var httpClient =
            CreateHttpClient(
                handler,
                includeBaseAddress: false
            );

        var client =
            CreateClient(httpClient);

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetObjectsAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.Configuration,
            exception.ErrorKind
        );

        Assert.Equal(
            "/api/objects",
            exception.Endpoint
        );

        Assert.Null(handler.LastRequestUri);
    }

    private static ConfigurationHttpApiClient CreateClient(
        HttpClient httpClient
    )
    {
        return new ConfigurationHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );
    }

    private static HttpClient CreateHttpClient(
        StubHttpMessageHandler handler,
        bool includeBaseAddress = true
    )
    {
        var httpClient =
            new HttpClient(handler)
            {
                Timeout = TimeSpan.FromSeconds(30)
            };

        if (includeBaseAddress)
        {
            httpClient.BaseAddress =
                new Uri(
                    "http://127.0.0.1:8080/"
                );
        }

        return httpClient;
    }

    private static HttpResponseMessage CreateJsonResponse(
        string json,
        HttpStatusCode statusCode = HttpStatusCode.OK,
        string? correlationId = null
    )
    {
        var response =
            new HttpResponseMessage(statusCode)
            {
                Content =
                    new StringContent(
                        json,
                        Encoding.UTF8,
                        "application/json"
                    )
            };

        if (!string.IsNullOrWhiteSpace(correlationId))
        {
            response.Headers.TryAddWithoutValidation(
                "X-Correlation-Id",
                correlationId
            );
        }

        return response;
    }
}
