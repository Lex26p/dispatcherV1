namespace Dispatcher.Frontend.Models;

public sealed class EventRecordViewModel
{
    public string Id { get; init; } = string.Empty;

    public DateTimeOffset Timestamp { get; init; } = DateTimeOffset.Now;

    public string Category { get; init; } = string.Empty;

    public string Severity { get; init; } = string.Empty;

    public string SourceType { get; init; } = string.Empty;

    public string SourceName { get; init; } = string.Empty;

    public string Message { get; init; } = string.Empty;

    public string CorrelationId { get; init; } = string.Empty;
}