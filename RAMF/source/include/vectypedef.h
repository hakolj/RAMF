#pragma once
#ifndef VECTYPEDEF_H
#define VECTYPEDEF_H



#include <Eigen/Dense>
#include <vector>

typedef Eigen::Matrix<double, 3, 1> vec3d;
typedef std::vector<Eigen::Matrix<double, 3, 1>> vectors3d;
typedef Eigen::Matrix<double, 4, 1> vec4d;
typedef std::vector<Eigen::Matrix<double, 4, 1>, Eigen::aligned_allocator<Eigen::Vector4d>> vectors4d;

namespace extv {
	// some self-defined vector operations
	template<typename T>
	inline T maxval(const std::vector<T>& vec) {
		return *(std::max_element(vec.begin(), vec.end()));
	}

	template<typename T>
	inline int maxloc(const std::vector<T>& vec) {
		return std::distance(vec.begin(),std::max_element(vec.begin(), vec.end()));
	}


	template<typename T>
	std::vector<T>& assign(std::vector<T>& vec, T value) {
		for (T& e : vec) {
			e = value;
		}
		return vec;
	}

	template<typename T>
	std::vector<std::vector<T>>& assign(std::vector<std::vector<T>>& vec, T value) {
		for (auto& e1 : vec) {
			for (T& e : e1) {
				e = value;
			}
		}
		return vec;
	}

}

#endif // !VECTYPEDEF_H