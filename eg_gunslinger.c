#define GS_IMPL
#define GS_NO_HIJACK_MAIN
#define GS_IMMEDIATE_DRAW_IMPL
//#define GS_GUI_IMPL
#include <gunslinger/gs.h>
#include <gunslinger/util/gs_idraw.h>
#include <gunslinger/util/gs_gui.h>

#include "flecs.h"
#include "System_Draw.h"
#include "eg_gunslinger.h"
#include "eg_geometry.h"
#include "eg_quantity.h"



ECS_COMPONENT_DECLARE(EgWindow);
ECS_COMPONENT_DECLARE(EgDraw);
ECS_COMPONENT_DECLARE(EgDraw1);

#define CAM_SPEED 5



enum {
	GUI_STYLE_ROOT = 0x00,
	GUI_STYLE_TITLE,
	GUI_STYLE_BUTTON,
	GUI_STYLE_COUNT
};

enum {
	GUI_FONT_LABEL = 0x00,
	GUI_FONT_BUTTON,
	GUI_FONT_BUTTONFOCUS,
	GUI_FONT_COUNT
};



typedef struct
{
	ecs_world_t * world;
	gs_command_buffer_t cb;
	gs_gui_context_t    gui;
	gs_immediate_draw_t gsi;
	gs_asset_font_t fonts[GUI_FONT_COUNT];
	gs_asset_texture_t logo;
	gs_asset_texture_t bg;
	gs_gui_style_sheet_t menu_style_sheet;
	gs_camera_t cam;
	ecs_query_t * query_draw3d_box;
	ecs_query_t * query_draw3d_box1;
	ecs_query_t * query_draw2d_rect;
	ecs_query_t * query_draw2d_text;
} app_t;
app_t g_app = {0};








static void gs_camera_offset_orientation1(gs_camera_t* cam, f32 yaw, f32 pitch, f32 roll)
{
	gs_quat x = gs_quat_angle_axis(gs_deg2rad(yaw), gs_v3(0.f, 1.f, 0.f));
	gs_quat y = gs_quat_angle_axis(gs_deg2rad(pitch), gs_v3(1.f, 0.f, 0.f));
	gs_quat z = gs_quat_angle_axis(gs_deg2rad(roll), gs_v3(0.f, 0.f, 1.f));
	cam->transform.rotation = gs_quat_mul(cam->transform.rotation, y);
	cam->transform.rotation = gs_quat_mul(cam->transform.rotation, x);
	cam->transform.rotation = gs_quat_mul(cam->transform.rotation, z);
}




#define rand_range(MIN, MAX) (rand() % (MAX - MIN + 1) + MIN)



static void app_init()
{
	app_t* app = gs_user_data(app_t);
	app->cb = gs_command_buffer_new();
	app->gsi = gs_immediate_draw_new(gs_platform_main_window());
	gs_gui_init(&app->gui, gs_platform_main_window());
	app->cam = gs_camera_perspective();
	app->cam.transform.position = gs_v3(4.f, 2.f, 4.f);
}







static void app_update()
{
	app_t* app = gs_user_data(app_t);
	ecs_world_t * world = app->world;
	gs_gui_context_t* gui = &app->gui;
	gs_command_buffer_t* cb = &app->cb;
	gs_immediate_draw_t* gsi = &app->gsi;
	gs_camera_t* cam = &app->cam;
	gs_camera_t cam2 = *cam;

	const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
	if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_quit();


	gs_platform_t* platform = gs_subsystem(platform);
	gs_vec3 dp;
	const float mod = gs_platform_key_down(GS_KEYCODE_LEFT_SHIFT) ? 2.f : 1.f;
	float dt = platform->time.delta;
	dp.x = gs_platform_key_down(GS_KEYCODE_LEFT) - gs_platform_key_down(GS_KEYCODE_RIGHT);
	dp.y = gs_platform_key_down(GS_KEYCODE_UP) - gs_platform_key_down(GS_KEYCODE_DOWN);
	dp.z = gs_platform_key_down(GS_KEYCODE_Q) - gs_platform_key_down(GS_KEYCODE_E);
	gs_camera_offset_orientation1(cam, dp.x, dp.y, dp.z);
	gs_vec3 vel = {0};
	if (gs_platform_key_down(GS_KEYCODE_W)) vel = gs_vec3_add(vel, gs_camera_forward(cam));
	if (gs_platform_key_down(GS_KEYCODE_S)) vel = gs_vec3_add(vel, gs_camera_backward(cam));
	if (gs_platform_key_down(GS_KEYCODE_A)) vel = gs_vec3_add(vel, gs_camera_left(cam));
	if (gs_platform_key_down(GS_KEYCODE_D)) vel = gs_vec3_add(vel, gs_camera_right(cam));
	if (gs_platform_key_down(GS_KEYCODE_SPACE)) vel = gs_vec3_add(vel, gs_camera_up(cam));
	if (gs_platform_key_down(GS_KEYCODE_LEFT_CONTROL)) vel = gs_vec3_add(vel, gs_camera_down(cam));
	cam->transform.position = gs_vec3_add(cam->transform.position, gs_vec3_scale(gs_vec3_norm(vel), dt * CAM_SPEED * mod));



	// Draw 3D
	gsi_camera(gsi, cam, fbs.x, fbs.y);
	gsi_depth_enabled(gsi, true);
	gsi_face_cull_enabled(gsi, true);
	System_Draw3D_Box(world, g_app.query_draw3d_box, gsi);

	cam2.transform.position = gs_v3(4.f, 2.f, 4.f);
	//gsi_camera(gsi, &cam2, fbs.x, fbs.y);
	System_Draw3D_Box1(world, g_app.query_draw3d_box1, gsi);


	// Draw 2D
	gsi_defaults(gsi);
	gsi_camera2D(gsi, fbs.x, fbs.y);
	System_Draw2D_Rect(world, g_app.query_draw2d_rect, gsi);
	System_Draw2D_Text(world, g_app.query_draw2d_text, gsi);
	gsi_renderpass_submit(gsi, cb, fbs.x, fbs.y, GS_COLOR_WHITE);


	// Draw GUI
	gs_gui_begin(gui, fbs);
	static bool debug_enabled = true;
	if (gs_platform_key_pressed(GS_KEYCODE_F1)) debug_enabled = !debug_enabled;
	gs_gui_demo_window(gui, gs_gui_rect(200, 100, 500, 250), &debug_enabled);
	gs_gui_end(gui);
	gs_gui_render(gui, cb);



	// Submit command buffer (syncs to GPU, MUST be done on main thread where you have your GPU context created)
	gs_graphics_command_buffer_submit(cb);


	//ecs_progress(app->world, 0);
}















void EgGunslingerImport(ecs_world_t *world)
{
	ECS_MODULE(world, EgGunslinger);
	ECS_IMPORT(world, EgQuantity);
	ECS_IMPORT(world, EgGeometry);

	ECS_COMPONENT_DEFINE(world, EgWindow);
	ECS_COMPONENT_DEFINE(world, EgDraw);
	ECS_COMPONENT_DEFINE(world, EgDraw1);

	ecs_set_name_prefix(world, "Eg");

	ecs_struct_init(world, &(ecs_struct_desc_t) {
	.entity.entity = ecs_id(EgWindow),
	.members = {
	{ .name = "flags", .type = ecs_id(ecs_u64_t) },
	{ .name = "counter", .type = ecs_id(ecs_u64_t) },
	{ .name = "should_destroy", .type = ecs_id(ecs_bool_t) },
	}
	});


	g_app.query_draw3d_box = ecs_query_init(world, &(ecs_query_desc_t) {
	.filter.expr = "EgDraw, EgPosition3F32, EgBoxF32, EgColor",
	.filter.instanced = true
	});

	g_app.query_draw3d_box1 = ecs_query_init(world, &(ecs_query_desc_t) {
	.filter.expr = "EgDraw1, EgPosition3F32, EgBoxF32, EgColor",
	.filter.instanced = true
	});

	g_app.query_draw2d_rect = ecs_query_init(world, &(ecs_query_desc_t) {
	.filter.expr = "EgDraw, EgPosition2F32, EgRectangleF32, EgColor",
	.filter.instanced = true
	});

	g_app.query_draw2d_text = ecs_query_init(world, &(ecs_query_desc_t) {
	.filter.expr = "EgDraw, EgPosition2F32, EgText, EgColor",
	.filter.instanced = true
	});

	gs_app_desc_t app = {0};
	g_app.world = world;
	app.user_data = &g_app;
	app.init = app_init;
	app.update = app_update;
	gs_create(app);
}



int32_t loop_gs(int32_t argv, char** argc, ecs_world_t * world)
{
	while (gs_app()->is_running)
	{
		gs_frame();
		ecs_progress(world, 1);
	}
	// Free engine
	gs_free(gs_instance());
	return 0;
}

