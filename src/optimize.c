#include "optimize.h"

#include "particle.h"
#include "swarm.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void optimize_SingleThread(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2)
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

static void resetParticleIndex(Swarm* swarm)
{
	pthread_mutex_lock(&swarm->Mutex);
	swarm->NextParticleIndex = 0;
	pthread_mutex_unlock(&swarm->Mutex);
}

static Particle* getNextParticle(Swarm* swarm)
{
	Particle* particle = NULL;
	pthread_mutex_lock(&swarm->Mutex);

	if (swarm->NextParticleIndex < swarm->NumParticles)
		particle = &swarm->Particles[swarm->NextParticleIndex++];

	pthread_mutex_unlock(&swarm->Mutex);
	return particle;
}

void* optimize_Thread(void* arg)
{
	Swarm* swarm = (Swarm*)arg;
	size_t self  = pthread_self();
	printf("Thread %lu started\n", self);

	while (sem_wait(&swarm->StartSemaphore))
	{
		Particle* particle = getNextParticle(swarm);
		float     fitness  = Particle_Evaluate(particle, evaluation);
		pthread_mutex_lock(&swarm->Mutex);
		if (fitness > swarm->GlobalBestFitness)
		{
			swarm->GlobalBestFitness = fitness;
			memcpy(swarm->GlobalBestPosition, particle->BestPosition, sizeof(swarm->GlobalBestPosition));
		}
		pthread_mutex_unlock(&swarm->Mutex);
		sem_post(&swarm->DoneSemaphore);
	}

	return NULL;
}

void optimize_MultiThreaded(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2)
{
	for (size_t itt = 0; itt < maxIterations; itt++)
	{
		resetParticleIndex(swarm);

		for (size_t i = 0; i < swarm->NumParticles; i++) { sem_post(&swarm->StartSemaphore); } // Start threads
		for (size_t i = 0; i < swarm->NumParticles; i++) { sem_wait(&swarm->DoneSemaphore); }  // Wait all done

		for (size_t i = 0; i < swarm->NumParticles; i++)
		{
			Particle_Update(&swarm->Particles[i], swarm->GlobalBestPosition, swarm->RandomGenerator, w, c1, c2);
		}
	}
}

#endif
