#pragma once
#ifndef SIMUMANAGER_H
#define SIMUMANAGER_H

#include "vectypedef.h"
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include "AgentInterface.h"
using std::endl;
class SimuManager
{
public:
	std::vector<int> episode_record;
	std::vector<double> reward_record;
	std::vector<double> loss_record;
	std::vector<double> qvalue_record;
	std::string workingDir;

	//SimuManager():episode_record(std::vector<int>(0)),
	//	reward_record(std::vector<double>(0)),
	//	loss_record(std::vector<double>(0)),
	//	qvalue_record(std::vector<double>(0)) {}
	SimuManager() = delete;

	SimuManager(const std::string& configpath);

	void recording(int episode, double reward, double loss, double qvalue);
	void writeRecord(const std::string& path);
	void readRecord(const std::string& path);

	//used for writing record of running (without training)
	void writeRunRec(const std::string& path, int startepisode);
	//void saveTraining(const std::string& path);
	//void loadTraining(const std::string& path);

	int sensestep;
	int learnstep;
	int totalstep;
	int dumpstep;
	double timestepsize;
	int episodenum;
	int saveinterval; //episode interval of saving training
	bool ilearn;
	bool iload;

};


class Trajectory {
public:
	vectors3d pos, p3;
	Trajectory() :Trajectory(100) {}
	Trajectory(int init_capacity) :pos(init_capacity), p3(init_capacity) {
	}
	Trajectory(const Trajectory& traj) {
		pos = traj.pos;
		p3 = traj.p3;
		return;
	}
	//void setAgent(std::shared_ptr<Agent> agent) {
	//	_agentptr = std::dynamic_pointer_cast<GetTransRotAble>(agent);
	//	return;
	//}
	void reset() {
		pos.clear();
		p3.clear();
		return;
	}
	void recordpos(vec3d newpos) {
		pos.push_back(newpos);
		return;
	}

	void recordp3(vec3d newp3) {
		p3.push_back(newp3);
		return;
	}
	void writeTraj(std::string dir) {
		std::ofstream outfile;
		outfile.open(dir);
		outfile << "xpos\t" << "ypos\t" << "zpos\t" << "p31\t" << "p32\t" << "p33\t" << endl;
		outfile << std::showpoint << std::setprecision(8) << std::left;
		for (unsigned i = 0; i < pos.size(); i++) {
			outfile << std::setw(16) << pos[i][0] << "\t" << pos[i][1] << "\t" << pos[i][2] << "\t"
				<< p3[i][0] << "\t" << p3[i][1] << "\t" << p3[i][2] << "\t" << endl;
		}
		outfile.close();
	}
};

#endif // !SIMUMANAGER_H
