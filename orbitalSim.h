/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 * @author Ariana Schiaffino
 * @author Rita Moschini
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"

/**
 * @brief Orbital body definition
 */
typedef struct
{
    const char* name; // Name
    float mass;		  // [kg]
    float radius;	  // [m]
    Color color;	  // Raylib color
    Vector3 position; // [m]
    Vector3 velocity; // [m/s]
}OrbitalBody_t;

typedef struct
{
    float timestep;
    float startTime;
    float currentTime;
    int numberOfBodies;
    int numberOfAsteroids;
    OrbitalBody_t *bodiesArray;
    OrbitalBody_t *asteroidsArray;
} OrbitalSim_t;

OrbitalSim_t *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim_t *sim);
void updateOrbitalSim(OrbitalSim_t *sim);

#endif
