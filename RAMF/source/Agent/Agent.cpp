#include "pch.h"
#include "Agent.h"
#include "InertialessSwimmer.h"
#include "Config.h"


using namespace std;
shared_ptr<Agent> AgentInitializer(const std::string& path) {
	//read config
	Config config(path, "AGENT");
	shared_ptr<Agent> pagent;
	cout << "pagent use count = " << pagent.use_count() << endl;

	string agentname = config.Read("type", string("_EMPTY"));
	unsigned agentnum = config.Read("agentnum", 1);
	if (agentname == "InertialessSwimmer") {
		pagent = make_shared<InertialessSwimmer>(agentnum);
		cout << "pagent use count = " << pagent.use_count() << endl;
	}

	else if (agentname == "_EMPTY") {
		cout << "Agent type is not defined. Check config file." << endl;
		exit(0);
	}
	else {
		cout << "Undefined actor name. Check config file.";
		exit(0);
	}
	pagent->initialize(path, config);
	cout << "Agent: " << agentname << " created." << endl;
	return pagent;
}

//void Agent::InitSenseStepCount(const Config& config) {
//	senseStep = config.Read<int>("sense step", 1);
//}

Agent::Agent(unsigned agentnum) :agentnum(agentnum) {};
//bool Agent::ReadyToSense(int idx) {
//	return (senseStepCount[idx] % senseStep) == 0;
//}