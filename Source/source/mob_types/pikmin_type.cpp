/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Pikmin type class and Pikmin type-related functions.
 */

#include "pikmin_type.h"

#include "../const.h"
#include "../functions.h"
#include "../game.h"
#include "../mob_fsms/pikmin_fsm.h"
#include "../mob_script.h"
#include "../mobs/leader.h"
#include "../utils/string_utils.h"
#include "../vars.h"

const float DEFAULT_SPROUT_EVOLUTION_TIME[N_MATURITIES] =
{ 2 * 60, 2 * 60, 3 * 60 };

/* ----------------------------------------------------------------------------
 * Creates a type of Pikmin.
 */
pikmin_type::pikmin_type() :
    mob_type(MOB_CATEGORY_PIKMIN),
    carry_strength(1),
    push_strength(1),
    max_throw_height(260),
    can_carry_tools(true),
    bmp_icon(nullptr) {
    
    for(size_t m = 0; m < N_MATURITIES; ++m) {
        sprout_evolution_time[m] = DEFAULT_SPROUT_EVOLUTION_TIME[m];
        bmp_top[m] = NULL;
        bmp_maturity_icon[m] = NULL;
    }
    
    weight = 1;
    show_health = false;
    
    mob_type::reach_struct idle_attack_reach;
    idle_attack_reach.angle_1 = TAU;
    idle_attack_reach.radius_1 = game.config.idle_task_range;
    reaches.push_back(idle_attack_reach);
    mob_type::reach_struct swarm_attack_reach;
    swarm_attack_reach.angle_1 = TAU;
    swarm_attack_reach.radius_1 = game.config.swarm_task_range;
    reaches.push_back(swarm_attack_reach);
    mob_type::reach_struct chase_reach;
    chase_reach.angle_1 = TAU;
    chase_reach.radius_1 = game.config.pikmin_chase_range;
    reaches.push_back(chase_reach);
    target_type = MOB_TARGET_TYPE_PLAYER;
    huntable_targets =
        MOB_TARGET_TYPE_PLAYER |
        MOB_TARGET_TYPE_ENEMY |
        MOB_TARGET_TYPE_WEAK_PLAIN_OBSTACLE |
        MOB_TARGET_TYPE_STRONG_PLAIN_OBSTACLE |
        MOB_TARGET_TYPE_PIKMIN_OBSTACLE |
        MOB_TARGET_TYPE_EXPLODABLE_PIKMIN_OBSTACLE;
    hurtable_targets =
        MOB_TARGET_TYPE_PLAYER |
        MOB_TARGET_TYPE_ENEMY |
        MOB_TARGET_TYPE_WEAK_PLAIN_OBSTACLE |
        MOB_TARGET_TYPE_STRONG_PLAIN_OBSTACLE |
        MOB_TARGET_TYPE_PIKMIN_OBSTACLE |
        MOB_TARGET_TYPE_EXPLODABLE_PIKMIN_OBSTACLE |
        MOB_TARGET_TYPE_FRAGILE;
        
    pikmin_fsm::create_fsm(this);
}


/* ----------------------------------------------------------------------------
 * Returns the vector of animation conversions.
 */
anim_conversion_vector pikmin_type::get_anim_conversions() {
    anim_conversion_vector v;
    v.push_back(std::make_pair(PIKMIN_ANIM_IDLING,     "idling"));
    v.push_back(std::make_pair(PIKMIN_ANIM_WALKING,    "walking"));
    v.push_back(std::make_pair(PIKMIN_ANIM_THROWN,     "thrown"));
    v.push_back(std::make_pair(PIKMIN_ANIM_ATTACKING,  "attacking"));
    v.push_back(std::make_pair(PIKMIN_ANIM_GRABBING,   "grabbing"));
    v.push_back(std::make_pair(PIKMIN_ANIM_SIGHING,    "sighing"));
    v.push_back(std::make_pair(PIKMIN_ANIM_CARRYING,   "carrying"));
    v.push_back(std::make_pair(PIKMIN_ANIM_SPROUT,     "sprout"));
    v.push_back(std::make_pair(PIKMIN_ANIM_PLUCKING,   "plucking"));
    v.push_back(std::make_pair(PIKMIN_ANIM_LYING,      "lying"));
    v.push_back(std::make_pair(PIKMIN_ANIM_DRINKING,   "drinking"));
    v.push_back(std::make_pair(PIKMIN_ANIM_PICKING_UP, "picking_up"));
    return v;
}


/* ----------------------------------------------------------------------------
 * Loads properties from a data file.
 */
void pikmin_type::load_properties(data_node* file) {
    reader_setter rs(file);
    
    rs.set("can_carry_tools", can_carry_tools);
    rs.set("carry_strength", carry_strength);
    rs.set("max_throw_height", max_throw_height);
    rs.set("push_strength", push_strength);
    rs.set("sprout_evolution_time_1", sprout_evolution_time[0]);
    rs.set("sprout_evolution_time_2", sprout_evolution_time[1]);
    rs.set("sprout_evolution_time_3", sprout_evolution_time[2]);
}


/* ----------------------------------------------------------------------------
 * Loads resources into memory.
 */
void pikmin_type::load_resources(data_node* file) {
    reader_setter rs(file);
    
    string top_leaf_str;
    string top_bud_str;
    string top_flower_str;
    string icon_str;
    string icon_leaf_str;
    string icon_bud_str;
    string icon_flower_str;
    data_node* top_leaf_node;
    data_node* top_bud_node;
    data_node* top_flower_node;
    data_node* icon_node;
    data_node* icon_leaf_node;
    data_node* icon_bud_node;
    data_node* icon_flower_node;
    
    rs.set("icon", icon_str, &icon_node);
    rs.set("icon_bud", icon_bud_str, &icon_bud_node);
    rs.set("icon_flower", icon_flower_str, &icon_flower_node);
    rs.set("icon_leaf", icon_leaf_str, &icon_leaf_node);
    rs.set("top_bud", top_bud_str, &top_bud_node);
    rs.set("top_flower", top_flower_str, &top_flower_node);
    rs.set("top_leaf", top_leaf_str, &top_leaf_node);
    
    bmp_icon = game.bitmaps.get(icon_str, icon_node);
    bmp_maturity_icon[0] = game.bitmaps.get(icon_leaf_str, icon_leaf_node);
    bmp_maturity_icon[1] = game.bitmaps.get(icon_bud_str, icon_bud_node);
    bmp_maturity_icon[2] = game.bitmaps.get(icon_flower_str, icon_flower_node);
    bmp_top[0] = game.bitmaps.get(top_leaf_str, top_leaf_node);
    bmp_top[1] = game.bitmaps.get(top_bud_str, top_bud_node);
    bmp_top[2] = game.bitmaps.get(top_flower_str, top_flower_node);
}


/* ----------------------------------------------------------------------------
 * Unloads resources from memory.
 */
void pikmin_type::unload_resources() {
    game.bitmaps.detach(bmp_icon);
    game.bitmaps.detach(bmp_maturity_icon[0]);
    game.bitmaps.detach(bmp_maturity_icon[1]);
    game.bitmaps.detach(bmp_maturity_icon[2]);
    game.bitmaps.detach(bmp_top[0]);
    game.bitmaps.detach(bmp_top[1]);
    game.bitmaps.detach(bmp_top[2]);
}
