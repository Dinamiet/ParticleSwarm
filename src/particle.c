#include "particle.h"

#include <math.h>
#include <string.h>

void Particle_Init(Particle* particle, const Swarm_RandomGenerator randomGenerator, const float lowerBound, const float upperBound)
{
	for (size_t i = 0; i < SWARM_DIMENSIONS; i++)
	{
		particle->Position[i]     = randomGenerator(lowerBound, upperBound);
		particle->Velocity[i]     = randomGenerator(-1, 1);
		particle->BestPosition[i] = particle->Position[i];
	}
	particle->BestFitness = -INFINITY;
}

void Particle_Update(Particle* particle, const float* globalBestPosition, const Swarm_RandomGenerator randomGenerator, const float w, const float c1, const float c2)
{
	for (size_t i = 0; i < SWARM_DIMENSIONS; i++)
	{
		float r1              = randomGenerator(0, 1);
		float r2              = randomGenerator(0, 1);
		float inertia         = w * particle->Velocity[i];
		float cognitive       = c1 * r1 * (particle->BestPosition[i] - particle->Position[i]);
		float social          = c2 * r2 * (globalBestPosition[i] - particle->Position[i]);
		particle->Velocity[i] = inertia + cognitive + social;
		particle->Position[i] += particle->Velocity[i];
	}
}

float Particle_Evaluate(Particle* particle, const Swarm_ObjectiveFunction evaluate)
{
	float fitness = evaluate(particle->Position);
	if (fitness > particle->BestFitness)
	{
		particle->BestFitness = fitness;
		memcpy(particle->BestPosition, particle->Position, sizeof(particle->BestPosition));
	}
	return fitness;
}
