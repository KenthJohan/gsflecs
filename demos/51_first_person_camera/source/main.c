#include "flecs.h"
#include "eg_gunslinger.h"
#include "eg_geometry.h"
#include "eg_quantity.h"
#include <stdlib.h>

typedef struct
{
	int amount;
} PillarSpawner;


#define rand_range(a, b) (((b)-(a))*((float)rand() / (float)RAND_MAX) + (a))
static void System_Init_Pillars(ecs_iter_t *it)
{
	PillarSpawner *p = ecs_term(it, PillarSpawner, 1);
	for (int i = 0; i < it->count; i ++)
	{
		for (int j = 0; j < p[i].amount; ++j)
		{
			ecs_entity_t e = ecs_new(it->world, 0);
			float h = rand_range(1.0f, 12.0f);
			float hy = h * 0.5f;
			float hx = 2.f;
			float hz = 2.f;
			ecs_set(it->world, e, EgPosition3F32, {rand_range(-15, 15), hy, rand_range(-15, 15)});
			ecs_set(it->world, e, EgBoxF32, {hx, h, hz});
			ecs_add(it->world, e, EgColor);
			ecs_add(it->world, e, EgDraw1);
		}
	}
}



int main(int argc, char *argv[])
{
	ecs_log_set_level(0);

	ecs_world_t *world = ecs_init();

	ECS_IMPORT(world, Module_Gunslinger);
	ECS_IMPORT(world, Module_EgQuantity);
	ECS_IMPORT(world, Module_EgGeometry);

	ECS_COMPONENT(world, PillarSpawner);
	ECS_TRIGGER(world, System_Init_Pillars, EcsOnSet, PillarSpawner);

	{
		ecs_entity_t e1 = ecs_new(world, 0);
		ecs_entity_t e2 = ecs_new(world, 0);
		ecs_entity_t e3 = ecs_new(world, 0);
		ecs_entity_t e4 = ecs_new(world, 0);


		ecs_set(world, e1, EgDraw1, {2});
		ecs_set(world, e2, EgDraw, {2});
		ecs_set(world, e3, EgDraw, {2});
		ecs_set(world, e4, EgDraw, {2});
		ecs_set(world, e1, EgColor, {20, 50, 220, 255});
		ecs_set(world, e2, EgColor, {20, 200, 220, 255});
		ecs_set(world, e3, EgColor, {150, 200, 20, 255});
		ecs_set(world, e4, EgColor, {100, 100, 100, 255});
		ecs_set(world, e1, EgBoxF32, {1.0f, 5.0f, 32.0f});
		ecs_set(world, e2, EgBoxF32, {1.0f, 5.0f, 32.0f});
		ecs_set(world, e3, EgBoxF32, {32.0f, 5.0f, 1.0f});
		ecs_set(world, e4, EgBoxF32, {32.f, 0.1f, 32.f});
		ecs_set(world, e1, EgPosition3F32, {-16.0f, 2.5f, 0.0f});
		ecs_set(world, e2, EgPosition3F32, {16.0f, 2.5f, 0.0f});
		ecs_set(world, e3, EgPosition3F32, {0.0f, 2.5f, 16.0f});
		ecs_set(world, e4, EgPosition3F32, {0.f, -0.1f * 0.5f, 0.f});
	}

	{
		ecs_entity_t e1 = ecs_new(world, 0);
		ecs_entity_t e2 = ecs_new(world, 0);
		ecs_entity_t e3 = ecs_new(world, 0);
		ecs_entity_t e4 = ecs_new(world, 0);
		ecs_entity_t e5 = ecs_new(world, 0);
		ecs_entity_t e6 = ecs_new(world, 0);
		ecs_set(world, e1, EgDraw, {2});
		ecs_set(world, e2, EgDraw, {1});
		ecs_set(world, e3, EgDraw, {2});
		ecs_set(world, e4, EgDraw, {1});
		ecs_set(world, e5, EgDraw, {2});
		ecs_set(world, e6, EgDraw, {1});
		ecs_set(world, e3, EgText, {"FPS Camera Controls:"});
		ecs_set(world, e4, EgText, {"- Move: W, A, S, D"});
		ecs_set(world, e5, EgText, {"- Mouse to look"});
		ecs_set(world, e6, EgText, {"- Shift to run"});
		ecs_set(world, e1, EgColor, {10, 50, 150, 128});
		ecs_set(world, e2, EgColor, {10, 50, 220, 255});
		ecs_set(world, e3, EgColor, {0, 0, 0, 255});
		ecs_set(world, e4, EgColor, {20, 20, 20, 255});
		ecs_set(world, e5, EgColor, {20, 20, 20, 255});
		ecs_set(world, e6, EgColor, {20, 20, 20, 255});
		ecs_set(world, e1, EgRectangleF32, {220.f, 70.f});
		ecs_set(world, e2, EgRectangleF32, {220.f, 70.f});
		ecs_set(world, e1, EgPosition2F32, {10.f, 10.f});
		ecs_set(world, e2, EgPosition2F32, {10.f, 10.f});
		ecs_set(world, e3, EgPosition2F32, {20.f, 25.f});
		ecs_set(world, e4, EgPosition2F32, {40.f, 40.f});
		ecs_set(world, e5, EgPosition2F32, {40.f, 55.f});
		ecs_set(world, e6, EgPosition2F32, {40.f, 70.f});
	}


	{
		ecs_set(world, 0, PillarSpawner, {3});
		ecs_set(world, 0, PillarSpawner, {3});
		ecs_set(world, 0, PillarSpawner, {4});
	}

	return loop_gs(argc, argv, world);
}






