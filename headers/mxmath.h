/*
	MXMath - Math Library for games and graphical applications
	Copyright (c) 2012 - Daniel Lawrence (daniellawrence99 -at- yahoo -dot- com)
	Version 0.1

	-- A later note to self: Stupid copyright notices are stupid.
	Solution: GPL2.
*/

#ifndef MXMATH_H
#define MXMATH_H

#include <math.h>

class MXVector
{
	public:
		static MXVector i, j, k;
        float x, y, z;
		double* v() const;

		MXVector();

		MXVector(float ax, float ay, float az);

		~MXVector();

		MXVector operator*(const float& S) const;

		MXVector& operator*=(const float& S);

		MXVector operator&(const MXVector& V) const;

        MXVector operator+(const MXVector& V) const;

        MXVector operator-(const MXVector& V) const;

        MXVector Inverse();

        float GetLength();

        MXVector GetNormal();

        void Normalize();

        float GetSQLength();

        float operator*(const MXVector& V) const;

        float operator*=(const MXVector& V);

        MXVector& operator+=(const MXVector& V);

        MXVector& operator-=(const MXVector& V);
};

class MXMatrix{};


#endif

