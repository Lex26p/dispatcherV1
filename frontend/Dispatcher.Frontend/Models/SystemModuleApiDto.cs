using System.Text.Json.Serialization;

namespace Dispatcher.Frontend.Models;

public sealed class SystemModuleApiDto
{
    [JsonPropertyName("code")]
    public string Code { get; init; } = string.Empty;

    [JsonPropertyName("name")]
    public string Name { get; init; } = string.Empty;

    [JsonPropertyName("description")]
    public string Description { get; init; } = string.Empty;

    [JsonPropertyName("status")]
    public string Status { get; init; } = "Unknown";

    public BackendModuleViewModel ToViewModel()
    {
        return new BackendModuleViewModel {
            Code = Code,
            Name = Name,
            Description = Description,
            Status = Status,
            IsFoundation = true
        };
    }
}
