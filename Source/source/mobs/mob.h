/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the mob class and mob-related functions.
 */

#pragma once

#include <float.h>
#include <map>
#include <vector>

#include <allegro5/allegro.h>

#include "../animation.h"
#include "../area/sector.h"
#include "../game_states/gameplay/in_world_hud.h"
#include "../misc_structs.h"
#include "../mob_script.h"
#include "../mob_script_action.h"
#include "../particle.h"
#include "../status.h"
#include "mob_utils.h"


using std::map;
using std::size_t;
using std::string;
using std::vector;


class mob_type;
class pikmin_type;

extern size_t next_mob_id;

namespace MOB {
extern const float CARRIED_MOB_ACCELERATION;
extern const float CARRY_STUCK_CIRCLING_RADIUS;
extern const float CARRY_STUCK_SPEED_MULTIPLIER;
extern const float CARRY_SWAY_TIME_MULT;
extern const float CARRY_SWAY_X_TRANSLATION_AMOUNT;
extern const float CARRY_SWAY_Y_TRANSLATION_AMOUNT;
extern const float CARRY_SWAY_ROTATION_AMOUNT;
extern const float DELIVERY_SUCK_SHAKING_TIME_MULT;
extern const float DELIVERY_SUCK_SHAKING_MULT;
extern const float DELIVERY_SUCK_TIME;
extern const float DELIVERY_TOSS_MULT;
extern const float DELIVERY_TOSS_TIME;
extern const float DELIVERY_TOSS_WINDUP_MULT;
extern const float DELIVERY_TOSS_X_OFFSET;
extern const float DAMAGE_SQUASH_DURATION;
extern const float DAMAGE_SQUASH_AMOUNT;
extern const float FREE_MOVE_THRESHOLD;
extern const float GRAVITY_ADDER;
extern const float GROUP_SHUFFLE_DIST;
extern const float GROUP_SPOT_INTERVAL;
extern const float GROUP_SPOT_MAX_DEVIATION;
extern const float HEIGHT_EFFECT_FACTOR;
extern const float KNOCKBACK_H_POWER;
extern const float KNOCKBACK_V_POWER;
extern const float OPPONENT_HIT_REGISTER_TIMEOUT;
extern const float PIKMIN_NEST_CALL_INTERVAL;
extern const float PUSH_EXTRA_AMOUNT;
extern const float PUSH_SOFTLY_AMOUNT;
extern const float PUSH_THROTTLE_FACTOR;
extern const float PUSH_THROTTLE_TIMEOUT;
extern const float SHADOW_STRETCH_MULT;
extern const float SHADOW_Y_MULT;
extern const float SMACK_PARTICLE_DUR;
extern const float SWARM_MARGIN;
extern const float SWARM_VERTICAL_SCALE;
extern const float STATUS_SHAKING_TIME_MULT;
extern const float THROW_PARTICLE_INTERVAL;
extern const float WAVE_RING_DURATION;
};



/**
 * @brief A mob, short for "mobile object" or "map object",
 * or whatever tickles your fancy, is any instance of
 * an object in the game world. It can move, follow a point,
 * has health, and can be a variety of different sub-types,
 * like leader, Pikmin, enemy, Onion, etc.
 */
class mob {

public:

    //--- Members ---
    
    //-Basic information-
    
    //What type of (generic) mob it is. (e.g. Olimar, Red Bulborb, etc.)
    mob_type* type = nullptr;
    
    //Schedule this mob to be deleted from memory at the end of the frame.
    bool to_delete = false;
    
    //-Position-
    
    //Coordinates.
    point pos;
    
    //Z coordinate. This is height; the higher the value, the higher in the sky.
    float z = 0.0f;
    
    //Current facing angle. 0 = right, PI / 2 = up, etc.
    float angle = 0.0f;
    
    //The highest ground below the entire mob.
    sector* ground_sector = nullptr;
    
    //Sector that the mob's center is on.
    sector* center_sector = nullptr;
    
    //Mob this mob is standing on top of, if any.
    mob* standing_on_mob = nullptr;
    
    //-Basic movement-
    
    //X/Y speed at which external movement is applied (i.e. not walking).
    point speed;
    
    //Same as speed, but for the Z coordinate.
    float speed_z = 0.0f;
    
    //Due to framerate imperfections, thrown Pikmin/leaders can reach higher
    //than intended. z_cap forces a cap. FLT_MAX = no cap.
    float z_cap = FLT_MAX;
    
    //Multiply the mob's gravity by this.
    float gravity_mult = 1.0f;
    
    //How much it's being pushed by another mob.
    float push_amount = 0.0f;
    
    //Angle that another mob is pushing it to.
    float push_angle = 0.0f;
    
    //How much the mob moved this frame, if it's walkable.
    point walkable_moved;
    
    //-Complex states-
    
    //Information about what it is chasing after.
    chase_t chase_info;
    
    //Information about the path it is following, if any.
    path_t* path_info = nullptr;
    
    //Information about the mob/point it's circling, if any.
    circling_t* circling_info = nullptr;
    
    //Riding a track. If nullptr, the mob is not riding on any track.
    track_t* track_info = nullptr;
    
    //Info on how this mob should be carried. Uncarriable if nullptr.
    carry_t* carry_info = nullptr;
    
    //Onion delivery info. If nullptr, the mob is not being delivered.
    delivery_t* delivery_info = nullptr;
    
    //-Physical space-
    
    //Current radius.
    float radius = 0.0f;
    
    //Current height.
    float height = 0.0f;
    
    //Current rectangular dimensions.
    point rectangular_dim;
    
    //-Scripting-
    
    //Finite-state machine.
    mob_fsm fsm;
    
    //The script-controlled timer.
    timer script_timer;
    
    //Variables.
    map<string, string> vars;
    
    //-Brain and behavior-
    
    //The mob it has focus on.
    mob* focused_mob = nullptr;
    
    //Further memory of focused mobs.
    map<size_t, mob*> focused_mob_memory;
    
    //Angle the mob wants to be facing.
    float intended_turn_angle;
    
    //Variable that holds the position the mob wants to be facing.
    point* intended_turn_pos = nullptr;
    
    //Starting coordinates; what the mob calls "home".
    point home;
    
    //Index of the reach to use for "X in reach" events.
    size_t far_reach = INVALID;
    
    //Index or the reach to use for "focused mob out of reach" events.
    size_t near_reach = INVALID;
    
    //How long it's been alive for.
    float time_alive = 0.0f;
    
    //Incremental ID. Used for misc. things.
    size_t id = 0;
    
    //-General state-
    
    //Current health.
    float health = 0.0f;
    
    //Maximum health.
    float max_health = 0.0f;
    
    //During this period, the mob cannot be attacked.
    timer invuln_period;
    
    //Mobs that it just hit. Used to stop hitboxes from hitting every frame.
    vector<std::pair<float, mob*> > hit_opponents;
    
    //How much damage did it take since the last time the itch event triggered?
    float itch_damage = 0.0f;
    
    //How much time has passed the last time the itch event triggered?
    float itch_time = 0.0f;
    
    //Status effects currently inflicted on the mob.
    vector<status> statuses;
    
    //Hazard of the sector the mob is currently on.
    hazard* on_hazard = nullptr;
    
    //If this mob is a sub-mob, this points to the parent mob.
    parent_t* parent = nullptr;
    
    //Miscellanous flags. Use MOB_FLAG_*.
    bitmask_16_t flags = 0;
    
    //-Interactions with other mobs-
    
    //Mobs it is linked to.
    vector<mob*> links;
    
    //If it's being held by another mob, the information is kept here.
    hold_t holder;
    
    //List of mobs it is holding.
    vector<mob*> holding;
    
    //If it's stored inside another mob, this indicates which mob it is.
    mob* stored_inside_another = nullptr;
    
    //List of body parts that will chomp Pikmin.
    vector<int> chomp_body_parts;
    
    //List of mobs currently in its mouth, i.e., chomped.
    vector<mob*> chomping_mobs;
    
    //Max number of mobs it can chomp in the current attack.
    size_t chomp_max = 0;
    
    //Mob's team (who it can damage).
    MOB_TEAM team = MOB_TEAM_NONE;
    
    //-Group-
    
    //The current mob is following this mob's group.
    mob* following_group = nullptr;
    
    //Index of this mob's spot in the leader's group spots.
    size_t group_spot_idx = INVALID;
    
    //The current subgroup type.
    subgroup_type* subgroup_type_ptr = nullptr;
    
    //Info on the group this mob is a leader of, if any.
    group_t* group = nullptr;
    
    //-Animation-
    
    //Current animation instance.
    animation_instance anim;
    
    //Force the usage of this specific sprite.
    sprite* forced_sprite = nullptr;
    
    //-Aesthetic-
    
    //If not LARGE_FLOAT, compare the Z with this to shrink/grow the sprite.
    float height_effect_pivot = LARGE_FLOAT;
    
    //Time left in the current damage squash-and-stretch animation.
    float damage_squash_time = 0.0f;
    
    //Particle generators attached to it.
    vector<particle_generator> particle_generators;
    
    //Data about its on-screen health wheel, if any.
    in_world_health_wheel* health_wheel = nullptr;
    
    //Data about its on-screen fraction numbers, if any.
    in_world_fraction* fraction = nullptr;
    
    //-Caches-
    
    //Cached value of the angle's cosine.
    float angle_cos = 0.0f;
    
    //Cached value of the angle's sine.
    float angle_sin = 0.0f;
    
    //Cached value of how far its hitboxes or radius can reach from the center.
    float max_span = 0.0f;
    
    //It's invisible due to a status effect. Cache for performance.
    bool has_invisibility_status = false;
    
    
    //--- Function declarations ---
    
    mob(const point &pos, mob_type* type, const float angle);
    virtual ~mob();
    
    void tick(const float delta_t);
    void draw_limb();
    virtual void draw_mob();
    
    void set_animation(
        const size_t nr,
        const bool pre_named = true,
        const START_ANIM_OPTION options = START_ANIM_OPTION_NORMAL
    );
    void set_animation(
        const string &name,
        const START_ANIM_OPTION options = START_ANIM_OPTION_NORMAL
    );
    void set_health(const bool add, const bool ratio, const float amount);
    void set_timer(const float time);
    void set_var(const string &name, const string &value);
    void set_radius(const float radius);
    void set_rectangular_dim(const point &rectangular_dim);
    void set_can_block_paths(const bool blocks);
    
    void become_carriable(const CARRY_DESTINATION destination);
    void become_uncarriable();
    
    void apply_attack_damage(
        mob* attacker, hitbox* attack_h, hitbox* victim_h, float damage
    );
    void add_to_group(mob* new_member);
    void apply_knockback(const float knockback, const float knockback_angle);
    bool calculate_carrying_destination(
        mob* added, mob* removed,
        pikmin_type** target_type, mob** target_mob, point* target_point
    ) const;
    bool calculate_damage(
        mob* victim, hitbox* attack_h, const hitbox* victim_h, float* damage
    ) const;
    void calculate_knockback(
        const mob* victim, const hitbox* attack_h,
        hitbox* victim_h, float* knockback, float* angle
    ) const;
    void cause_spike_damage(mob* victim, const bool is_ingestion);
    void chomp(mob* m, const hitbox* hitbox_info);
    void get_sprite_data(
        sprite** out_cur_sprite_ptr, sprite** out_next_sprite_ptr,
        float* out_interpolation_factor
    ) const;
    void get_hitbox_hold_point(
        const mob* mob_to_hold, const hitbox* h_ptr,
        float* offset_dist, float* offset_angle, float* vertical_dist
    ) const;
    size_t get_latched_pikmin_amount() const;
    float get_latched_pikmin_weight() const;
    void do_attack_effects(
        const mob* attacker, const hitbox* attack_h, const hitbox* victim_h,
        const float damage, const float knockback
    );
    bool is_stored_inside_mob() const;
    bool is_off_camera() const;
    bool is_point_on(const point &p) const;
    void focus_on_mob(mob* m);
    void unfocus_from_mob();
    void leave_group();
    void hold(
        mob* m, const size_t hitbox_idx,
        const float offset_dist, const float offset_angle,
        const float vertical_dist,
        const bool above_holder, const HOLD_ROTATION_METHOD rotation_method
    );
    void release(mob* m);
    bool can_hurt(mob* m) const;
    bool can_hunt(mob* m) const;
    mob_type::vulnerability_t get_hazard_vulnerability(
        hazard* h_ptr
    ) const;
    bool is_resistant_to_hazards(const vector<hazard*> &hazards) const;
    size_t play_sound(size_t sfx_data_idx);
    void swallow_chomped_pikmin(size_t nr);
    void start_height_effect();
    void stop_height_effect();
    void store_mob_inside(mob* m);
    void release_chomped_pikmin();
    void release_stored_mobs();
    void send_message(mob* receiver, string &msg) const;
    mob* spawn(const mob_type::spawn_t* info, mob_type* type_ptr = nullptr);
    void start_dying();
    void finish_dying();
    void respawn();
    dist get_distance_between(
        const mob* m2_ptr, const dist* regular_distance_cache = nullptr
    ) const;
    hitbox* get_hitbox(const size_t nr) const;
    hitbox* get_closest_hitbox(
        const point &p, const size_t h_type = INVALID, dist* d = nullptr
    ) const;
    bool has_clear_line(const mob* target_mob) const;
    
    void chase(
        point* orig_coords, float* orig_z,
        const point &offset = point(), const float offset_z = 0.0f,
        const unsigned char flags = 0,
        const float target_distance = PATHS::DEF_CHASE_TARGET_DISTANCE,
        const float speed = LARGE_FLOAT, const float acceleration = LARGE_FLOAT
    );
    void chase(
        const point &coords, const float coords_z,
        const bitmask_8_t flags = 0,
        const float target_distance = PATHS::DEF_CHASE_TARGET_DISTANCE,
        const float speed = LARGE_FLOAT, const float acceleration = LARGE_FLOAT
    );
    void stop_chasing();
    void stop_turning();
    bool follow_path(
        const path_follow_settings &settings,
        const float speed, const float acceleration
    );
    void stop_following_path();
    void circle_around(
        mob* m, const point &p, const float radius, const bool clockwise,
        const float speed, const bool can_free_move
    );
    void stop_circling();
    void face(
        const float new_angle, point* new_pos, const bool instantly = false
    );
    point get_chase_target(float* out_z = nullptr) const;
    virtual float get_base_speed() const;
    float get_speed_multiplier() const;
    
    void arachnorb_head_turn_logic();
    void arachnorb_plan_logic(const MOB_ACTION_ARACHNORB_PLAN_LOGIC_TYPE goal);
    void arachnorb_foot_move_logic();
    
    void apply_status_effect(
        status_type* s, const bool given_by_parent, const bool from_hazard
    );
    void delete_old_status_effects();
    void remove_particle_generator(const MOB_PARTICLE_GENERATOR_ID id);
    ALLEGRO_BITMAP* get_status_bitmap(float* bmp_scale) const;
    virtual bool can_receive_status(status_type* s) const;
    virtual void get_group_spot_info(
        point* out_spot, float* out_dist
    ) const;
    virtual bool get_fraction_numbers_info(
        float* fraction_value_nr, float* fraction_req_nr,
        ALLEGRO_COLOR* fraction_color
    ) const;
    virtual void handle_status_effect_gain(status_type* sta_type);
    virtual void handle_status_effect_loss(status_type* sta_type);
    virtual void read_script_vars(const script_var_reader &svr);
    virtual void start_dying_class_specifics();
    virtual void finish_dying_class_specifics();
    bool tick_track_ride();
    void stop_track_ride();
    
    void get_sprite_bitmap_effects(
        sprite* s_ptr, sprite* next_s_ptr, float interpolation_factor,
        bitmap_effect_t* info, bitmask_16_t effects
    ) const;
    
    string print_state_history() const;
    
protected:

    //--- Members ---
    
    //Is it currently capable of blocking paths?
    bool can_block_paths;
    
    
    //--- Function declarations ---
    
    pikmin_type* decide_carry_pikmin_type(
        const unordered_set<pikmin_type*> &available_types,
        mob* added, mob* removed
    ) const;
    mob* get_mob_to_walk_on() const;
    H_MOVE_RESULT get_movement_edge_intersections(
        const point &new_pos, vector<edge*>* intersecting_edges
    ) const;
    H_MOVE_RESULT get_physics_horizontal_movement(
        const float delta_t, const float move_speed_mult, point* move_speed
    );
    H_MOVE_RESULT get_wall_slide_angle(
        const edge* e_ptr, unsigned char wall_sector, const float move_angle,
        float* slide_angle
    ) const;
    void move_to_path_end(const float speed, const float acceleration);
    void tick_animation(const float delta_t);
    void tick_brain(const float delta_t);
    void tick_horizontal_movement_physics(
        const float delta_t, const point &attempted_move_speed,
        bool* touched_wall
    );
    void tick_misc_logic(const float delta_t);
    void tick_physics(const float delta_t);
    void tick_rotation_physics(
        const float delta_t, const float move_speed_mult
    );
    void tick_script(const float delta_t);
    void tick_vertical_movement_physics(
        const float delta_t, const float pre_move_ground_z,
        const bool was_teleport = false
    );
    void tick_walkable_riding_physics(const float delta_t);
    virtual void tick_class_specifics(const float delta_t);
    
};


/**
 * @brief See mob_type_with_anim_groups.
 */
class mob_with_anim_groups {

public:

    //--- Members ---
    
    //Index number of its current base animation.
    size_t cur_base_anim_idx = INVALID;
    
    
    //--- Function declarations ---
    
    size_t get_animation_idx_from_base_and_group(
        const size_t base_anim_idx, const size_t group_idx,
        const size_t base_anim_total
    ) const;
    
};
