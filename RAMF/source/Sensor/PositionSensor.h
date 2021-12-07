#ifndef POSITIONSENSOR_H
#define POSITIONSENSOR_H

#include "Sensor.h"


class PositionSensor :Sensor {
public:
	int dimension = 3;

	virtual int dim() const {
		return dimension;
	};

	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate);
	virtual void initialize(const std::string& path, const Config& config);
};


PositionSensor::getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
	auto itf = dynamic_cast<GetTransRotAble*>(agent);
	vectors3d pos = itf->getPos();
	for (unsigned i = 0; i < agent->agentnum; i++) {
		newstate[i][0] = pos[i][0];
		newstate[i][1] = pos[i][1];

		if (dimension == 3)	newstate[i][2] = pos[i][2];
	}
	return;
}

PositionSensor::initialize(const std::string& path, const Config& config) {
	dimension = config.Read<int>("Dimension", 3);
}


// relative position to a target
class RelaPositionAngleSensor :Sensor {
public:
	int dimension = 3;
	vec3d target;


	RelaPositionSensor();
	virtual int dim() const {
		return dimension + 1; //dim in pos + 1 dim in angle
	};

	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate);
	virtual void initialize(const std::string& path, const Config& config);
};

RelaPositionAngleSensor::RelaPositionSensor() :target{ vec3d::Zero() } {};

void RelaPositionAngleSensor::getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
	auto itf = dynamic_cast<GetTransRotAble*>(agent);
	vectors3d pos = itf->getPos();
	vectors3d p3 = itf->getP3();
	for (unsigned i = 0; i < agent->agentnum; i++) {
		newstate[i][0] = pos[i][0] - target[0];
		newstate[i][1] = pos[i][1] - target[1];

		if (dimension == 3)	newstate[i][2] = pos[i][2] - target[2];

		newstate[i][dimension + 1] = atan2(p3[1], p3[0]); // angle
	}
	return;
}

void RelaPositionAngleSensor::initialize(const std::string& path, const Config& config) {
	dimension = config.Read<int>("Dimension", 3);
}



void PositionSensor::reset(std::shared_ptr<Task> task) {
	target = std::dynamic_pointer_cast<GetTargetable>(task)->getTarget();
}

#endif