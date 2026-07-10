namespace Dispatcher.Frontend.Services;

public sealed class DispatcherApiClientOptions
{
    public string BaseUrl { get; init; } = "http://localhost:8080";

    public bool UseMockData { get; init; } = true;

    public string ApiBasePath { get; init; } = "/api";
}
