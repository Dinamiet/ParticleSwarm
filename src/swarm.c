#include "swarm.h"

#include "optimize.h"
#include "particle.h"

#include <math.h>
#include <string.h>

void Swarm_Init(Swarm* swarm, Particle* particles, const size_t numParticles, const Swarm_RandomGenerator randomGenerator, const float lowerBound, const float upperBound)
{
	swarm->RandomGenerator   = randomGenerator;
	swarm->NumParticles      = numParticles;
	swarm->Particles         = particles;
	swarm->GlobalBestFitness = -INFINITY;
	memset(swarm->GlobalBestPosition, 0.0f, sizeof(swarm->GlobalBestPosition));

	for (size_t i = 0; i < numParticles; ++i) { Particle_Init(&swarm->Particles[i], randomGenerator, lowerBound, upperBound); }
}

void Swarm_Optimize(
		Swarm*                        swarm,
		const Swarm_ObjectiveFunction evaluate,
		const size_t                  maxIterations,
		const float                   w,
		const float                   c1,
		const float                   c2,
		const Swarm_ProgressReport    progress)
{
#if (SWARM_THREADS > 1)
	optimize_MultiThreaded(swarm, evaluate, maxIterations, w, c1, c2, progress);
#else
	optimize_SingleThread(swarm, evaluate, maxIterations, w, c1, c2, progress);
#endif
}

float Swarm_GetBestFitness(const Swarm* swarm) { return swarm->GlobalBestFitness; }

float* Swarm_GetBestPosition(Swarm* swarm) { return swarm->GlobalBestPosition; }
