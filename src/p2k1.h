#ifndef __p2k1_h__
#define __p2k1_h__


#include <iostream>
#include <math.h>
#include "ggponet.h"       // ggpo
#include "platform_unix.h" // ggpo
#include "raylib.h"        // raylib
#include "raymath.h"
#include "fixmath.h"       // libfixmath
#include "GCAdapter.h"     // gcaplus
//#include "fgl_transform.h" // fgl_transform, fgl_matrix, fgl_vertex
#include "fgl.h"
//#include "p2k1_capsule_collision.h"
#include "p2k1_game_state.h"

#define DEBUG_FRAME_INCRIMENT 5
#define DEBUG_ANIM_SPEED 20.0f

void fix16_print(fix16_t f) {
	printf("%+.4f\n", fix16_to_dbl(f));
}


// fix16_to_float(fix16_t) is provided in fix16.h TODO: remove from this file
//static inline Vector3 fgl_vec3_to_float_vector3(fgl_vec3_t v){ return (Vector3) {fix16_to_float(v.x),fix16_to_float(v.y),fix16_to_float(v.z)}; }

typedef enum {
    TITLE, // TODO: can we give these the value of Scene pointers? need a way of associating Scene pointers with SceneIDs 
    MAIN1,
    GAME,
    GGPO_CONNECT,
} SceneID;

typedef void (*NextTick)(/* NonGameState *ngs, GameState *gs, Inputs *inputs */ ); // NextTick function does not HAVE to mutate either gamestate, inputs struct should be immutable (TODO: is there a way to make it const or something in C lol) 
typedef void (*Render)( /* NonGameState *ngs, GameState *gs */ ); // will NOT mutate the states (TODO: look into making them consts or something idk how it works)
typedef void (*Load)(/* Assets *assets */); // 
typedef void (*Unload)(/* Assets *assets */);
typedef void (*Init)(/* NonGameState *ngs */);

typedef struct Scene{
    NextTick nextTick;
    Render render;
    Load load;
    Init init;
    Unload unload;
    // may add more if I want per-scene loading screens

} Scene;

//void pushAndLoadScene(Scene *scene, NonGameState *ngs); // move into another scene (submenu or game)
//void resetStackAndLoadScene(Scene *scene, NonGameState *ngs); // clears the stack, used for init or resetting to title after credits or something. 
//void popNAndLoad(NonGameState *ngs, unsigned int n); // check if last in scene in stack (this means we are on the main menu), 

// nongamestate will prob be broken up further

/*
int DRAFT_main(){
    // setup

    trySetUpGCA(&ngs); // can be called later in another menu if we want

    while(!WindowShouldClose()){

        Menu *curScene = ngs.sceneStack.top();
        if(!curScene) { 
            // panic & quit
        }

        if(ngs.ggpoInSession){
            // TODO: handle ggpo polling, rendering and game state updating, will this fit?
        }
        else{
            pollInputDevices(&inputs);
            curScene->nextTick(&inputs, &ngs, &gs);
            curScene->render(&ngs, &gs); 
        }

        // timekeeping for locked framerate 
    }

    // teardown
}
*/

#endif // __p2k1_h__