#pragma once
#ifndef AGENT_H
#define AGENT_H
#include "vectypedef.h"
#include "AgentInterface.h"
#include "Environment.h"
#include "Config.h"
#include "EnvironmentInterface.h"


class Agent {
public:
	unsigned agentnum;
	Agent(unsigned agentnum); 
	virtual void update(double timestep) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0; //initialize parameters using config file
	virtual void reset() = 0; //reset agent. Used in: episode initialization
	virtual void getInfo() = 0; //get flow info
	virtual void useInfo() = 0; // preprocess flow info before update
	virtual void setEnv(std::shared_ptr<Environment> env) = 0;
	virtual void dump(const char* path, int step) = 0; //dump agent info
	virtual void BoundaryCondition(std::shared_ptr<Environment> env) {};
	//virtual bool ReadyToSense(int idx);
	//void UpdateSenseStepCount() { for (int i = 0; i < agentnum; i++) (++senseStepCount[i]) %= senseStep; }

protected:
	//std::vector<int> senseStepCount;
	//int senseStep = 0;
	//void InitSenseStepCount(const Config& config);
	
};

extern std::shared_ptr<Agent> AgentInitializer(const std::string& path);

#endif



