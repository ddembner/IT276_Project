#include <stdlib.h>
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "gf2d_entity.h"

typedef struct {

	Entity *entityList;
	Uint32 maxEntities;

}EntityManager;

static EntityManager entityManager = { 0 };

void gf2d_entity_system_close()
{
	int i;
	if (entityManager.entityList != NULL)
	{
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			gf2d_entity_free(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	memset(&entityManager, 0, sizeof(EntityManager));
	slog("entity system closed");
}

void gf2d_entity_system_init(Uint32 maxEntities) {
	if (!maxEntities) {
		slog("Cannot allocate zero entities.");
		return;
	}
	entityManager.entityList = (Entity*)malloc(sizeof(Entity)*maxEntities);
	if (!entityManager.entityList) {
		slog("failed to allocate new entity");
		gf2d_entity_system_close();
		return;
	}
	memset(entityManager.entityList, 0, sizeof(Entity)*maxEntities);
	entityManager.maxEntities = maxEntities;
	atexit(gf2d_entity_system_close);
	slog("entity system initialized");

}



void gf2d_entity_free(Entity *self)
{
	int i;
	if (!self)return;
	memset(self, 0, sizeof(Entity));
}

void gf2d_entity_draw(Entity * self)
{

	if (!self)return;
	if (!self->_inuse)return;

	gf2d_sprite_draw(
		self->sprite,
		self->position,
		self->scale,
		self->scalecenter,
		self->rotation,
		NULL,
		NULL,
		NULL);

	if (self->draw != NULL) {
		self->draw(self);
	}

}

void gf2d_entity_update(Entity *self) {

	if (!self)return;
	if (!self->_inuse)return;

	if (self->update != NULL) {
		self->update(self);
	}
}
