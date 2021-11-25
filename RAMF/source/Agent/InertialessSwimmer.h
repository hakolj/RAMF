#pragma once
#ifndef INERTIALESSSWIMMER_H
#define INERTIALESSSWIMMER_H


#include "Agent.h"
#include "PointParticle.h"
class InertialessSwimmer : 
	public PointParticle, 	
	public ChangeMassCenterAble,
	public ChangeSwimVelAble
{
public:

	Eigen::Vector3d eg; //gravity direction


protected:
	double _Lda;
	double _rhop, _rhof, _nu, _gravity;

	bool iFluidInertTorq;
	//fluid inertial torque const
	double _Mi; 
	//aspect ratio & radius
	double lda, a; 
	std::vector<double> vswim, gyro;
	vec3d vsettle;
	vectors3d mdisp; //mass center displacement
	vectors3d _swimAngVel; //active swimming angular velocity


public:
	InertialessSwimmer(int agentnum, double lda = 1.0, double a = 0.0);
	virtual void initialize(const std::string& path, const Config& config);
	virtual void update(double timestep);
	virtual void reset();
	virtual void getInfo();
	inline virtual void useInfo() { convertFrame(); };
	void dump(const char* path, int step); //dump agent info

	inline void setPhysics(double rhop, double rhof, double nu, double gravity) {
		_rhop = rhop; _rhof = rhof; _nu = nu; _gravity = gravity;
		return;
	}

	inline void setShape(double a, double lda) {
		this->a = a;
		this->lda = lda;
		_Lda = (lda * lda - 1) / (lda * lda + 1);
		return;
	};
	void setMotility(double vswim, double B);
	void setVsettle();
	void convertFrame();


	inline void setMassCenter(const vectors3d& newMassCenter) {
		mdisp = newMassCenter;
		return;
	}

	inline vectors3d getMassCenter() const { return mdisp; }
	inline void setSwimVel(const std::vector<double>& newvswim) { vswim = newvswim; }
	inline vectors3d& swimAngVel() { return _swimAngVel; }

public:
	// return the F_{\beta}, used for calculating fluid inertia constant
	double _Fbeta(double lda);
	double _solveMi(double lda);
	// setting fluid inertial torque constant Mi
	void setInertialTorqueConst();

};

#endif // !INERTIALESSSWIMMER_H