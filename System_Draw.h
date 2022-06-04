#pragma once
#include "eg_gunslinger.h"
#include "eg_geometry.h"
#include "eg_quantity.h"
#include "eg_camera.h"

#include <gunslinger/gs.h>
#include <gunslinger/util/gs_idraw.h>
#include <gunslinger/util/gs_gui.h>

static void System_Draw3D_Box(ecs_world_t * world, ecs_query_t *query, gs_immediate_draw_t * gsi)
{
	ecs_iter_t it = ecs_query_iter(world, query);
	while (ecs_query_next(&it))
	{
		EgScene *scene = ecs_term(&it, EgScene, 1); //Parent
		EgDraw *draw = ecs_term(&it, EgDraw, 2);
		EgPosition3F32 *position = ecs_term(&it, EgPosition3F32, 3);
		EgBoxF32 *box = ecs_term(&it, EgBoxF32, 4);
		EgColor *color = ecs_term(&it, EgColor, 5);
		for (int i = 0; i < it.count; i ++)
		{
			gs_graphics_primitive_type t = draw[i].kind;
			if (t == 10)
			{
				gsi_box(gsi,
				position[i].x,position[i].y,position[i].z,
				box[i].x,box[i].y,box[i].z,
				color[i].r^0xFF,color[i].g^0xFF,color[i].b^0xFF,0xFF,
				GS_GRAPHICS_PRIMITIVE_LINES);
				t = GS_GRAPHICS_PRIMITIVE_TRIANGLES;
			}

			gsi_box(gsi,
			position[i].x,position[i].y,position[i].z,
			box[i].x,box[i].y,box[i].z,
			color[i].r,color[i].g,color[i].b,color[i].a,
			t);

		}
	}
}


static void System_Draw3D_Box1(ecs_world_t * world, ecs_query_t *query, gs_immediate_draw_t * gsi)
{
	ecs_iter_t it = ecs_query_iter(world, query);
	while (ecs_query_next(&it))
	{
		//EgScene *scene = ecs_term(&it, EgScene, 1);
		//EgDraw1 *draw = ecs_term(&it, EgDraw1, 2);
		EgPosition3F32 *position = ecs_term(&it, EgPosition3F32, 3);
		EgBoxF32 *box = ecs_term(&it, EgBoxF32, 4);
		EgColor *color = ecs_term(&it, EgColor, 5);
		for (int i = 0; i < it.count; i ++)
		{
			gsi_box(gsi,
			position[i].x,position[i].y,position[i].z,
			box[i].x,box[i].y,box[i].z,
			color[i].r,color[i].g,color[i].b,color[i].a,
			GS_GRAPHICS_PRIMITIVE_TRIANGLES);

			gsi_box(gsi,
			position[i].x,position[i].y,position[i].z,
			box[i].x,box[i].y,box[i].z,
			//color[i].r^0xFF,color[i].g^0xFF,color[i].b^0xFF,0xFF,
			0x00,0x00,0x00,0xFF,
			GS_GRAPHICS_PRIMITIVE_LINES);
		}
	}
}


static void System_Draw2D_Rect(ecs_world_t * world, ecs_query_t *query, gs_immediate_draw_t * gsi)
{
	ecs_iter_t it = ecs_query_iter(world, query);
	while (ecs_query_next(&it))
	{
		EgDraw *draw = ecs_term(&it, EgDraw, 1);
		EgPosition2F32 *position = ecs_term(&it, EgPosition2F32, 2);
		EgRectangleF32 *rect = ecs_term(&it, EgRectangleF32, 3);
		EgColor *color = ecs_term(&it, EgColor, 4);
		for (int i = 0; i < it.count; i ++)
		{
			gsi_rectvd(gsi,
			gs_v2(position[i].x,position[i].y),
			gs_v2(rect[i].width,rect[i].height),
			gs_v2(0.f, 0.f),
			gs_v2(1.f, 1.f),
			gs_color(color[i].r,color[i].g,color[i].b,color[i].a),
			draw[i].kind);
		}
	}
}


static void System_Draw2D_Text(ecs_world_t * world, ecs_query_t *query, gs_immediate_draw_t * gsi)
{
	ecs_iter_t it = ecs_query_iter(world, query);
	while (ecs_query_next(&it))
	{
		EgDraw *draw = ecs_term(&it, EgDraw, 1);
		EgPosition2F32 *position = ecs_term(&it, EgPosition2F32, 2);
		EgText *text = ecs_term(&it, EgText, 3);
		EgColor *color = ecs_term(&it, EgColor, 4);
		for (int i = 0; i < it.count; i ++)
		{
			gsi_text(gsi, position[i].x,position[i].y, text[i].value, NULL, false, color[i].r,color[i].g,color[i].b,color[i].a);
		}
	}
}





static void System_Draw_Scene(ecs_world_t * world, ecs_query_t *query, gs_immediate_draw_t * gsi, gs_camera_t* cam)
{
	const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
	ecs_iter_t it = ecs_query_iter(world, query);
	while (ecs_query_next(&it))
	{
		EgScene *s = ecs_term(&it, EgScene, 1);
		for (int i = 0; i < it.count; i ++)
		{
			gsi_defaults(gsi);
			//if (ecs_has(it.world, s[i].camera, EgPosition3F32))
			{
				gsi_camera(gsi, cam, fbs.x, fbs.y);
			};
			gsi_depth_enabled(gsi, s[i].enabled_depth);
			gsi_face_cull_enabled(gsi, s[i].enabled_facecull);
			System_Draw3D_Box1(world, s[i].query, gsi);
		}
	}
}







static void System_Camera3D_Controller(ecs_world_t * world, ecs_query_t *query, float dt)
{
	ecs_iter_t it = ecs_query_iter(world, query);
	while (ecs_query_next(&it))
	{
		EgCamera3D *c = ecs_term(&it, EgCamera3D, 1);
		gs_vec3 *p = ecs_term(&it, EgPosition3F32, 2);
		gs_quat *q = ecs_term(&it, EgQuaternionF32, 3);
		for (int i = 0; i < it.count; i ++)
		{

			gs_platform_t* platform = gs_subsystem(platform);
			gs_vec3 dp;
			const float mod = gs_platform_key_down(GS_KEYCODE_LEFT_SHIFT) ? 2.f : 1.f;
			dp.x = gs_platform_key_down(GS_KEYCODE_LEFT) - gs_platform_key_down(GS_KEYCODE_RIGHT);
			dp.y = gs_platform_key_down(GS_KEYCODE_UP) - gs_platform_key_down(GS_KEYCODE_DOWN);
			dp.z = gs_platform_key_down(GS_KEYCODE_Q) - gs_platform_key_down(GS_KEYCODE_E);
			//gs_camera_offset_orientation1(cam, dp.x, dp.y, dp.z);

			gs_quat x = gs_quat_angle_axis(gs_deg2rad(dp.x), gs_v3(0.f, 1.f, 0.f));
			gs_quat y = gs_quat_angle_axis(gs_deg2rad(dp.y), gs_v3(1.f, 0.f, 0.f));
			gs_quat z = gs_quat_angle_axis(gs_deg2rad(dp.z), gs_v3(0.f, 0.f, 1.f));
			q[i] = gs_quat_mul(q[i], y);
			q[i] = gs_quat_mul(q[i], x);
			q[i] = gs_quat_mul(q[i], z);

			gs_vec3 vel = {0};
			if (gs_platform_key_down(GS_KEYCODE_W))            vel = gs_quat_rotate(q[i], gs_v3( 0.0f,  0.0f, -1.0f));
			if (gs_platform_key_down(GS_KEYCODE_S))            vel = gs_quat_rotate(q[i], gs_v3( 0.0f,  0.0f,  1.0f));
			if (gs_platform_key_down(GS_KEYCODE_A))            vel = gs_quat_rotate(q[i], gs_v3(-1.0f,  0.0f,  0.0f));
			if (gs_platform_key_down(GS_KEYCODE_D))            vel = gs_quat_rotate(q[i], gs_v3( 1.0f,  0.0f,  0.0f));
			if (gs_platform_key_down(GS_KEYCODE_SPACE))        vel = gs_quat_rotate(q[i], gs_v3( 0.0f,  1.0f,  0.0f));
			if (gs_platform_key_down(GS_KEYCODE_LEFT_CONTROL)) vel = gs_quat_rotate(q[i], gs_v3( 0.0f, -1.0f,  0.0f));

			float cam_speed = 5.0f;
			p[i] = gs_vec3_add(p[i], gs_vec3_scale(gs_vec3_norm(vel), dt * cam_speed * mod));
		}
	}
}

