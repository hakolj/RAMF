#pragma once
#ifndef NAVIGATIONPOINT_H
#define NAVIGATIONPOINT_H


//#include "pch.h"
#include "Task.h"

class NavigationPoint : 
	public Task,
	public GetTargetable {
public:
	vec3d target;
	double bonusrange;
	double panelty, bonus; //panelty: ... ; bonus: positive _reward if agent is in bonus range;
private:
	std::vector<double> _recorder; // to record the initial position. used for calculating total _reward
	//std::vector<double> _reward;
public:
	NavigationPoint();
private:
	NavigationPoint(double x, double y, double z, double bonusrange, double panelty = -0.5, double bonus = 1.0);
public:
	void initialize(const std::string& path, const Config& config);


	void getReward(const SimuManager& simuManager, std::vector<double>& reward);
	std::vector<double> getTotalReward(const SimuManager& simuManager);
	void reset(const SimuManager& simuManager);

	virtual vec3d getTarget() { return target; };

	

};

#endif // !NAVIGATIONPOINT_H