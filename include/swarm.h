#ifndef _SWARM_H_
#define _SWARM_H_

#include <stddef.h>

#define DIMENSIONS 1

typedef float (*Swarm_ObjectiveFunction)(const float* parameters);
typedef float (*Swarm_RandomGenerator)(const float min, const float max);

typedef struct _Particle_
{
	float BestFitness;
	float BestPosition[DIMENSIONS];
	float Position[DIMENSIONS];
	float Velocity[DIMENSIONS];
} Particle;

typedef struct _Swarm_
{
	Swarm_RandomGenerator RandomGenerator;
	size_t                NumParticles;
	Particle*             Particles;
	float                 GlobalBestFitness;
	float                 GlobalBestPosition[DIMENSIONS];
} Swarm;

void Swarm_Init(
		Swarm*                      swarm,
		Particle*                   particles,
		const size_t                numParticles,
		const Swarm_RandomGenerator randomGenerator,
		const float                 lowerBound,
		const float                 upperBound);

void  Swarm_Optimize(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2);
float Swarm_GetBestFitness(const Swarm* swarm);
float* Swarm_GetBestPosition(Swarm* swarm);

#endif
