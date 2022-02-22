#include "pch.h"
#include "DistanceFromBoundary.h"


using namespace std;

DistanceFromBoundary::DistanceFromBoundary() {};

void DistanceFromBoundary::initialize(const std::string& path, const Config& config) {
	boundaryID = config.Read<int>("selected Boundary");
	direction = config.Read<int>("direction");

};


void DistanceFromBoundary::getReward(const SimuManager& simuManager, std::vector<double>& reward) {
	double boundaryPos[3];
	simuManager.env->getDomainSize(boundaryPos[0], boundaryPos[1], boundaryPos[2]);

	auto itf = dynamic_pointer_cast<const GetTransRotAble>(simuManager.agent);
	vectors3d pos(itf->getPos());


	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {

		reward[i] = pow(pos[i][boundaryID]/boundaryPos[boundaryID], 2) + pow(1.0 - pos[i][boundaryID]/boundaryPos[boundaryID], 2);
		reward[i] *= direction;
	}
}


std::vector<double> DistanceFromBoundary::getTotalReward(const SimuManager& simuManager) {
	std::vector<double> reward(simuManager.agent->agentnum, 0.0);
	getReward(simuManager, reward);
	return reward;
}
void DistanceFromBoundary::reset(const Agent* agent) {}