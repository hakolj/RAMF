#pragma once
#ifndef FOP_H
#define FOP_H

#include "pch.h"

namespace Fop {
	extern void Trim(std::string& inout_s);
	extern bool copyFile(const std::string& from, const std::string& to);

	template<typename T>
	std::vector<std::vector<T>> loadvec2d(const std::string& path, int skiprow = 0) {
		const char whitespace[] = " \n\t\v\r\f";
		std::ifstream is;
		is.open(path, std::ios::in);
		if (!is) {
			std::cout << "File:" + path + " not found in function loadvec2d" << std::endl;
			exit(0);
		}
		std::string line;
		std::vector<T> tempvec;
		std::vector<std::vector<T>> vec;
		int maxlength = 0;
		for (int i = 0; i < skiprow; i++) {
			getline(is, line); //skip rows
		}
		while (!is.eof()) {
			getline(is, line);
			if (line.length() <= 0) continue;
			int index = 0;

			Trim(line);
			std::size_t delim = 0;
			std::size_t head = 0;
			while (delim != std::string::npos) {
				delim = line.find_first_of(whitespace, head);
				T item;
				std::stringstream ss;
				ss << line.substr(head, delim - head);
				ss >> item;
				if (index < maxlength) {
					tempvec[index] = item;
				}
				else {
					tempvec.push_back(item);
					maxlength++;
				}
				index++;

				//head = delim + 1;
				head = line.find_first_not_of(whitespace, delim + 1);
				//std::cout << item << " ";
			}
			//std::cout << std::endl;
			vec.push_back(tempvec);
		}

		//for (int i = 0; i < vec.size(); i++) {
		//	for (int j = 0; j < vec[0].size(); j++) {
		//		std::cout << vec[i][j] << " ";
		//	}
		//	std::cout << std::endl;
		//}

		return vec;
	}



	template<typename T>
	void writevec2d(std::vector<std::vector<T>> vec, std::ostream& of) {

		for (unsigned i = 0; i < vec.size(); i++) {
			for (unsigned j = 0; j < vec[i].size(); j++) {
				of << std::scientific << vec[i][j] << "    ";
			}
			of << std::endl;
		}
		return;

	}

	template<typename T>
	void writevec2d(std::vector<std::vector<T>> vec, const std::string& path) {
		std::ofstream of;
		of.open(path);
		writevec2d<T>(vec, of);
		of.close();
		return;

	}

	template<typename T>
	std::vector<T> loadvec1d(const std::string& line) {
		const char whitespace[] = " \n\t\v\r\f";
		std::vector<T> tempvec;
		std::vector<T> vec(0);
		int maxlength = 0;
		std::size_t delim = 0;
		std::size_t head = 0;

		while (delim != std::string::npos) {
			delim = line.find_first_of(whitespace, head);
			T item;
			std::stringstream ss;
			ss << line.substr(head, delim - head);
			ss >> item;
			vec.push_back(item);


			//head = delim + 1;
			head = line.find_first_not_of(whitespace, delim + 1);
			//std::cout << item << " ";
		}

		return vec;
	}

	bool makeDir(const std::string& path);

}

#endif // !FOP_H




