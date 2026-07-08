-- Dispatcher
-- Migration: 0001_object_model
-- Status: draft
--
-- Purpose:
--   Defines the initial database schema draft for the Dispatcher object model.
--
-- Notes:
--   This migration is not executed by the application yet.
--   PostgreSQL integration will be added in a later sprint.
--   The schema is intentionally conservative and focused on the current domain model.

BEGIN;

CREATE TABLE IF NOT EXISTS object_nodes
(
    id UUID PRIMARY KEY,
    parent_id UUID NULL REFERENCES object_nodes(id) ON DELETE RESTRICT,

    object_type TEXT NOT NULL,
    code TEXT NOT NULL,
    name TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',

    sort_order INTEGER NOT NULL DEFAULT 0,
    is_enabled BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT uq_object_nodes_code UNIQUE (code),

    CONSTRAINT chk_object_nodes_object_type CHECK
    (
        object_type IN
        (
            'Site',
            'Building',
            'Floor',
            'Room',
            'Zone',
            'Cabinet',
            'Rack',
            'Line',
            'EquipmentGroup',
            'Custom'
        )
    ),

    CONSTRAINT chk_object_nodes_code_not_empty CHECK
    (
        length(trim(code)) > 0
    ),

    CONSTRAINT chk_object_nodes_name_not_empty CHECK
    (
        length(trim(name)) > 0
    ),

    CONSTRAINT chk_object_nodes_not_self_parent CHECK
    (
        parent_id IS NULL OR parent_id <> id
    )
);

CREATE INDEX IF NOT EXISTS ix_object_nodes_parent_id
    ON object_nodes(parent_id);

CREATE INDEX IF NOT EXISTS ix_object_nodes_object_type
    ON object_nodes(object_type);

CREATE INDEX IF NOT EXISTS ix_object_nodes_is_enabled
    ON object_nodes(is_enabled);

CREATE TABLE IF NOT EXISTS responsibility_zones
(
    id UUID PRIMARY KEY,

    code TEXT NOT NULL,
    name TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',

    is_enabled BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT uq_responsibility_zones_code UNIQUE (code),

    CONSTRAINT chk_responsibility_zones_code_not_empty CHECK
    (
        length(trim(code)) > 0
    ),

    CONSTRAINT chk_responsibility_zones_name_not_empty CHECK
    (
        length(trim(name)) > 0
    )
);

CREATE INDEX IF NOT EXISTS ix_responsibility_zones_is_enabled
    ON responsibility_zones(is_enabled);

CREATE TABLE IF NOT EXISTS responsibility_zone_objects
(
    zone_id UUID NOT NULL REFERENCES responsibility_zones(id) ON DELETE CASCADE,
    object_id UUID NOT NULL REFERENCES object_nodes(id) ON DELETE CASCADE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (zone_id, object_id)
);

CREATE INDEX IF NOT EXISTS ix_responsibility_zone_objects_object_id
    ON responsibility_zone_objects(object_id);

CREATE TABLE IF NOT EXISTS responsibility_zone_users
(
    zone_id UUID NOT NULL REFERENCES responsibility_zones(id) ON DELETE CASCADE,

    user_id UUID NOT NULL,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (zone_id, user_id)
);

CREATE INDEX IF NOT EXISTS ix_responsibility_zone_users_user_id
    ON responsibility_zone_users(user_id);

CREATE TABLE IF NOT EXISTS responsibility_zone_roles
(
    zone_id UUID NOT NULL REFERENCES responsibility_zones(id) ON DELETE CASCADE,

    role_code TEXT NOT NULL,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (zone_id, role_code),

    CONSTRAINT chk_responsibility_zone_roles_role_code_not_empty CHECK
    (
        length(trim(role_code)) > 0
    )
);

CREATE INDEX IF NOT EXISTS ix_responsibility_zone_roles_role_code
    ON responsibility_zone_roles(role_code);

COMMIT;