using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Tests.Models;

public sealed class SystemApiDtoMappingTests
{
    [Fact]
    public void HealthResponse_ToViewModel_MapsAllFields()
    {
        var response = new SystemHealthApiResponse {
            Status = "OK",
            Product = "Dispatcher",
            Executable = "dispatcher_server",
            Version = "0.1.0",
            Mode = "development",
            Api = "available",
            Transport = "http",
            Endpoint = "/api/system/health"
        };

        var viewModel = response.ToViewModel();

        Assert.Equal("OK", viewModel.Status);
        Assert.Equal("Dispatcher", viewModel.Product);
        Assert.Equal(
            "dispatcher_server",
            viewModel.Executable
        );

        Assert.Equal("0.1.0", viewModel.Version);
        Assert.Equal("development", viewModel.Mode);
        Assert.Equal("available", viewModel.Api);
        Assert.Equal("http", viewModel.Transport);

        Assert.Equal(
            "/api/system/health",
            viewModel.Endpoint
        );

        Assert.True(viewModel.IsRealBackend);
        Assert.True(viewModel.IsHealthy);
    }

    [Fact]
    public void ModuleDto_ToViewModel_MapsAllFields()
    {
        var dto = new SystemModuleApiDto {
            Code = "scada_http",
            Name = "HTTP transport",
            Description = "Drogon HTTP transport",
            Status = "Running"
        };

        var viewModel = dto.ToViewModel();

        Assert.Equal("scada_http", viewModel.Code);
        Assert.Equal("HTTP transport", viewModel.Name);

        Assert.Equal(
            "Drogon HTTP transport",
            viewModel.Description
        );

        Assert.Equal("Running", viewModel.Status);
        Assert.True(viewModel.IsFoundation);
    }

    [Fact]
    public void ModulesResponse_ToViewModels_MapsCollection()
    {
        var response = new SystemModulesApiResponse {
            Count = 2,
            Modules = [
                new SystemModuleApiDto {
                    Code = "scada_http",
                    Name = "HTTP",
                    Description = "HTTP transport",
                    Status = "Running"
                },
                new SystemModuleApiDto {
                    Code = "scada_api",
                    Name = "API",
                    Description = "API foundation",
                    Status = "Running"
                }
            ]
        };

        var viewModels = response.ToViewModels();

        Assert.True(response.HasModules);
        Assert.Equal(2, response.Count);
        Assert.Equal(2, viewModels.Count);
        Assert.Equal("scada_http", viewModels[0].Code);
        Assert.Equal("scada_api", viewModels[1].Code);
    }

    [Fact]
    public void EmptyModulesResponse_ReturnsEmptyCollection()
    {
        var response = new SystemModulesApiResponse();

        var viewModels = response.ToViewModels();

        Assert.False(response.HasModules);
        Assert.Equal(0, response.Count);
        Assert.Empty(viewModels);
    }
}
