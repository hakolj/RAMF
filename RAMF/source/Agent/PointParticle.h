#pragma once
#ifndef POINTPARTICLE_H
#define POINTPARTICLE_H


#include "Agent.h"

class PointParticle :
	public Agent,
	public GetFlowInfoAble,
	public GetTransRotAble
{
public:
	vectors3d pos;
	vectors4d ee;
	vectors3d p1, p2, p3;
	vectors3d vp_new, vp_old;
	vectors4d deedt_new, deedt_old;
	std::string boundaryType;

protected:
	double lda, a;
	vectors3d uf, ufpf; //fluid velocity at agent position
	vectors3d gradu, gradv, gradw; //fluid gradients
	vectors3d gradupf, gradvpf, gradwpf;
	bool _istep1trans, _istep1rot; //if it is the first step for Adam-Bashforth scheme
	double _initRange; //initial position in domainsize
	int _dimension; //2d or 3d
	std::shared_ptr<InfoAtPointAble> envInfoAtPoint;
	double envDomain[3] = { 0,0,0 }; // boundary range of env

public:
	PointParticle(unsigned agentnum);
	virtual void update(double timestep) = 0;
	virtual void initialize(const std::string& path, const Config& config) = 0;
	virtual void reset() = 0;
	virtual void getInfo() = 0; //get flow info
	virtual void useInfo() = 0;
	inline virtual void setEnv(std::shared_ptr<Environment> env) {
		boundaryType = env->boundaryType();
		env->getDomainSize(envDomain[0], envDomain[1], envDomain[2]);
		envInfoAtPoint = std::dynamic_pointer_cast<InfoAtPointAble>(env);
		return;
	}

	//fully elastic colliding or periodic boundary condition for point-particle.
	virtual void BoundaryCondition(std::shared_ptr<Environment> env);

	static Eigen::Matrix3d ee2R(const vec4d& ee);
	static Eigen::Matrix3d ee2R(const vec4d& ee, vec3d& p1, vec3d& p2, vec3d& p3);
	static vec4d nn2ee(vec3d nn);
	static vec3d vecpf2vec(const vec3d& vecpf, const vec3d& n1, const vec3d& n2, const vec3d& n3);
	static vec3d vec2vecpf(const vec3d& vec, const vec3d& n1, const vec3d& n2, const vec3d& n3);
	void initOrient2D();
	void initOrient3D();
	void initPos2D(double xmax, double ymax);
	void initPos2DPoint(double x, double y, double r);
	void initPos3D(double xmax, double ymax, double zmax);
	void resetFlowInfo();
	void _updateTrans(int i, const vec3d& vppf, double timestep);
	void _updateRot(int i, const vec3d& angvel, double timestep);

	inline void setInitRange(double range) { _initRange = range; };
	inline void setDim(int dim) { _dimension = dim; };

	inline const vectors3d& getPos() const { return pos; };
	inline const vectors4d& getEE() const { return ee; };
	inline const vectors3d& getP1() const { return p1; };
	inline const vectors3d& getP2() const { return p2; };
	inline const vectors3d& getP3() const { return p3; };
	inline const vectors3d& getVel() const { return vp_new; }

	inline const vectors3d& getFluidVel(bool ifpf) const { return ifpf ? ufpf : uf; }
	inline const vectors3d& getGradU(bool ifpf) const { return ifpf ? gradupf : gradu; }
	inline const vectors3d& getGradV(bool ifpf) const { return ifpf ? gradvpf : gradv; }
	inline const vectors3d& getGradW(bool ifpf) const { return ifpf ? gradwpf : gradw; }
	

};

#endif // !POINTPARTICLE_H