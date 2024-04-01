/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the treasure finite state machine logic.
 */

#pragma once

#include "../mob_types/mob_type.h"


/**
 * @brief Functions about the treasure's finite state machine and behavior.
 */
namespace treasure_fsm {
void create_fsm(mob_type* typ);

void respawn(mob* m, void* info1, void* info2);
void stand_still(mob* m, void* info1, void* info2);
}
