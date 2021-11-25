#pragma once
#ifndef TASK_H
#define TASK_H



#include <vector>
#include "Agent.h"
#include "Config.h"
class Task {
public:
	virtual void getReward(const Agent* const agent, std::vector<double>& reward) = 0;
	virtual std::vector<double> getTotalReward(const Agent* agent) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual void reset(const Agent* agent) = 0;
};

extern std::shared_ptr<Task> TaskInitializer(const std::string& path);

#endif // !TASK_H