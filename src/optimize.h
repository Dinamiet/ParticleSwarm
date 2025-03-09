#ifndef _OPTIMIZE_H_
#define _OPTIMIZE_H_

#include "swarm.h"

void optimize_SingleThread(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2);

#ifdef USE_THREADS
void optimize_MultiThreaded(Swarm* swarm, const Swarm_ObjectiveFunction evaluate, const size_t maxIterations, const float w, const float c1, const float c2);
void* optimize_Thread(void* arg);
#endif

#endif
