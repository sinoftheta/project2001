/*******************************************************************************************
*
*   combination of core_custom_frame_control.c and core_3d_camera_free.c
*   https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux building using these steps for now
*
********************************************************************************************/
#include "p2k1.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "raylib barebones fighting engine ");

    // try to init GCA+
    ControllerStatus adapter_buffer[4];
    bool gcaLoaded = gca::Setup();

	


    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type


    



    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    // Custom timming variables
    double previousTime = GetTime();    // Previous time measure
    double currentTime = 0.0;           // Current time measure
    double updateDrawTime = 0.0;        // Update + Draw time
    double waitTime = 0.0;              // Wait time (if target fps required)
    float deltaTime = 0.0f;             // Frame time (Update + Draw + Wait time)
    
    // gameState
    int frameCount = 0;
    bool pause = false;                 // Pause control flag
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    
    int targetFPS = 60;                 // Our initial target fps
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        PollInputEvents();              // Poll input events (SUPPORT_CUSTOM_FRAME_CONTROL)

        if(gcaLoaded){
            gca::Process(adapter_buffer);
            std::cout << adapter_buffer[0] << std::endl;
        }
        
        UpdateCamera(&camera);

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;
        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        if (IsKeyPressed(KEY_UP)) targetFPS += DEBUG_FRAME_INCRIMENT;
        else if (IsKeyPressed(KEY_DOWN)) targetFPS -= DEBUG_FRAME_INCRIMENT;

        if (targetFPS < DEBUG_FRAME_INCRIMENT) targetFPS = DEBUG_FRAME_INCRIMENT;
        


        if (!pause)
        {
            // Update game state

            //position += 200*deltaTime;  // We move at 200 pixels per second
            //if (position >= GetScreenWidth()) position = 0;

            if(gcaLoaded && adapter_buffer[0].connected){
                cubePosition = (Vector3){
                    0.5f * adapter_buffer[0].mainStickHorizontal / 80.0f,
                    0.0f,
                    0.5f * adapter_buffer[0].mainStickVertical   / 80.0f
                };
            }
            else{
                //cubePosition = (Vector3){ 0.5f * sinf(frameCount / DEBUG_ANIM_SPEED ), 0.0f , 0.5f * cosf(frameCount / DEBUG_ANIM_SPEED ) };

                fgl_vertex_t fixed_cube_pos = (fgl_vertex_t) {
                    fix16_sin( fix16_div(fix16_from_int(frameCount), fix16_from_float(DEBUG_ANIM_SPEED))),
                    fix16_one >> 1,
                    fix16_cos( fix16_div(fix16_from_int(frameCount), fix16_from_float(DEBUG_ANIM_SPEED)))
                };

                fgl_vertex_print(fixed_cube_pos);

                cubePosition = fix16_vert_to_vector3(fixed_cube_pos);

            }


            frameCount++;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                DrawSphereEx_  (cubePosition, 2.0f,  5,    10,   RED);
                DrawSphereWires(cubePosition, 2.0f,  5,    10,   MAROON);

                DrawGrid(10, 1.0f);

            EndMode3D();

            // Draw ui

            DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 320, 133, BLUE);

            DrawText("Free camera default controls:", 20, 20, 10, BLACK);
            DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
            DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
            DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
            DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
            DrawText("PRESS SPACE to PAUSE FRAME ADVANCE", 10, GetScreenHeight() - 60, 20, GRAY);

            DrawText(TextFormat("TARGET  FPS: %i",   targetFPS),             GetScreenWidth() - 220, 10, 20, DARKGREEN);
            DrawText(TextFormat("CURRENT FPS: %i",   (int)(1.0f/deltaTime)), GetScreenWidth() - 220, 40, 20, LIME);
            DrawText(TextFormat("FRAME: %i", frameCount),                    GetScreenWidth() - 220, 70, 20, GREEN);

            if(gcaLoaded){
                DrawText("GCA+ loaded",         GetScreenWidth() - 220, 100, 20, GREEN);
            }
            else{
                DrawText("GCA+ failed to load", GetScreenWidth() - 220, 100, 20, RED);
            }

        EndDrawing();

        // NOTE: In case raylib is configured to SUPPORT_CUSTOM_FRAME_CONTROL, 
        // Events polling, screen buffer swap and frame time control must be managed by the user

        SwapScreenBuffer();         // Flip the back buffer to screen (front buffer)
        
        currentTime = GetTime();
        updateDrawTime = currentTime - previousTime;
        
        if (targetFPS > 0)          // We want a fixed frame rate
        {
            waitTime = (1.0f/(float)targetFPS) - updateDrawTime;
            if (waitTime > 0.0) 
            {
                WaitTime((float)waitTime);
                currentTime = GetTime();
                deltaTime = (float)(currentTime - previousTime);
            }
        }
        else deltaTime = updateDrawTime;    // Framerate could be variable

        previousTime = currentTime;
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    gca::Stop();          // stop gca

    return 0;
}
