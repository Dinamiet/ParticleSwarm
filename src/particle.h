#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "swarm.h"

void Particle_Init(Particle* particle, const Swarm_RandomGenerator randomFunction, const float lowerBound, const float upperBound);
void Particle_Update(
		Particle*                   particle,
		const float*                globalBestPosition,
		const Swarm_RandomGenerator randomFunction,
		const float                 w,
		const float                 c1,
		const float                 c2);
float Particle_Evaluate(Particle* particle, const Swarm_ObjectiveFunction evaluate);

#endif
