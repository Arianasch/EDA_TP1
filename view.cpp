/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 * @author Ariana Schiaffino
 * @author Rita Moschini
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <time.h>

#include "view.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *        to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
static const char *getISODate(float timestamp)
{
    // Timestamp epoch: 1/1/2022
    struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

    // Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
    time_t unixEpoch = mktime(&unichEpochTM);
    time_t unixTimestamp = unixEpoch + (time_t)timestamp;

    // Returns ISO date
    struct tm *localTM = localtime(&unixTimestamp);
    return TextFormat("%04d-%02d-%02d",
                      1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
View *constructView(int fps)
{
    View *view = new View();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EDA Orbital Simulation");
    SetTargetFPS(fps);
    DisableCursor();

    view->camera.position = {10.0f, 10.0f, 10.0f};
    view->camera.target = {0.0f, 0.0f, 0.0f};
    view->camera.up = {0.0f, 1.0f, 0.0f};
    view->camera.fovy = 45.0f;
    view->camera.projection = CAMERA_PERSPECTIVE;

    return view;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(View *view)
{
    CloseWindow();

    delete view;
}

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(View *view)
{
    return !WindowShouldClose();
}

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
void renderView(View* view, OrbitalSim_t* sim)
{
    float camAsteroidDistance;
    UpdateCamera(&view->camera, CAMERA_FREE);

    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode3D(view->camera);

    // Fill in your 3D drawing code here:
    for (int i = 0; i < sim->numberOfBodies; i++) {
       DrawSphere( Vector3Scale(sim->bodiesArray[i].position, 1E-11f),
                    0.005F * logf(sim->bodiesArray[i].radius),
                    sim->bodiesArray[i].color );

        DrawPoint3D(Vector3Scale(sim->bodiesArray[i].position, 1E-11f), sim->bodiesArray[i].color);
    }

    /* If the camera position is not too far away from the asteroid, a sphere is drawn. Otherwise,
       a point is drawn .This is for efficiency. More in the README file. */
    for (int i = 0; i < sim->numberOfAsteroids; i++) {
        camAsteroidDistance = Vector3Distance( Vector3Scale(sim->asteroidsArray[i].position,1E-11f),
                                                view->camera.position );
        
        if (camAsteroidDistance < 8) {
            DrawSphere( Vector3Scale(sim->asteroidsArray[i].position, 1E-11f),
                        0.005F * logf(sim->asteroidsArray[i].radius),
                        sim->asteroidsArray[i].color );  
        }
        else {
            DrawPoint3D( Vector3Scale(sim->asteroidsArray[i].position, 1E-11f), sim->asteroidsArray[i].color );
        }
    }

    DrawGrid(10, 10.0f);
    EndMode3D();

    // Fill in your 2D drawing code here:
    DrawFPS( WINDOW_WIDTH - 1260, WINDOW_HEIGHT - 710);
	DrawText(getISODate(sim->currentTime), WINDOW_WIDTH - 1260, WINDOW_HEIGHT - 680, 20, WHITE);
    
    EndDrawing();
}
