#pragma once
#ifndef NAVIGATIONUPWARD_H
#define NAVIGATIONUPWARD_H


//#include "pch.h"
#include "Task.h"

class NavigationUpward :
	public Task {

private:
	std::vector<double> _recorder; // to record the initial position. used for calculating total _reward
	std::vector<double> _cache; // to record the last vertical position
	//std::vector<double> _reward;
public:
	NavigationUpward();

public:
	void initialize(const std::string& path, const Config& config);


	void getReward(const SimuManager& simuManager, std::vector<double>& reward);
	std::vector<double> getTotalReward(const SimuManager& simuManager);
	void reset(const SimuManager& simuManager);
};

#endif // !NAVIGATIONUPWARD_H