#ifndef HADRON_PARTICLE_HPP
#define HADRON_PARTICLE_HPP

#include "../core/precision.hpp"
#include "../math/vector3.hpp"

namespace Hadron {
	class Particle
	{
	private:
		// Self explanatory really
		Vector3<real> position, velocity, acceleration;

		// A value of damping to apply to linear motion - cancelling out any numerical instabilities in the integrator
		real damping;

		// 1 / mass
		// ^- The logic behind this is simple: acceleration = force / mass => acceleration = force * invMass
		// ^- Meaning we can easily have infinite masses (immovable objects), but not have 0 mass particles
		real inverseMass;

		// Force accumulator
		// Any forces applied to the particle will get added onto this vector
		Vector3<real> forceAccum;

		// Simply specifies if this particle is alive or not
		bool alive;

		// Integrates the particle's position forward in time
		// ^- dT is the time step to integrate across
		// ^- Uses Newton-Euler integration
		void integrate(real dT);

		// Simply clears the force accumulator
		void clearAccumulator();

	public:
		// Default constructor
		Particle();

		// Getters
		real GetKineticEnergy() const;

		real GetX() const;
		real GetY() const;
		real GetZ() const;

		real GetMass() const;

		bool IsAlive() const;

		// Setters
		// Self explanatory
		void SetPosition(const Vector3<real> &Position);
		void SetPosition(real X, real Y, real Z);
		void SetX(real X);
		void SetY(real Y);
		void SetZ(real Z);
		
		void SetVelocity(const Vector3<real> &Velocity);
		void SetVelocity(real X, real Y, real Z);
		void SetVelocityX(real X);
		void SetVelocityY(real Y);
		void SetVelocityZ(real Z);

		void SetAcceleration(const Vector3<real> &Acceleration);
		void SetAcceleration(real X, real Y, real Z);
		void SetAccelerationX(real X);
		void SetAccelerationY(real Y);
		void SetAccelerationZ(real Z);

		void SetMass(real Mass);

		void SetAlive(bool Alive);

		// Methods
		// Calls all the necessary methods to update the particle
		void Update(real dT);
		
		// Applies a force vector to the particle
		void ApplyForce(const Vector3<real> &Force);
		void ApplyForce(real X, real Y, real Z);
	};
};

#endif // HADRON_PARTICLE_HPP