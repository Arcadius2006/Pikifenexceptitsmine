/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Leader mob category class.
 */

#include <algorithm>

#include "leader_category.h"

#include "../game.h"
#include "../mobs/leader.h"


/* ----------------------------------------------------------------------------
 * Creates an instance of the leader category.
 */
leader_category::leader_category() :
    mob_category(
        MOB_CATEGORY_LEADERS, "Leader", "Leaders",
        "Leaders", al_map_rgb(73, 204, 204)
    ) {
    
}


/* ----------------------------------------------------------------------------
 * Clears the list of registered types of leader.
 */
void leader_category::clear_types() {
    for(auto &t : game.mob_types.leader) {
        delete t.second;
    }
    game.mob_types.leader.clear();
}


/* ----------------------------------------------------------------------------
 * Creates a leader and adds it to the list of leaders.
 */
mob* leader_category::create_mob(
    const point &pos, mob_type* type, const float angle
) {
    leader* m = new leader(pos, (leader_type*) type, angle);
    game.gameplay_state->mobs.leader.push_back(m);
    return m;
}


/* ----------------------------------------------------------------------------
 * Creates a new, empty type of leader.
 */
mob_type* leader_category::create_type() {
    return new leader_type();
}


/* ----------------------------------------------------------------------------
 * Clears a leader from the list of leaders.
 */
void leader_category::erase_mob(mob* m) {
    game.gameplay_state->mobs.leader.erase(
        find(game.gameplay_state->mobs.leader.begin(), game.gameplay_state->mobs.leader.end(), (leader*) m)
    );
}


/* ----------------------------------------------------------------------------
 * Returns a type of leader given its name, or NULL on error.
 */
mob_type* leader_category::get_type(const string &name) {
    auto it = game.mob_types.leader.find(name);
    if(it == game.mob_types.leader.end()) return NULL;
    return it->second;
}


/* ----------------------------------------------------------------------------
 * Returns all types of leader by name.
 */
void leader_category::get_type_names(vector<string> &list) {
    for(auto &t : game.mob_types.leader) {
        list.push_back(t.first);
    }
}


/* ----------------------------------------------------------------------------
 * Registers a created type of leader.
 */
void leader_category::register_type(mob_type* type) {
    game.mob_types.leader[type->name] = (leader_type*) type;
}
