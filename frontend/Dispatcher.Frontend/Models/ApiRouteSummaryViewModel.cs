namespace Dispatcher.Frontend.Models;

public sealed class ApiRouteSummaryViewModel
{
    public int RouteCount { get; init; }

    public int ReadEndpointCount { get; init; }

    public int RealtimeChannelCount { get; init; }

    public bool UsesMockData { get; init; } = true;

    public string Notes { get; init; } = "Frontend API client currently uses mock data because backend HTTP transport is not implemented yet.";
}