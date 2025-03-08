#include "particle.h"

#include <math.h>
#include <string.h>

void Particle_Init(Particle* particle, Swarm_RandomGenerator randomGenerator, float lowerBound, float upperBound)
{
	for (size_t i = 0; i < DIMENSIONS; i++)
	{
		particle->Position[i]     = randomGenerator(lowerBound, upperBound);
		particle->Velocity[i]     = randomGenerator(-1, 1);
		particle->BestPosition[i] = particle->Position[i];
	}
	particle->BestFitness = -INFINITY;
}

void Particle_Update(Particle* particle, float* globalBestPosition, Swarm_RandomGenerator randomGenerator, float w, float c1, float c2)
{
	for (size_t i = 0; i < DIMENSIONS; i++)
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

float Particle_Evaluate(Particle* particle, Swarm_ObjectiveFunction evaluate)
{
	float fitness = evaluate(particle->Position);
	if (fitness > particle->BestFitness)
	{
		particle->BestFitness = fitness;
		memcpy(particle->BestPosition, particle->Position, sizeof(particle->BestPosition));
	}
	return fitness;
}
