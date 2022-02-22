#include "pch.h"
#include "Task.h"
#include "NavigationPoint.h"
#include "NavigationUpward.h"
#include "DistanceFromBoundary.h"

using namespace std;
shared_ptr<Task> TaskInitializer(const std::string& path) {
	//read config
	Config config(path, "TASK");
	shared_ptr<Task> ptask;
	string taskname = config.Read<string>("type",string("_EMPTY"));
	if (taskname == "NavigationPoint") {
		ptask = make_shared<NavigationPoint>();
	}
	else if (taskname == "NavigationUpward") {
		ptask = make_shared<NavigationUpward>();
	}
	else if (taskname == "DistanceFromBoundary") {
		ptask = make_shared<DistanceFromBoundary>();
	}
	else if (taskname == "_EMPTY") {
		cout << "Task type is not defined. Check config file." << endl;
		exit(0);
	}
	else {
		cout << "Undefined Task name. Check config file.";
		exit(0);
	}
	ptask->initialize(path, config);
	cout << "Task: " << taskname << " created." << endl;
	return ptask;
}
