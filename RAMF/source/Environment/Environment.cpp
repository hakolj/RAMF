#include "pch.h"
#include "Environment.h"
#include "TaylorGreenVortex.h"
#include "Quiescent.h"
#include "HomoIsoTurb.h"
#include "LoadedFlow.h"

using namespace std;
shared_ptr<Environment> EnvInitializer(const std::string& path) {
	//read config
	Config config(path, "ENV");
	shared_ptr<Environment> penv;
	string envname = config.Read<string>("type", string("_EMPTY"));
	if (envname == "TGV") {
		penv = make_shared<TaylorGreenVortex>(1.0,1.0);
	}
	else if (envname == "quiescent") {
		penv = make_shared<Quiescent>();
	}
	else if (envname == "HIT") {
		penv = HomoIsoTurb::makeInstance(config);
	}

	else if (envname == "LoadedFlow") {
		penv = LoadedFlow::makeInstance(config);
	}


	else if (envname == "_EMPTY") {
		cout << "Env type is not defined. Check config file." << endl;
		exit(0);
	}
	else {
		cout << "Undefined Env name. Check config file.";
		exit(0);
	}
	penv->initialize(path, config);
	cout << "Environment: " << envname << " created." << endl;
	return penv;
}