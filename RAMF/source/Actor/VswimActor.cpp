#include "pch.h"
#include "VswimActor.h"
#include <string>
#include "Fop.h"

using namespace std;


void VswimActor::initialize(const std::string& path, const Config& config){
	
	std::string vswimstr = config.Read<string>("Vswim ratio");
	vswimRatio = Fop::loadvec1d<double>(vswimstr);
	_actionNum = vswimRatio.size();
	maxVswim = config.Read<double>("Max Vswim");

}

void VswimActor::takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive) {
	auto itfvswim = dynamic_pointer_cast<ChangeSwimVelAble>(ag);
	//std::vector<double> newvswim(ag->agentnum, 0.0);
	if (inaive) {
		for (unsigned i = 0; i < ag->agentnum; i++) {
			itfvswim->swimVel()[i] = maxVswim;
		}
	}
	else {
		for (unsigned i = 0; i < ag->agentnum; i++) {
			itfvswim->swimVel()[i] = maxVswim * vswimRatio[action[i]];;
		}
	}

}