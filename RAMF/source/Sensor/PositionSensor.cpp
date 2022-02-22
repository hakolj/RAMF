#include "pch.h"
#include "PositionSensor.h"
void PositionSensor::getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
	auto itf = dynamic_cast<GetTransRotAble*>(agent);
	vectors3d pos = itf->getPos();

	if (dimension == 3) {
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][0];
			newstate[i][1] = pos[i][1];
			newstate[i][2] = pos[i][2];

		}
	}
	else if (dimension == 2) {
		int idx0 = (direction + 1) % 3;
		int idx1 = (direction + 2) % 3;
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][idx0];
			newstate[i][1] = pos[i][idx1];

		}
	}
	else if (dimension == 1) {
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][direction];
		}
	}

	return;
}

void PositionSensor::initialize(const std::string& path, const Config& config) {
	dimension = config.Read<int>("Dimension", 3);
	direction = config.Read<int>("Direction", 0);
}

// -------------------------RelaPositionAngleSensor-----------------------//

RelaPositionAngleSensor::RelaPositionAngleSensor() :target{ vec3d::Zero() } {};

void RelaPositionAngleSensor::getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
	auto itf = dynamic_cast<GetTransRotAble*>(agent);
	vectors3d pos = itf->getPos();
	vectors3d p3 = itf->getP3();
	for (unsigned i = 0; i < agent->agentnum; i++) {
		newstate[i][0] = pos[i][0] - target[0];
		newstate[i][1] = pos[i][1] - target[1];

		if (dimension == 3)	newstate[i][2] = pos[i][2] - target[2];

		newstate[i][dimension + 1] = atan2(p3[i][1], p3[i][0]); // angle

	}


	if (dimension == 3) {
		int idx0 = (direction + 1) % 3;
		int idx1 = (direction + 2) % 3;
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][0] - target[0];
			newstate[i][1] = pos[i][1] - target[1];
			newstate[i][2] = pos[i][2] - target[2];
			newstate[i][dimension + 1] = atan2(p3[i][idx1], p3[i][idx0]); // angle
		}
	}
	else if (dimension == 2) {
		int idx0 = (direction + 1) % 3;
		int idx1 = (direction + 2) % 3;
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][idx0] - target[idx0];
			newstate[i][1] = pos[i][idx1] - target[idx1];
			newstate[i][dimension + 1] = atan2(p3[i][idx1], p3[i][idx0]); // angle
		}
	}
	else if (dimension == 1) {
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][direction] - target[direction];
			newstate[i][dimension + 1] = acos(p3[i][direction]); // angle
		}
	}


	return;
}

void RelaPositionAngleSensor::initialize(const std::string& path, const Config& config) {
	dimension = config.Read<int>("Dimension", 3);
	direction = config.Read<int>("Direction", 3);
}



void RelaPositionAngleSensor::reset(std::shared_ptr<Task> task) {
	target = std::dynamic_pointer_cast<GetTargetable>(task)->getTarget();
}