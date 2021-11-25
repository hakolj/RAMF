#include "pch.h"
#include "Actor.h"
#include "MassCenterAndVswim.h"
#include "ExtraAngVel.h"

using namespace std;
shared_ptr<Actor> ActorInitializer(const std::string& path) {
	//read config
	Config config(path, "ACTOR");
	shared_ptr<Actor> pactor;
	string actorname = config.Read("type", string("_EMPTY"));
	if (actorname == "MassCenterVswim") {
		pactor = make_shared<MassCenterAndVswim>();
	}
	else if (actorname == "ExtraAngVel") {
		pactor = make_shared<ExtraAngVel>();
	}
	else if (actorname == "ExtraAngVel3D") {
		pactor = make_shared<ExtraAngVel3D>();
	}
	else if (actorname == "_EMPTY") {
		cout << "Actor type is not defined. Check config file." << endl;
		exit(0);
	}
	else {
		cout << "Undefined actor name. Check config file.";
		exit(0);
	}
	pactor->initialize(path,config);
	cout << "Actor: " << actorname << " created." << endl;
	return pactor;
}

