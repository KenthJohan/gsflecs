#include "flecs.h"


typedef struct
{
	int level;
} Hunger;

static void System_Init_Hunger(ecs_iter_t *it)
{
	Hunger *h = ecs_term(it, Hunger, 1);
	for (int i = 0; i < it->count; i ++)
	{
		ecs_assert(h, ECS_INVALID_PARAMETER, "h is null");
		h[i].level = 0;
	}
}



int main(int argc, char *argv[])
{
	ecs_log_set_level(0);

	ecs_world_t *world = ecs_init();
	ECS_COMPONENT(world, Hunger);
	ECS_TRIGGER(world, System_Init_Hunger, EcsOnAdd, Hunger);

	/*
	{
		ecs_entity_t Human = ecs_new(world, 0);
		ecs_set(world, Human, Hunger, {0});
		ecs_entity_t Bob = ecs_new_w_pair(world, EcsIsA, Human);
		ecs_set(world, Bob, Hunger, {1});
	}
	*/

	{
		ecs_entity_t Human = ecs_new(world, 0);
		ecs_add(world, Human, Hunger);
		ecs_entity_t Bob = ecs_new_w_pair(world, EcsIsA, Human);
		ecs_add(world, Bob, Hunger);
	}
	return 0;
}






