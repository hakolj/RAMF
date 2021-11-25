#pragma once
#ifndef ACTOR_H
#define ACTOR_H

#include "Agent.h"
#include "AgentInterface.h"
#include <array>
#include "Config.h"
class Actor {
public:
	virtual void takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual int num() const = 0;
};

extern std::shared_ptr<Actor> ActorInitializer(const std::string& path);

#endif // !ACTOR_H




