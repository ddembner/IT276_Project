#include "spawn.h"
#include "level.h"
#include "player.h"
#include "simple_logger.h"

static Spawn spawnlist[] =
{
	{
		"player_start",
		player_spawn
	},
	{0}
};

void spawn_entity(const char *name, cpVect position, Uint32 id, SJson *args, cpSpace *space)
{
	Spawn *spawn;
	Entity *ent;
	if (!name)
	{
		slog("no spawn name provided");
		return;
	}
	for (spawn = spawnlist; spawn->name != 0; spawn++)
	{
		if (strcmp(spawn->name, name) == 0)
		{
			if (spawn->spawn)
			{
				ent = spawn->spawn(position, args, space);
				if (ent)
				{
					ent->id = id;
				}
				return;
			}
		}
	}
	slog("failed to spawn entity %s", name);
}
