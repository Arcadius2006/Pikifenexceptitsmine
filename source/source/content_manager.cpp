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
#include "load.h"
#include "utils/allegro_utils.h"


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
        return &global_anim_dbs;
        break;
    } case CONTENT_TYPE_GUI: {
        return &gui_defs;
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
        return &mob_anim_dbs;
        break;
    } case CONTENT_TYPE_MOB_TYPE: {
        return &mob_types;
        break;
    } case CONTENT_TYPE_SONG: {
        return &songs;
        break;
    } case CONTENT_TYPE_SONG_TRACK: {
        return &song_tracks;
        break;
    } case CONTENT_TYPE_SOUND: {
        return &sounds;
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
 * @brief Creates a new pack and updates the list of packs.
 *
 * @param internal_name Internal name of the pack, i.e. the pack's folder name.
 * @param name Proper name of the pack.
 * @param description Description.
 * @param maker Maker(s).
 * @return Whether it succeeded.
 */
bool content_manager::create_pack(
    const string &internal_name, const string &name,
    const string &description, const string &maker
) {
    string pack_path = FOLDER_PATHS_FROM_ROOT::GAME_DATA + "/" + internal_name;
    
    //Create the folder first.
    bool could_make_folder = al_make_directory(pack_path.c_str());
    if(!could_make_folder) return false;
    
    //Create the data file.
    data_node data;
    data.add(new data_node("name", name));
    data.add(new data_node("description", description));
    data.add(new data_node("maker", maker));
    data.add(new data_node("version", "1.0.0"));
    data.add(new data_node("engine_version", get_engine_version_string()));
    data.add(new data_node("tags", ""));
    data.add(new data_node("dependencies", ""));
    data.add(new data_node("conflicts", ""));
    data.add(new data_node("notes", ""));
    data.save_file(pack_path + "/" + FILE_NAMES::PACK_DATA, true, true);
    
    //Update the list and manifest.
    packs.unload_all();
    packs.clear_manifests();
    packs.fill_manifests();
    packs.load_all();
    
    return true;
}


/**
 * @brief Loads an area as the "current area". This does not load it into
 * the vector of areas.
 *
 * @param requested_area_path Path to the area folder.
 * @param manif_ptr Set the manifest pointer to this. If nullptr, it'll be
 * set from the list of manifests.
 * @param type Type of area this is.
 * @param level Level to load at.
 * @param from_backup If true, load from a backup, if any.
 * @return Whether it succeeded.
 */
bool content_manager::load_area_as_current(
    const string &requested_area_path, content_manifest* manif_ptr,
    CONTENT_LOAD_LEVEL level, bool from_backup
) {
    engine_assert(
        game.cur_area_data == nullptr,
        "Tried to load area \"" + requested_area_path + "\" as the current "
        "one even though there is already a loaded current area, \"" +
        (
            game.cur_area_data->manifest ?
            game.cur_area_data->manifest->path :
            "(unsaved)"
        ) + "\"!"
    );
    
    game.cur_area_data = new area_data();
    bool success =
        areas.load_area(
            game.cur_area_data, requested_area_path, manif_ptr,
            level, from_backup
        );
        
    if(!success) {
        unload_current_area(level);
    }
    return success;
}


/**
 * @brief Reloads all packs.
 * This only loads their manifests and metadata, not their content!
 */
void content_manager::reload_packs() {
    packs.unload_all();
    packs.clear_manifests();
    
    packs.fill_manifests();
    packs.load_all();
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


/**
 * @brief Clears all loaded manifests.
 */
void pack_manager::clear_manifests() {
    manifests_sans_base.clear();
    manifests_with_base.clear();
}


/**
 * @brief Fills in the manifests.
 */
void pack_manager::fill_manifests() {
    vector<string> folders = folder_to_vector(FOLDER_PATHS_FROM_ROOT::GAME_DATA, true);
    
    for(size_t f = 0; f < folders.size(); f++) {
        if(folders[f] != FOLDER_NAMES::BASE_PACK) {
            manifests_sans_base.push_back(folders[f]);
        }
    }
    
    manifests_with_base.push_back(FOLDER_NAMES::BASE_PACK);
    manifests_with_base.insert(
        manifests_with_base.end(),
        manifests_sans_base.begin(),
        manifests_sans_base.end()
    );
}


/**
 * @brief Loads all packs in the manifests, including the base pack.
 * This only loads their metadata, not their content!
 */
void pack_manager::load_all() {
    for(size_t p = 0; p < manifests_with_base.size(); p++) {
        data_node pack_file =
            load_data_file(
                FOLDER_PATHS_FROM_ROOT::GAME_DATA + "/" +
                manifests_with_base[p] + "/" +
                FILE_NAMES::PACK_DATA
            );
            
        pack pack_data;
        pack_data.name = manifests_with_base[p];
        reader_setter rs(&pack_file);
        rs.set("name", pack_data.name);
        rs.set("description", pack_data.description);
        rs.set("tags", pack_data.tags);
        rs.set("maker", pack_data.maker);
        rs.set("version", pack_data.version);
        rs.set("engine_version", pack_data.engine_version);
        rs.set("dependencies", pack_data.dependencies);
        rs.set("conflicts", pack_data.conflicts);
        rs.set("notes", pack_data.notes);
        
        list[manifests_with_base[p]] = pack_data;
    }
}


/**
 * @brief Unloads all loaded packs.
 * This only unloads their metadata, not their content!
 */
void pack_manager::unload_all() {
    list.clear();
}