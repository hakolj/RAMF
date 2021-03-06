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

RelaPositionAngleSensor::RelaPositionAngleSensor() :target{ vec3d::Zero() }, _selftarget{ vec3d::Zero() } {};

void RelaPositionAngleSensor::getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
	auto itf = dynamic_cast<GetTransRotAble*>(agent);
	vectors3d pos = itf->getPos();
	vectors3d p3 = itf->getP3();



	if (dimension == 3) {
		int idx0 = (direction + 1) % 3;
		int idx1 = (direction + 2) % 3;
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][0] - target[0];
			newstate[i][1] = pos[i][1] - target[1];
			newstate[i][2] = pos[i][2] - target[2];
			newstate[i][dimension] = atan2(p3[i][idx1], p3[i][idx0]); // angle
		}
	}
	else if (dimension == 2) {
		int idx0 = (direction + 1) % 3;
		int idx1 = (direction + 2) % 3;
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][idx0] - target[idx0];
			newstate[i][1] = pos[i][idx1] - target[idx1];
			newstate[i][dimension] = atan2(p3[i][idx1], p3[i][idx0]); // angle
		}
	}
	else if (dimension == 1) {
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][direction] - target[direction];
			newstate[i][dimension] = acos(p3[i][direction]); // angle
		}
	}


	return;
}

void RelaPositionAngleSensor::initialize(const std::string& path, const Config& config) {
	dimension = config.Read<int>("Dimension", 3);
	direction = config.Read<int>("Direction", 3);
	std::string targetstr = config.Read<std::string>("self target", "");
	if (targetstr == "") {
		_selftarget = vec3d::Zero();
	}
	else {
		std::vector<double> tempvec = Fop::loadvec1d<double>(targetstr);
		if (tempvec.size() != 3) {
			std::cout << "wrong target size in sensor: RelaPositionAngleSensor" << std::endl;
			exit;
		}
		else {
			_selftarget[0] = tempvec[0];
			_selftarget[1] = tempvec[1];
			_selftarget[2] = tempvec[2];
		}
		
	}

}



void RelaPositionAngleSensor::reset(std::shared_ptr<Task> task) {
	if (isExternalTarget) target = std::dynamic_pointer_cast<GetTargetable>(task)->getTarget();
	else target = _selftarget;
}