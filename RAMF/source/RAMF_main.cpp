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
#include "Fop.h"
#include "TorchAPI.h"
#include "Timer.h"
#include "../MachineLearning/QLearning.h"
#include "../MachineLearning/DDQL.h"

using namespace Eigen;
using namespace std;
using namespace rd;



int main()
{
	bool itest = false;

	if (itest) {
		performanceTest();
		return 0;
	}


	bool ilearn = true;
	omp_set_num_threads(6);

#pragma omp parallel
	std::cout << "omp thread: " << omp_get_thread_num() << "\n" << endl;

	


	//clock_t tic, toc;
	//timespec tic_1, tic_2;
	//Initialization
	//at::init_num_threads();
#ifdef __linux__
	std::string configpath = "/mnt/disk2/qjr/ramf/work/config.txt";
#else
	std::string configpath = "D:/Homework/Y4-1/2021-09-07-RAMF_cmake/work/config.txt";	
#endif
	SimuManager smanager = SimuManager(configpath);
	shared_ptr<Environment> env = EnvInitializer(configpath);
	shared_ptr<Agent> agent = AgentInitializer(configpath);
	shared_ptr<Task> task = TaskInitializer(configpath);
	shared_ptr<Sensor> sensor = SensorInitializer(configpath);
	shared_ptr<Actor> actor = ActorInitializer(configpath);
	
	agent->setEnv(env);
	std::vector<Trajectory> trajs(min(int(agent->agentnum), 50));
	auto agentKine = dynamic_pointer_cast<GetTransRotAble>(agent); //for recording trajectories


	//ag.setMotility(0.05, 3);
	//ag.setShape(0, 3);
	int dimState = sensor->dim();
	int numAction = actor->num();
	vector<double> zero(dimState, 0.0);
	vector<vector<double>> state(agent->agentnum, zero);
	vector<vector<double>> state_new(agent->agentnum, zero);
	vector<double> reward(agent->agentnum, 0);
	vector<int> action(agent->agentnum);
	vector<int> isterminated(agent->agentnum, 0);

	double explr_eps = smanager.ilearn ? 0.05 : 0.0;  //exploration rate

	shared_ptr<MachineLearning> ml = MLInitializer(configpath, sensor, actor);
	//MachineLearning* ml;
	//DeepQLearning ddql(dimState, 30, numAction);	
	//QLearning ql(dimState, numAction, Config());

	//ml = &ql;
	//ml->initialize(configpath, Config());

	Timer timer;

	Memory mem(20000, dimState, 1);

	int startepisode = 0;
	if (smanager.iload) {
		mem.readMemory(smanager.workingDir);
		ml->model_loader(smanager.workingDir);
		smanager.readRecord(smanager.workingDir);
		startepisode = smanager.episode_record.back()+1;
		

	}
	else {
		//if (ilearn) ddql.clearRec(smanager.workingDir.c_str());
	}
	
	//----------------------- main roop ---------------------------------------//
	//ml.copyParam();
	for (int episode = startepisode; episode < smanager.episodenum+startepisode; episode++) {

		for (auto& traj : trajs) {
			traj.reset();
		}
		ml->setNewEp(episode);
		env->reset();
		agent->reset();
		agent->getInfo();
		agent->useInfo();//ag.convertFrame();
		task->reset(agent.get());
		sensor->reset(task);
		sensor->getState(agent.get(), state); //initial state

		agent->dump(smanager.workingDir.c_str(), 0);

		timer.Tic();


		int statestep = 0;
		for (int i = 0; i < smanager.totalstep; i++) {
			if (i % smanager.sensestep == 0) {
				action = ml->decideAction(state, explr_eps);
				//for (int j = 0; j < action.size(); j++) {
				//	action[j] = 1;
				//}
				actor->takeAction(agent, action);
			}

			agent->update(smanager.timestepsize);
			//agent->UpdateSenseStepCount();
			agent->BoundaryCondition(env);
			env->update(smanager.timestepsize);
			agent->getInfo();
			agent->useInfo();//ag.convertFrame();

			if ((i + 1) % smanager.sensestep == 0) {
				sensor->getState(agent.get(), state_new);
				task->getReward(agent.get(), reward);
				mem.storeMemory(agent->agentnum, state, action, reward, state_new, isterminated);
				//state = state_new;
				state.swap(state_new);
				statestep++;
				if (ilearn) {
					if ((statestep % smanager.learnstep == 0) || (i == smanager.totalstep)) {
						ml->train(&mem);
					}
				}
			}

			if ((episode % smanager.saveinterval == 0) || (episode == smanager.episodenum + startepisode-1)){
				for (unsigned j = 0; j < trajs.size(); j++) {
					trajs[j].recordp3(agentKine->getP3()[j]);
					trajs[j].recordpos(agentKine->getPos()[j]);
				}

				// dump agent info
				if ((i + 1) % smanager.dumpstep == 0) {
					agent->dump(smanager.workingDir.c_str(), i+1);
				}
			}


		}

		//end episode
		vector<double> totalreward = task->getTotalReward(agent.get());
		double meantotalreward = 0;
		for (double trwd : totalreward) {
			meantotalreward += trwd;
		}
		meantotalreward /= (smanager.totalstep * smanager.timestepsize);
		meantotalreward /= agent->agentnum;


		timer.Toc();
		std::cout << "Episode " << episode << ": total reward = " << meantotalreward << ". ";
		std::cout << "time cost: " << timer.elaspe() << std::endl;


		//smanager.recording(episode, meantotalreward, ddql.loss_record, ddql.qvalue_record);
		smanager.recording(episode, meantotalreward, 0, 0);

		if ((episode % smanager.saveinterval == 0) || (episode == smanager.episodenum + startepisode - 1)) {
			if (ilearn) {
				mem.writeMemory(smanager.workingDir);
				ml->model_saver(smanager.workingDir);
				smanager.writeRecord(smanager.workingDir);
				ml->recParam(smanager.workingDir.c_str());

				char idx[9];
				sprintf(idx, "%.7i", episode);
				string idxstr = string(idx);
				
				ml->model_saver(smanager.workingDir + "/his/", idxstr);
			}
			else {
				smanager.writeRunRec(smanager.workingDir, startepisode);
			}

		}

		
	}
	

	for (unsigned i = 0; i < trajs.size(); i++) {
		trajs[i].writeTraj(smanager.workingDir + "/" + std::to_string(i) + ".txt");
	}


	return 0;

}


