-- Dispatcher
-- Migration: 0007_events_alarms
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for system events and alarms.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   Event Manager and Alarm Manager are not implemented yet.
--
--   This migration depends on:
--     - 0003_tag_model.sql
--
--   Events are append-oriented records.
--   Alarms have lifecycle state and transition history.

BEGIN;

CREATE TABLE IF NOT EXISTS event_records
(
    id UUID PRIMARY KEY,

    category TEXT NOT NULL,
    severity TEXT NOT NULL,
    source_type TEXT NOT NULL,

    code TEXT NOT NULL,
    title TEXT NOT NULL,
    message TEXT NOT NULL DEFAULT '',

    source_id TEXT NOT NULL DEFAULT '',
    object_id TEXT NOT NULL DEFAULT '',
    device_id TEXT NOT NULL DEFAULT '',
    tag_id TEXT NOT NULL DEFAULT '',

    correlation_id TEXT NOT NULL DEFAULT '',

    event_timestamp TIMESTAMPTZ NOT NULL,

    sequence_number BIGINT NOT NULL DEFAULT 0,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_event_records_category CHECK
    (
        category IN
        (
            'System',
            'Runtime',
            'Device',
            'Tag',
            'Communication',
            'Historian',
            'Security',
            'UserAction',
            'Integration',
            'Maintenance',
            'Alarm'
        )
    ),

    CONSTRAINT chk_event_records_severity CHECK
    (
        severity IN
        (
            'Trace',
            'Information',
            'Warning',
            'Error',
            'Critical'
        )
    ),

    CONSTRAINT chk_event_records_source_type CHECK
    (
        source_type IN
        (
            'Unknown',
            'System',
            'Object',
            'Device',
            'Tag',
            'Runtime',
            'Historian',
            'Polling',
            'Protocol',
            'Alarm',
            'User',
            'Integration',
            'Script'
        )
    ),

    CONSTRAINT chk_event_records_sequence_number CHECK
    (
        sequence_number >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_event_records_category
    ON event_records(category);

CREATE INDEX IF NOT EXISTS ix_event_records_severity
    ON event_records(severity);

CREATE INDEX IF NOT EXISTS ix_event_records_source_type
    ON event_records(source_type);

CREATE INDEX IF NOT EXISTS ix_event_records_source_id
    ON event_records(source_id);

CREATE INDEX IF NOT EXISTS ix_event_records_object_id
    ON event_records(object_id);

CREATE INDEX IF NOT EXISTS ix_event_records_device_id
    ON event_records(device_id);

CREATE INDEX IF NOT EXISTS ix_event_records_tag_id
    ON event_records(tag_id);

CREATE INDEX IF NOT EXISTS ix_event_records_correlation_id
    ON event_records(correlation_id);

CREATE INDEX IF NOT EXISTS ix_event_records_event_timestamp
    ON event_records(event_timestamp DESC);

CREATE TABLE IF NOT EXISTS alarm_records
(
    id UUID PRIMARY KEY,

    event_id UUID NULL REFERENCES event_records(id) ON DELETE SET NULL,

    source_type TEXT NOT NULL,
    severity TEXT NOT NULL,
    priority TEXT NOT NULL,
    state TEXT NOT NULL,

    code TEXT NOT NULL,
    title TEXT NOT NULL,
    message TEXT NOT NULL DEFAULT '',

    source_id TEXT NOT NULL DEFAULT '',
    object_id TEXT NOT NULL DEFAULT '',
    device_id TEXT NOT NULL DEFAULT '',
    tag_id TEXT NOT NULL DEFAULT '',

    correlation_id TEXT NOT NULL DEFAULT '',

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    activated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    acknowledged_at TIMESTAMPTZ NULL,
    cleared_at TIMESTAMPTZ NULL,
    closed_at TIMESTAMPTZ NULL,

    acknowledged_by TEXT NOT NULL DEFAULT '',
    closed_by TEXT NOT NULL DEFAULT '',

    occurrence_count BIGINT NOT NULL DEFAULT 0,

    requires_acknowledgement BOOLEAN NOT NULL DEFAULT TRUE,

    CONSTRAINT chk_alarm_records_source_type CHECK
    (
        source_type IN
        (
            'Unknown',
            'System',
            'Object',
            'Device',
            'Tag',
            'Communication',
            'Runtime',
            'Historian',
            'Polling',
            'Protocol',
            'Script',
            'Integration',
            'Manual'
        )
    ),

    CONSTRAINT chk_alarm_records_severity CHECK
    (
        severity IN
        (
            'Information',
            'Warning',
            'Minor',
            'Major',
            'Critical'
        )
    ),

    CONSTRAINT chk_alarm_records_priority CHECK
    (
        priority IN
        (
            'Low',
            'Medium',
            'High',
            'Critical'
        )
    ),

    CONSTRAINT chk_alarm_records_state CHECK
    (
        state IN
        (
            'New',
            'Active',
            'Acknowledged',
            'Cleared',
            'Closed',
            'Shelved',
            'Suppressed'
        )
    ),

    CONSTRAINT chk_alarm_records_occurrence_count CHECK
    (
        occurrence_count >= 0
    )
);

CREATE INDEX IF NOT EXISTS ix_alarm_records_event_id
    ON alarm_records(event_id);

CREATE INDEX IF NOT EXISTS ix_alarm_records_state
    ON alarm_records(state);

CREATE INDEX IF NOT EXISTS ix_alarm_records_severity
    ON alarm_records(severity);

CREATE INDEX IF NOT EXISTS ix_alarm_records_priority
    ON alarm_records(priority);

CREATE INDEX IF NOT EXISTS ix_alarm_records_source_type
    ON alarm_records(source_type);

CREATE INDEX IF NOT EXISTS ix_alarm_records_source_id
    ON alarm_records(source_id);

CREATE INDEX IF NOT EXISTS ix_alarm_records_object_id
    ON alarm_records(object_id);

CREATE INDEX IF NOT EXISTS ix_alarm_records_device_id
    ON alarm_records(device_id);

CREATE INDEX IF NOT EXISTS ix_alarm_records_tag_id
    ON alarm_records(tag_id);

CREATE INDEX IF NOT EXISTS ix_alarm_records_correlation_id
    ON alarm_records(correlation_id);

CREATE INDEX IF NOT EXISTS ix_alarm_records_created_at
    ON alarm_records(created_at DESC);

CREATE INDEX IF NOT EXISTS ix_alarm_records_active
    ON alarm_records(state, priority, severity, created_at DESC)
    WHERE state IN ('New', 'Active', 'Acknowledged');

CREATE TABLE IF NOT EXISTS alarm_transitions
(
    id UUID PRIMARY KEY,

    alarm_id UUID NOT NULL REFERENCES alarm_records(id) ON DELETE CASCADE,

    transition_type TEXT NOT NULL,

    previous_state TEXT NOT NULL,
    new_state TEXT NOT NULL,

    transition_timestamp TIMESTAMPTZ NOT NULL,

    actor TEXT NOT NULL DEFAULT '',
    reason TEXT NOT NULL DEFAULT '',

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_alarm_transitions_transition_type CHECK
    (
        transition_type IN
        (
            'Activate',
            'Acknowledge',
            'Clear',
            'Close',
            'Shelve',
            'Unshelve',
            'Suppress',
            'Unsuppress',
            'Reactivate'
        )
    ),

    CONSTRAINT chk_alarm_transitions_previous_state CHECK
    (
        previous_state IN
        (
            'New',
            'Active',
            'Acknowledged',
            'Cleared',
            'Closed',
            'Shelved',
            'Suppressed'
        )
    ),

    CONSTRAINT chk_alarm_transitions_new_state CHECK
    (
        new_state IN
        (
            'New',
            'Active',
            'Acknowledged',
            'Cleared',
            'Closed',
            'Shelved',
            'Suppressed'
        )
    )
);

CREATE INDEX IF NOT EXISTS ix_alarm_transitions_alarm_id
    ON alarm_transitions(alarm_id);

CREATE INDEX IF NOT EXISTS ix_alarm_transitions_transition_timestamp
    ON alarm_transitions(transition_timestamp DESC);

CREATE INDEX IF NOT EXISTS ix_alarm_transitions_type
    ON alarm_transitions(transition_type);

CREATE TABLE IF NOT EXISTS alarm_rules
(
    id UUID PRIMARY KEY,

    rule_type TEXT NOT NULL,
    rule_state TEXT NOT NULL,

    source_type TEXT NOT NULL,
    severity TEXT NOT NULL,
    priority TEXT NOT NULL,
    comparison TEXT NOT NULL,

    code TEXT NOT NULL,
    title TEXT NOT NULL,
    message TEXT NOT NULL DEFAULT '',

    tag_id UUID NULL REFERENCES tags(id) ON DELETE CASCADE,
    object_id UUID NULL,
    device_id UUID NULL,

    threshold DOUBLE PRECISION NOT NULL DEFAULT 0.0,

    expected_quality TEXT NOT NULL DEFAULT '',

    require_good_quality BOOLEAN NOT NULL DEFAULT FALSE,
    requires_acknowledgement BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT chk_alarm_rules_rule_type CHECK
    (
        rule_type IN
        (
            'NumericThreshold',
            'QualityEquals',
            'QualityNotGood',
            'QualityBad'
        )
    ),

    CONSTRAINT chk_alarm_rules_rule_state CHECK
    (
        rule_state IN
        (
            'Draft',
            'Enabled',
            'Disabled'
        )
    ),

    CONSTRAINT chk_alarm_rules_source_type CHECK
    (
        source_type IN
        (
            'Unknown',
            'System',
            'Object',
            'Device',
            'Tag',
            'Communication',
            'Runtime',
            'Historian',
            'Polling',
            'Protocol',
            'Script',
            'Integration',
            'Manual'
        )
    ),

    CONSTRAINT chk_alarm_rules_severity CHECK
    (
        severity IN
        (
            'Information',
            'Warning',
            'Minor',
            'Major',
            'Critical'
        )
    ),

    CONSTRAINT chk_alarm_rules_priority CHECK
    (
        priority IN
        (
            'Low',
            'Medium',
            'High',
            'Critical'
        )
    ),

    CONSTRAINT chk_alarm_rules_comparison CHECK
    (
        comparison IN
        (
            'GreaterThan',
            'GreaterOrEqual',
            'LessThan',
            'LessOrEqual',
            'Equal',
            'NotEqual'
        )
    ),

    CONSTRAINT chk_alarm_rules_expected_quality CHECK
    (
        expected_quality = ''
        OR expected_quality IN
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
    )
);

CREATE INDEX IF NOT EXISTS ix_alarm_rules_rule_state
    ON alarm_rules(rule_state);

CREATE INDEX IF NOT EXISTS ix_alarm_rules_rule_type
    ON alarm_rules(rule_type);

CREATE INDEX IF NOT EXISTS ix_alarm_rules_tag_id
    ON alarm_rules(tag_id);

CREATE INDEX IF NOT EXISTS ix_alarm_rules_object_id
    ON alarm_rules(object_id);

CREATE INDEX IF NOT EXISTS ix_alarm_rules_device_id
    ON alarm_rules(device_id);

CREATE INDEX IF NOT EXISTS ix_alarm_rules_enabled
    ON alarm_rules(rule_state, rule_type)
    WHERE rule_state = 'Enabled';

COMMIT;
