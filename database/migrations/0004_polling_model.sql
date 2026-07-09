-- Dispatcher
-- Migration: 0004_polling_model
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for polling configuration.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   This migration depends on:
--     - 0002_device_model.sql
--     - 0003_tag_model.sql
--
--   Polling runtime execution history is intentionally not included here.
--   Runtime polling statistics and events will be added later.

BEGIN;

CREATE TABLE IF NOT EXISTS polling_groups
(
    id UUID PRIMARY KEY,

    device_id UUID NOT NULL REFERENCES devices(id) ON DELETE CASCADE,

    protocol TEXT NOT NULL,
    mode TEXT NOT NULL,

    code TEXT NOT NULL,
    name TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',

    interval_ms INTEGER NOT NULL DEFAULT 1000,
    timeout_ms INTEGER NOT NULL DEFAULT 1000,
    max_batch_size INTEGER NOT NULL DEFAULT 64,
    priority INTEGER NOT NULL DEFAULT 100,

    is_enabled BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT uq_polling_groups_code UNIQUE (code),

    CONSTRAINT chk_polling_groups_code_not_empty CHECK
    (
        length(trim(code)) > 0
    ),

    CONSTRAINT chk_polling_groups_name_not_empty CHECK
    (
        length(trim(name)) > 0
    ),

    CONSTRAINT chk_polling_groups_protocol CHECK
    (
        protocol IN
        (
            'ModbusTcp',
            'Snmp',
            'Simulator',
            'Custom'
        )
    ),

    CONSTRAINT chk_polling_groups_mode CHECK
    (
        mode IN
        (
            'Sequential',
            'Batch'
        )
    ),

    CONSTRAINT chk_polling_groups_interval_ms CHECK
    (
        interval_ms > 0
    ),

    CONSTRAINT chk_polling_groups_timeout_ms CHECK
    (
        timeout_ms > 0
    ),

    CONSTRAINT chk_polling_groups_max_batch_size CHECK
    (
        max_batch_size > 0
    ),

    CONSTRAINT chk_polling_groups_priority CHECK
    (
        priority >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_polling_groups_device_id
    ON polling_groups(device_id);

CREATE INDEX IF NOT EXISTS ix_polling_groups_protocol
    ON polling_groups(protocol);

CREATE INDEX IF NOT EXISTS ix_polling_groups_mode
    ON polling_groups(mode);

CREATE INDEX IF NOT EXISTS ix_polling_groups_is_enabled
    ON polling_groups(is_enabled);

CREATE TABLE IF NOT EXISTS polling_tasks
(
    id UUID PRIMARY KEY,

    group_id UUID NOT NULL REFERENCES polling_groups(id) ON DELETE CASCADE,
    device_id UUID NOT NULL REFERENCES devices(id) ON DELETE CASCADE,
    tag_id UUID NOT NULL REFERENCES tags(id) ON DELETE CASCADE,

    protocol TEXT NOT NULL,

    address_protocol TEXT NOT NULL,
    address TEXT NOT NULL DEFAULT '',
    protocol_address_json TEXT NOT NULL DEFAULT '',

    bit_index INTEGER NULL,
    byte_offset INTEGER NULL,
    mask BIGINT NULL,
    shift INTEGER NULL,
    bit_length INTEGER NULL,

    state TEXT NOT NULL DEFAULT 'Waiting',

    interval_ms INTEGER NOT NULL DEFAULT 1000,
    timeout_ms INTEGER NOT NULL DEFAULT 1000,
    priority INTEGER NOT NULL DEFAULT 100,

    is_enabled BOOLEAN NOT NULL DEFAULT TRUE,

    description TEXT NOT NULL DEFAULT '',

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT uq_polling_tasks_group_tag UNIQUE (group_id, tag_id),

    CONSTRAINT chk_polling_tasks_protocol CHECK
    (
        protocol IN
        (
            'ModbusTcp',
            'Snmp',
            'Simulator',
            'Custom'
        )
    ),

    CONSTRAINT chk_polling_tasks_address_protocol CHECK
    (
        address_protocol IN
        (
            'ModbusTcp',
            'Snmp',
            'Simulator',
            'Custom'
        )
    ),

    CONSTRAINT chk_polling_tasks_protocol_match CHECK
    (
        protocol = address_protocol
    ),

    CONSTRAINT chk_polling_tasks_state CHECK
    (
        state IN
        (
            'Disabled',
            'Waiting',
            'Due',
            'Running',
            'Succeeded',
            'Failed',
            'Skipped'
        )
    ),

    CONSTRAINT chk_polling_tasks_interval_ms CHECK
    (
        interval_ms > 0
    ),

    CONSTRAINT chk_polling_tasks_timeout_ms CHECK
    (
        timeout_ms > 0
    ),

    CONSTRAINT chk_polling_tasks_priority CHECK
    (
        priority >= 0
    ),

    CONSTRAINT chk_polling_tasks_address_required CHECK
    (
        length(trim(address)) > 0
        OR length(trim(protocol_address_json)) > 0
    ),

    CONSTRAINT chk_polling_tasks_bit_index CHECK
    (
        bit_index IS NULL OR
        (
            bit_index >= 0
            AND bit_index <= 63
        )
    ),

    CONSTRAINT chk_polling_tasks_byte_offset CHECK
    (
        byte_offset IS NULL OR byte_offset >= 0
    ),

    CONSTRAINT chk_polling_tasks_mask CHECK
    (
        mask IS NULL OR mask >= 0
    ),

    CONSTRAINT chk_polling_tasks_shift CHECK
    (
        shift IS NULL OR
        (
            shift >= 0
            AND shift <= 63
        )
    ),

    CONSTRAINT chk_polling_tasks_bit_length CHECK
    (
        bit_length IS NULL OR
        (
            bit_length >= 1
            AND bit_length <= 64
        )
    )
);

CREATE INDEX IF NOT EXISTS ix_polling_tasks_group_id
    ON polling_tasks(group_id);

CREATE INDEX IF NOT EXISTS ix_polling_tasks_device_id
    ON polling_tasks(device_id);

CREATE INDEX IF NOT EXISTS ix_polling_tasks_tag_id
    ON polling_tasks(tag_id);

CREATE INDEX IF NOT EXISTS ix_polling_tasks_protocol
    ON polling_tasks(protocol);

CREATE INDEX IF NOT EXISTS ix_polling_tasks_state
    ON polling_tasks(state);

CREATE INDEX IF NOT EXISTS ix_polling_tasks_is_enabled
    ON polling_tasks(is_enabled);

COMMIT;