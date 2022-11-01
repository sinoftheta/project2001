#ifndef __p2k1_game_state_h__
#define __p2k1_game_state_h__

#include "fgl_transform.h" // fgl_transform, fgl_matrix, fgl_vertex
#include "raylib.h"




/* 
    contains all game state for p2k1. based off of vectorwar example in ggpo, and more directly off of the ggpo developer guide.
*/
// GameRenderState will not be used by ggpo, it's contents are unique to each player 
// (different players will have different camera positions for the game) and it smooths the camera movement over rollbacks
typedef struct GameRenderState 
{
  Vector3 prev_camera_position;
  Vector3 prev_camera_target;
  float prev_camera_zoom;
  unsigned int render_frame_number;

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

// will not mutate anything in any GameInputs objects, will mutate GameState
void p2k1_advance_game_state(GameInputs *p1_input, GameInputs *p2_input, GameState *gs);


// will not mutate anything in GameState, will mutate GameRenderState. TODO: may take a delta time parameter for rendering as fast as possible
// is_player_1 is is a placeholder for "perspective" Whos perspective of the game are we rendering? p1? p2? a spectator? debug controls? Perhapse it is a rudamentary version of "camera mode"
// if we were to do scene based rendering, this function would be "apply_state_to_scene_and_render" or something
void p2k1_render_current_frame(GameState *gs, GameRenderState *grs, bool is_player_1 /* delta time */);

#endif // __p2k1_game_state_h__