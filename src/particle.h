#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "particle_swarm.h"

void Particle_Init(Particle* particle, Random randomFunction, float lowerBound, float upperBound);
void Particle_Update(Particle* particle, float* globalBestPosition, Random randomFunction, float w, float c1, float c2);
float Particle_Evaluate(Particle* particle, Objective evaluate);

#endif
