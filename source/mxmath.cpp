/*
	MXMath - Math Library for games and graphical applications
	Copyright (c) 2012 - Daniel Lawrence (daniellawrence99 -at- yahoo -dot- com)
	Version 0.1

	-- A later note to self: Stupid copyright notices are stupid.
	Solution: GPL2.
*/

#include <math.h>
#include <mxmath.h>


// NOTE: Don't use this.
double* MXVector::v() const
{
    return 0;
}

MXVector::MXVector()
{
    x = y = z = 0.0F;
}

MXVector::MXVector(float ax, float ay, float az):x(ax),y(ay),z(az)
{

}

MXVector::~MXVector() {}

MXVector MXVector::operator*(const float& S) const
{
    MXVector tmp(x*S,y*S,z*S);
    return tmp;
}

MXVector& MXVector::operator*=(const float& S)
{
    x *= S;
    y *= S;
    z *= S;
    return (*this);
}

MXVector MXVector::operator&(const MXVector& V) const
{
    return MXVector(y*V.z - V.z*z, z*V.x - V.z*x, x*V.y - V.x*y);
}

MXVector MXVector::operator+(const MXVector& V) const
{
    MXVector tmp = MXVector(x,y,z);
    tmp.x += V.x;
    tmp.y += V.y;
    tmp.z += V.z;

    return tmp;
}

MXVector MXVector::operator-(const MXVector& V) const
{
    MXVector tmp = MXVector(x,y,z);
    tmp.x -= V.x;
    tmp.y -= V.y;
    tmp.z -= V.z;
    return tmp;
}

MXVector MXVector::Inverse()
{
    MXVector tmp(-x,-y,-z);
    return tmp;
}

float MXVector::GetLength()
{
    // Computationally expensive (Relatively speaking).
    // Use wisely
    float l = 0.0;
    l = (x*x)+(y*y)+(z*z);
    l = sqrt(l);
    return l;
}

MXVector MXVector::GetNormal()
{
    // Returns the normalized version of the vector
    float len = GetLength();
    if(len == 0.0F) return MXVector(0.0F,0.0F,0.0F);
    return MXVector(x/len, y/len, z/len);
}

void MXVector::Normalize()
{
    MXVector i = GetNormal();
    x = i.x;
    y = i.y;
    z = i.z;

}

float MXVector::GetSQLength()
{
    return (x*x + y*y + z*z);
}

float MXVector::operator*(const MXVector& V) const
{
    // Compute the dot product
    return (x*V.x + y*V.y + z*V.z);
}

float MXVector::operator*=(const MXVector& V)
{
    float ret = float(x*V.x + y*V.y + z*V.z);
    return ret;
}

MXVector& MXVector::operator+=(const MXVector& V)
{
    x += V.x;
    y += V.y;
    z += V.z;
    return (*this);
}

MXVector& MXVector::operator-=(const MXVector& V)
{
    x -= V.x;
    y -= V.y;
    z -= V.z;
    return (*this);
}


MXVector MXVector::i = MXVector(1,0,0);
MXVector MXVector::j = MXVector(0,1,0);
MXVector MXVector::k = MXVector(0,0,1);




