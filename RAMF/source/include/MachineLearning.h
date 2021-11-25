#pragma once
#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H
#include "Memory.h"
#include "Config.h"
#include "Sensor.h"
#include "Actor.h"

class MachineLearning {
public:
	//virtual std::vector<int> decideAction(const std::vector<std::vector<double>>& state, double epsilon) = 0;
	virtual std::vector<int> decideAction(const std::vector<std::vector<double>>& state, bool iexplore) = 0;
	virtual void train(Memory* memory) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual void setNewEp(int ep) {}; // do sth for a new episode/epoch
	virtual void model_saver(const std::string& path, const std::string& suffix) = 0;
	virtual void model_saver(const std::string& path) { model_saver(path, ""); };

	virtual void model_loader(const std::string& path) = 0;
	virtual void recParam(const char* path) = 0;
};

std::shared_ptr<MachineLearning> MLInitializer(const std::string& path, const std::shared_ptr<const Sensor> psensor, const std::shared_ptr<const Actor> pactor);


#endif //MACHINELEARNING_H