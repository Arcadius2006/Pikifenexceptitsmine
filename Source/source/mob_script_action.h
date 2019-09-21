/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the mob script action classes and
 * related functions.
 */


#ifndef MOB_SCRIPT_ACTION_INCLUDED
#define MOB_SCRIPT_ACTION_INCLUDED

#include "mob_script.h"


//Types of script action.
enum MOB_ACTION_TYPES {
    MOB_ACTION_UNKNOWN,
    MOB_ACTION_ADD_HEALTH,
    MOB_ACTION_ARACHNORB_PLAN_LOGIC,
    MOB_ACTION_CALCULATE,
    MOB_ACTION_DELETE,
    MOB_ACTION_ELSE,
    MOB_ACTION_END_IF,
    MOB_ACTION_FINISH_DYING,
    MOB_ACTION_FOCUS,
    MOB_ACTION_GET_CHOMPED,
    MOB_ACTION_IF,
    MOB_ACTION_MOVE_TO_ABSOLUTE,
    MOB_ACTION_MOVE_TO_RELATIVE,
    MOB_ACTION_MOVE_TO_TARGET,
    MOB_ACTION_ORDER_RELEASE,
    MOB_ACTION_PLAY_SOUND,
    MOB_ACTION_RANDOMIZE_TIMER,
    MOB_ACTION_RANDOMIZE_VAR,
    MOB_ACTION_RECEIVE_STATUS,
    MOB_ACTION_RELEASE,
    MOB_ACTION_REMOVE_STATUS,
    MOB_ACTION_SEND_MESSAGE_TO_LINKS,
    MOB_ACTION_SEND_MESSAGE_TO_NEARBY,
    MOB_ACTION_SET_ANIMATION,
    MOB_ACTION_SET_FAR_REACH,
    MOB_ACTION_SET_GRAVITY,
    MOB_ACTION_SET_HEALTH,
    MOB_ACTION_SET_HIDING,
    MOB_ACTION_SET_HOLDABLE,
    MOB_ACTION_SET_LIMB_ANIMATION,
    MOB_ACTION_SET_NEAR_REACH,
    MOB_ACTION_SET_STATE,
    MOB_ACTION_SET_TANGIBLE,
    MOB_ACTION_SET_TEAM,
    MOB_ACTION_SET_TIMER,
    MOB_ACTION_SET_VAR,
    MOB_ACTION_SHOW_MESSAGE_FROM_VAR,
    MOB_ACTION_SPAWN,
    MOB_ACTION_STABILIZE_Z,
    MOB_ACTION_START_CHOMPING,
    MOB_ACTION_START_DYING,
    MOB_ACTION_START_HEIGHT_EFFECT,
    MOB_ACTION_START_PARTICLES,
    MOB_ACTION_STOP,
    MOB_ACTION_STOP_CHOMPING,
    MOB_ACTION_STOP_HEIGHT_EFFECT,
    MOB_ACTION_STOP_PARTICLES,
    MOB_ACTION_STOP_VERTICALLY,
    MOB_ACTION_SWALLOW,
    MOB_ACTION_SWALLOW_ALL,
    MOB_ACTION_TELEPORT_TO_ABSOLUTE,
    MOB_ACTION_TELEPORT_TO_RELATIVE,
    MOB_ACTION_TURN_TO_ABSOLUTE,
    MOB_ACTION_TURN_TO_RELATIVE,
    MOB_ACTION_TURN_TO_TARGET,
    
    N_MOB_ACTIONS
};

//Arachnorb plan logic action sub-types.
enum MOB_ACTION_ARACHNORB_PLAN_LOGIC_TYPES {
    MOB_ACTION_ARACHNORB_PLAN_LOGIC_HOME,
    MOB_ACTION_ARACHNORB_PLAN_LOGIC_FORWARD,
    MOB_ACTION_ARACHNORB_PLAN_LOGIC_CW_TURN,
    MOB_ACTION_ARACHNORB_PLAN_LOGIC_CCW_TURN,
};

//Face action sub-types.
enum MOB_ACTION_TURN_TYPES {
    MOB_ACTION_TURN_ARACHNORB_HEAD_LOGIC,
    MOB_ACTION_TURN_FOCUSED_MOB,
    MOB_ACTION_TURN_HOME,
    MOB_ACTION_TURN_RANDOMLY,
};

//Focus action sub-types.
enum MOB_ACTION_FOCUS_TYPES {
    MOB_ACTION_FOCUS_TRIGGER,
    MOB_ACTION_FOCUS_PARENT,
};

//If action operator types.
enum MOB_ACTION_IF_OPERATOR_TYPES {
    MOB_ACTION_IF_OP_EQUAL,
    MOB_ACTION_IF_OP_NOT,
    MOB_ACTION_IF_OP_LESS,
    MOB_ACTION_IF_OP_MORE,
    MOB_ACTION_IF_OP_LESS_E,
    MOB_ACTION_IF_OP_MORE_E,
};

//If action LHS comparand type.
enum MOB_ACTION_IF_LHS_TYPES {
    MOB_ACTION_IF_LHS_VAR,
    MOB_ACTION_IF_LHS_BODY_PART,
    MOB_ACTION_IF_LHS_CHOMPED_PIKMIN,
    MOB_ACTION_IF_LHS_DAY_MINUTES,
    MOB_ACTION_IF_LHS_FIELD_PIKMIN,
    MOB_ACTION_IF_LHS_FRAME_SIGNAL,
    MOB_ACTION_IF_LHS_HEALTH,
    MOB_ACTION_IF_LHS_LATCHED_PIKMIN,
    MOB_ACTION_IF_LHS_LATCHED_PIKMIN_WEIGHT,
    MOB_ACTION_IF_LHS_MESSAGE,
    MOB_ACTION_IF_LHS_MESSAGE_SENDER,
    MOB_ACTION_IF_LHS_MOB_CATEGORY,
    MOB_ACTION_IF_LHS_MOB_TYPE,
    MOB_ACTION_IF_LHS_OTHER_BODY_PART,
};

//If action RHS comparand types.
enum MOB_ACTION_IF_RHS_TYPES {
    MOB_ACTION_IF_RHS_VAR,
    MOB_ACTION_IF_RHS_CONST,
};

//Moving action sub-types.
enum MOB_ACTION_MOVE_TYPES {
    MOB_ACTION_MOVE_AWAY_FROM_FOCUSED_MOB,
    MOB_ACTION_MOVE_FOCUSED_MOB,
    MOB_ACTION_MOVE_FOCUSED_MOB_POS,
    MOB_ACTION_MOVE_HOME,
    MOB_ACTION_MOVE_ARACHNORB_FOOT_LOGIC,
    MOB_ACTION_MOVE_LINKED_MOB_AVERAGE,
    MOB_ACTION_MOVE_RANDOMLY,
};

//Set var action sub-types.
enum MOB_ACTION_SET_VAR_TYPES {
    MOB_ACTION_SET_VAR_SUM,
    MOB_ACTION_SET_VAR_SUBTRACT,
    MOB_ACTION_SET_VAR_MULTIPLY,
    MOB_ACTION_SET_VAR_DIVIDE,
    MOB_ACTION_SET_VAR_MODULO,
};

//Stabilize Z action sub-types.
enum MOB_ACTION_STABILIZE_Z_TYPES {
    MOB_ACTION_STABILIZE_Z_HIGHEST,
    MOB_ACTION_STABILIZE_Z_LOWEST,
};


//Types of parameters that an action can receive.
enum MOB_ACTION_PARAM_TYPE {
    //Can be constants or variables.
    MOB_ACTION_PARAM_FREE_INT,
    MOB_ACTION_PARAM_FREE_FLOAT,
    MOB_ACTION_PARAM_FREE_BOOL,
    MOB_ACTION_PARAM_FREE_STRING,
    //Can be constants or variables, and an array of them (minimum 0).
    MOB_ACTION_PARAM_FREE_INT_EXTRAS,
    MOB_ACTION_PARAM_FREE_FLOAT_EXTRAS,
    MOB_ACTION_PARAM_FREE_BOOL_EXTRAS,
    MOB_ACTION_PARAM_FREE_STRING_EXTRAS,
    //Can be constants only.
    MOB_ACTION_PARAM_CONST_INT,
    MOB_ACTION_PARAM_CONST_FLOAT,
    MOB_ACTION_PARAM_CONST_BOOL,
    MOB_ACTION_PARAM_CONST_STRING,
    //Can be constants only, and an array of them (minimum 0).
    MOB_ACTION_PARAM_CONST_INT_EXTRAS,
    MOB_ACTION_PARAM_CONST_FLOAT_EXTRAS,
    MOB_ACTION_PARAM_CONST_BOOL_EXTRAS,
    MOB_ACTION_PARAM_CONST_STRING_EXTRAS,
};


struct mob_action_param {
    MOB_ACTION_PARAM_TYPE type;
    string name;
    mob_action_param(const MOB_ACTION_PARAM_TYPE type, const string &name);
};


struct mob_action_run_data {
    mob* m;
    mob_action_call* call;
    vector<int> &i_params;
    vector<float> &f_params;
    vector<string> &s_params;
    void* custom_data_1;
    void* custom_data_2;
    bool return_value;
    mob_action_run_data();
};


typedef void (mob_action_code)(mob_action_run_data &data);


struct mob_action {
    unsigned char type;
    string name;
    mob_action_code* code;
    vector<mob_action_param> parameters;
    
    mob_action();
};


struct mob_action_call {
    unsigned char type;
    custom_action_code code;
    bool valid;
    vector<int> i_args;
    vector<float> f_args;
    vector<string> s_args;
    vector<bool> arg_is_var;
    MOB_EVENT_TYPES parent_event;
    
    bool run(
        mob* m, void* custom_data_1, void* custom_data_2,
        const size_t parent_event
    );
    mob_action_call(data_node* dn, vector<mob_state*>* states, mob_type* mt);
    mob_action_call(unsigned char type);
    mob_action_call(custom_action_code code);
};


namespace mob_action_code_runners {
void add_health(mob_action_run_data &data);
void add_health(mob_action_run_data &data);
void arachnorb_plan_logic(mob_action_run_data &data);
void calculate(mob_action_run_data &data);
void delete_function(mob_action_run_data &data);
void finish_dying(mob_action_run_data &data);
void focus(mob_action_run_data &data);
void get_chomped(mob_action_run_data &data);
void if_function(mob_action_run_data &data);
void move_to_absolute(mob_action_run_data &data);
void move_to_relative(mob_action_run_data &data);
void move_to_target(mob_action_run_data &data);
void order_release(mob_action_run_data &data);
void play_sound(mob_action_run_data &data);
void randomize_timer(mob_action_run_data &data);
void randomize_var(mob_action_run_data &data);
void receive_status(mob_action_run_data &data);
void release(mob_action_run_data &data);
void remove_status(mob_action_run_data &data);
void send_message_to_links(mob_action_run_data &data);
void send_message_to_nearby(mob_action_run_data &data);
void set_animation(mob_action_run_data &data);
void set_far_reach(mob_action_run_data &data);
void set_gravity(mob_action_run_data &data);
void set_health(mob_action_run_data &data);
void set_hiding(mob_action_run_data &data);
void set_holdable(mob_action_run_data &data);
void set_limb_animation(mob_action_run_data &data);
void set_near_reach(mob_action_run_data &data);
void set_state(mob_action_run_data &data);
void set_tangible(mob_action_run_data &data);
void set_team(mob_action_run_data &data);
void set_timer(mob_action_run_data &data);
void set_var(mob_action_run_data &data);
void show_message_from_var(mob_action_run_data &data);
void spawn(mob_action_run_data &data);
void stabilize_z(mob_action_run_data &data);
void start_chomping(mob_action_run_data &data);
void start_dying(mob_action_run_data &data);
void start_height_effect(mob_action_run_data &data);
void start_particles(mob_action_run_data &data);
void stop(mob_action_run_data &data);
void stop_chomping(mob_action_run_data &data);
void stop_height_effect(mob_action_run_data &data);
void stop_particles(mob_action_run_data &data);
void stop_vertically(mob_action_run_data &data);
void swallow(mob_action_run_data &data);
void swallow_all(mob_action_run_data &data);
void teleport_to_absolute(mob_action_run_data &data);
void teleport_to_relative(mob_action_run_data &data);
void turn_to_absolute(mob_action_run_data &data);
void turn_to_relative(mob_action_run_data &data);
void turn_to_target(mob_action_run_data &data);
};


bool assert_if_actions(const vector<mob_action_call*> &actions, data_node* dn);
void load_init_actions(
    mob_type* mt, data_node* node, vector<mob_action_call*>* actions
);


#endif //ifndef MOB_SCRIPT_ACTION_INCLUDED
