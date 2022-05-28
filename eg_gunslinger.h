#pragma once
#include "flecs.h"


typedef struct
{
	ecs_u64_t flags;
	ecs_u64_t counter;
	ecs_bool_t should_destroy;
} EgWindow;

typedef struct
{
	ecs_f32_t position;
} EgCamera;



typedef struct
{
	ecs_u32_t kind;
} EgDraw;

typedef struct
{
	ecs_u32_t dummy;
} EgDraw1;

extern ECS_COMPONENT_DECLARE(EgWindow);
extern ECS_COMPONENT_DECLARE(EgDraw);
extern ECS_COMPONENT_DECLARE(EgDraw1);


void EgGunslingerImport(ecs_world_t *world);





int32_t loop_gs(int32_t argv, char** argc, ecs_world_t * world);


void eg_add_immediate_draw(ecs_entity_t e);
void eg_add_camera(ecs_entity_t e);
