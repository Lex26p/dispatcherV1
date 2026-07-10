namespace Dispatcher.Frontend.Services;

public sealed class DispatcherApiClientOptions
{
    public const string SectionName = "DispatcherApi";

    public string BaseUrl { get; init; } = "http://127.0.0.1:8080";

    public string ApiBasePath { get; init; } = "/api";

    public bool UseMockData { get; init; } = false;

    public int RequestTimeoutSeconds { get; init; } = 10;

    public bool PreferRealBackend => !UseMockData;

    public string NormalizedBaseUrl
    {
        get
        {
            if (string.IsNullOrWhiteSpace(BaseUrl)) {
                return "http://127.0.0.1:8080";
            }

            return BaseUrl.Trim().TrimEnd('/');
        }
    }

    public string NormalizedApiBasePath
    {
        get
        {
            if (string.IsNullOrWhiteSpace(ApiBasePath)) {
                return "/api";
            }

            var value = ApiBasePath.Trim();

            if (!value.StartsWith('/')) {
                value = "/" + value;
            }

            return value.TrimEnd('/');
        }
    }

    public Uri BackendBaseUri => new($"{NormalizedBaseUrl}/", UriKind.Absolute);

    public TimeSpan RequestTimeout => TimeSpan.FromSeconds(
        RequestTimeoutSeconds > 0 ? RequestTimeoutSeconds : 10
    );

    public string BuildApiPath(string endpointPath)
    {
        if (string.IsNullOrWhiteSpace(endpointPath)) {
            return NormalizedApiBasePath;
        }

        var normalizedEndpointPath = endpointPath.Trim();

        if (!normalizedEndpointPath.StartsWith('/')) {
            normalizedEndpointPath = "/" + normalizedEndpointPath;
        }

        if (
            normalizedEndpointPath.Equals(
                NormalizedApiBasePath,
                StringComparison.OrdinalIgnoreCase
            )
        ) {
            return normalizedEndpointPath;
        }

        if (
            normalizedEndpointPath.StartsWith(
                NormalizedApiBasePath + "/",
                StringComparison.OrdinalIgnoreCase
            )
        ) {
            return normalizedEndpointPath;
        }

        return $"{NormalizedApiBasePath}{normalizedEndpointPath}";
    }
}
