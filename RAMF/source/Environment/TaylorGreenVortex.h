#pragma once
#ifndef TAYLORGREENVORTEX_H
#define TAYLORGREENVORTEX_H
#endif // !TAYLORGREENVORTEX_H

#include "Environment.h"
class TaylorGreenVortex :
	public Environment,
	public InfoAtPointAble
{
private:
	double _uscale, _lscale;

public:
	TaylorGreenVortex(double _uscale = 1.0, double _lscale = 1.0);
	void infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw);
	void reset(){}
	void initialize(const std::string& path, const Config& config);
	inline double uscale() { return _uscale; }
	inline double lscale() { return _lscale; }
	inline void setscale(double uscale, double lscale) {
		_uscale = uscale;
		_lscale = lscale;
		return;
	}
	void update(double dt) {};
};