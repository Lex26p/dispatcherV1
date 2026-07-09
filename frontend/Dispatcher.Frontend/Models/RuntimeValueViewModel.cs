namespace Dispatcher.Frontend.Models;

public sealed class RuntimeValueViewModel
{
    public string TagCode { get; init; } = string.Empty;

    public string TagName { get; init; } = string.Empty;

    public string ObjectPath { get; init; } = string.Empty;

    public string DeviceName { get; init; } = string.Empty;

    public string Value { get; init; } = string.Empty;

    public string Unit { get; init; } = string.Empty;

    public string Quality { get; init; } = "Unknown";

    public string Source { get; init; } = "Mock";

    public DateTimeOffset Timestamp { get; init; } = DateTimeOffset.Now;
}