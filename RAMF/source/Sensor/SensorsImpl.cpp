#include "pch.h"
#include "../Sensor/SensorsImpl.h"

//#include "Fop.h"

using namespace std;

void RelaVelSensorDiscret::getState(Agent* agent, std::vector<std::vector<double>>& newstate) {
	RelaVelSensor::getState(agent, newstate);
	discretize(newstate);
	//std::cout << newstate << std::endl;

}
void RelaVelSensorDiscret::initialize(const std::string& path, const Config& config) {
	threshold = std::vector<std::vector<double>>(dim(), std::vector<double>(0));
	//std::string strthre = config.Read<std::string>("threshold1");
	std::stringstream ss;
	for (int i = 0; i < dim(); i++) {
		ss.str("");
		ss << i;
		std::string strthre = config.Read<std::string>("threshold" + ss.str());
		threshold[i] = Fop::loadvec1d<double>(strthre);

	}
	//std::cout << threshold << std::endl;
};

void RelaVelSensorDiscret::discretize(std::vector<std::vector<double>>& state) {
	
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < dim(); j++) {
			int n = 0;
			while (n<threshold[j].size()) {
				if (state[i][j] < threshold[j][n++]) {
					n--;
					break;					
				}
				
			}
			state[i][j] = n;
			if (n == 3) {
				std::cout << "?" << std::endl;
			}
		}
	}

}