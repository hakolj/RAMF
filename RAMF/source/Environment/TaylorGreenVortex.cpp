#include "pch.h"
#include "TaylorGreenVortex.h"
#include "Environment.h"
#include <math.h>
#include <iostream>
using namespace std;

TaylorGreenVortex::TaylorGreenVortex(double _uscale, double _lscale) :_uscale(_uscale), _lscale(_lscale) {}
void TaylorGreenVortex::infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) const {
	if ((pos.size() != uf.size()) | (pos.size() != gradu.size()) | (pos.size() != gradv.size()) | (pos.size() != gradw.size())) {
		cerr << "Vectors size mismatch. Check TaylorGreenVortex::getInfo." << endl;
	}
	double xp, yp, sx, sy, cx, cy;
	for (unsigned i = 0; i < pos.size(); i++) {
		uf[i].setZero();
		gradu[i].setZero();
		gradv[i].setZero();
		gradw[i].setZero();
		xp = pos[i][0] / _lscale;
		yp = pos[i][1] / _lscale;
		sx = std::sin(xp);
		sy = std::sin(yp);
		cx = std::cos(xp);
		cy = std::cos(yp);
		uf[i][0] = 0.5 * _uscale * cx * sy;
		uf[i][1] = -0.5 * _uscale * sx * cy;
		gradu[i][0] = -0.5 * _uscale / _lscale * sx * sy;
		gradu[i][1] = 0.5 * _uscale / _lscale * cx * cy;
		gradu[i][2] = 0.0;
		gradv[i][0] = -gradu[i][1];
		gradv[i][1] = -gradu[i][0];

	}
	return;
}

void TaylorGreenVortex::initialize(const string& path, const Config& config) {
	_uscale = config.Read("u0", _uscale);
	_lscale = config.Read("L0", _lscale);
	return;
}