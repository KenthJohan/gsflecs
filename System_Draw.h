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

