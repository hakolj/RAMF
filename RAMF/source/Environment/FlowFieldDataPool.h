#pragma once
#ifndef FLOWFIELDDATAPOOL_H
#define FLOWFIELDDATAPOOL_H

#include "Scalar.h"

class FlowFieldDataPool
{

public:
	FlowFieldDataPool();


	std::vector<Scalar> upool;
	std::vector<Scalar> vpool;
	std::vector<Scalar> wpool;

	void LoadData(const Mesh& ms, const std::string& datapath, const std::vector<int>& idxlist);

private:
	std::string _datapath;
};



#endif