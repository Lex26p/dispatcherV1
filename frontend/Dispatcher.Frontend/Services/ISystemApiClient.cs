using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public interface ISystemApiClient
{
    Task<BackendHealthViewModel> GetHealthAsync(
        CancellationToken cancellationToken = default
    );

    Task<IReadOnlyList<BackendModuleViewModel>> GetModulesAsync(
        CancellationToken cancellationToken = default
    );
}
