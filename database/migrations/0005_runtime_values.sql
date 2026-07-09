-- Dispatcher
-- Migration: 0005_runtime_values
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for runtime value snapshots
--   and runtime value events.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   This migration depends on:
--     - 0003_tag_model.sql
--
--   Runtime snapshots are not tag history.
--   Runtime events are not the full Event Manager.
--   Historian and Event Manager will be added later.

BEGIN;

CREATE TABLE IF NOT EXISTS runtime_value_snapshots
(
    tag_id UUID PRIMARY KEY REFERENCES tags(id) ON DELETE CASCADE,

    value_type TEXT NOT NULL,
    quality TEXT NOT NULL,
    source TEXT NOT NULL,

    raw_value TEXT NOT NULL DEFAULT '',
    engineering_value TEXT NOT NULL DEFAULT '',

    value_timestamp TIMESTAMPTZ NOT NULL,
    source_timestamp TIMESTAMPTZ NOT NULL,
    server_timestamp TIMESTAMPTZ NOT NULL,

    last_good_value TEXT NOT NULL DEFAULT '',
    last_good_timestamp TIMESTAMPTZ NULL,

    change_counter BIGINT NOT NULL DEFAULT 0,

    saved_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_runtime_value_snapshots_value_type CHECK
    (
        value_type IN
        (
            'Boolean',
            'Int32',
            'Int64',
            'Float',
            'Double',
            'String',
            'Enum',
            'Json'
        )
    ),

    CONSTRAINT chk_runtime_value_snapshots_quality CHECK
    (
        quality IN
        (
            'Good',
            'Uncertain',
            'Bad',
            'CommunicationError',
            'Timeout',
            'DeviceError',
            'ProtocolError',
            'OutOfRange',
            'Manual',
            'Simulation',
            'Maintenance',
            'Stale',
            'Disabled',
            'NotInitialized'
        )
    ),

    CONSTRAINT chk_runtime_value_snapshots_source CHECK
    (
        source IN
        (
            'Unknown',
            'Device',
            'Manual',
            'Calculation',
            'System',
            'Simulation',
            'External'
        )
    ),

    CONSTRAINT chk_runtime_value_snapshots_change_counter CHECK
    (
        change_counter >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_runtime_value_snapshots_quality
    ON runtime_value_snapshots(quality);

CREATE INDEX IF NOT EXISTS ix_runtime_value_snapshots_source
    ON runtime_value_snapshots(source);

CREATE INDEX IF NOT EXISTS ix_runtime_value_snapshots_server_timestamp
    ON runtime_value_snapshots(server_timestamp);

CREATE INDEX IF NOT EXISTS ix_runtime_value_snapshots_updated_at
    ON runtime_value_snapshots(updated_at);

CREATE TABLE IF NOT EXISTS runtime_value_events
(
    id UUID PRIMARY KEY,

    tag_id UUID NOT NULL REFERENCES tags(id) ON DELETE CASCADE,

    event_type TEXT NOT NULL,
    change_kind TEXT NOT NULL,

    previous_quality TEXT NOT NULL,
    current_quality TEXT NOT NULL,

    event_timestamp TIMESTAMPTZ NOT NULL,

    change_counter BIGINT NOT NULL DEFAULT 0,

    message TEXT NOT NULL DEFAULT '',

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_runtime_value_events_event_type CHECK
    (
        event_type IN
        (
            'ValueInitialized',
            'ValueChanged',
            'QualityChanged',
            'ValueAndQualityChanged',
            'SourceChanged'
        )
    ),

    CONSTRAINT chk_runtime_value_events_change_kind CHECK
    (
        change_kind IN
        (
            'None',
            'Initialized',
            'ValueChanged',
            'QualityChanged',
            'ValueAndQualityChanged',
            'SourceChanged'
        )
    ),

    CONSTRAINT chk_runtime_value_events_previous_quality CHECK
    (
        previous_quality IN
        (
            'Good',
            'Uncertain',
            'Bad',
            'CommunicationError',
            'Timeout',
            'DeviceError',
            'ProtocolError',
            'OutOfRange',
            'Manual',
            'Simulation',
            'Maintenance',
            'Stale',
            'Disabled',
            'NotInitialized'
        )
    ),

    CONSTRAINT chk_runtime_value_events_current_quality CHECK
    (
        current_quality IN
        (
            'Good',
            'Uncertain',
            'Bad',
            'CommunicationError',
            'Timeout',
            'DeviceError',
            'ProtocolError',
            'OutOfRange',
            'Manual',
            'Simulation',
            'Maintenance',
            'Stale',
            'Disabled',
            'NotInitialized'
        )
    ),

    CONSTRAINT chk_runtime_value_events_change_counter CHECK
    (
        change_counter >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_runtime_value_events_tag_id
    ON runtime_value_events(tag_id);

CREATE INDEX IF NOT EXISTS ix_runtime_value_events_event_type
    ON runtime_value_events(event_type);

CREATE INDEX IF NOT EXISTS ix_runtime_value_events_change_kind
    ON runtime_value_events(change_kind);

CREATE INDEX IF NOT EXISTS ix_runtime_value_events_event_timestamp
    ON runtime_value_events(event_timestamp);

CREATE INDEX IF NOT EXISTS ix_runtime_value_events_created_at
    ON runtime_value_events(created_at);

COMMIT;