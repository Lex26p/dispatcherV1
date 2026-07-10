using System.Text.Json.Serialization;

namespace Dispatcher.Frontend.Models;

public sealed class SystemHealthApiResponse
{
    [JsonPropertyName("status")]
    public string Status { get; init; } = "unknown";

    [JsonPropertyName("product")]
    public string Product { get; init; } = "Dispatcher";

    [JsonPropertyName("executable")]
    public string Executable { get; init; } = "dispatcher_server";

    [JsonPropertyName("version")]
    public string Version { get; init; } = "unknown";

    [JsonPropertyName("mode")]
    public string Mode { get; init; } = "unknown";

    [JsonPropertyName("api")]
    public string Api { get; init; } = "unknown";

    [JsonPropertyName("transport")]
    public string Transport { get; init; } = "unknown";

    [JsonPropertyName("endpoint")]
    public string Endpoint { get; init; } = "/api/system/health";

    public BackendHealthViewModel ToViewModel()
    {
        return new BackendHealthViewModel {
            Status = Status,
            Product = Product,
            Executable = Executable,
            Version = Version,
            Mode = Mode,
            Api = Api,
            Transport = Transport,
            Endpoint = Endpoint,
            IsRealBackend = true
        };
    }
}
