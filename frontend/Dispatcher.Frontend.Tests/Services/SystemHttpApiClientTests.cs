using System.Net;
using System.Text;

using Dispatcher.Frontend.Services;

namespace Dispatcher.Frontend.Tests.Services;

public sealed class SystemHttpApiClientTests
{
    [Fact]
    public async Task GetHealthAsync_ValidJson_ReturnsMappedHealth()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse(
                    """
                    {
                      "status": "ok",
                      "product": "Dispatcher",
                      "executable": "dispatcher_server",
                      "version": "0.1.0",
                      "mode": "development",
                      "api": "available",
                      "transport": "http",
                      "endpoint": "/api/system/health"
                    }
                    """
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var health =
            await client.GetHealthAsync();

        Assert.True(health.IsHealthy);
        Assert.True(health.IsRealBackend);
        Assert.Equal("Dispatcher", health.Product);
        Assert.Equal("0.1.0", health.Version);
        Assert.Equal("http", health.Transport);

        Assert.Equal(
            HttpMethod.Get,
            handler.LastMethod
        );

        Assert.Equal(
            new Uri(
                "http://127.0.0.1:8080/api/system/health"
            ),
            handler.LastRequestUri
        );
    }

    [Fact]
    public async Task GetModulesAsync_ValidJson_ReturnsMappedModules()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse(
                    """
                    {
                      "modules": [
                        {
                          "code": "scada_http",
                          "name": "HTTP transport",
                          "description": "Drogon transport",
                          "status": "Running"
                        },
                        {
                          "code": "scada_api",
                          "name": "API",
                          "description": "API foundation",
                          "status": "Running"
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

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var modules =
            await client.GetModulesAsync();

        Assert.Equal(2, modules.Count);
        Assert.Equal("scada_http", modules[0].Code);
        Assert.Equal("scada_api", modules[1].Code);

        Assert.All(
            modules,
            module => Assert.True(
                module.IsFoundation
            )
        );

        Assert.Equal(
            new Uri(
                "http://127.0.0.1:8080/api/system/modules"
            ),
            handler.LastRequestUri
        );
    }

    [Fact]
    public async Task GetHealthAsync_ErrorEnvelope_MapsBackendError()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse(
                    """
                    {
                      "error": {
                        "code": "service_unavailable",
                        "message": "Backend is unavailable.",
                        "correlationId": "backend-error-104",
                        "details": null
                      }
                    }
                    """,
                    HttpStatusCode.ServiceUnavailable,
                    "header-error-104"
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.HttpStatus,
            exception.ErrorKind
        );

        Assert.Equal(
            HttpStatusCode.ServiceUnavailable,
            exception.StatusCode
        );

        Assert.Equal(
            "service_unavailable",
            exception.ApiErrorCode
        );

        Assert.Equal(
            "backend-error-104",
            exception.CorrelationId
        );

        Assert.Equal(
            "Backend is unavailable.",
            exception.Message
        );

        Assert.True(exception.HasCorrelationId);
    }

    [Fact]
    public async Task GetHealthAsync_ErrorWithoutEnvelope_UsesFallback()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse(
                    "{invalid-error-json",
                    HttpStatusCode.BadGateway,
                    "fallback-error-104"
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.HttpStatus,
            exception.ErrorKind
        );

        Assert.Equal(
            HttpStatusCode.BadGateway,
            exception.StatusCode
        );

        Assert.Null(exception.ApiErrorCode);

        Assert.Equal(
            "fallback-error-104",
            exception.CorrelationId
        );

        Assert.Contains(
            "502",
            exception.Message,
            StringComparison.Ordinal
        );
    }

    [Fact]
    public async Task GetHealthAsync_InvalidJson_ThrowsInvalidResponse()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse(
                    "{invalid-json",
                    correlationId:
                        "invalid-json-104"
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.InvalidResponse,
            exception.ErrorKind
        );

        Assert.Equal(
            "invalid-json-104",
            exception.CorrelationId
        );
    }

    [Fact]
    public async Task GetHealthAsync_NullJson_ThrowsInvalidResponse()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse(
                    "null",
                    correlationId:
                        "null-json-104"
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.InvalidResponse,
            exception.ErrorKind
        );

        Assert.Equal(
            "null-json-104",
            exception.CorrelationId
        );

        Assert.Contains(
            "пустой ответ",
            exception.Message,
            StringComparison.OrdinalIgnoreCase
        );
    }

    [Fact]
    public async Task GetHealthAsync_ConnectionFailure_ThrowsConnectionError()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromException<HttpResponseMessage>(
                new HttpRequestException(
                    "Connection failed."
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.Connection,
            exception.ErrorKind
        );

        Assert.True(
            exception.IsConnectionFailure
        );

        Assert.False(exception.IsTimeout);
    }

    [Fact]
    public async Task GetHealthAsync_InternalCancellation_ThrowsTimeoutError()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromException<HttpResponseMessage>(
                new TaskCanceledException(
                    "Request timeout."
                )
            )
        );

        using var httpClient =
            CreateHttpClient(handler);

        var options =
            new DispatcherApiClientOptions {
                RequestTimeoutSeconds = 4
            };

        var client = new SystemHttpApiClient(
            httpClient,
            options
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.Timeout,
            exception.ErrorKind
        );

        Assert.True(exception.IsTimeout);

        Assert.Contains(
            "4",
            exception.Message,
            StringComparison.Ordinal
        );
    }

    [Fact]
    public async Task GetHealthAsync_CallerCancellation_IsNotConverted()
    {
        var handler = new StubHttpMessageHandler(
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

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        await Assert.ThrowsAnyAsync<OperationCanceledException>(
            () => client.GetHealthAsync(
                cancellation.Token
            )
        );
    }

    [Fact]
    public async Task GetHealthAsync_MissingBaseAddress_ThrowsConfigurationError()
    {
        var handler = new StubHttpMessageHandler(
            (_, _) => Task.FromResult(
                CreateJsonResponse("{}")
            )
        );

        using var httpClient =
            CreateHttpClient(
                handler,
                includeBaseAddress: false
            );

        var client = new SystemHttpApiClient(
            httpClient,
            new DispatcherApiClientOptions()
        );

        var exception =
            await Assert.ThrowsAsync<DispatcherApiException>(
                () => client.GetHealthAsync()
            );

        Assert.Equal(
            DispatcherApiErrorKind.Configuration,
            exception.ErrorKind
        );

        Assert.Equal(
            "/api/system/health",
            exception.Endpoint
        );

        Assert.Null(
            handler.LastRequestUri
        );
    }

    private static HttpClient CreateHttpClient(
        StubHttpMessageHandler handler,
        bool includeBaseAddress = true
    )
    {
        var httpClient =
            new HttpClient(handler) {
                Timeout =
                    TimeSpan.FromSeconds(30)
            };

        if (includeBaseAddress) {
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
            new HttpResponseMessage(statusCode) {
                Content = new StringContent(
                    json,
                    Encoding.UTF8,
                    "application/json"
                )
            };

        if (
            !string.IsNullOrWhiteSpace(
                correlationId
            )
        ) {
            response.Headers.TryAddWithoutValidation(
                "X-Correlation-Id",
                correlationId
            );
        }

        return response;
    }
}
