#include "particle.h"

#include <math.h>
#include <string.h>

void Particle_Init(Particle* particle, Random randomFunction, float lowerBound, float upperBound)
{
	for (size_t i = 0; i < DIMENSIONS; i++)
	{
		particle->Position[i]     = randomFunction(lowerBound, upperBound);
		particle->Velocity[i]     = randomFunction(-1, 1);
		particle->BestPosition[i] = particle->Position[i];
	}
	particle->BestFitness = -INFINITY;
}

void Particle_Update(Particle* particle, float* globalBestPosition, Random randomFunction, float w, float c1, float c2)
{
	for (size_t i = 0; i < DIMENSIONS; i++)
	{
		float r1              = randomFunction(0, 1);
		float r2              = randomFunction(0, 1);
		float inertia         = w * particle->Velocity[i];
		float cognitive       = c1 * r1 * (particle->BestPosition[i] - particle->Position[i]);
		float social          = c2 * r2 * (globalBestPosition[i] - particle->Position[i]);
		particle->Velocity[i] = inertia + cognitive + social;
		particle->Position[i] += particle->Velocity[i];
	}
}

float Particle_Evaluate(Particle* particle, Objective evaluate)
{
	float fitness = evaluate(particle->Position);
	if (fitness > particle->BestFitness)
	{
		particle->BestFitness = fitness;
		memcpy(particle->BestPosition, particle->Position, sizeof(particle->BestPosition));
	}
	return fitness;
}
