namespace Dispatcher.Frontend.Models;

public sealed class GatewaySummaryViewModel
{
    public string Mode { get; init; } = "ApiAndRealtime";

    public string StartupStatus { get; init; } = "Prepared";

    public string BindAddress { get; init; } = "127.0.0.1";

    public int HttpPort { get; init; } = 8080;

    public int RealtimePort { get; init; } = 8080;

    public string ApiBasePath { get; init; } = "/api";

    public string RealtimePath { get; init; } = "/api/realtime";

    public bool ApiEnabled { get; init; } = true;

    public bool RealtimeEnabled { get; init; } = true;

    public bool RealTransportAvailable { get; init; } = false;

    public string Message { get; init; } = "Gateway startup plan prepared. Real HTTP/WebSocket transport is not implemented yet.";
}