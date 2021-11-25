#include "pch.h"
#include "Interpolation.h"



void Lag2nd3D::interp3d(const vec3d& pos, const Scalar& sclx, const Scalar& scly, const Scalar& sclz, vec3d& info) {
	int ic, jc, kc;
	
	double xp2, yp2, zp2;
	int Nx = sclx.ms.Nx;
	int Ny = sclx.ms.Ny;
	int Nz = sclx.ms.Nz;

	int id[3], jd[3], kd[3]; // store i+-1,k+-1,j+-1

	
	ic = floor(pos[0] / dx) + 1;
	jc = floor(pos[1] / dy) + 1;
	kc = floor(pos[2] / dz) + 1;

	if (ic == Nx + 1) ic = Nx;
	if (jc == Ny + 1) jc = Ny;
	if (kc == Nz + 1) kc = Nz;

	// overflow check....

	xp2 = pos[0] - (ic - 1) * dx - dx / 2;
	yp2 = pos[1] - (jc - 1) * dy - dy / 2;
	zp2 = pos[2] - (kc - 1) * dz - dz / 2;

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
