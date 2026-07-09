#pragma once

#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace dispatcher::protocols
{
    using ProtocolTimestamp = std::chrono::system_clock::time_point;

    struct ProtocolConnectionCheckResult
    {
        bool success = false;

        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        std::uint32_t latency_ms = 0;
        std::string message;

        ProtocolTimestamp timestamp{};

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct ProtocolAddressTestResult
    {
        bool success = false;

        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        std::string normalized_address;
        std::string message;

        ProtocolTimestamp timestamp{};

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct ProtocolReadResult
    {
        dispatcher::tags::TagId tag_id;

        bool success = false;

        dispatcher::tags::TagValuePayload raw_value;
        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        std::uint32_t latency_ms = 0;
        std::string message;

        ProtocolTimestamp source_timestamp{};
        ProtocolTimestamp server_timestamp{};

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_value() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct ProtocolBatchReadResult
    {
        bool success = false;

        std::vector<ProtocolReadResult> items;
        std::string message;

        ProtocolTimestamp server_timestamp{};

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct ProtocolWriteResult
    {
        dispatcher::tags::TagId tag_id;

        bool success = false;
        bool dry_run = false;

        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        std::uint32_t latency_ms = 0;
        std::string message;

        ProtocolTimestamp timestamp{};

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };
}