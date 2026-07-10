namespace Dispatcher.Frontend.Models;

public sealed class BackendHealthViewModel
{
    public string Status { get; init; } = "unknown";

    public string Product { get; init; } = "Dispatcher";

    public string Executable { get; init; } = "dispatcher_server";

    public string Version { get; init; } = "unknown";

    public string Mode { get; init; } = "unknown";

    public string Api { get; init; } = "unknown";

    public string Transport { get; init; } = "unknown";

    public string Endpoint { get; init; } = "/api/system/health";

    public bool IsRealBackend { get; init; }

    public bool IsHealthy => string.Equals(
        Status,
        "ok",
        StringComparison.OrdinalIgnoreCase
    );
}
