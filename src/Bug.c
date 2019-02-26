#include "Bug.h"

Entity *bug = NULL;

void bug_think(Entity *self);
void bug_update(Entity *self);

Entity *bug_new(cpVect position) {

	
	bug = gf2d_entity_new();

	if (!bug) {
		slog("failed to allocate new bug");
		return;
	}
	bug->cpbody = cpBodyNewStatic();   //(1, cpMomentForBox(1, 60, 60));
	cpBodySetPosition(bug->cpbody, position);
	bug->cpshape = cpBoxShapeNew(bug->cpbody, 60, 60, 60);
	vector2d_copy(bug->position, position);
	gf2d_actor_load(&bug->actor, "actor/Bug.actor");
	gf2d_actor_set_action(&bug->actor, "idle");
	bug->think = bug_think;
	bug->update = bug_update;

	slog("allocated new bug");
	return bug;


}


void bug_think(Entity *self) {

	/*cpVect playerPos = cpBodyGetPosition(bug->cpbody);

	slog("position is %5.2f , %5.2f", playerPos.x, playerPos.y);*/
}

void bug_update(Entity *self) {

	Vector2D dir;
	if (!self)return;

	vector2d_normalize(&dir);
	vector2d_copy(self->velocity, dir);

	gf2d_actor_next_frame(&bug->actor);

}