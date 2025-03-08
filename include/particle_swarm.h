#ifndef _PARTICLE_SWARM_H_
#define _PARTICLE_SWARM_H_

#include <stddef.h>

#define DIMENSIONS 1

typedef float (*Objective)(float* parameters);
typedef float (*Random)(float min, float max);

typedef struct _Particle_
{
	float BestFitness;
	float BestPosition[DIMENSIONS];
	float Position[DIMENSIONS];
	float Velocity[DIMENSIONS];
} Particle;

typedef struct _Swarm_
{
	Random    RandomFunction;
	size_t    NumParticles;
	Particle* Particles;
	float     GlobalBestFitness;
	float     GlobalBestPosition[DIMENSIONS];
} Swarm;

void   Swarm_Init(Swarm* swarm, Particle* particles, size_t numParticles, Random randomFunction, float lowerBound, float upperBound);
void   Swarm_Optimize(Swarm* swarm, Objective evaluate, size_t maxIterations, float w, float c1, float c2);
float  Swarm_GetBestFitness(Swarm* swarm);
float* Swarm_GetBestPosition(Swarm* swarm);

#endif
