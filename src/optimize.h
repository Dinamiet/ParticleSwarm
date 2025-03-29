#ifndef _OPTIMIZE_H_
#define _OPTIMIZE_H_

#include "swarm.h"

#if (SWARM_THREADS > 1)
void optimize_MultiThreaded(
		Swarm*                        swarm,
		const Swarm_ObjectiveFunction evaluate,
		const size_t                  maxIterations,
		const float                   w,
		const float                   c1,
		const float                   c2,
		const Swarm_ProgressReport    progress);

void* optimize_Thread(void* arg);
#else
void optimize_SingleThread(
		Swarm*                        swarm,
		const Swarm_ObjectiveFunction evaluate,
		const size_t                  maxIterations,
		const float                   w,
		const float                   c1,
		const float                   c2,
		const Swarm_ProgressReport    progress);
#endif

#endif
