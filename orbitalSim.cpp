/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 * @author Ariana Schiaffino
 * @author Rita Moschini
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

static void calculatePlanetsAccelerations(OrbitalSim_t*, Vector3*);

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
static float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/*
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody_t* body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);
    
    //phi = 0; // This is the easter egg. Further explanation in the README file.

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radius = 2E3F; // Typical asteroid radius: 2km
    body->color = GRAY;
    body->position = {r * cosf(phi), 0, r * sinf(phi)};
    body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

/*
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim_t* constructOrbitalSim(float timeStep)
{
    OrbitalSim_t* sim = (OrbitalSim_t*)malloc(sizeof(OrbitalSim_t));
    if (!sim) {
        std::cout << "Error while allocating memory for OrbitalSim";
        exit(1);
    }

    sim->timestep = timeStep; 
    sim->startTime = 0.0;
    sim->currentTime = sim->startTime;
    sim->numberOfBodies = 9; // 8 planets, plus the Sun
    sim->numberOfAsteroids = 500;

    sim->bodiesArray = (OrbitalBody_t*) malloc(sizeof(OrbitalBody_t) * sim->numberOfBodies);
    if (!sim->bodiesArray) {
        std::cout << "Error while allocating memory for bodiesArray";
        free(sim);
        exit(1);
    }
    sim->asteroidsArray = (OrbitalBody_t*)malloc(sizeof(OrbitalBody_t) * sim->numberOfAsteroids);
    if (!sim->asteroidsArray) {
        std::cout << "Error while allocating memory for asteroidsArray";
        free(sim->bodiesArray);
        free(sim);
        exit(1);
    }

    for (int i = 0; i < sim->numberOfBodies; i++) {
        sim->bodiesArray[i].name = solarSystem[i].name;
        sim->bodiesArray[i].mass = solarSystem[i].mass;
        sim->bodiesArray[i].radius = solarSystem[i].radius;
        sim->bodiesArray[i].color = solarSystem[i].color;

        sim->bodiesArray[i].position = solarSystem[i].position;
        sim->bodiesArray[i].velocity = solarSystem[i].velocity;
    }
    for (int i = 0; i < sim->numberOfAsteroids; i++) {
        configureAsteroid( &(sim->asteroidsArray[i]), sim->bodiesArray[0].mass );
    }
    
    return sim;
}

/**
 * @brief Destroys an orbital simulation
 * 
 * @param sim The orbital simulation
 */
void destroyOrbitalSim(OrbitalSim_t *sim)
{
    free(sim->bodiesArray);
    free(sim->asteroidsArray);
    free(sim);
}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim_t *sim)
{
    Vector3 differencePositions, unitVectors, asteroidAcceleration, *accelerationsArray;
    float squareNorm;

    accelerationsArray = (Vector3*) calloc( sim->numberOfBodies, sizeof(Vector3) );
    if (!accelerationsArray) {
            std::cout << "Error while allocating memory for accelerationsArray";
            free(sim->bodiesArray);
            free(sim->asteroidsArray);
            free(sim);
            exit(1);
    }
    
    calculatePlanetsAccelerations(sim, accelerationsArray);

    /* Every asteroid's acceleration is calculated only according to the Sun, 
       therefore the operations are not the same as for the planets. */
   
    for (int i = 0; i < sim->numberOfAsteroids; i++) {
            differencePositions = Vector3Subtract(sim->asteroidsArray[i].position, sim->bodiesArray[0].position);
            unitVectors = Vector3Normalize(differencePositions);
            squareNorm = differencePositions.x * differencePositions.x +
                         differencePositions.y * differencePositions.y +
                         differencePositions.z * differencePositions.z;
            asteroidAcceleration = Vector3Scale( unitVectors,
                                                 sim->bodiesArray[0].mass * (-1) * GRAVITATIONAL_CONSTANT / squareNorm );
        
            sim->asteroidsArray[i].velocity = Vector3Add( sim->asteroidsArray[i].velocity, 
                                                          Vector3Scale(asteroidAcceleration, sim->timestep) );
        
            sim->asteroidsArray[i].position = Vector3Add( sim->asteroidsArray[i].position, 
                                                          Vector3Scale(sim->asteroidsArray[i].velocity, sim->timestep) );
    }

    for (int j = 0; j < sim->numberOfBodies; j++) {
        sim->bodiesArray[j].velocity = Vector3Add( sim->bodiesArray[j].velocity, 
                                                   Vector3Scale(accelerationsArray[j], sim->timestep) );
    }
    for (int j = 0; j < sim->numberOfBodies; j++) {
        sim->bodiesArray[j].position = Vector3Add( sim->bodiesArray[j].position, 
                                                   Vector3Scale(sim->bodiesArray[j].velocity, sim->timestep) );
    }

    free(accelerationsArray);
    sim->currentTime += sim->timestep;
}

/**
 * @brief Calculates the acceleration of all planets and the Sun.
 *
 * @param sim The orbital simulation 
 * @param accelerationsArray The address of an array containing the accelerations of the planets and the sun
 */
static void calculatePlanetsAccelerations(OrbitalSim_t *sim, Vector3 *accelerationsArray)
{
    Vector3 differencePositions, unitVectors;
    float squareNorm;

    for (int i = 0; i < sim->numberOfBodies; i++) {
        for (int j = i+1; j < sim->numberOfBodies ; j++) {
            /* j = i+1 means that the gravitational force between two planets is calculated once.
            Further explanation in the README file */
            differencePositions = Vector3Subtract(sim->bodiesArray[i].position, sim->bodiesArray[j].position);
            unitVectors = Vector3Normalize(differencePositions);
         
            squareNorm = differencePositions.x * differencePositions.x +
                         differencePositions.y * differencePositions.y + 
                         differencePositions.z * differencePositions.z;

            accelerationsArray[i] = Vector3Add( accelerationsArray[i], 
                                                Vector3Scale(unitVectors, (-1) * GRAVITATIONAL_CONSTANT * sim->bodiesArray[j].mass / squareNorm) );
            accelerationsArray[j] = Vector3Add( accelerationsArray[j], 
                                                Vector3Scale(unitVectors, GRAVITATIONAL_CONSTANT * sim->bodiesArray[i].mass / squareNorm) );
            /* When looking at the equations, you can see that to calculate the force from i to j it is
            multiplied by (-1). Then, considering the Law of Action and Reaction, to calcualte the force
            from j to i a similar operation is made and the result should be multiplied by (-1) twice, but
            since (-1)*(-1)=1, we simply don't multiply by (-1) inr order to reduce the number of operations. */
        }
    }
}