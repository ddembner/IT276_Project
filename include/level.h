#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_collision.h"
#include "gf2d_sprite.h"
#include "gf2d_audio.h"
#include "gf2d_entity.h"
#include "simple_json.h"
#include "gf2d_cpSpace.h"
#include "gf2d_particles.h"

/**
 * @Purpose game specific level code
 * This game is going to be a 2d action RPG
 */



 /**
 * @brief The level structure holds data for a given game level.
 */
typedef struct
{
	TextLine    backgroundImage;    /**<background image for this level*/
	TextLine    backgroundMusic;    /**<background music for this level*/
	TextLine    tileSet;            /**<tiles to use for this level*/
	Vector2D    tileSize;           /**<how tile space width & height*/
	Vector2D    tileMapSize;        /**<how tile space width & height*/
	int        *tileMap;            /**<tile data*/
	SJson      *spawnList;
	int			*framesperline;		/**<how many tiles per line in set*/
	Vector2D	backgroundSize;		/**<size of background image*/
	SJson		*transitionList;	/**<data for level transitions*/
}LevelInfo;

/**
 * @brief clears all assets loaded for the live level
 * Called at program exit and before loading the next level
 */
void level_clear();

void level_music_stop();


void level_draw();
void level_update();

/**
 * @brief create the live level based on the provided level info
 * @param linfo the level info to create the level for
 * @param space if true, create the physical space for the game to run
 */
void level_init(LevelInfo *linfo, Uint8 space);

void level_info_free(LevelInfo *linfo);
LevelInfo *level_info_load(char *filename);

/**
 * @brief create a new level info and populate based on parameters
 * @param backgroundImage the image file to use as the background
 * @param backgroundMusic the music the play for this level
 * @param tileSet the sprite sheet to use for tiles on this level
 * @param tileSize the size of the tiles to use for the level
 * @param tileMapSize how many tiles (width & height) to use fo this level
 */
LevelInfo *level_info_create(
	const char *backgroundImage,
	const char *backgroundMusic,
	const char *tileSet,
	Vector2D    tileSize,
	Vector2D    tileMapSize
);

/**
 * @brief update the tile information in the levelInfo struct provided
 * @note: does not auto update the internal structures for drawing / collision
 * @param linfo the pointer to the data to update
 * @param position the place of the new tile to update If position outside of the range of the map, this will be a no-op and log an error
 * @param tile what tile to set the data to.  Note that 0 is tile clear
 */
void level_update_tile(LevelInfo *linfo, Vector2D position, Uint32 tile);

/**
 * @brief convert a screen position to a tile
 * @param linfo pointer to the level info to base the calculation on
 * @param position the position in world space (camera is not considered)
 * @returns (-1,-1) on error or the tile position (in integer space) otherwise
 */
Vector2D level_position_to_tile(LevelInfo *linfo, Vector2D position);

/**
 * @brief make or remake tile layer for rendering based on level info;
 */
void level_make_tile_layer(LevelInfo *linfo);

/**
 * @brief adds an entity to the level physics space
 * @note entity must have a Body defined
 * @param ent the entity to add to the space
 */
void level_add_entity(Entity *ent);

/**
 * @brief removes an entity from the level physics space
 * @note this should not be done during a space update (ie: collision callbacks)
 * @param ent the entity to be removed
 */
void level_remove_entity(Entity *ent);

/**
 * @brief build the collision space
 * @note unneeded in editor mode
 */
void level_make_space();

/**
 * @brief free the collision space
 */
void free_space();

/**
 * @brief load up a new game level and move the player into it
 * @param
 */
void level_transition(char *filename, const char *playerTarget, Uint32 targetId);

/**
* @brief gets information from jason file to make transition tiles
* @param transitionList The list of transitions that can occur in a level from a json file
* @param value The value that is used to identify which level to id with
* @param shape The shape to store the data in
*/
void level_transition_data(SJson *transitionList, int value, cpShape *shape);

/**
* @brief get the particle emitter for the level
* @return NULL if not available, or a pointer to the particle emitter
*/
ParticleEmitter * level_get_particle_emitter();
#endif
