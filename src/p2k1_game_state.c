#include "p2k1_game_state.h"

#include "p2k1_capsule_collision.h"

#define TIP_MAX_SPEED  (1 << 7)           // fix16 units/frame
#define BASE_MAX_SPEED (1 << 7) + ( 1 << 6)

#define TIP_HEIGHT_OFFSET fix16_one
#define TIP_AMPLITUDE (1 << 5)
#define BASE_AMPLITUDE (1 << 5)
#define WIGGLE_FREQUENCY (1 << 3)
#define BASE_WIGGLE_PHASE (1 << 3)
#define RADIUS_OFFSET (1 << 7)
#define RADIUS_GROWTH_FACTOR 1
#define STICK_SENSITIVITY 1 // scalar for raw int, not fix16
#define MAX_LOCKOUT_FRAMES 8
#define TIE_THRESHOLD 5 // fix16_t but should be reeeeallly small


// TODO make these static const?
//fgl_vec3_t p1_start_position = (fgl_vec3_t) { 0, fix16_one, 0 }; 
//fgl_vec3_t p2_start_position = (fgl_vec3_t) { 0, fix16_one * -1, 0 }; 

void p2k1_advance_game_state(GameInputs *p1_input, GameInputs *p2_input, GameState *gs)
{
    // calc next capsule positions & radius

    // p1 capsule
    gs->p1_tip = (fgl_vec3_t)
    {
        /* X */ gs->p1_tip.x + fix16_clamp(STICK_SENSITIVITY * p1_input->mainStickHorizontal, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ TIP_HEIGHT_OFFSET + fix16_mul( TIP_AMPLITUDE, fix16_sin(fix16_mul(gs->frame_number, WIGGLE_FREQUENCY))), // bob and weave
        /* Z */ gs->p1_tip.z + fix16_clamp(STICK_SENSITIVITY * p1_input->mainStickVertical,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    };
    gs->p1_base = (fgl_vec3_t)
    {
        /* X */ gs->p1_tip.x + fix16_clamp(STICK_SENSITIVITY * p1_input->cStickHorizontal, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ fix16_mul( BASE_AMPLITUDE, fix16_sin(BASE_WIGGLE_PHASE + fix16_mul(gs->frame_number, WIGGLE_FREQUENCY))),
        /* Z */ gs->p1_tip.z + fix16_clamp(STICK_SENSITIVITY * p1_input->cStickVertical,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    };
    gs->p1_rad = fix16_add(RADIUS_OFFSET, fix16_mul(RADIUS_GROWTH_FACTOR, p1_input->triggerResult));


    // p2 capsule
    gs->p2_tip = (fgl_vec3_t)
    {
        /* X */ gs->p2_tip.x + fix16_clamp(STICK_SENSITIVITY * p2_input->mainStickHorizontal, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ TIP_HEIGHT_OFFSET + fix16_mul( TIP_AMPLITUDE, fix16_cos(fix16_mul(gs->frame_number, WIGGLE_FREQUENCY))), // bob and weave
        /* Z */ gs->p2_tip.z + fix16_clamp(STICK_SENSITIVITY * p2_input->mainStickVertical,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    };
    gs->p2_base = (fgl_vec3_t)
    {
        /* X */ gs->p2_tip.x + fix16_clamp(STICK_SENSITIVITY * p2_input->cStickHorizontal, TIP_MAX_SPEED * -1, TIP_MAX_SPEED), 
        /* Y */ fix16_mul( BASE_AMPLITUDE, fix16_cos(BASE_WIGGLE_PHASE + fix16_mul(gs->frame_number, WIGGLE_FREQUENCY))),
        /* Z */ gs->p2_tip.z + fix16_clamp(STICK_SENSITIVITY * p2_input->cStickVertical,   TIP_MAX_SPEED * -1, TIP_MAX_SPEED) 
    };
    gs->p2_rad = fix16_add(RADIUS_OFFSET, fix16_mul(RADIUS_GROWTH_FACTOR, p2_input->triggerResult));

    // decriment collision lockout TODO: max
    //gs->collision_lockout = max(gs->collision_lockout - 1, 0);


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
}

void p2k1_render_current_frame(GameState *gs, GameRenderState *grs, bool is_player_1 /* delta time */)
{
    // calc colors based on collision_lockout & MAX_LOCKOUT_FRAMES

    // render capsules

    // render ground

    // render fluff (boxes scattered around the edge of the field)
}