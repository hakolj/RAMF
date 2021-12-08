#pragma once
#ifndef AGENTITF_H
#define AGENTITF_H

#include "vectypedef.h"
class GetFlowInfoAble {
public:
	//virtual void getFlowInfo(vectors3d& ufpf, vectors3d& gradupf, vectors3d& gradvpf, vectors3d& gradwpf) = 0;
	virtual const vectors3d& getFluidVel(bool ifpf = false) const = 0;
	virtual const vectors3d& getGradU(bool ifpf = false) const = 0;
	virtual const vectors3d& getGradV(bool ifpf = false) const = 0;
	virtual const vectors3d& getGradW(bool ifpf = false) const = 0;
};

class GetTransRotAble {
public:
	virtual const vectors3d& getPos() const = 0;
	virtual const vectors4d& getEE() const = 0;
	virtual const vectors3d& getP1() const = 0;
	virtual const vectors3d& getP2() const = 0;
	virtual const vectors3d& getP3() const = 0;
	virtual const vectors3d& getVel() const = 0;
};


class ChangeMassCenterAble {
public:
	virtual void setMassCenter(const vectors3d& newMassCenter) = 0;
	virtual vectors3d getMassCenter() const = 0;
};
class ChangeSwimVelAble {
public:
	virtual void setSwimVel(const std::vector<double>& newvswim) = 0;
	virtual vectors3d& swimAngVel() = 0;
};

class ChangeJumpVelAble {
public:
	virtual std::vector<double>& jumpTime() = 0;

};

#endif // !AGENTITF_H


