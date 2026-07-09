-- Dispatcher
-- Migration: 0006_tag_history
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for tag history samples.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   TimescaleDB support can be added later by converting tag_history_samples
--   into a hypertable.
--
--   This migration depends on:
--     - 0003_tag_model.sql
--
--   Tag history is append-only by design.
--   Runtime snapshots are stored separately in runtime_value_snapshots.

BEGIN;

CREATE TABLE IF NOT EXISTS tag_history_samples
(
    id UUID PRIMARY KEY,

    tag_id UUID NOT NULL REFERENCES tags(id) ON DELETE CASCADE,

    value_type TEXT NOT NULL,
    quality TEXT NOT NULL,
    source TEXT NOT NULL,

    raw_value TEXT NOT NULL DEFAULT '',
    engineering_value TEXT NOT NULL DEFAULT '',

    sample_timestamp TIMESTAMPTZ NOT NULL,
    source_timestamp TIMESTAMPTZ NOT NULL,
    server_timestamp TIMESTAMPTZ NOT NULL,

    change_counter BIGINT NOT NULL DEFAULT 0,

    engineering_unit TEXT NOT NULL DEFAULT '',
    message TEXT NOT NULL DEFAULT '',

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_tag_history_samples_value_type CHECK
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

    CONSTRAINT chk_tag_history_samples_quality CHECK
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

    CONSTRAINT chk_tag_history_samples_source CHECK
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

    CONSTRAINT chk_tag_history_samples_change_counter CHECK
    (
        change_counter >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_tag_id
    ON tag_history_samples(tag_id);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_tag_id_sample_timestamp
    ON tag_history_samples(tag_id, sample_timestamp DESC);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_sample_timestamp
    ON tag_history_samples(sample_timestamp DESC);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_quality
    ON tag_history_samples(quality);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_source
    ON tag_history_samples(source);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_value_type
    ON tag_history_samples(value_type);

CREATE INDEX IF NOT EXISTS ix_tag_history_samples_created_at
    ON tag_history_samples(created_at);

COMMIT;