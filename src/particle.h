#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "swarm.h"

/**
 * Initializes a particle with a random position and velocity.
 * \param particle The particle to initialize.
 * \param randomFunction The function to use for generating random positions and velocities
 * \param lowerBound The lower bound for possible starting positions
 * \param upperBound The upper bound for possible starting positions
 */
void Particle_Init(Particle* particle, const Swarm_RandomGenerator randomFunction, const float lowerBound, const float upperBound);

/**
 * Updates the position and velocity of a particle.
 * \param particle The particle to update.
 * \param globalBestPosition The best position found by the swarm so far.
 * \param randomFunction The function to use for generating random cognitive and social factors.
 * \param w The inertia weight for the velocity update.
 * \param c1 The cognitive coefficient for the velocity update.
 * \param c2 The social coefficient for the velocity update.
 */
void Particle_Update(
		Particle*                   particle,
		const float*                globalBestPosition,
		const Swarm_RandomGenerator randomFunction,
		const float                 w,
		const float                 c1,
		const float                 c2);

/**
 * Evaluates the fitness of a particle.
 * \param particle The particle to evaluate.
 * \param evaluate The function to use for evaluating the fitness of the particle.
 * \return The fitness of the particle.
 */
float Particle_Evaluate(Particle* particle, const Swarm_ObjectiveFunction evaluate);

#endif
