#pragma once

#include <string>
#include <string_view>

namespace dispatcher::api
{
    enum class ApiHttpMethod
    {
        Get,
        Post,
        Put,
        Patch,
        Delete
    };

    enum class ApiEndpointArea
    {
        System,
        Objects,
        Devices,
        Tags,
        Runtime,
        Historian,
        Events,
        Alarms,
        Realtime,
        Unknown
    };

    struct ApiEndpoint
    {
        ApiHttpMethod method = ApiHttpMethod::Get;
        ApiEndpointArea area = ApiEndpointArea::Unknown;

        std::string path;
        std::string name;
        std::string description;

        bool requires_authentication = true;
        bool realtime_capable = false;

        [[nodiscard]] bool has_path() const noexcept;
        [[nodiscard]] bool has_name() const noexcept;
        [[nodiscard]] bool has_description() const noexcept;
        [[nodiscard]] bool is_public() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    [[nodiscard]] ApiEndpoint make_api_endpoint(
        ApiHttpMethod method,
        ApiEndpointArea area,
        std::string path,
        std::string name,
        std::string description
    );

    [[nodiscard]] std::string_view to_string(
        ApiHttpMethod method
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        ApiEndpointArea area
    ) noexcept;
}
