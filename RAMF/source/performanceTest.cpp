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
#include "..//source/MachineLearning/QLearning.h"

using namespace std;
using namespace Eigen;

static double timecost(clock_t tic, clock_t toc) {
	return  (double(toc) - double(tic)) / double(CLOCKS_PER_SEC);
}

extern void performanceTest() {




	//torch::Tensor tensor = torch::rand({ 9,9 });
	//std::cout << tensor << std::endl;


	std::string configpath = "D:\\Homework\\Y4-1\\2021-09-07-RAMF_cmake\\work\\config.txt";

	shared_ptr<Environment> env = EnvInitializer(configpath);
	shared_ptr<Agent> agent = AgentInitializer(configpath);
	shared_ptr<Task> task = TaskInitializer(configpath);
	shared_ptr<Sensor> sensor = SensorInitializer(configpath);
	shared_ptr<Actor> actor = ActorInitializer(configpath);
	SimuManager smanager = SimuManager(configpath, env, agent, task, actor, sensor);

	shared_ptr<MachineLearning> ml = MLInitializer(configpath, sensor, actor);
	
	shared_ptr<QLearning> ql = dynamic_pointer_cast<QLearning>(ml);
	//ml->setNewEp(0);
	//vector<double> result = ql->toState(9);
	//for (int i : result)	cout << i << endl;

 //   cout << ql->toIndex(result) << endl;

	//env->reset();
	//env->update(1.0);


	//shared_ptr<LoadedFlow> flow = dynamic_pointer_cast<LoadedFlow>(env);
	////cout << flow->datapool.upool[0](1, 2, 1) << endl;

	//string path = "D:\\Homework\\Y4-1\\2021-09-07-RAMF_cmake\\work\\outputgrad\\";
	//flow->reset();

	//const Mesh* ms = &flow->u->ms;
	//for (int step = 0; step < 100; step++) {
	//	ofstream os;
	//	os.open(path+"prof"+to_string(step)+ ".txt", ios::out | ios::trunc);

	//	for (int j = 1; j <= flow->u->ms.Ny; j++) {
	//		double sum = 0;
	//		for (int k = 1; k < ms->Nz; k++) {
	//			for (int i = 1; i < ms->Nx; i++) {
	//				sum += flow->u->operator()(i, j, k);
	//			}

	//		}

	//		os << ms->y(j) << " " << sum/(ms->Nz-1)/(ms->Nx-1) << endl;
	//	}

	//	os.close();
	//	flow->update(1);
	//}





	//vec3d uf;
	//uf[0] = 0;
	//uf[1] = 0;
	//uf[2] = 0;
	//vec3d uf2;
	//uf2[0] = 0;
	//uf2[1] = 0;
	//uf2[2] = 0;

	//Timer timer;
	////timer.Tic();
	////for (int i = 0; i < 1000000; i++) {
	////	vec3d temppos;
	////	temppos = Matrix<double, 3, 1>::Random();
	////	temppos[0] += 1;
	////	temppos[1] += 1;
	////	temppos[2] += 1;
	////	hit->interpolater.interp3d_old(temppos, *(hit->u), *hit->v, *hit->w, uf);

	////}
	////timer.Toc();
	//cout <<"old interp" << timer.elaspe() << endl;
	//double dx = 2 * M_PI / 96;




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
	



	return;
}

