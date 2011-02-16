#ifndef HADRON_VECTOR3_HPP
#define HADRON_VECTOR3_HPP

#include <math.h>

#include "../core/precision.hpp"

namespace Hadron {
	template<typename T>
	class Vector3
	{
	private:

	public:
		// Data members
		T x, y, z;

		// Static members
		static const Vector3<T> GRAVITY;
		static const Vector3<T> HIGH_GRAVITY;
		static const Vector3<T> UP;
		static const Vector3<T> RIGHT;
		static const Vector3<T> FORWARD;
		static const Vector3<T> ZERO;

		// Constructors
		Vector3();
		Vector3(const Vector3<T> &Vec);
		Vector3(T X, T Y, T Z);

		// Operator overloads
		void operator=(const Vector3<T> &Vec);

		const Vector3<T> operator+(const Vector3<T> &Vec) const;
		const Vector3<T> operator-(const Vector3<T> &Vec) const;

		const Vector3<T> operator*(real Scalar) const;
		const Vector3<T> operator/(real Scalar) const;

		void operator+=(const Vector3<T> &Vec);
		void operator-=(const Vector3<T> &Vec);

		void operator*=(real Scalar);
		void operator/=(real Scalar);

		// Getters
		T Length() const;
		T LengthSquared() const;
		real Dot(const Vector3<T> &Vec) const;
		const Vector3<T> &Cross(const Vector3<T> &Vec) const;
		const Vector3<T> &Normalised() const;

		// Setters
		void AddScaledVector(const Vector3<T> &Vec, real Scale);
		
		// Methods
		void Clear();
	};

	template<typename T>
	const Vector3<T> Vector3<T>::GRAVITY = Vector3<T>((T)0.0, (T)-9.81, (T)0.0);

	template<typename T>
	const Vector3<T> Vector3<T>::HIGH_GRAVITY = Vector3<T>((T)0.0, (T)-19.62, (T)0.0);

	template<typename T>
	const Vector3<T> Vector3<T>::UP = Vector3<T>((T)0.0, (T)1.0, (T)0.0);

	template<typename T>
	const Vector3<T> Vector3<T>::RIGHT = Vector3<T>((T)1.0, (T)0.0, (T)0.0);

	template<typename T>
	const Vector3<T> Vector3<T>::FORWARD = Vector3<T>((T)0.0, (T)0.0, (T)-1.0);

	template<typename T>
	const Vector3<T> Vector3<T>::ZERO = Vector3<T>((T)0.0, (T)0.0, (T)0.0);

	// Default constructor
	template<typename T>
	Vector3<T>::Vector3():
	x((T)0),
	y((T)0),
	z((T)0)
	{ }

	// Copy constructor
	template<typename T>
	Vector3<T>::Vector3(const Vector3<T> &Vec):
	x(Vec.x),
	y(Vec.y),
	z(Vec.z)
	{ }

	// Basic initialisation constructor
	template<typename T>
	Vector3<T>::Vector3(T X, T Y, T Z):
	x(X),
	y(Y),
	z(Z)
	{ }

	// Assignment operator
	template<typename T>
	void Vector3<T>::operator=(const Vector3<T> &Vec)
	{
		x = Vec.x;
		y = Vec.y;
		z = Vec.z;
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator+(const Vector3<T> &Vec) const
	{
		return Vector3<T>(x + Vec.x, y + Vec.y, z + Vec.z);
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator-(const Vector3<T> &Vec) const
	{
		return Vector3<T>(x - Vec.x, y - Vec.y, z - Vec.z);
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator*(real Scalar) const
	{
		return Vector3<T>(x * Scalar, y * Scalar, z * Scalar);
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator/(real Scalar) const
	{
		return Vector3<T>(x / Scalar, y / Scalar, z / Scalar);
	}

	template<typename T>
	void Vector3<T>::operator+=(const Vector3<T> &Vec)
	{
		x += Vec.x;
		y += Vec.y;
		z += Vec.z;
	}

	template<typename T>
	void Vector3<T>::operator-=(const Vector3<T> &Vec)
	{
		x -= Vec.x;
		y -= Vec.y;
		z -= Vec.z;
	}

	template<typename T>
	void Vector3<T>::operator*=(real Scalar)
	{
		x *= Scalar;
		y *= Scalar;
		z *= Scalar;
	}

	template<typename T>
	void Vector3<T>::operator/=(real Scalar)
	{
		x /= Scalar;
		y /= Scalar;
		z /= Scalar;
	}

	// Returns the magnitude of the vector
	template<typename T>
	T Vector3<T>::Length() const
	{
		return (T)sqrt((x * x) + (y * y) + (z * z));
	}

	// Returns the magnitude^2 of the vector
	// ^- Cuts out the call to sqrt(), meaning it's a bit faster
	template<typename T>
	T Vector3<T>::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	// Returns the dot product of our vector with another
	template<typename T>
	real Vector3<T>::Dot(const Vector3<T> &Vec) const
	{
		return (real)((x * Vec.x) + (y * Vec.y) + (z * Vec.z));
	}

	// Returns the cross product of our vector with another
	template<typename T>
	const Vector3<T> &Vector3<T>::Cross(const Vector3<T> &Vec) const
	{
		return Vector3<T>(
			y * Vec.z - z * Vec.y,
			z * Vec.x - x * Vec.z,
			x * Vec.y - y * Vec.x
		);
	}

	// Returns the unit vector of our vector
	template<typename T>
	const Vector3<T> &Vector3<T>::Normalised() const
	{
		real len = Length();
		if(len == 0) return Vector3<T>::ZERO;
		
		return Vector3<T>(x / len, y / len, z / len);
	}

	template<typename T>
	void Vector3<T>::AddScaledVector(const Vector3<T> &Vec, real Scale)
	{
		(*this) += Vec * Scale;
	}

	template<typename T>
	void Vector3<T>::Clear()
	{
		(*this) = Vector3<T>::ZERO;
	}
};

#endif // HADRON_VECTOR3_HPP