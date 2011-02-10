#include <math.h>
#include "particle.hpp"

namespace Hadron {
	Particle::Particle():
	position((real)0.0, (real)0.0, (real)0.0),
	velocity((real)0.0, (real)0.0, (real)0.0),
	acceleration(Vector3<real>::GRAVITY),
	damping((real)0.9999),
	inverseMass((real)1.0),
	forceAccum((real)0.0, (real)0.0, (real)0.0),
	alive(false)
	{ }

	void Particle::integrate(real dT)
	{
		// Infinite mass! Don't bother trying to move it
		if(inverseMass <= (real)0.0) return;

		// Update the linear position
		position.AddScaledVector(velocity, dT);

		// Work out acceleration from forces on the particle
		Vector3<real> resultAcc = acceleration;
		resultAcc.AddScaledVector(forceAccum, inverseMass);

		// Update velocity
		velocity.AddScaledVector(resultAcc, dT);

		// Add on drag
		velocity *= real_pow(damping, dT);

		// Clear forces
		forceAccum.Clear();
	}

	real Particle::GetKineticEnergy() const
	{
		return alive ? (real)(0.5 * (1.0 / inverseMass) * velocity.LengthSquared()) : (real)0.0;
	}

	real Particle::GetX() const
	{
		return position.x;
	}

	real Particle::GetY() const
	{
		return position.y;
	}

	real Particle::GetZ() const
	{
		return position.z;
	}

	real Particle::GetMass() const
	{
		// Infinite mass
		if(inverseMass == 0)
			return REAL_MAX;

		return (real)1.0 / inverseMass;
	}

	bool Particle::IsAlive() const
	{
		return alive;
	}

	void Particle::SetPosition(const Vector3<real> &Position)
	{
		position = Position;
	}

	void Particle::SetPosition(real X, real Y, real Z)
	{
		position = Vector3<real>(X, Y, Z);
	}

	void Particle::SetX(real X)
	{
		position.x = X;
	}

	void Particle::SetY(real Y)
	{
		position.y = Y;
	}

	void Particle::SetZ(real Z)
	{
		position.z = Z;
	}

	void Particle::SetVelocity(const Vector3<real> &Velocity)
	{
		velocity = Velocity;
	}

	void Particle::SetVelocity(real X, real Y, real Z)
	{
		velocity = Vector3<real>(X, Y, Z);
	}

	void Particle::SetVelocityX(real X)
	{
		velocity.x = X;
	}

	void Particle::SetVelocityY(real Y)
	{
		velocity.y = Y;
	}

	void Particle::SetVelocityZ(real Z)
	{
		velocity.z = Z;
	}

	void Particle::SetAcceleration(const Vector3<real> &Acceleration)
	{
		acceleration = Acceleration;
	}

	void Particle::SetAcceleration(real X, real Y, real Z)
	{
		acceleration = Vector3<real>(X, Y, Z);
	}

	void Particle::SetAccelerationX(real X)
	{
		acceleration.x = X;
	}

	void Particle::SetAccelerationY(real Y)
	{
		acceleration.y = Y;
	}

	void Particle::SetAccelerationZ(real Z)
	{
		acceleration.z = Z;
	}

	void Particle::SetMass(real Mass)
	{
		if(Mass <= (real)0.0)
		{
			// Don't have zero mass, just have very, very low
			inverseMass = (real)1.0 / REAL_MAX;
		}
		else
		{
			inverseMass = (real)1.0 / real_abs(Mass);
		}
	}

	void Particle::SetAlive(bool Alive)
	{
		alive = Alive;
		if(alive) forceAccum.Clear();	// Just in case
	}

	void Particle::Update(real dT)
	{
		// We're not alive, don't bother doing anything
		if(!alive) return;

		integrate(dT);
	}

	void Particle::ApplyForce(const Vector3<real> &Force)
	{
		forceAccum += Force;
	}

	void Particle::ApplyForce(real X, real Y, real Z)
	{
		forceAccum += Vector3<real>(X, Y, Z);
	}
};