#include "pch.h"
#include "Interpolation.h"
//#include "LagrangianInterp.h"


void Lag2nd3D::interp3d_old(const vec3d& pos, const Scalar& sclx, const Scalar& scly, const Scalar& sclz, vec3d& info) {
	int ic, jc, kc;
	
	double xp2, yp2, zp2;
	int Nx = sclx.ms.Nx;
	int Ny = sclx.ms.Ny;
	int Nz = sclx.ms.Nz;

	int id[3], jd[3], kd[3]; // store i+-1,k+-1,j+-1

	// the index of the grid where the particle is
	// Nx = 7
	// 0    1    2    3    4    5    6    7
	// :----|----|----|----|----|----|----|----:
	//   0     1    2   3    4    5     6   7
	// if periodic: 0=6, 1=7
	// actual grid range: 1 to 6
	ic = floor(pos[0] / dx) + 1;
	jc = floor(pos[1] / dy) + 1;
	kc = floor(pos[2] / dz) + 1;

	//overflow check, deal with the right-most case
	//2021-11-26 update: Nx -> Nx-1, because Nx=96 -> Nx=97
	if (ic == Nx) ic = Nx-1;
	if (jc == Ny) jc = Ny-1;
	if (kc == Nz) kc = Nz-1;


	//minus half length of a mesh because the data is stored at the center of mesh
	xp2 = pos[0] - ((ic - 1) * dx + dx / 2);
	yp2 = pos[1] - ((jc - 1) * dy + dy / 2);
	zp2 = pos[2] - ((kc - 1) * dz + dz / 2);

	double Qx = 0.0;
	double Qy = 0.0;
	double Qz = 0.0;

	id[0] = sclx.ms.ima(ic);
	id[1] = ic;
	id[2] = sclx.ms.ipa(ic);
	jd[0] = sclx.ms.jma(jc);
	jd[1] = jc;
	jd[2] = sclx.ms.jpa(jc);
	kd[0] = sclx.ms.kma(kc);
	kd[1] = kc;
	kd[2] = sclx.ms.kpa(kc);

	for (int mx = -1; mx <= 1; mx++) {
		for (int my = -1; my <= 1; my++) {
			for (int mz = -1; mz <= 1; mz++) {
				double coeffA = (A(mx + 1, 0) * pow(xp2, 2) + A(mx + 1, 1) * xp2 + A(mx + 1, 2));
				double coeffB = (B(my + 1, 0) * pow(yp2, 2) + B(my + 1, 1) * yp2 + B(my + 1, 2));
				double coeffC = (C(mz + 1, 0) * pow(zp2, 2) + C(mz + 1, 1) * zp2 + C(mz + 1, 2));
				double mult = coeffA * coeffB * coeffC;

				Qx += sclx(id[mx + 1], jd[my + 1], kd[mz + 1]) * mult;
				Qy += scly(id[mx + 1], jd[my + 1], kd[mz + 1]) * mult;
				Qz += sclz(id[mx + 1], jd[my + 1], kd[mz + 1]) * mult;
			}
		}
	}
	info[0] = Qx;
	info[1] = Qy;
	info[2] = Qz;


}

void Lag2nd3D::interp3d(const vec3d& pos, const Scalar& sclx, const Scalar& scly, const Scalar& sclz, vec3d& info, const FieldStoreType storeType) {
	int ic = 0, jc = 0, kc = 0;

	double xp2, yp2, zp2;
	int Nx = sclx.ms.Nx;
	int Ny = sclx.ms.Ny;
	int Nz = sclx.ms.Nz;

	int id[3], jd[3], kd[3]; // store i+-1,k+-1,j+-1

	// the index of the grid where the particle is
	// Nx = 7
	// 0    1    2    3    4    5    6    7
	// :----|----|----|----|----|----|----|----:
	//   0     1    2   3    4    5     6   7
	// if periodic: 0=6, 1=7
	// actual grid range: 1 to 6


	for (int i = 1; i <= Nx; i++) {
		if (pos(0) < sclx.ms.x(i)) { ic = i - 1; break; }
		
	}
	for (int j = 1; j <= Ny; j++) {
		if (pos(1) < sclx.ms.y(j)) { jc = j - 1; break; }
	}

	for (int k = 1; k <= Nz; k++) {
		if (pos(2) < sclx.ms.z(k)) { kc = k - 1; break; }
	}


	// the position of grid centers at left, current, right mesh
	double xcenter[3], ycenter[3], zcenter[3];

	xcenter[0] = sclx.ms.xc(ic - 1);
	xcenter[1] = sclx.ms.xc(ic);
	xcenter[2] = sclx.ms.xc(ic + 1);

	ycenter[0] = sclx.ms.yc(jc - 1);
	ycenter[1] = sclx.ms.yc(jc);
	ycenter[2] = sclx.ms.yc(jc + 1);

	zcenter[0] = sclx.ms.zc(kc - 1);
	zcenter[1] = sclx.ms.zc(kc);
	zcenter[2] = sclx.ms.zc(kc + 1);


	id[0] = sclx.ms.ima(ic);
	id[1] = ic;
	id[2] = sclx.ms.ipa(ic);
	jd[0] = sclx.ms.jma(jc);
	jd[1] = jc;
	jd[2] = sclx.ms.jpa(jc);
	kd[0] = sclx.ms.kma(kc);
	kd[1] = kc;
	kd[2] = sclx.ms.kpa(kc);

	double valx = 0;
	double valy = 0;
	double valz = 0;
	//double mult1 = 0;
	double basex[3]{}, basey[3]{}, basez[3]{};
	Lag2Bases(pos(0), xcenter[0], xcenter[1], xcenter[2], basex);
	Lag2Bases(pos(1), ycenter[0], ycenter[1], ycenter[2], basey);
	Lag2Bases(pos(2), zcenter[0], zcenter[1], zcenter[2], basez);

	//double Qx = 0.0;
	//double Qy = 0.0;
	//double Qz = 0.0;

	//minus half length of a mesh because the data is stored at the center of mesh
	//xp2 = pos[0] - ((ic - 1) * dx + dx / 2);
	//yp2 = pos[1] - ((jc - 1) * dy + dy / 2);
	//zp2 = pos[2] - ((kc - 1) * dz + dz / 2);

	//bug: when idx = 1, coeff is different using old and new method

	if (storeType == FieldStoreType::CCC) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				for (int k = -1; k <= 1; ++k) {
					double mult1 = basex[i + 1] * basey[j + 1] * basez[k + 1];
					//mult1 = c1 * c2 * c3;
					valx += sclx(id[i + 1], jd[j + 1], kd[k + 1]) * mult1;
					valy += scly(id[i + 1], jd[j + 1], kd[k + 1]) * mult1;
					valz += sclz(id[i + 1], jd[j + 1], kd[k + 1]) * mult1;


					//double coeffA = (A(i + 1, 0) * pow(xp2, 2) + A(i + 1, 1) * xp2 + A(i + 1, 2));
					//double coeffB = (B(j + 1, 0) * pow(yp2, 2) + B(j + 1, 1) * yp2 + B(j + 1, 2));
					//double coeffC = (C(k + 1, 0) * pow(zp2, 2) + C(k + 1, 1) * zp2 + C(k + 1, 2));
					//double mult = coeffA * coeffB * coeffC;

					//Qx += sclx(id[i + 1], jd[j + 1], kd[k + 1]) * mult;
					//Qy += scly(id[i + 1], jd[j + 1], kd[k + 1]) * mult;
					//Qz += sclz(id[i + 1], jd[j + 1], kd[k + 1]) * mult;


				}
			}
		}
	}
	else if (storeType == FieldStoreType::CEC) {
		double baseyy[3]{}; // used for interp of v
		int m = (jc < Ny / 2.0) ? jc + 1 : jc; // middle index of yc
		Lag2Bases(pos(1), scly.ms.y(m-1), scly.ms.y(m), scly.ms.y(m + 1), baseyy); // used only for interp of v

		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				for (int k = -1; k <= 1; ++k) {
					double mult1 = basex[i + 1] * basey[j + 1] * basez[k + 1];
					double multy = basex[i + 1] * baseyy[j + 1] * basez[k + 1];
					//mult1 = c1 * c2 * c3;
					valx += sclx(id[i + 1], jd[j + 1], kd[k + 1]) * mult1;
					valy += scly(id[i + 1], jd[j + 1], kd[k + 1]) * multy;
					valz += sclz(id[i + 1], jd[j + 1], kd[k + 1]) * mult1;


					//double coeffA = (A(i + 1, 0) * pow(xp2, 2) + A(i + 1, 1) * xp2 + A(i + 1, 2));
					//double coeffB = (B(j + 1, 0) * pow(yp2, 2) + B(j + 1, 1) * yp2 + B(j + 1, 2));
					//double coeffC = (C(k + 1, 0) * pow(zp2, 2) + C(k + 1, 1) * zp2 + C(k + 1, 2));
					//double mult = coeffA * coeffB * coeffC;

					//Qx += sclx(id[i + 1], jd[j + 1], kd[k + 1]) * mult;
					//Qy += scly(id[i + 1], jd[j + 1], kd[k + 1]) * mult;
					//Qz += sclz(id[i + 1], jd[j + 1], kd[k + 1]) * mult;


				}
			}
		}
	}




	info[0] = valx;
	info[1] = valy;
	info[2] = valz;

#if DEBUG
	if (isnan(info[0])||isnan(info[1])||isnan(info[2])) {
		std::cout << "nan value in interp" << std::endl;
	}
#endif


}


double Lag2nd3D::Lag2Base(const int& iflag, const double xp, const double x0, const double x1, const double x2) {
	if (iflag == 0) {
		return (xp - x1) * (xp - x2) / (x0 - x1) / (x0 - x2);
	}
	else if (iflag == 1) {
		return (xp - x0) * (xp - x2) / (x1 - x0) / (x1 - x2);
	}
	else if (iflag == 2) {
		return (xp - x0) * (xp - x1) / (x2 - x0) / (x2 - x1);
	}
	else {
		std::cout << "[error]at [Lag2Base()]:check the input iflag" << std::endl;
		return 0;
	}
}


void Lag2nd3D::Lag2Bases(const double xp, const double x0, const double x1, const double x2, double coeff[3]) {
	double xpx0 = xp - x0;
	double xpx1 = xp - x1;
	double xpx2 = xp - x2;
	double x0x1 = x0 - x1;
	double x1x2 = x1 - x2;
	double x0x2 = x0 - x2;

	coeff[0] = xpx1 * xpx2 / (x0x1) / (x0x2);
	coeff[1] = xpx0 * (xpx2) / (-x0x1) / (x1x2);
	coeff[2] = (xpx0) * (xpx1) / (-x0x2) / (-x1x2);

}


void Lag2nd3D::interpCoef(double dx, double dy, double dz) {
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;

	for (int m = -1; m <= 1; m++) {
		A(m + 1, 0) = 1.0 / pow(dx, 2);
		for (int m1 = -1; m1 <= 1; m1++) {
			if (m1 != m) A(m + 1, 0) /= (1.0 * (m - m1));
		}

		A(m + 1, 1) = 0;
		for (int m1 = -1; m1 <= 1; m1++) {
			for (int m2 = -1; m2 <= 1; m2++) {
				if ((m1 != m) & (m1 != m2) & (m2 != m)) {
					A(m + 1, 1) += -1.0 * m2 / (1.0 * (m - m1) * (m - m2));
				}
			}
		}
		A(m + 1, 1) /= dx;

		A(m + 1, 2) = 1.0;
		for (int m1 = -1; m1 <= 1; m1++) {
			if (m1 != m) A(m + 1, 2) *= m1 / (1.0 * (m - m1));
		}
	}

	for (int m = -1; m <= 1; m++) {
		B(m + 1, 0) = 1.0 / pow(dx, 2);
		for (int m1 = -1; m1 <= 1; m1++) {
			if (m1 != m) B(m + 1, 0) /= (1.0 * (m - m1));
		}

		B(m + 1, 1) = 0;
		for (int m1 = -1; m1 <= 1; m1++) {
			for (int m2 = -1; m2 <= 1; m2++) {
				if ((m1 != m) & (m1 != m2) & (m2 != m)) {
					B(m + 1, 1) += -1.0 * m2 / (1.0 * (m - m1) * (m - m2));
				}
			}
		}
		B(m + 1, 1) /= dx;

		B(m + 1, 2) = 1.0;
		for (int m1 = -1; m1 <= 1; m1++) {
			if (m1 != m) B(m + 1, 2) *= m1 / (1.0 * (m - m1));
		}
	}

	for (int m = -1; m <= 1; m++) {
		C(m + 1, 0) = 1.0 / pow(dx, 2);
		for (int m1 = -1; m1 <= 1; m1++) {
			if (m1 != m) C(m + 1, 0) /= (1.0 * (m - m1));
		}

		C(m + 1, 1) = 0;
		for (int m1 = -1; m1 <= 1; m1++) {
			for (int m2 = -1; m2 <= 1; m2++) {
				if ((m1 != m) & (m1 != m2) & (m2 != m)) {
					C(m + 1, 1) += -1.0 * m2 / (1.0 * (m - m1) * (m - m2));
				}
			}
		}
		C(m + 1, 1) /= dx;

		C(m + 1, 2) = 1.0;
		for (int m1 = -1; m1 <= 1; m1++) {
			if (m1 != m) C(m + 1, 2) *= m1 / (1.0 * (m - m1));
		}
	}
	//std::cout << A << std::endl;
	//std::cout << B << std::endl;
	//std::cout << C << std::endl;
	return;
}
