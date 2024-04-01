/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the tool type class and tool type-related functions.
 */

#pragma once

#include "../libs/data_file.h"
#include "mob_type.h"


/**
 * @brief A type of tool. A type of hand-held explosive, for instance.
 */
class tool_type : public mob_type {

public:
    
    //--- Members ---

    //Icon used to represent this tool in the HUD group info.
    ALLEGRO_BITMAP* bmp_icon = nullptr;

    //If true, the Pikmin holding it can be swapped for the tool, when chomped.
    bool can_be_hotswapped = true;

    //Should it be dropped if the Pikmin carrying it is whistled?
    bool dropped_when_pikmin_is_whistled = false;

    //Should it be dropped if the Pikmin carrying it lands from a throw?
    bool dropped_when_pikmin_lands = true;

    //Should it be dropped if the Pikmin carrying it lands on an opponent?
    bool dropped_when_pikmin_lands_on_opponent = false;

    //Should it stay stuck to the opponent if the thrown Pikmin drops it there?
    bool stuck_when_pikmin_lands_on_opponent = false;
    
    //Should the Pikmin return to the group after using this tool?
    bool pikmin_returns_after_using = true;
    

    //--- Function declarations ---
    
    tool_type();
    void load_properties(data_node* file) override;
    void load_resources(data_node* file) override;
    
};
