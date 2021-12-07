#include "pch.h"
#include "Quiescent.h"

using namespace std;
void Quiescent::initialize(const std::string& path, const Config& config) {
	string str;
	stringstream ss;
	str = config.Read<string>("DomainX");
	int iPI;
	bool pflag;

	iPI = str.find("PI");
	pflag = iPI != string::npos;
	if (pflag) {
		str = str.substr(0, iPI);
	}
	ss.str("");
	ss.clear();
	ss << str;
	ss >> Lx;
	if (pflag) Lx *= M_PI;


	str = config.Read<string>("DomainY");
	iPI = str.find("PI");
	pflag = iPI != string::npos;
	if (pflag) {
		str = str.substr(0, iPI);
	}
	ss.str("");
	ss.clear();
	ss << str;
	ss >> Ly;
	if (pflag) Ly *= M_PI;

	str = config.Read<string>("DomainZ");
	iPI = str.find("PI");
	pflag = iPI != string::npos;
	if (pflag) {
		str = str.substr(0, iPI);
	}
	ss.str("");
	ss.clear();
	ss << str;
	ss >> Lz;
	if (pflag) Lz *= M_PI;

}