#include "pch.h"
#include "MachineLearning.h"

#include "QLearning.h"
#include "DDQL.h"
using namespace std;


std::shared_ptr<MachineLearning> MLInitializer(const std::string& path, const std::shared_ptr<const Sensor> sensor, const std::shared_ptr<const Actor> actor) {

	Config config(path, "ML");
	shared_ptr<MachineLearning> pml;
	string mlname = config.Read<string>("type", string("_EMPTY"));
	if (mlname == "QLearning") {
		pml = make_shared<QLearning>(sensor->dim(), actor->num(),config);
	}
	else if (mlname == "DDQL") {

		pml = DoubleDeepQLearning::makeInstance(config, sensor->dim(), actor->num());
	}
	
	pml->initialize(path, config);
	return pml;
}