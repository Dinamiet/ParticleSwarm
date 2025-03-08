#include "particle.h"
#include "particle_swarm.h"

#include <math.h>
#include <string.h>

void Swarm_Init(Swarm* swarm, Particle* particles, size_t numParticles, Swarm_RandomGenerator randomGenerator, float lowerBound, float upperBound)
{
	swarm->RandomGenerator   = randomGenerator;
	swarm->NumParticles      = numParticles;
	swarm->Particles         = particles;
	swarm->GlobalBestFitness = -INFINITY;
	memset(swarm->GlobalBestPosition, 0.0f, sizeof(swarm->GlobalBestPosition));

	for (size_t i = 0; i < numParticles; ++i) { Particle_Init(&swarm->Particles[i], randomGenerator, lowerBound, upperBound); }
}

void Swarm_Optimize(Swarm* swarm, Swarm_ObjectiveFunction evaluate, size_t maxIterations, float w, float c1, float c2)
{
	for (size_t itt = 0; itt < maxIterations; itt++)
	{
		for (size_t i = 0; i < swarm->NumParticles; i++)
		{
			float fitness = Particle_Evaluate(&swarm->Particles[i], evaluate);
			if (fitness > swarm->GlobalBestFitness)
			{
				swarm->GlobalBestFitness = fitness;
				memcpy(swarm->GlobalBestPosition, swarm->Particles[i].BestPosition, sizeof(swarm->GlobalBestPosition));
			}
		}

		for (size_t i = 0; i < swarm->NumParticles; i++)
		{
			Particle_Update(&swarm->Particles[i], swarm->GlobalBestPosition, swarm->RandomGenerator, w, c1, c2);
		}
	}
}

float Swarm_GetBestFitness(Swarm* swarm) { return swarm->GlobalBestFitness; }

float* Swarm_GetBestPosition(Swarm* swarm) { return swarm->GlobalBestPosition; }
