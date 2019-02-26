#include "Player.h"
#include "simple_logger.h"
#include "gf2d_input.h"
#include "camera.h"
#include "simple_json.h"


static Entity *player = NULL;

void player_think(Entity *self);
void player_update(Entity *self);


Entity *player_new(cpVect position) {

	player = gf2d_entity_new();

	if (!player) {
		slog("failed to allocate new player");
		return NULL;
	}

	//Chipmunk physics here
	cpFloat moment = cpMomentForCircle(20, 20, 20, cpv(16,16));
	player->cpbody = cpBodyNew(5, moment);
	cpBodySetPosition(player->cpbody, position);
	player->cpshape = cpCircleShapeNew(player->cpbody, 5, cpvzero);
	player->shape = gf2d_shape_circle(32, 32, 20);
	player->position = cpvector_to_gf2dvector(position);
	gf2d_line_cpy(player->name, "player");
	gf2d_actor_load(&player->actor, "actor/player.actor");
	gf2d_actor_set_action(&player->actor, "idle_down");
	vector2d_copy(player->scale, player->actor.al->scale);
	//player->actor.frame = 0;
	//vector2d_set(player->flip, 1, 0);
	player->dir = ED_Down;
	player->think = player_think;
	player->touch = player_think;
	player->update = player_update;
	player->state = ES_Idle;
	return player;
}

Entity *player_get() {
	return player;
}

void player_set_position(Vector2D position) {

	if (!player) {
		slog("player not loaded");
		return;
	}

	vector2d_copy(player->position, position);
	vector2d_copy(player->body.position, position);

}


int player_touch(Entity *self, Entity *other){

	//When player touches something

}

void player_think(Entity *self) {

	cpFloat moveX = 0;
	cpFloat moveY = 0;

	//Need to fix walking animation to easily
	//transition from idle to walking
	if (gf2d_input_command_held("walkup")) {
		if (self->state != ES_Attack)
			moveY -= PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Up;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_up");
		}



	}
	else if (gf2d_input_command_get_state("walkup") == IE_Release && self->state != ES_Attack) { //Need to check if player attacks to prevent action conflicts
		gf2d_actor_set_action(&self->actor, "idle_up");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walkdown")) {
		if (self->state != ES_Attack)
			moveY += PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Down;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_down");
		}


	}
	else if (gf2d_input_command_get_state("walkdown") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walkleft")) {
		if (self->state != ES_Attack)
			moveX -= PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Left;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_left");
		}


	}
	else if (gf2d_input_command_get_state("walkleft") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_left");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walkright")) {
		if (self->state != ES_Attack)
			moveX += PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Right;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_right");
		}


	}
	else if (gf2d_input_command_get_state("walkright") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_right");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_pressed("melee")) {

		if (self->state == ES_Walk) {
			moveX = 0;
			moveY = 0;
			self->cpbody->v.x = 0;
			self->cpbody->v.y = 0;
		}

		switch (self->dir) {
		case ED_Down:
			gf2d_actor_set_action(&player->actor, "attack_down");
			break;
		case ED_Left:
			gf2d_actor_set_action(&player->actor, "attack_left");
			break;
		case ED_Right:
			gf2d_actor_set_action(&player->actor, "attack_right");
			break;
		case ED_Up:
			gf2d_actor_set_action(&player->actor, "attack_up");
			break;
		default:
			slog("WTF direction are you?!");
		}
		self->state = ES_Attack;
	}

	if (gf2d_input_command_held("walkup") && gf2d_input_command_held("walkdown")) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		self->state = ES_Idle;
	}

	if (self->actor.at == ART_END && self->state == ES_Attack) {
		switch (self->dir) {
		case ED_Down:
			gf2d_actor_set_action(&player->actor, "idle_down");
			break;
		case ED_Left:
			gf2d_actor_set_action(&player->actor, "idle_left");
			break;
		case ED_Right:
			gf2d_actor_set_action(&player->actor, "idle_right");
			break;
		case ED_Up:
			gf2d_actor_set_action(&player->actor, "idle_up");
			break;
		default:
			slog("WTF direction are you?!");
		}
		self->state = ES_Idle;
	}
	//Change velocity of player and copy it to position to update sprite???
	self->cpbody->v.x = moveX;
	self->cpbody->v.y = moveY;
	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&self->actor);
}

void player_update(Entity *self) {
	

	cpVect playercpPos = cpBodyGetPosition(player->cpbody);

	slog("CPposition is %5.2f , %5.2f", playercpPos.x, playercpPos.y);

	Vector2D playerPos = self->position;

	slog("position is %5.2f , %5.2f", playerPos.x, playerPos.y);
}

