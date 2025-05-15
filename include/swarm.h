#ifndef _SWARM_H_
#define _SWARM_H_

/**
 * \file
 * Particle Swarm Optimization
 *
 * Particle Swarm Optimization (PSO) maximizes the objective function.
 */

#include <stddef.h>

/**
 * Dimensions of the problem space.
 * Defined by build process.
 *
 * Check CMake CACHE variable PARTICLE_SWARM_NUM_DIMENSIONS
 */
#ifndef SWARM_DIMENSIONS
#error "SWARM_DIMENSIONS must be defined"
#endif

/**
 * Objective function template which will evaluate the current particle parameters.
 * \param parameters The current particle parameters.
 * \return The fitness of the current particle parameters.
 */
typedef float (*Swarm_ObjectiveFunction)(const float* parameters);

/**
 * Random number generator template which will generate a random float between and including min and max.
 * Uniform distribution.
 * \param min The minimum value of the random number.
 * \param max The maximum value of the random number.
 * \return A random float between and including min and max.
 */
typedef float (*Swarm_RandomGenerator)(const float min, const float max);

/**
 * Progress report callback template which will be called after each iteration when optimizing.
 * \param itt The current iteration number.
 * \param bestFitness The current best fitness found.
 * \param bestParameters The current best parameters found.
 */
typedef void (*Swarm_ProgressReport)(const size_t itt, const float bestFitness, const float* bestParameters);

/**
 * Particle information.
 */
typedef struct _Particle_
{
	float BestFitness;
	float BestPosition[SWARM_DIMENSIONS];
	float Position[SWARM_DIMENSIONS];
	float Velocity[SWARM_DIMENSIONS];
} Particle;

/**
 * Swarm information.
 */
typedef struct _Swarm_
{
	Swarm_RandomGenerator RandomGenerator;
	size_t                NumParticles;
	Particle*             Particles;
	float                 GlobalBestFitness;
	float                 GlobalBestPosition[SWARM_DIMENSIONS];
} Swarm;

/**
 * Initializes a swarm for optimization. Creates the required particles and sets their initial parameters.
 * \param swarm The swarm to initialize.
 * \param particles The array of particles to initialize. Should contain atleast numParticles elements.
 * \param numParticles The number of particles in the swarm.
 * \param randomGenerator Random number generator function to use when random numbers are needed.
 * \param lowerBound The lower bound of the search space. All particles' positions will be within this range.
 * \param upperBound The upper bound of the search space. All particles' positions will be within this range.
 */
void Swarm_Init(
		Swarm*                      swarm,
		Particle*                   particles,
		const size_t                numParticles,
		const Swarm_RandomGenerator randomGenerator,
		const float                 lowerBound,
		const float                 upperBound);

/**
 * Maximizes the objective function using a particle swarm optimization algorithm.
 * \param swarm The swarm to use for optimization.
 * \param evaluate The objective function to maximize.
 * \param maxIterations The maximum number of iterations to perform.
 * \param w The inertia weight.
 * \param c1 The cognitive coefficient.
 * \param c2 The social coefficient.
 * \param progress A callback function that will be called periodically during optimization. Can be NULL if no progress reporting is needed.
 */
void Swarm_Optimize(
		Swarm*                        swarm,
		const Swarm_ObjectiveFunction evaluate,
		const size_t                  maxIterations,
		const float                   w,
		const float                   c1,
		const float                   c2,
		const Swarm_ProgressReport    progress);

/**
 * Returns the best fitness value found by the swarm.
 * \param swarm The swarm to query.
 * \return The best fitness value found by the swarm.
 */
float Swarm_GetBestFitness(const Swarm* swarm);

/**
 * Returns the position best position found by the swarm.
 * \param swarm The swarm to query.
 * \return The best position found by the swarm.
 */
float* Swarm_GetBestPosition(Swarm* swarm);

#endif
