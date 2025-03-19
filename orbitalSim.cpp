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

static Vector3 calculateAcceleration(OrbitalSim_t* sim, int i);

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

/*
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

    sim->timestep = 0.01; // CALIBRATE
    sim->startTime = 0.0;
    sim->numberOfBodies = 10; // 9 planets, counting Pluto, plus the Sun

    sim->bodiesArray = (OrbitalBody_t**)malloc(sizeof(OrbitalBody_t*) * sim->numberOfBodies);
    if (!sim->bodiesArray) {
        std::cout << "Error while allocating memory for bodiesArray";
        free(sim);
        exit(1);
    }

    int i;
    for (i=0; i < sim->numberOfBodies; i++) {
        sim->bodiesArray[i] = (OrbitalBody_t*)malloc(sizeof(OrbitalBody_t));
        if (!sim->bodiesArray[i]) {
            std::cout << "Error while allocating memory for body " << i << std::endl;
            
            for (int j=0; j<i; j++) {
                free(sim->bodiesArray[j]);
            }
            free(sim->bodiesArray);
            free(sim);
            exit(1);
        }
    }
    
    for (i=0; i < sim->numberOfBodies; i++) {
        sim->bodiesArray[i]->name = solarSystem[i].name;
        sim->bodiesArray[i]->mass = solarSystem[i].mass;
        sim->bodiesArray[i]->radius = solarSystem[i].radius;
        sim->bodiesArray[i]->color = solarSystem[i].color;

        sim->bodiesArray[i]->position.x = solarSystem[i].position.x;
        sim->bodiesArray[i]->position.y = solarSystem[i].position.y;
        sim->bodiesArray[i]->position.z = solarSystem[i].position.z;

        sim->bodiesArray[i]->velocity.x = solarSystem[i].velocity.x;
        sim->bodiesArray[i]->velocity.y = solarSystem[i].velocity.y;
        sim->bodiesArray[i]->velocity.z = solarSystem[i].velocity.z;
    }
    
    return sim; // This should return your orbital sim
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim_t *sim)
{
    for (int i=0; i < sim->numberOfBodies; i++) {
		free(sim->bodiesArray[i]);
    }
    free(sim->bodiesArray);
    free(sim);
}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim_t *sim)
{
    for (int j=0; j < sim->numberOfBodies; j++) {
        Vector3 bodyAcceleration = calculateAcceleration(sim, j);
        sim->bodiesArray[j]->velocity += bodyAcceleration * sim->timestep;
        sim->bodiesArray[j]->position += sim->bodiesArray[j]->velocity * sim->timestep;
    }
}

/**
 * @brief Calculates the acceleration of a body.
 *
 * @param The orbital simulation and the index of the body whose acceleration it will calculate.
 */
static Vector3 calculateAcceleration(OrbitalSim_t *sim, int i) {
    Vector3 aux = { 0, 0, 0 };
    
	// Calculates the sum of the force all bodies exert on the body i except for itself.
    for (int j=0; j < sim->numberOfBodies; j++) {
        if (i != j) {
            Vector3 differencePositions = Vector3Subtract(sim->bodiesArray[i]->position, sim->bodiesArray[j]->position);
            Vector3 normalizedDifference = Vector3Normalize(differencePositions);
            
            float module = sqrt(differencePositions.x * differencePositions.x + differencePositions.y * differencePositions.y + differencePositions.z * differencePositions.z);

            aux.x += sim->bodiesArray[j]->mass * normalizedDifference.x / module;
            aux.y += sim->bodiesArray[j]->mass * normalizedDifference.y / module;
            aux.z += sim->bodiesArray[j]->mass * normalizedDifference.z / module;
        }
    }
    return Vector3Scale(aux, -GRAVITATIONAL_CONSTANT);
}
