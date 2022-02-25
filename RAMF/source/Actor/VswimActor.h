#pragma once
#ifndef VSWIM_H
#define VSWIM_H


#include "Actor.h"


class VswimActor :
	public Actor
{
public:
	double maxVswim;
	
private:
	std::vector<double> vswimRatio;
	int _actionNum;

public:
	VswimActor() :maxVswim(0),_actionNum(0),vswimRatio(1) {};
	void takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive);
	void initialize(const std::string& path, const Config& config);
	inline int num() const { return _actionNum; }
};
#endif // !MASSCENTERANDVSWIM_H


