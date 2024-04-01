/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the liquid class and liquid-related functions.
 */

#pragma once

#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "animation.h"


using std::string;


/**
 * @brief A liquid type defines how a sector should look to make it look
 * like water.
 *
 * This is considered a "liquid" and not specifically "water" because the
 * engine allows creating other water-like things, like acid, lava, etc.
 * Each have their own color, reflectivity, etc.
 * A hazard can be associated with a liquid. It's the way the
 * engine has to know if a sector is to be shown as a liquid or not.
 */
struct liquid {

    //--- Members ---
    
    //Name of this liquid.
    string name;
    
    //Color that best represents this liquid.
    ALLEGRO_COLOR main_color = COLOR_EMPTY;
    
    //Color used for this liquid in the radar.
    ALLEGRO_COLOR radar_color = COLOR_EMPTY;
    
    //Animation database containing its liquid surface animation.
    animation_database anim_db;
    
    //Animation instance for the liquid surface.
    animation_instance anim_instance;
    
    //Speed at which to scroll each layer of the surface.
    float surface_speed[2] = { 10.0f, -13.0f };
    
    //Opacity of the surface.
    unsigned char surface_alpha = 255;
    
};
