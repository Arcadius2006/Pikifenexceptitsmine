/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Controls manager class and controls manager related functions.
 */

#include <algorithm>
#include <cmath>

#include "controls_manager.h"


/* ----------------------------------------------------------------------------
 * Constructs a new control bind.
 */
control_bind::control_bind() :
    action_type_id(0),
    player_nr(0) {
}


/* ----------------------------------------------------------------------------
 * When a game controller stick input is received, it should be checked with
 * the state of that entire stick to see if it needs to be normalized,
 * deadzones should be applied, etc.
 * device_nr:
 *   Number of the game controller.
 * stick_nr:
 *   Number of the game controller's stick.
 */
void controls_manager::clean_stick(
    int device_nr, int stick_nr
) {
    const float raw_x = raw_sticks[device_nr][stick_nr][0];
    const float raw_y = raw_sticks[device_nr][stick_nr][1];
    const float angle = (float) atan2(raw_y, raw_x);
    
    //Clamp the magnitude between the minimum and maximum allowed.
    float magnitude = sqrt(raw_x * raw_x + raw_y * raw_y);
    magnitude = std::max(magnitude, options.stick_min_deadzone);
    magnitude = std::min(magnitude, options.stick_max_deadzone);
    
    //Interpolate the magnitude.
    magnitude =
        (magnitude - options.stick_min_deadzone) /
        (float) (options.stick_max_deadzone - options.stick_min_deadzone);
        
    clean_sticks[device_nr][stick_nr][0] = (float) cos(angle) * magnitude;
    clean_sticks[device_nr][stick_nr][1] = (float) sin(angle) * magnitude;
}


/* ----------------------------------------------------------------------------
 * Returns the oldest action in the queue. Returns true if there is one,
 * false if not.
 * action:
 *   Action to fill.
 */
bool controls_manager::poll_action(player_action &action) {
    if(action_queue.empty()) return false;
    action = action_queue.front();
    action_queue.erase(action_queue.begin());
    return true;
}


/* ----------------------------------------------------------------------------
 * Returns a list of action types that get triggered by the given input.
 */
vector<int> controls_manager::get_action_types_from_input(
    const player_input &input
) {
    vector<int> action_types;
    
    for(size_t b = 0; b < binds.size(); ++b) {
    
        const control_bind &bind = binds[b];
        
        if(bind.input.type != input.type) continue;
        
        switch(input.type) {
        case INPUT_TYPE_NONE:
        case INPUT_TYPE_UNKNOWN: {
            continue;
            break;
        } case INPUT_TYPE_KEYBOARD_KEY:
        case INPUT_TYPE_MOUSE_BUTTON: {
            if(
                bind.input.button_nr != input.button_nr
            ) {
                continue;
            }
            break;
        }
        case INPUT_TYPE_MOUSE_WHEEL_UP:
        case INPUT_TYPE_MOUSE_WHEEL_DOWN:
        case INPUT_TYPE_MOUSE_WHEEL_LEFT:
        case INPUT_TYPE_MOUSE_WHEEL_RIGHT: {
            break;
        }
        case INPUT_TYPE_CONTROLLER_BUTTON: {
            if(
                bind.input.device_nr != input.device_nr ||
                bind.input.button_nr != input.button_nr
            ) {
                continue;
            }
            break;
        }
        case INPUT_TYPE_CONTROLLER_AXIS_POS:
        case INPUT_TYPE_CONTROLLER_AXIS_NEG: {
            if(
                bind.input.device_nr != input.device_nr ||
                bind.input.stick_nr != input.stick_nr ||
                bind.input.axis_nr != input.axis_nr
            ) {
                continue;
            }
            break;
        }
        }
        
        action_types.push_back(bind.action_type_id);
        
    }
    
    return action_types;
}


/* ----------------------------------------------------------------------------
 * Handles an input from hardware.
 */
void controls_manager::handle_input(
    const player_input &input
) {
    //First, clean any game controller stick inputs.
    //https://www.gamedeveloper.com/
    //  disciplines/doing-thumbstick-dead-zones-right
    //https://www.gamedeveloper.com/
    //  design/interpreting-analog-sticks-in-inversus
    if(
        input.type == INPUT_TYPE_CONTROLLER_AXIS_POS ||
        input.type == INPUT_TYPE_CONTROLLER_AXIS_NEG
    ) {
        raw_sticks[input.device_nr][input.stick_nr][input.axis_nr] =
            input.value;
        clean_stick(input.device_nr, input.stick_nr);
    }
    
    //Find what game action types are associated with this input.
    vector<int> action_types = get_action_types_from_input(input);
    
    //Update each game action type's current input state, to be reported later.
    for(size_t a = 0; a < action_types.size(); ++a) {
        action_type_values[action_types[a]] = input.value;
    }
}


/* ----------------------------------------------------------------------------
 * Begins a new frame of gameplay.
 */
void controls_manager::new_frame() {
    action_queue.clear();
    
    for(auto &a : action_type_values) {
        if(old_action_type_values[a.first] != a.second) {
            player_action new_action;
            new_action.action_type_id = a.first;
            new_action.value = a.second;
            action_queue.push_back(new_action);
        }
    }
    
    old_action_type_values = action_type_values;
}


/* ----------------------------------------------------------------------------
 * Constructs a new controls manager options struct.
 */
controls_manager_options::controls_manager_options() :
    stick_min_deadzone(0.0f),
    stick_max_deadzone(1.0f) {
    
}


/* ----------------------------------------------------------------------------
 * Constructs a new player action.
 */
player_action::player_action() :
    action_type_id(0),
    value(0.0f) {
}


/* ----------------------------------------------------------------------------
 * Constructs a new player input.
 */
player_input::player_input() :
    type(INPUT_TYPE_NONE),
    device_nr(0),
    button_nr(0),
    stick_nr(0),
    axis_nr(0),
    value(0.0f) {
}
