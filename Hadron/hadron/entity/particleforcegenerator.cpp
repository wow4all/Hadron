#include "particleforcegenerator.hpp"

namespace Hadron {
	void ParticleForceRegistry::Add(Particle *P, ParticleForceGenerator *ForceGen)
	{
		ParticleForceRegistration r;
		r.particle = P;
		r.forceGen = ForceGen;
		
		registrations.push_back(r);
	}

	void ParticleForceRegistry::Remove(Particle *P, ParticleForceGenerator *ForceGen)
	{
		Registry::iterator i = registrations.begin();

		for(; i != registrations.end(); ++i)
		{
			if(i->particle == P && i->forceGen == ForceGen)
			{
				registrations.erase(i);
				break;
			}
		}
	}

	void ParticleForceRegistry::Clear()
	{
		registrations.clear();
	}

	void ParticleForceRegistry::ApplyForces(real dT)
	{
		Registry::iterator i = registrations.begin();

		for(; i != registrations.end(); ++i)
		{
			i->forceGen->ApplyForce(i->particle, dT);
		}
	}

	/*-------------------------------------*\
	|* Define all custom force generators  *|
	|* below here!                         *|
	\*-------------------------------------*/
	void ParticleGravitation::SetGravityPosition(const Vector3<real> &Position)
	{
		gravPosition = Position;
	}

	void ParticleGravitation::SetGravityPosition(real X, real Y, real Z)
	{
		gravPosition = Vector3<real>(X, Y, Z);
	}

	void ParticleGravitation::ApplyForce(Particle *P, real dT)
	{
		if(!P->IsAlive()) return;
		
		// X, Y and Z distances
		real xDiff = P->GetX() - gravPosition.x;
		real yDiff = P->GetY() - gravPosition.y;
		real zDiff = P->GetZ() - gravPosition.z;

		// Radius
		real radiusSquared = (xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff);

		// F = -GMm / r^2
		// + a little modification to make it game-suitable
		real mass = P->GetMass();
		real force = -(mass * (real)100.0) / radiusSquared;

		P->ApplyForce(xDiff * force, yDiff * force, zDiff * force);
	}

	ParticleDrag::ParticleDrag(real VelCoeff, real VelSqCoeff):
	k1(VelCoeff),
	k2(VelSqCoeff)
	{ }

	void ParticleDrag::ApplyForce(Particle *P, real dT)
	{
		Vector3<real> force = P->GetVelocity();

		// Calculate total drag coefficient
		real dragCoeff = force.Length();
		dragCoeff = (k1 * dragCoeff) + (k2 * dragCoeff * dragCoeff);

		P->ApplyForce(force.Normalised() * -dragCoeff);
	}

	ParticleSpring::ParticleSpring():
	other(NULL),
	k((real)0.0),
	restLength((real)30.0)
	{ }

	ParticleSpring::ParticleSpring(Particle *Other, real SpringConstant, real RestLength):
	other(Other),
	k(SpringConstant),
	restLength(RestLength)
	{ }

	void ParticleSpring::SetParentParticle(Particle *Other)
	{
		other = Other;
	}

	void ParticleSpring::SetSpringConstant(real K)
	{
		k = K;
	}

	void ParticleSpring::SetRestLength(real RestLength)
	{
		restLength = RestLength;
	}

	void ParticleSpring::ApplyForce(Particle *P, real dT)
	{
		if(other == NULL) return;
		else if(!P->IsAlive() || !other->IsAlive()) return;

		// Spring's vector
		Vector3<real> springVec = P->GetPosition() - other->GetPosition();

		// The force to apply
		Vector3<real> force = springVec.Normalised() * (-k * (springVec.Length() - restLength));

		P->ApplyForce(force);
	}
};