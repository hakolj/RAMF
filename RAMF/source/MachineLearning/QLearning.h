#ifndef QLEARNING_H
#define QLEARNING_H

#include "MachineLearning.h"

class QLearning :public MachineLearning {
public:
	int numAction, numState;
	int dimState; //dimension of states
	//int totalState; // number of all possible states

	std::vector<std::vector<double>> threshold;
	double loss_record;
	double qvalue_record;
	const double GAMMA = 0.99;
	std::vector<std::vector<double>> qtable;
	//double eps; // exploration rate
	double lr; //learning rate
	std::vector<int> levels; //how many levels for each state dimension

private:
	int _episode; // current episode
	double _lr_initial; // initial lr
	int _sigma; // decreasing coeff;
	int _episode_0; // exploration rate decreasing coeff;
	double _epsi; // exploration rate
	int _epsi_initial; // initial exploration rate

public:
	//QLearning(int dimState, int numAction);
	QLearning(int dimState, int numAction, const Config& config);
	//QLearning(int dimState, int numAction, const std::vector<int>& levels);
	virtual void train(Memory* memory);
	virtual std::vector<int> decideAction(const std::vector<std::vector<double>>& state, bool iexplore);
	virtual void initialize(const std::string& path, const Config& config);
	void setNewEp(int ep) override;

	virtual void model_saver(const std::string& path, const std::string& suffix);
	virtual void model_saver(const std::string& path) { model_saver(path, ""); };

	virtual void model_loader(const std::string& path);
	virtual void recParam(const char* path);

private:
	void discretize(std::vector<std::vector<double>>& state); //discretize the state and replace it
	void discretize(const std::vector<std::vector<double>>& state, std::vector<std::vector<double>>& state_desc); //discretize the state and as a new one
	int toIndex(const std::vector<double>& state);
	std::vector<double> toState(int index);

private:
	std::vector<std::vector<double>> deltaq;
};

#endif