#include "pch.h"
#include "Sensor.h"
#include "SensorsImpl.h"
#include "PositionSensor.h"
using namespace std;
shared_ptr<Sensor> SensorInitializer(std::string path) {
	//read config
	Config config(path, "SENSOR");

	shared_ptr<Sensor> psensor;
	string sensorname = config.Read("type", string("_EMPTY"));
	
	if (sensorname == "PSP") {
		psensor = make_shared<PSPSensor>();
	}
	else if(sensorname == "PSQ") {
		psensor = make_shared<PSQSensor>();
	}
	else if (sensorname == "RelaPosOrient2D") {
		psensor = make_shared<RelaPosOrient2DSensor>();
	}
	else if (sensorname == "RelaVelocity") {
		psensor = make_shared<RelaVelSensor>();
	}
	else if (sensorname == "RelaVelocityP1") {
		psensor = make_shared<RelaVelP1Sensor>();
	}
	else if (sensorname == "RelaVelocityD") {
		psensor = make_shared<RelaVelSensorDiscret>();
	}
	else if (sensorname == "Position") {		
		psensor = make_shared<PositionSensor>();
	}
	else if (sensorname == "RelaPositionAngle") {
		psensor = make_shared<RelaPositionAngleSensor>();
	}
	
	else if (sensorname == "_EMPTY") {
		cout << "Sensor type is not defined. Check config file." << endl;
		exit(0);
	}

	else {
		cout << "Undefined sensor name. Check config file.";
		exit(0);
	}	
	psensor->initialize(path,config);
	cout << "Sensor: " << sensorname << " created." << endl;
	return psensor;

}