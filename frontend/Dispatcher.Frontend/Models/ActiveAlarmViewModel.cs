namespace Dispatcher.Frontend.Models;

public sealed class ActiveAlarmViewModel
{
    public string Id { get; init; } = string.Empty;

    public DateTimeOffset RaisedAt { get; init; } = DateTimeOffset.Now;

    public string ObjectPath { get; init; } = string.Empty;

    public string SourceName { get; init; } = string.Empty;

    public string Severity { get; init; } = string.Empty;

    public string Priority { get; init; } = string.Empty;

    public string State { get; init; } = string.Empty;

    public string Message { get; init; } = string.Empty;

    public bool Acknowledged { get; init; }
}