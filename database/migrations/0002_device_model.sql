-- Dispatcher
-- Migration: 0002_device_model
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for the Dispatcher device model.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   This migration depends on 0001_object_model.sql because devices may reference object_nodes.

BEGIN;

CREATE TABLE IF NOT EXISTS devices
(
    id UUID PRIMARY KEY,

    object_id UUID NULL REFERENCES object_nodes(id) ON DELETE SET NULL,

    code TEXT NOT NULL,
    name TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',

    protocol TEXT NOT NULL,
    state TEXT NOT NULL,

    connection_protocol TEXT NOT NULL,
    host TEXT NOT NULL DEFAULT '',
    port INTEGER NOT NULL DEFAULT 0,
    ip_mode TEXT NOT NULL DEFAULT 'Auto',
    network_interface TEXT NOT NULL DEFAULT '',

    is_enabled BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT uq_devices_code UNIQUE (code),

    CONSTRAINT chk_devices_code_not_empty CHECK
    (
        length(trim(code)) > 0
    ),

    CONSTRAINT chk_devices_name_not_empty CHECK
    (
        length(trim(name)) > 0
    ),

    CONSTRAINT chk_devices_protocol CHECK
    (
        protocol IN
        (
            'ModbusTcp',
            'Snmp',
            'Simulator',
            'Custom'
        )
    ),

    CONSTRAINT chk_devices_state CHECK
    (
        state IN
        (
            'Draft',
            'Commissioning',
            'Active',
            'Maintenance',
            'Disabled',
            'OutOfService',
            'Decommissioned',
            'Simulation'
        )
    ),

    CONSTRAINT chk_devices_connection_protocol CHECK
    (
        connection_protocol IN
        (
            'ModbusTcp',
            'Snmp',
            'Simulator',
            'Custom'
        )
    ),

    CONSTRAINT chk_devices_protocol_match CHECK
    (
        protocol = connection_protocol
    ),

    CONSTRAINT chk_devices_ip_mode CHECK
    (
        ip_mode IN
        (
            'IPv4',
            'IPv6',
            'Auto'
        )
    ),

    CONSTRAINT chk_devices_port_range CHECK
    (
        port >= 0 AND port <= 65535
    ),

    CONSTRAINT chk_devices_network_endpoint CHECK
    (
        protocol = 'Simulator'
        OR
        (
            length(trim(host)) > 0
            AND port > 0
        )
    )
);

CREATE INDEX IF NOT EXISTS ix_devices_object_id
    ON devices(object_id);

CREATE INDEX IF NOT EXISTS ix_devices_protocol
    ON devices(protocol);

CREATE INDEX IF NOT EXISTS ix_devices_state
    ON devices(state);

CREATE INDEX IF NOT EXISTS ix_devices_is_enabled
    ON devices(is_enabled);

CREATE TABLE IF NOT EXISTS device_diagnostics
(
    device_id UUID PRIMARY KEY REFERENCES devices(id) ON DELETE CASCADE,

    communication_status TEXT NOT NULL DEFAULT 'Unknown',
    health_status TEXT NOT NULL DEFAULT 'Unknown',

    successful_requests BIGINT NOT NULL DEFAULT 0,
    failed_requests BIGINT NOT NULL DEFAULT 0,
    timeout_count BIGINT NOT NULL DEFAULT 0,
    reconnect_count BIGINT NOT NULL DEFAULT 0,

    last_latency_ms INTEGER NOT NULL DEFAULT 0,
    last_error TEXT NOT NULL DEFAULT '',

    last_success_at TIMESTAMPTZ NULL,
    last_failure_at TIMESTAMPTZ NULL,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_device_diagnostics_communication_status CHECK
    (
        communication_status IN
        (
            'Unknown',
            'NotConfigured',
            'Online',
            'Degraded',
            'Offline',
            'Timeout',
            'ProtocolError',
            'Disabled'
        )
    ),

    CONSTRAINT chk_device_diagnostics_health_status CHECK
    (
        health_status IN
        (
            'Unknown',
            'Healthy',
            'Warning',
            'Critical',
            'Offline',
            'Disabled'
        )
    ),

    CONSTRAINT chk_device_diagnostics_successful_requests CHECK
    (
        successful_requests >= 0
    ),

    CONSTRAINT chk_device_diagnostics_failed_requests CHECK
    (
        failed_requests >= 0
    ),

    CONSTRAINT chk_device_diagnostics_timeout_count CHECK
    (
        timeout_count >= 0
    ),

    CONSTRAINT chk_device_diagnostics_reconnect_count CHECK
    (
        reconnect_count >= 0
    ),

    CONSTRAINT chk_device_diagnostics_last_latency_ms CHECK
    (
        last_latency_ms >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_device_diagnostics_communication_status
    ON device_diagnostics(communication_status);

CREATE INDEX IF NOT EXISTS ix_device_diagnostics_health_status
    ON device_diagnostics(health_status);

CREATE INDEX IF NOT EXISTS ix_device_diagnostics_updated_at
    ON device_diagnostics(updated_at);

COMMIT;