#include "pch.h"
#include "SimuManager.h"
#include "Config.h"
#include "Fop.h"

using namespace std;
SimuManager::SimuManager(const std::string& configpath) :episode_record(std::vector<int>(0)),
reward_record(std::vector<double>(0)),
loss_record(std::vector<double>(0)),
qvalue_record(std::vector<double>(0)){
	Config config(configpath, "OVERALL");
	workingDir = config.Read<string>("workingDir", "./traj/");
	Fop::makeDir(workingDir);
	if (!Fop::copyFile(configpath, workingDir)) {
		cout << "Working direcotry is unavailable. Check config.txt" << endl;
		exit(0);
	}
	Fop::makeDir(workingDir + "/his/");
	cout << "Working directory is set at " + workingDir << endl;

	sensestep = config.Read<int>("sense step");
	learnstep = config.Read<int>("learn step");
	totalstep = config.Read<int>("total step");
	timestepsize = config.Read<double>("time step");
	dumpstep = config.Read<int>("dump step", totalstep);
	episodenum = config.Read<int>("episode num");
	saveinterval = config.Read<int>("save interval"); //episode interval of saving training
	ilearn = config.Read<bool>("ilearn");
	iload = config.Read<bool>("iload");
	return;
}

void SimuManager::recording(int episode, double reward, double loss, double qvalue) {
	episode_record.push_back(episode);
	reward_record.push_back(reward);
	loss_record.push_back(loss);
	qvalue_record.push_back(qvalue);
	return;
}
void SimuManager::writeRecord(const std::string& path) {
	std::ofstream outfile;
	outfile.open(path+"/trainrec.txt",ios::out);
	outfile << "episode\t" << "reward\t" << "zplossos\t" << "qvalue\t" << endl;
	//outfile << std::showpoint << std::left << std::fixed << std::setprecision(8);
	outfile.setf(ios::showpoint|ios::left|ios::scientific);
	outfile.precision(8);
	for (unsigned i = 0; i < episode_record.size(); i++) {
		outfile << std::setw(8) << episode_record[i] << " ";
		outfile << std::setw(16) << reward_record[i] << " ";
		outfile << std::setw(16) << loss_record[i] << " ";
		outfile << std::setw(16) << qvalue_record[i] << endl;
	}
	outfile.close();
}

void SimuManager::readRecord(const std::string& path) {
	episode_record.clear();
	reward_record.clear();
	loss_record.clear();
	qvalue_record.clear();
	std::ifstream infile;
	infile.open(path+"/trainrec.txt",ios::in);
	std::string line;
	getline(infile, line);//skip header
	while (infile.peek()!=EOF) {
		getline(infile, line); 
		stringstream ss;
		ss << line;
		int episode;
		double reward;
		double loss;
		double qvalue;
		ss >> episode >> reward >> loss >> qvalue;
		episode_record.push_back(int(episode));
		reward_record.push_back(reward);
		loss_record.push_back(loss);
		qvalue_record.push_back(qvalue);

		//cout << episode << " " << reward << " " << loss << " " << qvalue << endl;
	}
	
}

void SimuManager::writeRunRec(const std::string& path, int startepisode) {
	std::ofstream outfile;
	outfile.open(path + "/runrec.txt", ios::out);
	outfile << "episode\t" << "reward\t" << endl;
	//outfile << std::showpoint << std::left << std::fixed << std::setprecision(8);
	outfile.setf(ios::showpoint | ios::left | ios::scientific);
	outfile.precision(8);
	for (unsigned i = startepisode; i < episode_record.size(); i++) {
		outfile << std::setw(8) << episode_record[i] << " ";
		outfile << std::setw(16) << reward_record[i] << endl;
	}
	outfile.close();
}

//void SimuManager::saveTraining(const std::string& path) {
//}
//void SimuManager::loadTraining(const std::string& path) {
//}