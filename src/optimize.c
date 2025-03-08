#include "optimize.h"

#include "particle.h"
#include "swarm.h"

#include <string.h>

void Optimize_SingleThread(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2)
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

#ifdef USE_THREADS

void Optimize_MultiThreaded(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2)
{
	(void)swarm;
	(void)evaluate;
	(void)maxIterations;
	(void)w;
	(void)c1;
	(void)c2;
	/** TODO: Implement optimization using multiple threads. */
	return;
}

#endif
