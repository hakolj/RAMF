#include "pch.h"
#include "DDQL.h"
#include <tuple>
#include <vector>
#include "TorchAPI.h"
#include "Fop.h"
using namespace std;


DoubleDeepQLearning::DoubleDeepQLearning(int64_t nin, int64_t nmid, int64_t nout) :
	actionNum(nout), stateNum(nin), target_net(DDQLNet(nin, nmid, nout)),
	eval_net(DDQLNet(nin, nmid, nout)), loss_func(torch::nn::MSELoss()),
	optimizer(eval_net->parameters(), torch::optim::AdamOptions(0.002)),
	loss_record(0) {
	eval_net->initNet();
	copyParam();

	return;

};


std::shared_ptr<DoubleDeepQLearning> DoubleDeepQLearning::makeInstance(Config config, int64_t dimState, int64_t numAction) {
	int numMid = config.Read<int>("num mid");
	return std::make_shared<DoubleDeepQLearning>(dimState, numMid, numAction);
}

void DoubleDeepQLearning::train(Memory* memory) {
	int req_batch_size = 50;

	vector<vector<double>> state0;
	vector<vector<double>> state1;
	vector<double> reward;
	vector<int> action;
	vector<int> isterminated;
	memory->randomBatch(req_batch_size, state0, action, reward, state1, isterminated);
	int realbatchsize = state0.size();

	torch::Tensor b_s = TorchAPI::from_vector(state0);
	torch::Tensor b_a = TorchAPI::from_vector(action).to(torch::kLong);
	torch::Tensor b_r = TorchAPI::from_vector(reward);
	torch::Tensor b_s_ = TorchAPI::from_vector(state1);

	torch::Tensor q_eval = eval_net(b_s).gather(1, b_a);

	torch::Tensor q_next = target_net(b_s_).detach();
	torch::Tensor q_target = b_r + GAMMA * get<0>(q_next.max(1)).unsqueeze(1);



	torch::Tensor loss = loss_func(q_eval, q_target);

	loss_record = loss.detach().item().toFloat();
	qvalue_record = q_eval.detach().mean().item().toFloat();
	optimizer.zero_grad();
	loss.backward();
	optimizer.step();
	if (_countTrain++ >= copyStep) {
		copyParam();
		_countTrain = 0;
	}
	//loss_record = loss.detach().item().toFloat();
};


std::vector<int> DoubleDeepQLearning::decideAction(const std::vector<std::vector<double>>& state, bool iexplore) {
	vector<int> action(state.size());
	torch::Tensor stateTensor = TorchAPI::from_vector(state);
	torch::Tensor actionTensor = get<1>(torch::max(eval_net->forward(stateTensor), 1)); //get max location 

	double epsilon = iexplore ? 0.01 : 0;
	auto accessor = actionTensor.accessor<int64_t,1>();
	for (unsigned i = 0; i < action.size(); i++) {
		double arand = rd::randd();
		action[i] = arand < epsilon ? rd::randi(0, this->actionNum) : accessor[i]; //epsilon greedy
	}

	return action;
}

void DoubleDeepQLearning::initialize(const std::string& path, const Config& config) {
	copyStep = 100; // every copyStep replace the parameters of target net
	_countTrain = 0;
}


void DoubleDeepQLearning::model_saver(const std::string& path, const std::string& suffix) {
	ofstream testf;
	testf.open(path + "/evalue_net"+suffix+".pt", ios::out);
	if (!testf) {
		cout << "Directory inexistent when saving model. Model not saved." << endl;
		return;
	}
	torch::save(eval_net, path + "/evalue_net" + suffix + ".pt");
	torch::save(target_net, path + "/target_net" + suffix + ".pt");
	return;
}


void DoubleDeepQLearning::model_loader(const std::string& path) {
	torch::load(eval_net, path + "/evalue_net.pt");
	torch::load(target_net, path + "/target_net.pt");
	return;
}

void DoubleDeepQLearning::recParam(const char* path) {	
	vector<vector<double>> whis = TorchAPI::as2Dvector<double>(target_net->f1->weight.detach());
	ofstream outfile;
	string dir(path);
	
	outfile.open(dir + "/whis.txt", ios_base::out | ios_base::app);
	for (int i = 0; i < whis.size(); i++) {
		for (int j = 0; j < whis[0].size(); j++) {
			outfile << whis[i][j] << " ";
		}
	}
	outfile << endl;
	outfile.close();	
	
	vector<vector<double>> his = TorchAPI::as2Dvector<double>(eval_net->f2->weight.detach());

	outfile.open(dir + "/bhis.txt", ios_base::out | ios_base::app);
	for (int i = 0; i < his.size(); i++) {
		for (int j = 0; j < his[0].size(); j++) {
			outfile << his[i][j] << " ";
		}
	}
	outfile << endl;
	outfile.close();
	return;
}

// to clear record file
void DoubleDeepQLearning::clearRec(const char* path) {
	string dir(path);
	ofstream outfile;
	outfile.open(dir + "/whis.txt", ios_base::out);
	outfile.close();
	ofstream outfile1;
	outfile1.open(dir + "/bhis.txt", ios_base::out);
	outfile1.close();
	return;
}

void DoubleDeepQLearning::copyParam() {
	torch::autograd::GradMode::set_enabled(false);
	auto eval_params = eval_net->named_parameters(true);
	auto target_params = target_net->named_parameters(true);
	for (auto& val : eval_params) {
		auto name = val.key();
		auto* t = target_params.find(name);
		if (t != nullptr) {
			t->copy_(val.value());
		}
		else {
			std::cout << "Key " << val.key() << " is not found" << std::endl;
		}
	}
	torch::autograd::GradMode::set_enabled(true);
	std::cout << "copy parameter" << endl;
}