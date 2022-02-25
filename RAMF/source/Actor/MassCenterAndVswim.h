#pragma once
#ifndef MASSCENTERANDVSWIM_H
#define MASSCENTERANDVSWIM_H


#include "Actor.h"

class MassCenterAndVswim :
	public Actor
{
public:
	double maxVswim;
	double maxd1, maxd3, mind1, mind3;
	MassCenterAndVswim();
	void takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action, bool inaive);
	void initialize(const std::string& path, const Config& config);
	inline int num() const { return 8; }
};
#endif // !MASSCENTERANDVSWIM_H