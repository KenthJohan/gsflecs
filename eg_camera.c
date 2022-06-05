#include "eg_camera.h"



ECS_COMPONENT_DECLARE(EgCamera3D);
ECS_COMPONENT_DECLARE(EgCamera2D);
ECS_COMPONENT_DECLARE(EgScene);






void EgCameraImport(ecs_world_t *world)
{
	ECS_MODULE(world, EgCamera);

	ECS_COMPONENT_DEFINE(world, EgCamera3D);
	ECS_COMPONENT_DEFINE(world, EgCamera2D);
	ECS_COMPONENT_DEFINE(world, EgScene);

	ecs_set_name_prefix(world, "Eg");

	ecs_struct_init(world, &(ecs_struct_desc_t) {
	.entity.entity = ecs_id(EgScene),
	.members = {
	{ .name = "a", .type = ecs_id(ecs_string_t) },
	{ .name = "enabled_depth", .type = ecs_id(ecs_bool_t) },
	{ .name = "enabled_facecull", .type = ecs_id(ecs_bool_t) },
	{ .name = "query", .type = ecs_id(ecs_uptr_t) }
	}
	});

	ecs_struct_init(world, &(ecs_struct_desc_t) {
	.entity.entity = ecs_id(EgCamera3D),
	.members = {
	{ .name = "x", .type = ecs_id(ecs_f32_t) },
	{ .name = "y", .type = ecs_id(ecs_f32_t) },
	{ .name = "z", .type = ecs_id(ecs_f32_t) }
	}
	});

	ecs_struct_init(world, &(ecs_struct_desc_t) {
	.entity.entity = ecs_id(EgCamera2D),
	.members = {
	{ .name = "x", .type = ecs_id(ecs_f32_t) },
	{ .name = "y", .type = ecs_id(ecs_f32_t) },
	{ .name = "z", .type = ecs_id(ecs_f32_t) }
	}
	});

}

