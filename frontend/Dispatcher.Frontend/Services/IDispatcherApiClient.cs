using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public interface IDispatcherApiClient
{
    Task<IReadOnlyList<BackendModuleViewModel>> GetBackendModulesAsync(
        CancellationToken cancellationToken = default
    );

    Task<GatewaySummaryViewModel> GetGatewaySummaryAsync(
        CancellationToken cancellationToken = default
    );

    Task<ApiRouteSummaryViewModel> GetApiRouteSummaryAsync(
        CancellationToken cancellationToken = default
    );
}