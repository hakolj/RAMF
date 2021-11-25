#pragma once
#ifndef MATRIXOP_H
#define MATRIXOP_H

//#include <Eigen/Dense>
#include "vectypedef.h"

template <class T1, class T2>
Eigen::Matrix<double, T1::RowsAtCompileTime, T2::ColsAtCompileTime> matmul(const T1& A, const T2& B) {
	Eigen::Matrix<double, T1::RowsAtCompileTime, T2::ColsAtCompileTime> C(T1::RowsAtCompileTime, T2::ColsAtCompileTime);
	C.setZero();
	for (int i = 0; i < T1::RowsAtCompileTime; i++) {
		for (int j = 0; j < T2::ColsAtCompileTime; j++) {
			for (int k = 0; k < T1::ColsAtCompileTime; k++) {
				C(i, j) += A(i, k) * B(k, j);
			}
		}
	}
	return C;
}

//vec3d cross(const vec3d& a, const vec3d& b) {
//	vec3d c(3, 1);
//	c.setZero();
//
//}

#endif // !MATRIXOP_H

