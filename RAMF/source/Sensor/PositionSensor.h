#ifndef POSITIONSENSOR_H
#define POSITIONSENSOR_H

#include "Sensor.h"
#include "Fop.h"

class PositionSensor :public Sensor {
public:
	int dimension = 3;
	
	// dim = 1: direction = 0, 1, 2 -> pos: x, y, z;
	// dim = 2: direction = 0, 1, 2 -> pos yz, zx, xy
	// dim = 3: direction = 0, 1, 2 -> pos: xyz;
	int direction = 0; 

	virtual int dim() const {
		return dimension;
	};

	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate);
	virtual void initialize(const std::string& path, const Config& config);
	PositionSensor() {};
};




// relative position to a target
class RelaPositionAngleSensor : public Sensor {
public:
	vec3d target;
	int dimension = 3;

	// dim = 1: direction = 0, 1, 2 -> pos: x, y, z;
	// dim = 2: direction = 0, 1, 2 -> pos yz, zx, xy
	// dim = 3: direction = 0, 1, 2 -> pos: xyz;
	int direction = 0;

private:
	bool isExternalTarget = false; // to decide whether target is from external task definition
	vec3d _selftarget;
public:

	RelaPositionAngleSensor();
	virtual int dim() const {
		return dimension + 1; //dim in pos + 1 dim in angle
	};

	virtual void getState(Agent* agent, std::vector<std::vector<double>>& newstate);
	virtual void initialize(const std::string& path, const Config& config);
	void reset(std::shared_ptr<Task> task) override;

};



#endif