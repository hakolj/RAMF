#pragma once
#ifndef SENSORSIMPL_H
#define SENSORSIMPL_H


#include "Sensor.h"
#include "Fop.h"
// take p3 dot (S dot p3) as state.
class PSPSensor :
	public Sensor {
public:

	PSPSensor(){}
	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
		auto itf = dynamic_cast<GetFlowInfoAble*>(agent);
		//auto itf = std::dynamic_pointer_cast<GetFlowInfoAble>(agent);
		/*vectors3d gradupf, gradvpf, gradwpf, temp;*/
		//aa->getFlowInfo(tekmp, gradupf, gradvpf, gradwpf);

		vectors3d gradwpf(itf->getGradW());
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = gradwpf[i][2];
		}
		return;
	}
	virtual void initialize(const std::string& path, const Config& config) {};
	virtual inline int dim() const { return 1; }
};

class PSQSensor :
	public Sensor {
public:

	PSQSensor(){}
	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
		GetFlowInfoAble* itf = dynamic_cast<GetFlowInfoAble*>(agent);
		//auto itf = std::dynamic_pointer_cast<GetFlowInfoAble>(agent);
		/*vectors3d gradupf, gradvpf, gradwpf, temp;*/
		//aa->getFlowInfo(tekmp, gradupf, gradvpf, gradwpf);
		vectors3d gradupf(itf->getGradU());
		vectors3d gradwpf(itf->getGradW());
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = 0.5 * (gradupf[i][2] + gradwpf[i][0]);
		}
		return;
	}
	virtual void initialize(const std::string& path, const Config& config) {};
	virtual inline int dim() const { return 1; }
};

class RelaPosOrient2DSensor :
	public Sensor {
public:
	vec3d target;
	RelaPosOrient2DSensor(): target(vec3d::Zero()) {};
private:
	bool _setTargetFromTask = false;
public:
	virtual inline int dim() const { return 3; }
	void setTarget(double x, double y) {
		target[0] = x;
		target[1] = y;
		return;
	};
	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
		auto itf = dynamic_cast<GetTransRotAble*>(agent);
		//auto itf = std::dynamic_pointer_cast<GetTransRotAble>(agent);
		vectors3d pos(itf->getPos());
		vectors3d p3(itf->getP3());
		for (unsigned i = 0; i < agent->agentnum; i++) {
			newstate[i][0] = pos[i][0] - target[0];
			newstate[i][1] = pos[i][1] - target[1];
			newstate[i][2] = atan2(p3[i][1], p3[i][0]);
			//newstate[i][0] /= 10;
			//newstate[i][1] /= 10;
			//newstate[i][2] /= 10;
		}

	}
	virtual void initialize(const std::string& path, const Config& config) {
		_setTargetFromTask = config.Read<bool>("Target From Task", false);
		if (!_setTargetFromTask) {
			// from config.txt
			double x = config.Read<double>("target x");
			double y = config.Read<double>("target y");
			setTarget(x, y);
		}

		return;
	};

	void reset(std::shared_ptr<Task> task) {
		if (_setTargetFromTask)	
			target = std::dynamic_pointer_cast<GetTargetable>(task)->getTarget();
	}

};

//Sensing relative velocity (lateral, not axial)
class RelaVelSensor :
	public Sensor {
public:

	RelaVelSensor(){}
	virtual inline int dim() const { return 2; }
	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
		auto itfflow = dynamic_cast<GetFlowInfoAble*>(agent);
		auto itfvel = dynamic_cast<GetTransRotAble*>(agent);
		//auto itf = std::dynamic_pointer_cast<GetFlowInfoAble>(agent);
		/*vectors3d gradupf, gradvpf, gradwpf, temp;*/
		//aa->getFlowInfo(tekmp, gradupf, gradvpf, gradwpf);
		//vectors3d gradupf(itf->getGradU());
		//vectors3d gradwpf(itf->getGradW());
		for (unsigned i = 0; i < agent->agentnum; i++) {
			vec3d dvel = itfflow->getFluidVel(false)[i] - itfvel->getVel()[i];

			vec3d p1 = itfvel->getP1()[i];
			vec3d p2 = itfvel->getP2()[i];

			double dv1 = dvel(0) * p1(0) + dvel(1) * p1(1) + dvel(2) * p1(2);
			double dv2 = dvel(0) * p2(0) + dvel(1) * p2(1) + dvel(2) * p2(2);
			newstate[i][0] = dv1;
			newstate[i][1] = dv2;
		}
		return;
	}
	virtual void initialize(const std::string& path, const Config& config) {
		std::cout << " RelaVelSensor initialized" << std::endl;
	};
};

class RelaVelP1Sensor :
	public Sensor {
public:

	RelaVelP1Sensor() {}
	virtual inline int dim() const { return 1; }
	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
		auto itfflow = dynamic_cast<GetFlowInfoAble*>(agent);
		auto itfvel = dynamic_cast<GetTransRotAble*>(agent);

		for (unsigned i = 0; i < agent->agentnum; i++) {
			vec3d dvel = itfflow->getFluidVel(false)[i] - itfvel->getVel()[i];

			vec3d p1 = itfvel->getP1()[i];
			double dv1 = dvel(0) * p1(0) + dvel(1) * p1(1) + dvel(2) * p1(2);

			newstate[i][0] = dv1;

		}
		return;
	}
	virtual void initialize(const std::string& path, const Config& config) {
		std::cout << " RelaVelSensor initialized" << std::endl;
	};
};

class RelaVelSensorDiscret : 
	public RelaVelSensor
{
private:
	std::vector<std::vector<double>> threshold;
public:	
	void getState(Agent* agent, std::vector<std::vector<double>>& newstate);
	void initialize(const std::string& path, const Config& config);

private:
	void discretize(std::vector<std::vector<double>>& state);

};

#endif // !SENSORSIMPL_H