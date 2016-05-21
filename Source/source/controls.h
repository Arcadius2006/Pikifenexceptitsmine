/*
 * Copyright (c) Andre 'Espyo' Silva 2013-2016.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included
 * README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the control-related classes and functions.
 * This refers to both hardware input receiving
 * and the corresponding in-game actions.
 */

#ifndef CONTROLS_INCLUDED
#define CONTROLS_INCLUDED

#include <string>

using namespace std;

/* ----------------------------------------------------------------------------
 * This holds information over a user-specified
 * control. It has info over what hardware input
 * is required for this in-game control,
 * and what action it triggers.
 */
struct control_info {
    //Action number. Use BUTTON_*.
    unsigned char action;
    //Type of control (hardware). Use CONTROL_TYPE_*.
    unsigned char type;
    //Device number. i.e. the gamepad number.
    int device_nr;
    //Button, whether the gamepad digital button, or the keyboard key.
    int button;
    //Stick on the gamepad.
    int stick;
    //Axis of the stick.
    int axis;
    
    control_info(unsigned char action, string s);
    string stringify();
};



void handle_game_controls(const ALLEGRO_EVENT &ev);
void handle_button(
    const unsigned int button, const unsigned char player, float pos
);
void active_control();



enum BUTTONS {
    BUTTON_NONE,
    BUTTON_THROW,
    BUTTON_WHISTLE,
    BUTTON_MOVE_RIGHT,
    BUTTON_MOVE_UP,
    BUTTON_MOVE_LEFT,
    BUTTON_MOVE_DOWN,
    BUTTON_MOVE_CURSOR_RIGHT,
    BUTTON_MOVE_CURSOR_UP,
    BUTTON_MOVE_CURSOR_LEFT,
    BUTTON_MOVE_CURSOR_DOWN,
    BUTTON_GROUP_MOVE_RIGHT,
    BUTTON_GROUP_MOVE_UP,
    BUTTON_GROUP_MOVE_LEFT,
    BUTTON_GROUP_MOVE_DOWN,
    BUTTON_GROUP_MOVE_GO_TO_CURSOR,
    BUTTON_SWITCH_LEADER_RIGHT,
    BUTTON_SWITCH_LEADER_LEFT,
    BUTTON_DISMISS,
    BUTTON_USE_SPRAY_1,
    BUTTON_USE_SPRAY_2,
    BUTTON_USE_SPRAY,
    BUTTON_SWITCH_SPRAY_RIGHT,
    BUTTON_SWITCH_SPRAY_LEFT,
    BUTTON_SWITCH_ZOOM,
    BUTTON_ZOOM_IN,
    BUTTON_ZOOM_OUT,
    BUTTON_SWITCH_TYPE_RIGHT,
    BUTTON_SWITCH_TYPE_LEFT,
    BUTTON_SWITCH_MATURITY_UP,
    BUTTON_SWITCH_MATURITY_DOWN,
    BUTTON_LIE_DOWN,
    BUTTON_PAUSE,
    
    N_BUTTONS,
};

enum CONTROL_TYPES {
    CONTROL_TYPE_NONE,
    CONTROL_TYPE_KEYBOARD_KEY,
    CONTROL_TYPE_MOUSE_BUTTON,
    CONTROL_TYPE_MOUSE_WHEEL_UP,
    CONTROL_TYPE_MOUSE_WHEEL_DOWN,
    CONTROL_TYPE_MOUSE_WHEEL_LEFT,
    CONTROL_TYPE_MOUSE_WHEEL_RIGHT,
    CONTROL_TYPE_JOYSTICK_BUTTON,
    CONTROL_TYPE_JOYSTICK_AXIS_POS,
    CONTROL_TYPE_JOYSTICK_AXIS_NEG,
};

#endif //ifndef CONTROLS_INCLUDED
