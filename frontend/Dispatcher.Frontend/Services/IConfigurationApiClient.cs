using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public interface IConfigurationApiClient
{
    Task<ConfigurationPage<ObjectViewModel>> GetObjectsAsync(
        ObjectReadRequest? request = null,
        CancellationToken cancellationToken = default
    );

    Task<ObjectTreeViewModel> GetObjectTreeAsync(
        ObjectTreeReadRequest? request = null,
        CancellationToken cancellationToken = default
    );

    Task<ConfigurationPage<DeviceViewModel>> GetDevicesAsync(
        DeviceReadRequest? request = null,
        CancellationToken cancellationToken = default
    );

    Task<ConfigurationPage<TagViewModel>> GetTagsAsync(
        TagReadRequest? request = null,
        CancellationToken cancellationToken = default
    );
}
