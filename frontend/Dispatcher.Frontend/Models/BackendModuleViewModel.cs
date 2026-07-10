namespace Dispatcher.Frontend.Models;

public sealed class BackendModuleViewModel
{
    public string Code { get; init; } = string.Empty;

    public string Name { get; init; } = string.Empty;

    public string Description { get; init; } = string.Empty;

    public string Status { get; init; } = "Unknown";

    public bool IsFoundation { get; init; } = true;
}
