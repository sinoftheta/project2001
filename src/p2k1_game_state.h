#ifndef __p2k1_game_state_h__
#define __p2k1_game_state_h__

#include "fgl_transform.h" // fgl_transform, fgl_matrix, fgl_vertex
#include "raylib.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* 
    contains all game state for p2k1. based off of vectorwar example in ggpo, and more directly off of the ggpo developer guide.
*/

// TODO: this is defined in 2 files, only define in one
static inline Vector3 fgl_vec3_to_float_vector3(fgl_vec3_t v){ return (Vector3) {fix16_to_float(v.x),fix16_to_float(v.y),fix16_to_float(v.z)}; }

typedef enum {
    PLAYER1,
    PLAYER2,
    SIDE,
    TOP,
    FIXED_SIDE,
    FIXED_TOPDOWN,
    FREE
} P2k1CameraMode;

// GameRenderState will not be used by ggpo, it's contents are unique to each player 
// (different players will have different camera positions for the game) and it smooths the camera movement over rollbacks
// scene data/loaded assets will be stored in GameRenderState
typedef struct GameRenderState 
{
    Vector3 camera_position;
    Vector3 camera_target;
    float camera_zoom;
    unsigned int render_frame_number;

    Color p1_tri_color;
    Color p1_wire_color;
    
    Color p2_tri_color;
    Color p2_wire_color;

    // is_player_1 is is a placeholder for "perspective" Whos perspective of the game are we rendering? p1? p2? a spectator? debug controls? Perhapse it is a rudamentary version of "camera mode"
    P2k1CameraMode camera_mode;

    // stuff needed for predicting and interpolating game ticks
    // renderer rng
} GameRenderState;

typedef struct GameInputs 
{
    int mainStickHorizontal;
    int mainStickVertical;
    int cStickHorizontal; 
    int cStickVertical;
    int triggerResult;
} GameInputs;

typedef struct GameState 
{
    fgl_vec3_t p1_tip;
    fgl_vec3_t p1_base;
    fix16_t p1_rad;
    unsigned int p1_hp;

    fgl_vec3_t p2_tip;
    fgl_vec3_t p2_base;
    fix16_t p2_rad;
    unsigned int p2_hp;

    int collision_lockout;
    unsigned int frame_number;

    // TODO: rng

    // TODO: include damage_was_dealt or prev_pX_hp fields?
} GameState;

/*
 * p2k1_init_game_render_state()
 * reads: game_settings_obj
 * writes: rs
 */
void p2k1_init_game_render_state( GameRenderState *rs /* game_settings_obj */);

/*
 * p2k1_init_game_render_state()
 * reads: game_settings_obj
 * writes: gs
 */
void p2k1_init_game_state( GameState *gs /* game_settings_obj */);

/*
 * p2k1_advance_game_state()
 * reads: p1_inputs, p2_inputs
 * writes: gs
 */
void p2k1_advance_game_state(const GameInputs *p1_input, const GameInputs *p2_input, GameState *gs);

/*
 * p2k1_advance_game_render_state()
 * reads: gs, delta_time
 * writes: rs
 */
void p2k1_advance_game_render_state(const GameState *gs, GameRenderState *rs /*, float/int delta_time */);

/*
 * p2k1_render_frame()
 * reads: gs, rs
 * side effects: raylib
 */
void p2k1_render_frame(const GameState *gs, const GameRenderState *rs);


#ifdef __cplusplus
}
#endif

#endif // __p2k1_game_state_h__