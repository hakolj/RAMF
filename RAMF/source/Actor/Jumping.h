#ifndef JUMPING_H
#define JUMPING_H

#include "Actor.h"

class Jumping : public Actor {
public:
	Jumping();
	virtual void takeAction(std::shared_ptr<Agent> ag, const std::vector<int>& action);
	virtual void initialize(const std::string& path, const Config& config);
	inline virtual int num() const { return 2; }
};


#endif //JUMPING_H
