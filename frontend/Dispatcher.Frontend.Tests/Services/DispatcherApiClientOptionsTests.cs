using Dispatcher.Frontend.Services;

namespace Dispatcher.Frontend.Tests.Services;

public sealed class DispatcherApiClientOptionsTests
{
    [Fact]
    public void Defaults_ExposeDevelopmentBackendConfiguration()
    {
        var options = new DispatcherApiClientOptions();

        Assert.Equal(
            "http://127.0.0.1:8080",
            options.NormalizedBaseUrl
        );

        Assert.Equal(
            "/api",
            options.NormalizedApiBasePath
        );

        Assert.Equal(
            new Uri("http://127.0.0.1:8080/"),
            options.BackendBaseUri
        );

        Assert.Equal(
            TimeSpan.FromSeconds(10),
            options.RequestTimeout
        );

        Assert.False(options.UseMockData);
        Assert.True(options.PreferRealBackend);
    }

    [Theory]
    [InlineData(
        " http://localhost:8080/ ",
        "http://localhost:8080"
    )]
    [InlineData(
        "https://dispatcher.example/",
        "https://dispatcher.example"
    )]
    public void NormalizedBaseUrl_TrimsWhitespaceAndTrailingSlash(
        string value,
        string expected
    )
    {
        var options = new DispatcherApiClientOptions {
            BaseUrl = value
        };

        Assert.Equal(
            expected,
            options.NormalizedBaseUrl
        );
    }

    [Fact]
    public void EmptyBaseUrl_UsesDevelopmentDefault()
    {
        var options = new DispatcherApiClientOptions {
            BaseUrl = " "
        };

        Assert.Equal(
            "http://127.0.0.1:8080",
            options.NormalizedBaseUrl
        );
    }

    [Theory]
    [InlineData(
        "system/health",
        "/api/system/health"
    )]
    [InlineData(
        "/system/health",
        "/api/system/health"
    )]
    [InlineData(
        "/api/system/health",
        "/api/system/health"
    )]
    [InlineData(
        "",
        "/api"
    )]
    public void BuildApiPath_NormalizesEndpointPath(
        string endpointPath,
        string expected
    )
    {
        var options = new DispatcherApiClientOptions();

        Assert.Equal(
            expected,
            options.BuildApiPath(endpointPath)
        );
    }

    [Fact]
    public void CustomApiBasePath_IsNormalizedAndApplied()
    {
        var options = new DispatcherApiClientOptions {
            ApiBasePath = " api/v1/ "
        };

        Assert.Equal(
            "/api/v1",
            options.NormalizedApiBasePath
        );

        Assert.Equal(
            "/api/v1/system/health",
            options.BuildApiPath("system/health")
        );
    }

    [Theory]
    [InlineData(0)]
    [InlineData(-1)]
    public void NonPositiveTimeout_UsesTenSecondFallback(
        int timeoutSeconds
    )
    {
        var options = new DispatcherApiClientOptions {
            RequestTimeoutSeconds = timeoutSeconds
        };

        Assert.Equal(
            TimeSpan.FromSeconds(10),
            options.RequestTimeout
        );
    }

    [Fact]
    public void MockMode_DisablesRealBackendPreference()
    {
        var options = new DispatcherApiClientOptions {
            UseMockData = true
        };

        Assert.True(options.UseMockData);
        Assert.False(options.PreferRealBackend);
    }
}
