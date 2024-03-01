/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the enemy type class and enemy type-related functions.
 */

#ifndef ENEMY_TYPE_INCLUDED
#define ENEMY_TYPE_INCLUDED

#include <string>
#include <vector>

#include "../libs/data_file.h"
#include "mob_type.h"


using std::size_t;


/**
 * @brief A type of enemy. A species, if you will.
 * Red Bulborb, Orange Bulborb, Cloaking Burrow-nit, etc.
 */
class enemy_type : public mob_type {

public:
    
    //--- Members ---

    //How many Pikmin seeds are generated by delivering to an Onion?
    size_t pikmin_seeds;

    //Does it drop a carriable corpse upon death?
    bool drops_corpse;

    //Can Pikmin perform grounded attacks on it?
    bool allow_ground_attacks;

    //Points worth for missions.
    size_t points;
    
    //True if it's a boss.
    bool is_boss;
    

    //--- Function declarations ---

    enemy_type();
    void load_properties(data_node* file) override;
    
};


#endif //ifndef ENEMY_TYPE_INCLUDED
