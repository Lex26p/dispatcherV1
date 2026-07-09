-- Dispatcher
-- Migration: 0003_tag_model
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for the Dispatcher tag model.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   This migration depends on:
--     - 0001_object_model.sql
--     - 0002_device_model.sql
--
--   Tag history is intentionally not included here.
--   Historian tables will be added in a later sprint.

BEGIN;

CREATE TABLE IF NOT EXISTS tags
(
    id UUID PRIMARY KEY,

    object_id UUID NULL REFERENCES object_nodes(id) ON DELETE SET NULL,
    device_id UUID NULL REFERENCES devices(id) ON DELETE SET NULL,

    tag_type TEXT NOT NULL,
    value_type TEXT NOT NULL,
    archive_policy TEXT NOT NULL,

    code TEXT NOT NULL,
    name TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',

    engineering_unit TEXT NOT NULL DEFAULT '',
    scale DOUBLE PRECISION NOT NULL DEFAULT 1.0,
    offset DOUBLE PRECISION NOT NULL DEFAULT 0.0,

    address_protocol TEXT NOT NULL,
    address TEXT NOT NULL DEFAULT '',
    protocol_address_json TEXT NOT NULL DEFAULT '',

    bit_index INTEGER NULL,
    byte_offset INTEGER NULL,
    mask BIGINT NULL,
    shift INTEGER NULL,
    bit_length INTEGER NULL,

    is_enabled BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT uq_tags_code UNIQUE (code),

    CONSTRAINT chk_tags_code_not_empty CHECK
    (
        length(trim(code)) > 0
    ),

    CONSTRAINT chk_tags_name_not_empty CHECK
    (
        length(trim(name)) > 0
    ),

    CONSTRAINT chk_tags_tag_type CHECK
    (
        tag_type IN
        (
            'DeviceTag',
            'ManualTag',
            'CalculatedTag',
            'SystemTag',
            'VirtualTag',
            'CommandTag',
            'SimulationTag',
            'ExternalTag'
        )
    ),

    CONSTRAINT chk_tags_value_type CHECK
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

    CONSTRAINT chk_tags_archive_policy CHECK
    (
        archive_policy IN
        (
            'Disabled',
            'OnChange',
            'Periodic',
            'OnChangeWithDeadband',
            'OnQualityChange',
            'OnAlarm',
            'AlwaysButThrottled'
        )
    ),

    CONSTRAINT chk_tags_address_protocol CHECK
    (
        address_protocol IN
        (
            'ModbusTcp',
            'Snmp',
            'Simulator',
            'Custom'
        )
    ),

    CONSTRAINT chk_tags_scale CHECK
    (
        scale <> 0.0
    ),

    CONSTRAINT chk_tags_bit_index CHECK
    (
        bit_index IS NULL OR
        (
            bit_index >= 0
            AND bit_index <= 63
        )
    ),

    CONSTRAINT chk_tags_byte_offset CHECK
    (
        byte_offset IS NULL OR byte_offset >= 0
    ),

    CONSTRAINT chk_tags_mask CHECK
    (
        mask IS NULL OR mask >= 0
    ),

    CONSTRAINT chk_tags_shift CHECK
    (
        shift IS NULL OR
        (
            shift >= 0
            AND shift <= 63
        )
    ),

    CONSTRAINT chk_tags_bit_length CHECK
    (
        bit_length IS NULL OR
        (
            bit_length >= 1
            AND bit_length <= 64
        )
    ),

    CONSTRAINT chk_tags_device_assignment CHECK
    (
        tag_type NOT IN ('DeviceTag', 'CommandTag')
        OR device_id IS NOT NULL
    ),

    CONSTRAINT chk_tags_object_assignment CHECK
    (
        tag_type = 'SystemTag'
        OR object_id IS NOT NULL
    ),

    CONSTRAINT chk_tags_required_address CHECK
    (
        tag_type NOT IN ('DeviceTag', 'CommandTag', 'ExternalTag')
        OR
        (
            length(trim(address)) > 0
            OR length(trim(protocol_address_json)) > 0
        )
    ),

    CONSTRAINT chk_tags_extraction_value_type CHECK
    (
        (
            bit_index IS NULL
            AND byte_offset IS NULL
            AND mask IS NULL
            AND shift IS NULL
            AND bit_length IS NULL
        )
        OR
        value_type IN
        (
            'Boolean',
            'Int32',
            'Int64',
            'Float',
            'Double',
            'Enum'
        )
    )
);

CREATE INDEX IF NOT EXISTS ix_tags_object_id
    ON tags(object_id);

CREATE INDEX IF NOT EXISTS ix_tags_device_id
    ON tags(device_id);

CREATE INDEX IF NOT EXISTS ix_tags_tag_type
    ON tags(tag_type);

CREATE INDEX IF NOT EXISTS ix_tags_value_type
    ON tags(value_type);

CREATE INDEX IF NOT EXISTS ix_tags_archive_policy
    ON tags(archive_policy);

CREATE INDEX IF NOT EXISTS ix_tags_is_enabled
    ON tags(is_enabled);

CREATE TABLE IF NOT EXISTS tag_current_values
(
    tag_id UUID PRIMARY KEY REFERENCES tags(id) ON DELETE CASCADE,

    value_type TEXT NOT NULL,
    quality TEXT NOT NULL DEFAULT 'NotInitialized',
    source TEXT NOT NULL DEFAULT 'Unknown',

    raw_value TEXT NULL,
    engineering_value TEXT NULL,

    timestamp TIMESTAMPTZ NULL,
    source_timestamp TIMESTAMPTZ NULL,
    server_timestamp TIMESTAMPTZ NULL,

    last_good_value TEXT NULL,
    last_good_timestamp TIMESTAMPTZ NULL,

    change_counter BIGINT NOT NULL DEFAULT 0,

    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_tag_current_values_value_type CHECK
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

    CONSTRAINT chk_tag_current_values_quality CHECK
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

    CONSTRAINT chk_tag_current_values_source CHECK
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

    CONSTRAINT chk_tag_current_values_change_counter CHECK
    (
        change_counter >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_tag_current_values_quality
    ON tag_current_values(quality);

CREATE INDEX IF NOT EXISTS ix_tag_current_values_source
    ON tag_current_values(source);

CREATE INDEX IF NOT EXISTS ix_tag_current_values_updated_at
    ON tag_current_values(updated_at);

COMMIT;