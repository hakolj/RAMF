#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#include <vector>
#include "AgentInterface.h"
#include "Agent.h"
#include "vectypedef.h"
#include "Config.h"
#include "Task.h"


class Sensor {
public:
	virtual int dim() const = 0;
	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual void reset(std::shared_ptr<Task> task) {}; //reset the sensor before every new episode

};

extern std::shared_ptr<Sensor> SensorInitializer(std::string path);



#endif // !SENSOR_H

