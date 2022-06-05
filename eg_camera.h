#pragma once
#include "flecs.h"



typedef struct
{
	ecs_f32_t x;
	ecs_f32_t y;
	ecs_f32_t z;
} EgCamera3D;

typedef struct
{
	ecs_f32_t x;
	ecs_f32_t y;
	ecs_f32_t z;
} EgCamera2D;

typedef struct
{
	ecs_string_t a;
	ecs_bool_t enabled_depth;
	ecs_bool_t enabled_facecull;
	ecs_query_t * query;
} EgScene;



extern ECS_COMPONENT_DECLARE(EgCamera3D);
extern ECS_COMPONENT_DECLARE(EgCamera2D);
extern ECS_COMPONENT_DECLARE(EgScene);


void EgCameraImport(ecs_world_t *world);
