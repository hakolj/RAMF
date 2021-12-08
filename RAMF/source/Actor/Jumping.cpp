#include "pch.h"
#include "Jumping.h"
#include "AgentInterface.h"


Jumping::Jumping() {}
void Jumping::takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action) {
	auto itf = std::dynamic_pointer_cast<ChangeJumpVelAble>(ag);

	for (int i = 0; i < ag->agentnum; i++) {
		itf->jumpTime()[i] = action[i] == 0 ? -1 : 1;
	}
	return;
}
void Jumping::initialize(const std::string& path, const Config& config) {

}