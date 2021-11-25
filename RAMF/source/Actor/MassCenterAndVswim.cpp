#include "pch.h"
#include "MassCenterAndVswim.h"

using namespace std;

MassCenterAndVswim::MassCenterAndVswim() :maxVswim(0), maxd1(1), maxd3(1), mind1(-1), mind3(-1) {};
void MassCenterAndVswim::initialize(const std::string& path, const Config& config) {
	//read config
	//maxVswim = 0.05;
	maxVswim = config.Read<double>("maxVswim");
	return;
}
void MassCenterAndVswim::takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action) {
	//ChangeMassCenterAble* itf = dynamic_cast<ChangeMassCenterAble*>(ag);
	//ChangeSwimVelAble* itfvswim = dynamic_cast<ChangeSwimVelAble*>(ag);

	auto itf = dynamic_pointer_cast<ChangeMassCenterAble>(ag);
	auto itfvswim = dynamic_pointer_cast<ChangeSwimVelAble>(ag);
	vectors3d currentmc(itf->getMassCenter());
	std::vector<double> newvswim(ag->agentnum, 0.0);

	const std::array<double, 8> arrd1{ 1, 0, -1, 0,	1, 0, -1, 0 };
	const std::array<double, 8> arrd3{ 0, 1, 0, -1, 0, 1, 0, -1 };
	const double vswimratio[8] = { 0,0,0,0,1,1,1,1 };
	//const std::array<double,18> arrd1{ -1, 0, 1, -1, 0, 1, -1, 0, 1,-1, 0, 1, -1, 0, 1, -1, 0, 1 };
	//const std::array<double,18> arrd3{ -1, -1, -1,0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	//const double vswimratio[18] = { 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 };
	//for (unsigned i = 0; i < ag->agentnum; i++) {
	//	currentmc[i][0] += arrd1[action[i]];
	//	currentmc[i][2] += arrd3[action[i]];
	//	currentmc[i][0] = std::min(currentmc[i][0], maxd1);
	//	currentmc[i][0] = std::max(currentmc[i][0], mind1);
	//	currentmc[i][2] = std::min(currentmc[i][2], maxd3);
	//	currentmc[i][2] = std::max(currentmc[i][2], mind3);
	//	newvswim[i] = maxVswim * vswimratio[action[i]];
	//	//if (action[i] == 18) {
	//	//	std::cout << arrd1[action[i]] << endl;
	//	//}
	//}

	for (unsigned i = 0; i < ag->agentnum; i++) {
		currentmc[i][0] = arrd1[action[i]];
		currentmc[i][2] = arrd3[action[i]];

		newvswim[i] = maxVswim * vswimratio[action[i]];

		itf->setMassCenter(currentmc);
		itfvswim->setSwimVel(newvswim);

		//itf->setMassCenter();
	}
}