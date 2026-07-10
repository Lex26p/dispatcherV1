using Dispatcher.Frontend;
using Dispatcher.Frontend.Services;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using MudBlazor.Services;

var builder = WebAssemblyHostBuilder.CreateDefault(args);

builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

builder.Services.AddMudServices();

var defaultApiOptions = new DispatcherApiClientOptions();
var apiConfiguration = builder.Configuration.GetSection(
    DispatcherApiClientOptions.SectionName
);

var apiOptions = new DispatcherApiClientOptions {
    BaseUrl = apiConfiguration["BaseUrl"] ?? defaultApiOptions.BaseUrl,
    ApiBasePath = apiConfiguration["ApiBasePath"] ?? defaultApiOptions.ApiBasePath,
    UseMockData = bool.TryParse(
        apiConfiguration["UseMockData"],
        out var useMockData
    )
        ? useMockData
        : defaultApiOptions.UseMockData,
    RequestTimeoutSeconds = int.TryParse(
        apiConfiguration["RequestTimeoutSeconds"],
        out var requestTimeoutSeconds
    )
        ? requestTimeoutSeconds
        : defaultApiOptions.RequestTimeoutSeconds
};

builder.Services.AddSingleton(apiOptions);

builder.Services.AddScoped(
    _ => new HttpClient {
        BaseAddress = apiOptions.BackendBaseUri,
        Timeout = apiOptions.RequestTimeout
    }
);

builder.Services.AddScoped<IDispatcherApiClient, DispatcherApiClient>();

await builder.Build().RunAsync();
