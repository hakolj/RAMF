#include "pch.h"
#include "Fop.h"
#if (defined(_WIN32) || defined(_WIN64))
#include "direct.h"
#elif defined(__linux__)
#include <sys/stat.h>  
#include <unistd.h>  
#include <sys/types.h> 
#endif
using namespace std;

void Fop::Trim(std::string& inout_s)
{
	// Remove leading and trailing whitespace  
	static const char whitespace[] = " \n\t\v\r\f";
	inout_s.erase(0, inout_s.find_first_not_of(whitespace));
	inout_s.erase(inout_s.find_last_not_of(whitespace) + 1U);
}

bool Fop::copyFile(const std::string& from, const std::string& to) {
	std::ifstream src;
	std::ofstream dst;

	src.open(from, std::ios::in | std::ios::binary);
	dst.open(to+"/config.txt", std::ios::out | std::ios::binary);
	if (!src) {
		std::cout << "Source file not found when copy file." << std::endl;
		src.close();
		dst.close();
		return false;
	}
	if (!dst) {
		std::cout << "Destined path unavailable when copy file." << std::endl;
		src.close();
		dst.close();
		return false;
	}

	dst << src.rdbuf();
	src.close();
	dst.close();
	return true;

}

bool Fop::makeDir(const std::string& path) {
#if (defined(_WIN32) || defined(_WIN64))
	int err = _mkdir(path.c_str());
	if (err == 2) {
		cout << "Unable to make directory: " + path << endl;
		return false;
	}
	else if (err == 17) {
		cout << "Warning: Direcotry already exists: " + path << endl;
	}
#elif defined(__linux__)
	int err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
	//if (err != 0) {
	//	cout << "?" << endl;
	//}

	return true;
}