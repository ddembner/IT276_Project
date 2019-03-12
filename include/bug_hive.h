#ifndef __BUG_HIVE_H__
#define __BUG_HIVE_H__

#include "gf2d_entity.h"
#include "simple_logger.h"

/**
* @brief create new bug hive entity
* @param position on where to create
* @param space to place it in
*/
Entity *bug_hive_new(cpVect position, cpSpace *space);

/**
* @brief spawn new bug hive
* @param position on where to create
* @param json arguments
* @param space to place it in
*/
Entity *bug_hive_spawn(cpVect position, SJson *args, cpSpace *space);


#endif // !__BUG_HIVE_H__