#pragma once
#ifndef EXTRAANGVEL_H
#define EXTRAANGVEL_H



#include "Actor.h"


class ExtraAngVel : public Actor {

public:
	double angvel;
	ExtraAngVel();
	virtual void takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive);
	virtual void initialize(const std::string& path, const Config& config);
	inline virtual int num() const { return 3; }
};

class ExtraAngVel3D : public ExtraAngVel {

public:
	ExtraAngVel3D(){};
	virtual void takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive);
	virtual void initialize(const std::string& path, const Config& config) { ExtraAngVel::initialize(path, config); };
	inline virtual int num() const { return 9; }
};



ExtraAngVel::ExtraAngVel():angvel(0){}


void ExtraAngVel::initialize(const std::string& path, const Config& config) {
	angvel = config.Read<double>("AngVel");
}

void ExtraAngVel::takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive) {
	auto itf = std::dynamic_pointer_cast<ChangeSwimVelAble>(ag);
	if (inaive) {
		for (unsigned i = 0; i < ag->agentnum; i++) {			
				itf->swimAngVel()[i][1] = 0.0;		
		}

	}
	else {
		for (unsigned i = 0; i < ag->agentnum; i++) {
			if (action[i] == 0) {
				itf->swimAngVel()[i][1] = angvel;
			}
			else if (action[i] == 1) {
				itf->swimAngVel()[i][1] = 0.0;
			}
			else {
				itf->swimAngVel()[i][1] = -angvel;
			}
		}
	}

}


void ExtraAngVel3D::takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive) {
	auto itf = std::dynamic_pointer_cast<ChangeSwimVelAble>(ag);
	if (inaive) {
		for (unsigned i = 0; i < ag->agentnum; i++) {
			itf->swimAngVel()[i][0] = 0.0;
			itf->swimAngVel()[i][1] = 0.0;
		}
	}
	else {
		for (unsigned i = 0; i < ag->agentnum; i++) {
			int wqidx = action[i] % 3;
			int wpidx = (action[i] - wqidx) / 3;


			if (wqidx == 0) {
				itf->swimAngVel()[i][1] = angvel;
			}
			else if (wqidx == 1) {
				itf->swimAngVel()[i][1] = 0.0;
			}
			else {
				itf->swimAngVel()[i][1] = -angvel;
			}

			if (wpidx == 0) {
				itf->swimAngVel()[i][0] = angvel;
			}
			else if (wpidx == 1) {
				itf->swimAngVel()[i][0] = 0.0;
			}
			else {
				itf->swimAngVel()[i][0] = -angvel;
			}
		}
	}

}

#endif // !EXTRAANGVEL_H