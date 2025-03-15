/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>

#include "OrbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
/*void configureAsteroid(OrbitalBody* body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    // body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    // body->radius = 2E3F; // Typical asteroid radius: 2km
    // body->color = GRAY;
    // body->position = {r * cosf(phi), 0, r * sinf(phi)};
    // body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
} */

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim_t *constructOrbitalSim(float timeStep)
{
 /*float timestep;
    float startTime;
    int numberOfBodies;
    OrbitalBody (*bodiesArray)[];*/

    OrbitalSim_t* OrbitalSim = (OrbitalSim_t* ) malloc(sizeof(OrbitalSim_t));
    if (!OrbitalSim) {
        std::cout << "Error while allocating memory for OrbitalSim";
        exit(1);
    }
    OrbitalSim->bodiesArray = (OrbitalBody_t**)malloc(sizeof(OrbitalBody_t*) * OrbitalSim->numberOfBodies);
    if (!OrbitalSim->bodiesArray) {
        std::cout << "Error while allocating memory for bodiesArray";
        exit(1);
    }

    OrbitalSim->timestep = 0.01; // CALIBRATE
    OrbitalSim->startTime = 0.0;
    OrbitalSim->numberOfBodies = 10; // //9 planets, counting Pluto, plus the Sun
    
    int i;
    for (i = 0; i < OrbitalSim->numberOfBodies; i++) {
        OrbitalSim->bodiesArray[i]->name = solarSystem[i].name;
        OrbitalSim->bodiesArray[i]->mass = solarSystem[i].mass;
        OrbitalSim->bodiesArray[i]->radius = solarSystem[i].radius;
        OrbitalSim->bodiesArray[i]->color = solarSystem[i].color;
        OrbitalSim->bodiesArray[i]->position = solarSystem[i].position;
        OrbitalSim->bodiesArray[i]->velocity = solarSystem[i].velocity;
    }
    

    return NULL; // This should return your orbital sim
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    // Your code goes here...


}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim)
{
    // Your code goes here...


}
