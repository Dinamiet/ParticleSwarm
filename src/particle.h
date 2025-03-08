#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "particle_swarm.h"

void  Particle_Init(Particle* particle, Swarm_RandomGenerator randomFunction, float lowerBound, float upperBound);
void  Particle_Update(Particle* particle, float* globalBestPosition, Swarm_RandomGenerator randomFunction, float w, float c1, float c2);
float Particle_Evaluate(Particle* particle, Swarm_ObjectiveFunction evaluate);

#endif
