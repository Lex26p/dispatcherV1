using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public sealed class DispatcherApiClient : IDispatcherApiClient
{
    private readonly HttpClient _httpClient;
    private readonly DispatcherApiClientOptions _options;

    public DispatcherApiClient(
        HttpClient httpClient
    )
    {
        _httpClient = httpClient;

        _options = new DispatcherApiClientOptions
        {
            UseMockData = true
        };
    }

    public Task<IReadOnlyList<BackendModuleViewModel>> GetBackendModulesAsync(
        CancellationToken cancellationToken = default
    )
    {
        IReadOnlyList<BackendModuleViewModel> modules =
        [
            new()
            {
                Code = "scada_common",
                Name = "Common",
                Description = "Shared common types and primitives.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_contracts",
                Name = "Contracts",
                Description = "Shared DTO and API-facing contracts.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_core",
                Name = "Core",
                Description = "Core application context and module contracts.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_objects",
                Name = "Objects",
                Description = "Object hierarchy foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_devices",
                Name = "Devices",
                Description = "Device model foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_tags",
                Name = "Tags",
                Description = "Tag model foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_protocols",
                Name = "Protocols",
                Description = "Protocol driver contracts and simulator driver.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_polling",
                Name = "Polling",
                Description = "Polling task, group and scheduler foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_runtime",
                Name = "Runtime",
                Description = "Current values and runtime event foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_historian",
                Name = "Historian",
                Description = "History sample, archive decision and buffer foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_events",
                Name = "Events",
                Description = "Event record foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_alarms",
                Name = "Alarms",
                Description = "Alarm lifecycle and rule foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_api",
                Name = "API",
                Description = "API route, response, mapper and read endpoint foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_realtime",
                Name = "Realtime",
                Description = "Realtime message and subscription foundation.",
                Status = "Running"
            },
            new()
            {
                Code = "scada_app",
                Name = "Application",
                Description = "Application composition and gateway startup foundation.",
                Status = "Running"
            }
        ];

        return Task.FromResult(modules);
    }

    public Task<GatewaySummaryViewModel> GetGatewaySummaryAsync(
        CancellationToken cancellationToken = default
    )
    {
        var summary = new GatewaySummaryViewModel
        {
            Mode = "ApiAndRealtime",
            StartupStatus = "Prepared",
            BindAddress = "127.0.0.1",
            HttpPort = 8080,
            RealtimePort = 8080,
            ApiBasePath = "/api",
            RealtimePath = "/api/realtime",
            ApiEnabled = true,
            RealtimeEnabled = true,
            RealTransportAvailable = false,
            Message = "Gateway startup plan is prepared, but real HTTP/WebSocket transport is not implemented yet."
        };

        return Task.FromResult(summary);
    }

    public Task<ApiRouteSummaryViewModel> GetApiRouteSummaryAsync(
        CancellationToken cancellationToken = default
    )
    {
        var summary = new ApiRouteSummaryViewModel
        {
            RouteCount = 7,
            ReadEndpointCount = 11,
            RealtimeChannelCount = 6,
            UsesMockData = _options.UseMockData,
            Notes = "Route/read endpoint counts are based on Sprint 009 foundation catalogs."
        };

        return Task.FromResult(summary);
    }
}