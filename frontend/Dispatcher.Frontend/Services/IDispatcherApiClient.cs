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

    Task<IReadOnlyList<RuntimeValueViewModel>> GetRuntimeValuesAsync(
        CancellationToken cancellationToken = default
    );

    Task<IReadOnlyList<EventRecordViewModel>> GetEventsAsync(
        CancellationToken cancellationToken = default
    );

    Task<IReadOnlyList<ActiveAlarmViewModel>> GetActiveAlarmsAsync(
        CancellationToken cancellationToken = default
    );
}