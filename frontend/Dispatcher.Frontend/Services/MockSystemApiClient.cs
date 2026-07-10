using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public sealed class MockSystemApiClient : ISystemApiClient
{
    private readonly IDispatcherApiClient _legacyApiClient;

    public MockSystemApiClient(
        IDispatcherApiClient legacyApiClient
    )
    {
        _legacyApiClient = legacyApiClient;
    }

    public Task<BackendHealthViewModel> GetHealthAsync(
        CancellationToken cancellationToken = default
    )
    {
        cancellationToken.ThrowIfCancellationRequested();

        var health = new BackendHealthViewModel {
            Status = "ok",
            Product = "Dispatcher",
            Executable = "frontend_mock",
            Version = "mock",
            Mode = "Mock",
            Api = "mock",
            Transport = "memory",
            Endpoint = "mock://system/health",
            IsRealBackend = false
        };

        return Task.FromResult(health);
    }

    public Task<IReadOnlyList<BackendModuleViewModel>> GetModulesAsync(
        CancellationToken cancellationToken = default
    )
    {
        return _legacyApiClient.GetBackendModulesAsync(
            cancellationToken
        );
    }
}
