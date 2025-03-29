#include "optimize.h"

#include "particle.h"
#include "swarm.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if (SWARM_THREADS > 1)

typedef struct _ThreadInfo_
{
	bool                    Continue;
	Swarm_ObjectiveFunction Evaluate;
	Swarm*                  Swarm;
	sem_t*                  StartSemaphore;
	sem_t*                  DoneSemaphore;
	size_t*                 NextParticleIndex;
	pthread_mutex_t*        Mutex;
} ThreadInfo;

static Particle* getNextParticle(ThreadInfo* info)
{
	Particle* particle = NULL;
	pthread_mutex_lock(info->Mutex);

	if (*info->NextParticleIndex < info->Swarm->NumParticles)
		particle = &info->Swarm->Particles[(*info->NextParticleIndex)++];

	pthread_mutex_unlock(info->Mutex);
	return particle;
}

void* optimize_Thread(void* arg)
{
	ThreadInfo* info = (ThreadInfo*)arg;

	while (sem_wait(info->StartSemaphore) == 0 && info->Continue)
	{
		Particle* particle = getNextParticle(info);
		if (particle)
		{
			float fitness = Particle_Evaluate(particle, info->Evaluate);
			pthread_mutex_lock(info->Mutex);
			if (fitness > info->Swarm->GlobalBestFitness)
			{
				info->Swarm->GlobalBestFitness = fitness;
				memcpy(info->Swarm->GlobalBestPosition, particle->BestPosition, sizeof(info->Swarm->GlobalBestPosition));
			}
			pthread_mutex_unlock(info->Mutex);
			sem_post(info->DoneSemaphore);
		}
	}

	return NULL;
}

void optimize_MultiThreaded(
		Swarm*                        swarm,
		const Swarm_ObjectiveFunction evaluate,
		const size_t                  maxIterations,
		const float                   w,
		const float                   c1,
		const float                   c2,
		const Swarm_ProgressReport    progress)
{
	pthread_t       threads[SWARM_THREADS];
	sem_t           startSemaphore;
	sem_t           doneSemaphore;
	pthread_mutex_t mutex;
	size_t          nextParticleIndex;
	ThreadInfo      info;

	info.Evaluate          = evaluate;
	info.Swarm             = swarm;
	info.StartSemaphore    = &startSemaphore;
	info.DoneSemaphore     = &doneSemaphore;
	info.NextParticleIndex = &nextParticleIndex;
	info.Mutex             = &mutex;
	info.Continue          = true;

	// Setup
	pthread_mutex_init(info.Mutex, NULL);
	sem_init(info.StartSemaphore, 0, 0);
	sem_init(info.DoneSemaphore, 0, 0);
	for (size_t i = 0; i < SWARM_THREADS; i++) { pthread_create(&threads[i], NULL, optimize_Thread, &info); }

	for (size_t itt = 0; itt < maxIterations; itt++)
	{
		nextParticleIndex = 0;

		for (size_t i = 0; i < swarm->NumParticles; i++) { sem_post(info.StartSemaphore); } // Start threads
		for (size_t i = 0; i < swarm->NumParticles; i++) { sem_wait(info.DoneSemaphore); }  // Wait all done

		for (size_t i = 0; i < swarm->NumParticles; i++)
		{
			Particle_Update(&swarm->Particles[i], swarm->GlobalBestPosition, swarm->RandomGenerator, w, c1, c2);
		}
		if (progress)
			progress(itt + 1, swarm->GlobalBestFitness, swarm->GlobalBestPosition);
	}

	// Cleanup
	info.Continue = false;
	for (size_t i = 0; i < SWARM_THREADS; i++) { sem_post(info.StartSemaphore); } // Unblock threads to exit
	for (size_t i = 0; i < SWARM_THREADS; i++) { pthread_join(threads[i], NULL); }
	sem_destroy(info.DoneSemaphore);
	sem_destroy(info.StartSemaphore);
	pthread_mutex_destroy(info.Mutex);
}

#else

void optimize_SingleThread(
		Swarm*                        swarm,
		const Swarm_ObjectiveFunction evaluate,
		const size_t                  maxIterations,
		const float                   w,
		const float                   c1,
		const float                   c2,
		const Swarm_ProgressReport    progress)
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

		if (progress)
			progress(itt + 1, swarm->GlobalBestFitness, swarm->GlobalBestPosition);
	}
}

#endif
