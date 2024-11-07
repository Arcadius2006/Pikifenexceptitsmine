/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Content manager class and related functions.
 */

#include "content_manager.h"

#include "functions.h"
#include "game.h"


/**
 * @brief Constructs a new content manager object.
 */
content_manager::content_manager() {
    for(size_t c = 0; c < N_CONTENT_TYPES; c++) {
        load_levels[c] = CONTENT_LOAD_LEVEL_UNLOADED;
    }
}


/**
 * @brief Returns the relevant content type manager for a given content type.
 *
 * @param type The content type.
 * @return Pointer to the manager.
 */
content_type_manager* content_manager::get_mgr_ptr(CONTENT_TYPE type) {
    switch(type) {
    case CONTENT_TYPE_AREA: {
        return &areas;
        break;
    } case CONTENT_TYPE_BITMAP: {
        return &bitmaps;
        break;
    } case CONTENT_TYPE_CUSTOM_PARTICLE_GEN: {
        return &custom_particle_gen;
        break;
    } case CONTENT_TYPE_GLOBAL_ANIMATION: {
        return &global_anims;
        break;
    } case CONTENT_TYPE_GUI: {
        return &gui;
        break;
    } case CONTENT_TYPE_HAZARD: {
        return &hazards;
        break;
    } case CONTENT_TYPE_LIQUID: {
        return &liquids;
        break;
    } case CONTENT_TYPE_MISC: {
        return &misc_configs;
        break;
    } case CONTENT_TYPE_MOB_ANIMATION: {
        return &mob_anims;
        break;
    } case CONTENT_TYPE_MOB_TYPE: {
        return &mob_types;
        break;
    } case CONTENT_TYPE_SAMPLE: {
        return &samples;
        break;
    } case CONTENT_TYPE_SONG: {
        return &songs;
        break;
    } case CONTENT_TYPE_SONG_TRACK: {
        return &song_tracks;
        break;
    } case CONTENT_TYPE_SPIKE_DAMAGE_TYPE: {
        return &spike_damage_types;
        break;
    } case CONTENT_TYPE_SPRAY_TYPE: {
        return &spray_types;
        break;
    } case CONTENT_TYPE_STATUS_TYPE: {
        return &status_types;
        break;
    } case CONTENT_TYPE_WEATHER_CONDITION: {
        return &weather_conditions;
        break;
    } case N_CONTENT_TYPES: {
        break;
    }
    }
    return nullptr;
}


/**
 * @brief Loads all pieces of game content of some type.
 * This begins by generating a manifest of all content on disk, with packs
 * in mind, and then reads all the files in the manifest.
 *
 * @param types Types of game content to load.
 * @param level Level to load at.
 */
void content_manager::load_all(const vector<CONTENT_TYPE> &types, CONTENT_LOAD_LEVEL level) {
    //Fill in all manifests first. This is because some content may rely on
    //another's manifest.
    for(size_t t = 0; t < types.size(); t++) {
        content_type_manager* mgr_ptr = get_mgr_ptr(types[t]);
        engine_assert(
            load_levels[types[t]] == CONTENT_LOAD_LEVEL_UNLOADED,
            "Tried to load all content of type " + mgr_ptr->get_name() +
            " even though it's already loaded!"
        );
        mgr_ptr->fill_manifests();
    }
    
    //Now load the content.
    for(size_t t = 0; t < types.size(); t++) {
        content_type_manager* mgr_ptr = get_mgr_ptr(types[t]);
        mgr_ptr->load_all(level);
        load_levels[types[t]] = level;
    }
    
}


/**
 * @brief Loads an area as the "current area". This does not load it into
 * the vector of areas.
 *
 * @param manifest Manifest of the area.
 * @param type Type of area this is.
 * @param level Level to load at.
 * @param from_backup If true, load from a backup, if any.
 */
void content_manager::load_area_as_current(
    content_manifest* manifest, AREA_TYPE type,
    CONTENT_LOAD_LEVEL level, bool from_backup
) {
    engine_assert(
        game.cur_area_data == nullptr,
        "Tried to load area \"" + manifest->path + "\" as the current "
        "one even though there is already a loaded current area, \"" +
        (
            game.cur_area_data->manifest ?
            game.cur_area_data->manifest->path :
            "(unsaved)"
        ) + "\"!"
    );
    
    game.cur_area_data = new area_data();
    areas.load_area(
        game.cur_area_data, manifest, type, level, from_backup
    );
}


/**
 * @brief Unloads the "current area".
 *
 * @param level Should match the level at which the content got loaded.
 */
void content_manager::unload_current_area(CONTENT_LOAD_LEVEL level) {
    if(!game.cur_area_data) return;
    game.cur_area_data->clear();
    delete game.cur_area_data;
    game.cur_area_data = nullptr;
}


/**
 * @brief Unloads some loaded content.
 *
 * @param types Types of content to unload.
 */
void content_manager::unload_all(const vector<CONTENT_TYPE> &types) {
    for(size_t t = 0; t < types.size(); t++) {
        content_type_manager* mgr_ptr = get_mgr_ptr(types[t]);
        
        engine_assert(
            load_levels[types[t]] != CONTENT_LOAD_LEVEL_UNLOADED,
            "Tried to unload all content of type " + mgr_ptr->get_name() +
            " even though it's already unloaded!"
        );
        
        mgr_ptr->unload_all(load_levels[types[t]]);
        mgr_ptr->clear_manifests();
        
        load_levels[types[t]] = CONTENT_LOAD_LEVEL_UNLOADED;
    }
}
