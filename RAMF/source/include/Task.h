#pragma once
#ifndef TASK_H
#define TASK_H



#include <vector>
#include "Agent.h"
#include "Config.h"
#include "TaskInterface.h"
#include "SimuManager.h"
class Task {
public:
	virtual void getReward(const SimuManager& simuManager, std::vector<double>& reward) = 0;
	//virtual void getReward(const Agent* const agent, std::vector<double>& reward) = 0;
	virtual std::vector<double> getTotalReward(const SimuManager& simuManager) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual void reset(const SimuManager& simuManager) = 0;
};

extern std::shared_ptr<Task> TaskInitializer(const std::string& path);

#endif // !TASK_H