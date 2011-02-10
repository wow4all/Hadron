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
};