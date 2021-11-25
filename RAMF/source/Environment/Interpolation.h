#pragma once
#ifndef INTERPOLATION_H
#define INTERPOLATION_H


#include "pch.h"
#include "Scalar.h"

class Lag2nd3D {
public:
	
	Eigen::Matrix<double, 3, 3> A;
	Eigen::Matrix<double, 3, 3> B;
	Eigen::Matrix<double, 3, 3> C;
private:
	double dx, dy, dz;
public:
	Lag2nd3D() :
		A(Eigen::Matrix3d::Zero()),
		B(Eigen::Matrix3d::Zero()),
		C(Eigen::Matrix3d::Zero()),
		dx(0), dy(0), dz(0)
	{

	}

	void interpCoef(double dx, double dy, double dz);
	void interp3d(const vec3d& pos, const Scalar& sclx, const Scalar& scly, const Scalar& sclz, vec3d& info);
};
#endif // !INTERPOLATION_H