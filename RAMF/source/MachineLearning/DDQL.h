#ifndef DDQL_H
#define DDQL_H

#include "MachineLearning.h"
#include "torch/torch.h"
#include "torch/script.h"

struct DDQLNetImpl :
	public torch::nn::Module
{
public:
	torch::nn::Linear f1;
	torch::nn::Linear f2;
	//torch::nn::BatchNorm1d bn1;
	std::vector<int64_t> nodeNum;
public:
	DDQLNetImpl(int64_t nin, int64_t nmid, int64_t nout) :
		f1(register_module("InputLayer", torch::nn::Linear(nin, nmid))),
		f2(register_module("FullyConnectedLayer", torch::nn::Linear(nmid, nout))),
		//bn1(register_module("InputBN", torch::nn::BatchNorm1d(nmid))),
		nodeNum({ nin,nmid,nout }) {};
	torch::Tensor forward(torch::Tensor input) {
		input = f1(input);
		input = torch::leaky_relu(input);
		input = f2(input);
		return input;

	}

public:
	void initNet() {
		torch::nn::init::kaiming_normal_(f1->weight);
		torch::nn::init::constant_(f1->bias, 0.0);
		torch::nn::init::kaiming_normal_(f2->weight);
		torch::nn::init::constant_(f2->bias, 0.0);

	}
};
TORCH_MODULE(DDQLNet);

class DoubleDeepQLearning : public MachineLearning {
public:
	int actionNum, stateNum;
	DDQLNet target_net;
	DDQLNet eval_net;

	torch::nn::MSELoss loss_func;
	torch::optim::Adam optimizer;
	double loss_record;
	double qvalue_record;
	const double GAMMA = 0.99;

	int copyStep;

	DoubleDeepQLearning(int64_t nin, int64_t nmid, int64_t nout);
	void copyParam();

	static std::shared_ptr<DoubleDeepQLearning> makeInstance(Config config, int64_t dimState, int64_t numAction);

	virtual void initialize(const std::string& path, const Config& config);
	virtual std::vector<int> decideAction(const std::vector<std::vector<double>>& state, bool iexplore);
	virtual void train(Memory* memory);
	//virtual void model_saver(const std::string& path);
	virtual void model_saver(const std::string& path, const std::string& suffix);
	virtual void model_loader(const std::string& path);

	//to record parameters to file
	void recParam(const char* path);
	void clearRec(const char* path);



private:
	int _countTrain;
};

#endif // !DDQL_H