/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

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
    int numberOfBodies;
    OrbitalBody_t **bodiesArray;
} OrbitalSim_t;

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim);

#endif
