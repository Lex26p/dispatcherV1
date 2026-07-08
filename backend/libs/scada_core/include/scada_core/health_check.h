#pragma once

#include <string>

namespace dispatcher::core
{
    enum class HealthStatus
    {
        Healthy,
        Degraded,
        Unhealthy
    };

    struct HealthCheckResult
    {
        HealthStatus status = HealthStatus::Healthy;
        std::string component;
        std::string message;
    };

    class IHealthCheck
    {
    public:
        virtual ~IHealthCheck() = default;

        [[nodiscard]] virtual HealthCheckResult check() const = 0;
    };
}