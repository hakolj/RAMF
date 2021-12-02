#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "vectypedef.h"
#include "Config.h"
#include "EnvironmentInterface.h"



class Environment
{
public:
	//get flow information at agent position
	//virtual void getInfo(const vectors3d& pos, vectors3d& u, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual void reset() = 0;
	virtual void update(double dt) = 0;

	//return the boundary type in XYZ. 
	//Example: PPP = periodic boundaries in 3 dirs. PWP = periodic boundaries in x and z, wall boundary in y
	//PPN: piriodic in x and y. No z direction (2D cases). 
	virtual std::string boundaryType() = 0;
	virtual void getDomainSize(double& Lx, double& Ly, double& Lz) = 0;
	virtual ~Environment() {}
};


// to initialize envs, return a shared_ptr for env
extern std::shared_ptr<Environment> EnvInitializer(const std::string& path);

#endif // !ENVIRONMENT_H





