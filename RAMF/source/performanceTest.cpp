#include "pch.h"
#include "performanceTest.h"
#include "SimuManager.h"
#include "Sensor.h"
#include "Timer.h"
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

	//env->reset();
	//env->update(1.0);


	shared_ptr<LoadedFlow> flow = dynamic_pointer_cast<LoadedFlow>(env);
	//cout << flow->datapool.upool[0](1, 2, 1) << endl;

	string path = "D:\\Homework\\Y4-1\\2021-09-07-RAMF_cmake\\work\\outputgrad\\";
	flow->reset();

	const Mesh* ms = &flow->u->ms;
	for (int step = 0; step < 100; step++) {
		ofstream os;
		os.open(path+"prof"+to_string(step)+ ".txt", ios::out | ios::trunc);

		for (int j = 1; j <= flow->u->ms.Ny; j++) {
			double sum = 0;
			for (int k = 1; k < ms->Nz; k++) {
				for (int i = 1; i < ms->Nx; i++) {
					sum += flow->u->operator()(i, j, k);
				}

			}

			os << ms->y(j) << " " << sum/(ms->Nz-1)/(ms->Nx-1) << endl;
		}

		os.close();
		flow->update(1);
	}

	//flow->update(1.0);
	//flow->update(1.0);
	////flow->makeGradient();
	//flow->u->FileIO(path.c_str(), "u", 'w');
	//flow->v->FileIO(path.c_str(), "v", 'w');
	//flow->w->FileIO(path.c_str(), "w", 'w');

	//flow->dudx.FileIO(path.c_str(), "dudx", 'w');
	//flow->dudy.FileIO(path.c_str(), "dudy", 'w');
	//flow->dudz.FileIO(path.c_str(), "dudz", 'w');

	//flow->dvdx.FileIO(path.c_str(), "dvdx", 'w');
	//flow->dvdy.FileIO(path.c_str(), "dvdy", 'w');
	//flow->dvdz.FileIO(path.c_str(), "dvdz", 'w');

	//flow->dwdx.FileIO(path.c_str(), "dwdx", 'w');
	//flow->dwdy.FileIO(path.c_str(), "dwdy", 'w');
	//flow->dwdz.FileIO(path.c_str(), "dwdz", 'w');



	vec3d uf;
	uf[0] = 0;
	uf[1] = 0;
	uf[2] = 0;
	vec3d uf2;
	uf2[0] = 0;
	uf2[1] = 0;
	uf2[2] = 0;

	Timer timer;
	//timer.Tic();
	//for (int i = 0; i < 1000000; i++) {
	//	vec3d temppos;
	//	temppos = Matrix<double, 3, 1>::Random();
	//	temppos[0] += 1;
	//	temppos[1] += 1;
	//	temppos[2] += 1;
	//	hit->interpolater.interp3d_old(temppos, *(hit->u), *hit->v, *hit->w, uf);

	//}
	//timer.Toc();
	cout <<"old interp" << timer.elaspe() << endl;
	double dx = 2 * M_PI / 96;




	//timer.Tic();
	//for (int i = 0; i < 1000000; i++) {
	//	vec3d temppos;
	//	temppos = Matrix<double, 3, 1>::Random();
	//	temppos[0] += 1;
	//	temppos[1] += 1;
	//	temppos[2] += 1;

	//
	//	//hit->interpolater.interp3d_old(temppos, *(hit->u), *hit->v, *hit->w, uf);

	//	flow->interpolater.interp3d(temppos, *(hit->u), *hit->v, *hit->w, uf2, FieldStoreType::CEC);
	//	flow->interpolater.interp3d_old(temppos, *(hit->u), *hit->v, *hit->w, uf);
	//	if ((uf2 - uf).maxCoeff() > 1e-14) { 
	//		cout << uf2 - uf << endl; 
	//		//hit->interpolater.interp3d_old(temppos, *(hit->u), *hit->v, *hit->w, uf);
	//		//hit->interpolater.interp3d(temppos, *(hit->u), *hit->v, *hit->w, uf2, FieldStoreType::CCC);
	//	}

	//}
	//timer.Toc();
	//cout << "new interp" << timer.elaspe() << endl;
	


	//cout << upool[0](1, 2, 1) << endl;
//cout << upool[1](1, 2, 1) << endl;
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

