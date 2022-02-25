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
	int _boundaryID; // which boundary to keep distance
	int _direction; // _direction of prefered boundary. 1 = keep close to boundary.  -1 =  keep away from boundary.
	//std::vector<double> _recorder; // to record the initial position. used for calculating total _reward
	std::vector<double> _cache; // to record the last vertical position
	int _rwPower; // power in reward function
	int _totalrwPower; // power in total reward function
	double _c2; // the coefficient for the delta value in reward function;
public:
	DistanceFromBoundary();

public:
	void initialize(const std::string& path, const Config& config);


	void getReward(const SimuManager& simuManager, std::vector<double>& reward);
	std::vector<double> getTotalReward(const SimuManager& simuManager);
	void reset(const SimuManager& simuManager);

private:
	void getDistanceValue(const SimuManager& simuManager, std::vector<double>& value, int power);
};

#endif // !NAVIGATIONUPWARD_H