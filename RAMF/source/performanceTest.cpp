#include "pch.h"
#include "performanceTest.h"
#include "SimuManager.h"
#include "Sensor.h"

#include "Actor.h"
#include "Random.h"
#include "Task.h"
#include "Environment.h"
#include "Memory.h"
#include "MachineLearning.h"
#include "Config.h"
#include "Agent.h"
#include "../Agent/InertialessSwimmer.h"
#include "..//source/Environment/Interpolation.h"
#include "..//source/Environment/Scalar.h"
#include "..//source/Environment/Geometry.h"
#include "..//source/Environment/HomoIsoTurb.h"
#include "..//source/Sensor/SensorsImpl.h"
#include "..//source/Environment/FlowFieldDataPool.h"
#include "..//source/Environment/LoadedFlow.h"

using namespace std;
using namespace Eigen;

static double timecost(clock_t tic, clock_t toc) {
	return  (double(toc) - double(tic)) / double(CLOCKS_PER_SEC);
}

extern void performanceTest() {





	//torch::Tensor tensor = torch::rand({ 9,9 });
	//std::cout << tensor << std::endl;


	std::string configpath = "D:\\Homework\\Y4-1\\2021-09-07-RAMF_cmake\\work\\config.txt";

	//SimuManager smanager = SimuManager(configpath);
	//shared_ptr<Agent> agent = AgentInitializer(configpath);
	shared_ptr<Environment> env = EnvInitializer(configpath);
	//shared_ptr<Task> task = TaskInitializer(configpath);
	//shared_ptr<Sensor> sensor = SensorInitializer(configpath);
	//shared_ptr<Actor> actor = ActorInitializer(configpath);

	env->reset();
	env->update(1.0);
	//int dimState = sensor->dim();
	//int numAction = actor->num();
	//MachineLearning* ml;
	//DeepQLearning ddql(dimState, 30, numAction);	
	//QLearning ql(dimState, numAction, { 3,3 });


	//agent->setEnv(env);
	//agent->reset();
	//agent->getInfo();
	//agent->useInfo();//ag.convertFrame();
	//sensor->getState(agent.get(), state); //initial state

	//vector<double> reward(agent->agentnum, 0);
	//vector<int> action(agent->agentnum);
	//QLearning ql(3, 3, { 3,2 ,8});
	//ql.initialize();
	//action = ql.decideAction(state, 0.0);

	//string path = "../../data/debugQL/";

	//state[0] = { 2,1,5 };
	//int index = ql.toIndex(state[0]);
	//cout << index << endl;

	//vector<double> restate = ql.toState(index);
	//cout << restate << endl;

	// old test below


	//Sensor* p1 = &s1;
	//Sensor* p2 = &s2;
	//Config config;
	//p1->initialize("123",config);
	//p2->initialize("123",config);


	//std::string configpath = "./config.txt";
	//shared_ptr<Environment> env = EnvInitializer(configpath);

	//auto hit = dynamic_pointer_cast<HomoIsoTurb>(env);
	//vec3d uf;
	//hit->interpolater.interp3d({ 4,5,6 }, hit->dudx, hit->dudy, hit->dudz, uf);
	//cout << uf << endl;
	//cout << endl;

	//hit->interpolater.interp3d({ 4,5,6 }, hit->dvdx, hit->dvdy, hit->dvdz, uf);
	//cout << uf << endl;
	//cout << endl;

	//hit->interpolater.interp3d({ 4,5,6 }, hit->dwdx, hit->dwdy, hit->dwdz, uf);
	//cout << uf << endl;
	//cout << endl;
	return;
}
