#include "p2k1_game_state.h"
#include "p2k1_capsule_collision.h"



void p2k1_init_game_state( GameState *gs /* game_settings_obj */)
{
    // set up inital positions, character states, stage states, frame number = 0
}

void p2k1_init_game_render_state( GameRenderState *rs /* game_settings_obj */)
{
    // load assets needed for game, init camera position, render frame number = 0
}



#define TIP_MAX_SPEED  (1 << 7)           // fix16 units/frame
#define BASE_MAX_SPEED (1 << 7) + ( 1 << 6)

#define TIP_HEIGHT_OFFSET fix16_one
#define TIP_AMPLITUDE (fix16_one >> 2)
#define BASE_AMPLITUDE (fix16_one >> 3)
#define WIGGLE_FREQUENCY (fix16_one >> 3)
#define BASE_WIGGLE_PHASE fix16_one
#define RADIUS_OFFSET fix16_one
#define RADIUS_GROWTH_FACTOR (fix16_one << 7) // FINAL
#define STICK_SENSITIVITY 1 // scalar for raw int, not fix16
#define MAX_LOCKOUT_FRAMES 8
#define TIE_THRESHOLD 5 // fix16_t but should be reeeeallly small


static fgl_vec3_t p1_tip_offset  = {0, fix16_one << 1, fix16_one * -4};
static fgl_vec3_t p1_base_offset = {0, 0,              fix16_one * -4};

static fgl_vec3_t p2_tip_offset  = {0, fix16_one << 1, fix16_one *  4};
static fgl_vec3_t p2_base_offset = {0, 0,              fix16_one *  4};

void p2k1_advance_game_state(const GameInputs *p1_input, const GameInputs *p2_input, GameState *gs)
{
    // calc next capsule positions & radius

    // p1 capsule
    gs->p1_tip = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ 0, // gs->p1_tip.x + fix16_clamp(STICK_SENSITIVITY * p1_input->primary_x, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ fix16_mul( TIP_AMPLITUDE, fix16_sin(fix16_mul(fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY))), // bob and weave
        /* Z */ 0,/*gs->p1_tip.z +*/ //fix16_clamp(STICK_SENSITIVITY * p1_input->primary_y,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    }, p1_tip_offset);

    gs->p1_base = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ 0, // gs->p1_tip.x + fix16_clamp(STICK_SENSITIVITY * p1_input->secondary_x, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ fix16_mul( BASE_AMPLITUDE, fix16_sin(BASE_WIGGLE_PHASE + fix16_mul(fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY))),
        /* Z */ 0 // gs->p1_tip.z + fix16_clamp(STICK_SENSITIVITY * p1_input->secondary_y,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    }, p1_base_offset);

    gs->p1_rad = fix16_add(RADIUS_OFFSET, fix16_mul(RADIUS_GROWTH_FACTOR, p1_input->trigger_result));


    // p2 capsule
    gs->p2_tip = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ 0, // gs->p2_tip.x + fix16_clamp(STICK_SENSITIVITY * p2_input->primary_x, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ fix16_mul( TIP_AMPLITUDE, fix16_cos(fix16_mul(fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY))),
        /* Z */ 0 // gs->p2_tip.z + fix16_clamp(STICK_SENSITIVITY * p2_input->primary_y,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    }, p2_tip_offset);

    gs->p2_base = fgl_vec3_add((fgl_vec3_t)
    {
        /* X */ 0, // gs->p2_tip.x + fix16_clamp(STICK_SENSITIVITY * p2_input->secondary_x, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ fix16_mul( BASE_AMPLITUDE, fix16_cos(BASE_WIGGLE_PHASE + fix16_mul(fix16_from_int(gs->frame_number), WIGGLE_FREQUENCY))),
        /* Z */ 0 // gs->p2_tip.z + fix16_clamp(STICK_SENSITIVITY * p2_input->secondary_y,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
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

#define CAMERA_UPDATE_WEIGHT 0.265f

void p2k1_advance_game_render_state(const GameState *gs, GameRenderState *rs /* delta time */)
{

    // calc camera position, zoom, & target

    // TODO: make camera position and target dependant on rs->camera_mode
    rs->camera_position = (Vector3){ 11.0f, 5.0f, 0.0f }; 


    rs->camera_target = Vector3Add(
                            Vector3Scale(rs->camera_target, CAMERA_UPDATE_WEIGHT),
                            Vector3Scale(
                                Vector3Add(
                                    Vector3Add(fgl_vec3_to_float_vector3(gs->p1_tip), fgl_vec3_to_float_vector3(gs->p1_base)),
                                    Vector3Add(fgl_vec3_to_float_vector3(gs->p2_tip), fgl_vec3_to_float_vector3(gs->p2_base))
                                ), 0.25f
                            )
                        );

    
    
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
        // TODO: change colors based on lockout value
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

        
        DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines( 10, 10, 320, 133, BLUE);
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