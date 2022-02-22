#pragma once
#ifndef DISTANCEFROMBOUNDARY_H
#define DISTANCEFROMBOUNDARY_H


//#include "pch.h"
#include "Task.h"

class DistanceFromBoundary :
	public Task {

//private:
	//std::vector<double> _recorder; // to record the initial position. used for calculating total _reward
	//std::vector<double> _cache; // to record the last vertical position
	//std::vector<double> _reward;
private:
	int boundaryID; // which boundary to keep distance
	int direction; // direction of prefered boundary. 1 = keep close to boundary.  -1 =  keep away from boundary.
public:
	DistanceFromBoundary();

public:
	void initialize(const std::string& path, const Config& config);


	void getReward(const SimuManager& simuManager, std::vector<double>& reward);
	std::vector<double> getTotalReward(const SimuManager& simuManager);
	void reset(const Agent* agent);
};

#endif // !NAVIGATIONUPWARD_H