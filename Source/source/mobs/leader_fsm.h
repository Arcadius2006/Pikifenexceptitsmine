/*
 * Copyright (c) Andre 'Espyo' Silva 2013-2016.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included
 * README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the leader finite state machine logic.
 */

#ifndef LEADER_FSM_INCLUDED
#define LEADER_FSM_INCLUDED

#include "mob.h"
#include "mob_type.h"

namespace leader_fsm {
void create_fsm(mob_type* typ);

void be_dismissed(mob* m, void* info1, void* info2);
void be_grabbed_by_friend(mob* m, void* info1, void* info2);
void be_released(mob* m, void* info1, void* info2);
void be_thrown(mob* m, void* info1, void* info2);
void chase_leader(mob* m, void* info1, void* info2);
void die(mob* m, void* info1, void* info2);
void dismiss(mob* m, void* info1, void* info2);
void do_throw(mob* m, void* info1, void* info2);
void enter_active(mob* m, void* info1, void* info2);
void enter_idle(mob* m, void* info1, void* info2);
void fall_asleep(mob* m, void* info1, void* info2);
void fall_down_pit(mob* m, void* info1, void* info2);
void focus(mob* m, void* info1, void* info2);
void get_knocked_back(mob* m, void* info1, void* info2);
void go_pluck(mob* m, void* info1, void* info2);
void grab_mob(mob* m, void* info1, void* info2);
void inactive_die(mob* m, void* info1, void* info2);
void inactive_lose_health(mob* m, void* info1, void* info2);
void inactive_search_seed(mob* m, void* info1, void* info2);
void join_group(mob* m, void* info1, void* info2);
void land(mob* m, void* info1, void* info2);
void lose_health(mob* m, void* info1, void* info2);
void move(mob* m, void* info1, void* info2);
void release(mob* m, void* info1, void* info2);
void search_seed(mob* m, void* info1, void* info2);
void set_stop_anim(mob* m, void* info1, void* info2);
void set_walk_anim(mob* m, void* info1, void* info2);
void spray(mob* m, void* info1, void* info2);
void start_pluck(mob* m, void* info1, void* info2);
void start_waking_up(mob* m, void* info1, void* info2);
void stop(mob* m, void* info1, void* info2);
void stop_in_group(mob* m, void* info1, void* info2);
void stop_pluck(mob* m, void* info1, void* info2);
void stop_whistle(mob* m, void* info1, void* info2);
void suffer_pain(mob* m, void* info1, void* info2);
void unfocus(mob* m, void* info1, void* info2);
void whistle(mob* m, void* info1, void* info2);
}

#endif //ifndef LEADER_FSM_INCLUDED