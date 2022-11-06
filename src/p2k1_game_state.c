#include "p2k1_game_state.h"
#include "p2k1_capsule_collision.h"

#include <string.h>



void p2k1_init_game_state( GameState *gs /* game_settings_obj */)
{
    // set up inital positions, character states, stage states, frame number = 0
    memset(gs, 0, sizeof(GameState));
}

#define CAMERA_ANCHOR (Vector3){ 11.0f, 5.0f, 0.0f } 

void p2k1_init_game_render_state( GameRenderState *rs /* game_settings_obj */)
{
    // load assets needed for game, init camera position, render frame number = 0

    rs->camera_target = (Vector3){0.0f, 0.0f, 0.0f};
    rs->camera_position = CAMERA_ANCHOR;
    rs->render_frame_number = 0;

}

#define TIP_HEIGHT_OFFSET fix16_one

#define TIP_AMPLITUDE (fix16_one >> 2)
#define BASE_AMPLITUDE (fix16_one >> 3)
#define WIGGLE_FREQUENCY (fix16_one >> 3)
#define BASE_WIGGLE_PHASE fix16_one

#define RADIUS_OFFSET fix16_one
#define RADIUS_GROWTH_FACTOR (fix16_one << 7) // FINAL

#define TIP_POSITION_SCALE (fix16_one >> 4)
#define BASE_POSITION_SCALE ((fix16_one >> 5) + (fix16_one >> 6))

#define MAX_LOCKOUT_FRAMES 40
#define TIE_THRESHOLD 5 // fix16_t but should be reeeeallly small


static fgl_vec3_t p1_tip_offset  = {0, fix16_one << 1, fix16_one * -4};
static fgl_vec3_t p1_base_offset = {0, 0,              fix16_one * -4};

static fgl_vec3_t p2_tip_offset  = {0, fix16_one << 1, fix16_one *  4};
static fgl_vec3_t p2_base_offset = {0, 0,              fix16_one *  4};

void p2k1_advance_game_state(const GameInputs *p1_input, const GameInputs *p2_input, GameState *gs)
{
    // calc next capsule positions & radius
    // note: typically we would have speed & possibly acceleration and use newtons method of integration to get position

    // p1 capsule
    gs->p1_tip = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ fix16_mul(fix16_from_int(p1_input->primary_y), TIP_POSITION_SCALE),
        /* Y */ fix16_mul( TIP_AMPLITUDE, fix16_sin(fix16_mul(fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY))), // bob and weave
        /* Z */ fix16_mul(fix16_from_int(p1_input->primary_x), TIP_POSITION_SCALE),
    }, p1_tip_offset);

    gs->p1_base = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ fix16_mul(fix16_from_int(p1_input->secondary_y), BASE_POSITION_SCALE),
        /* Y */ fix16_mul( BASE_AMPLITUDE, fix16_sin( fix16_add( BASE_WIGGLE_PHASE, fix16_mul( fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY )))),
        /* Z */ fix16_mul(fix16_from_int(p1_input->secondary_x), BASE_POSITION_SCALE),
    }, p1_base_offset);

    gs->p1_rad = fix16_add(RADIUS_OFFSET, fix16_mul(RADIUS_GROWTH_FACTOR, p1_input->trigger_result));


    // p2 capsule
    gs->p2_tip = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ fix16_mul(fix16_from_int(p2_input->primary_y), TIP_POSITION_SCALE),
        /* Y */ fix16_mul( TIP_AMPLITUDE, fix16_cos(fix16_mul(fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY))), // bob and weave
        /* Z */ fix16_mul(fix16_from_int(p2_input->primary_x), TIP_POSITION_SCALE),
    }, p2_tip_offset);

    gs->p2_base = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ fix16_mul(fix16_from_int(p2_input->secondary_y), BASE_POSITION_SCALE),
        /* Y */ fix16_mul( BASE_AMPLITUDE, fix16_cos( fix16_add( BASE_WIGGLE_PHASE, fix16_mul( fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY )))),
        /* Z */ fix16_mul(fix16_from_int(p2_input->secondary_x), BASE_POSITION_SCALE),
    }, p2_base_offset);

    gs->p2_rad = fix16_add(RADIUS_OFFSET, fix16_mul(RADIUS_GROWTH_FACTOR, p2_input->trigger_result));

    // decriment collision lockout TODO: max
    gs->collision_lockout = (gs->collision_lockout == 0) ? 0 : gs->collision_lockout - 1;

    /*
    gs->p1_tip  = (fgl_vec3_t){0, fix16_one, fix16_one * -4};
    gs->p1_base = (fgl_vec3_t){0, 0,         fix16_one * -4};
    
    gs->p2_tip  = (fgl_vec3_t){0, fix16_one, fix16_one *  4};
    gs->p2_base = (fgl_vec3_t){0, 0,         fix16_one *  4};
    */

    // collision detection
    if(capsule_collision(gs->p1_tip, gs->p1_base, gs->p1_rad, gs->p2_tip, gs->p2_base, gs->p2_rad))
    {
        if(gs->collision_lockout == 0)
        {
            // determine who loses the interaction
            // each player computes the distance from their tip to the opponents base
            // the player with the smaller distance wins the interaction

            // compute the damage to be dealt in the interaction
            // TODO: impl
            int damage = 1;

            fix16_t p1_tip_to_opp_base = fgl_vec3_magnitude(fgl_vec3_sub(
                gs->p1_tip,
                gs->p2_base
            ));

            fix16_t p2_tip_to_opp_base = fgl_vec3_magnitude(fgl_vec3_sub(
                gs->p2_tip,
                gs->p1_base
            ));

            // apply damage to loser of the interaction
            if     (p1_tip_to_opp_base > (p2_tip_to_opp_base + TIE_THRESHOLD))
            {
                gs->p2_hp -= damage;
            }
            else if(p2_tip_to_opp_base > (p1_tip_to_opp_base + TIE_THRESHOLD))
            {
                gs->p1_hp -= damage;
            }
            else // TIE
            {
                // TODO: impl
            }
        }

        // reset collision lockout
        gs->collision_lockout = MAX_LOCKOUT_FRAMES;
    }

    gs->frame_number += 1;
}

#define CAMERA_UPDATE_WEIGHT 0.9f  // 0.9f = 90% previous camera values, 10% curent camera values. closer to 1.0f means slower camera movement
#define CAMERA_ZOOM_SCALE 0.1f
// see  CAMERA_ANCHOR

void p2k1_advance_game_render_state(const GameState *gs, GameRenderState *rs /* delta time */)
{

    // calc camera position, zoom, & target
    // TODO: independant vec3 library would be nice to have

    Vector3 camera_target = Vector3Add(
                                Vector3Scale(rs->camera_target, CAMERA_UPDATE_WEIGHT),
                                Vector3Scale(
                                    Vector3Add(
                                        Vector3Add(fgl_vec3_to_float_vector3(gs->p1_tip), fgl_vec3_to_float_vector3(gs->p1_base)),
                                        Vector3Add(fgl_vec3_to_float_vector3(gs->p2_tip), fgl_vec3_to_float_vector3(gs->p2_base))
                                    ), 0.25f * (1.0f - CAMERA_UPDATE_WEIGHT)
                                )
                            );

    // TODO: make camera position and target dependant on rs->camera_mode
    Vector3 camera_dir = Vector3Subtract(CAMERA_ANCHOR, rs->camera_target); 

    // proportional to largest distance between p1 an p2
    float d1 = Vector3Length(Vector3Subtract(fgl_vec3_to_float_vector3(gs->p1_tip),  fgl_vec3_to_float_vector3(gs->p2_tip) ));
    float d2 = Vector3Length(Vector3Subtract(fgl_vec3_to_float_vector3(gs->p1_tip),  fgl_vec3_to_float_vector3(gs->p2_base)));
    float d3 = Vector3Length(Vector3Subtract(fgl_vec3_to_float_vector3(gs->p1_base), fgl_vec3_to_float_vector3(gs->p2_base)));

    float max_dist = d1;
    if(d2 > max_dist) max_dist = d2;
    if(d3 > max_dist) max_dist = d3;


    float camera_distance = 0.9 + max_dist * CAMERA_ZOOM_SCALE; // looks good between about 1.1f and 1.3f
    
    rs->camera_position =   Vector3Add(
                                Vector3Scale( rs->camera_position, CAMERA_UPDATE_WEIGHT),
                                Vector3Scale( Vector3Add(Vector3Scale(camera_dir, camera_distance), rs->camera_target), 1.0f - CAMERA_UPDATE_WEIGHT)
                            );

    //rs->camera_position = Vector3Add(Vector3Scale(camera_dir, camera_distance), rs->camera_target);
    rs->camera_target = camera_target;
    /*
    switch(rs->camera_mode)
    {
        case PLAYER1:
        break;
        case PLAYER2:
        break;
        case SIDE:
        break;
        case TOP:
        break;
        case FIXED_SIDE:
        break;
        case FIXED_TOPDOWN:
        break;
        case FREE:
    }
    */

    // calc colors based on collision_lockout & MAX_LOCKOUT_FRAMES
    rs->p1_tri_color = VIOLET;
    rs->p1_wire_color = PURPLE;
    
    rs->p2_tri_color = GRAY;
    rs->p2_wire_color = DARKGREEN;

    if(gs->collision_lockout > 0)
    {
        // change colors based on lockout value
        rs->p1_tri_color  = (Color){gs->collision_lockout * 2 + 200, gs->collision_lockout * 3, gs->collision_lockout * 4, 255};        
        rs->p2_tri_color  = (Color){gs->collision_lockout * 2, gs->collision_lockout * 3 + 50, gs->collision_lockout * 4 + 50, 255};
    }

    rs->render_frame_number += 1;
}

void p2k1_render_frame(const GameState *gs, const GameRenderState *rs)
{
    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    camera.position = rs->camera_position;
    camera.target = rs->camera_target;

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

            // render capsules
            DrawCapsule     (fgl_vec3_to_float_vector3(gs->p1_base), fgl_vec3_to_float_vector3(gs->p1_tip), fix16_to_float(gs->p1_rad), 8, 8, rs->p1_tri_color );
            DrawCapsuleWires(fgl_vec3_to_float_vector3(gs->p1_base), fgl_vec3_to_float_vector3(gs->p1_tip), fix16_to_float(gs->p1_rad), 8, 8, rs->p1_wire_color);

            DrawCapsule     (fgl_vec3_to_float_vector3(gs->p2_base), fgl_vec3_to_float_vector3(gs->p2_tip), fix16_to_float(gs->p2_rad), 8, 8, rs->p2_tri_color );
            DrawCapsuleWires(fgl_vec3_to_float_vector3(gs->p2_base), fgl_vec3_to_float_vector3(gs->p2_tip), fix16_to_float(gs->p2_rad), 8, 8, rs->p2_wire_color);

            // render ground
            DrawGrid(10, 1.0f);

            // render fluff (boxes scattered around the edge of the field)
            // TODO: see above comment

        EndMode3D();

        // Draw ui

        
        //DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
        //DrawRectangleLines( 10, 10, 320, 133, BLUE);
        /*
        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
        DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
        DrawText("PRESS SPACE to PAUSE FRAME ADVANCE", 10, GetScreenHeight() - 60, 20, GRAY);
        */
        
        /*
        DrawText(TextFormat("TARGET  FPS: %i",   targetFPS),             GetScreenWidth() - 220, 10, 20, DARKGREEN);
        DrawText(TextFormat("CURRENT FPS: %i",   (int)(1.0f/deltaTime)), GetScreenWidth() - 220, 40, 20, LIME);
        DrawText(TextFormat("FRAME: %i", frameCount),                    GetScreenWidth() - 220, 70, 20, GREEN);

        if(gcaLoaded){
            DrawText("GCA+ loaded",         GetScreenWidth() - 220, 100, 20, GREEN);
        }
        else{
            DrawText("GCA+ failed to load", GetScreenWidth() - 220, 100, 20, RED);
        }
        */

    EndDrawing();
    SwapScreenBuffer();
}