#ifndef HADRON_PARTICLEFORCEGENERATOR_HPP
#define HADRON_PARTICLEFORCEGENERATOR_HPP

#include <list>

#include "../core/precision.hpp"
#include "particle.hpp"
#include "../math/vector3.hpp"

namespace Hadron {
	class ParticleForceGenerator
	{
	private:

	public:
		// This method must be overridden
		// It can use the ApplyForce function on the particle to do what it needs to
		virtual void ApplyForce(Particle *P, real dT) = 0;
	};

	class ParticleForceRegistry
	{
	private:
		// An entry in the registry
		struct ParticleForceRegistration
		{
			Particle *particle;
			ParticleForceGenerator *forceGen;
		};

		// List of registrations
		typedef std::list<ParticleForceRegistration> Registry;
		Registry registrations;

	public:
		// Registers the given force generator and particle
		void Add(Particle *P, ParticleForceGenerator *ForceGen);

		// Removes the given registered pair from the registry
		void Remove(Particle *P, ParticleForceGenerator *ForceGen);

		// Clears all registrations
		void Clear();

		// Makes all force generators apply their forces to their respective particles
		void ApplyForces(real dT);
	};

	/*-------------------------------------*\
	|* Declare all custom force generators *|
	|* below here!                         *|
	\*-------------------------------------*/
	class ParticleGravitation : public ParticleForceGenerator
	{
	private:
		Vector3<real> gravPosition;

	public:
		void SetGravityPosition(const Vector3<real> &Position);
		void SetGravityPosition(real X, real Y, real Z);
		void ApplyForce(Particle *P, real dT);
	};
};

#endif // HADRON_PARTICLEFORCEGENERATOR_HPP