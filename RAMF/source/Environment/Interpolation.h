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
	void interp3d_old(const vec3d& pos, const Scalar& sclx, const Scalar& scly, const Scalar& sclz, vec3d& info);
	void interp3d(const vec3d& pos, const Scalar& sclx, const Scalar& scly, const Scalar& sclz, vec3d& info);

	// calculate Lagrangian interpolation coefficient using local grid information
	double Lag2Base(const int& iflag, const double xp, const double x0, const double x1, const double x2);

	// calculate Lagrangian interpolation coefficients for three interp points using local grid information
	//out: coeff
	void Lag2Bases(const double xp, const double x0, const double x1, const double x2, double coeff[3]);
};
#endif // !INTERPOLATION_H