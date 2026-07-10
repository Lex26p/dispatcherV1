using System.Text.Json.Serialization;

namespace Dispatcher.Frontend.Models;

public sealed class SystemModulesApiResponse
{
    [JsonPropertyName("modules")]
    public IReadOnlyList<SystemModuleApiDto> Modules { get; init; } = [];

    [JsonPropertyName("count")]
    public int Count { get; init; }

    public bool HasModules => Modules.Count > 0;

    public IReadOnlyList<BackendModuleViewModel> ToViewModels()
    {
        return Modules
            .Select(module => module.ToViewModel())
            .ToArray();
    }
}
