#include "pch.h"
#include "Agent.h"
#include "PointParticle.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Random.h"
#include "MatrixOp.h"


PointParticle::PointParticle(unsigned agentnum) :
	Agent(agentnum), pos(vectors3d(agentnum, vec3d::Zero())),
	lda(lda), a(a),
	ee(vectors4d(agentnum, vec4d::Zero())), p1(vectors3d(agentnum, vec3d::Zero())),
	p2(vectors3d(agentnum, vec3d::Zero())), p3(vectors3d(agentnum, vec3d::Zero())),
	vp_new(vectors3d(agentnum, vec3d::Zero())), vp_old(vectors3d(agentnum, vec3d::Zero())),
	deedt_new(vectors4d(agentnum, vec4d::Zero())), deedt_old(vectors4d(agentnum, vec4d::Zero())),
	uf(vectors3d(agentnum, vec3d::Zero())), ufpf(vectors3d(agentnum, vec3d::Zero())),
	gradu(vectors3d(agentnum, vec3d::Zero())), gradv(vectors3d(agentnum, vec3d::Zero())), gradw(vectors3d(agentnum, vec3d::Zero())),
	gradupf(vectors3d(agentnum, vec3d::Zero())), gradvpf(vectors3d(agentnum, vec3d::Zero())), gradwpf(vectors3d(agentnum, vec3d::Zero())),
	_istep1trans(false), _istep1rot(false), envInfoAtPoint(NULL), envDomain()
{
}

vec4d PointParticle::nn2ee(vec3d nn) {
	double ex = -nn(1);
	double ey = nn(0);
	double ez = 0;
	double norme = sqrt(ex * ex + ey * ey + ez * ez);
	ex /= norme;
	ey /= norme;
	double dphi = acos(nn(2))/2.0;
	vec4d ee;
	ee(0) = cos(dphi);
	ee(1) = ex * sin(dphi);
	ee(2) = ey * sin(dphi);
	ee(3) = ez * sin(dphi);
	return ee;
}

Eigen::Matrix3d PointParticle::ee2R(const vec4d& ee) {
	Eigen::Matrix3d R(3, 3);
	R(0, 0) = ee(0) * ee(0) + ee(1) * ee(1) - ee(2) * ee(2) - ee(3) * ee(3);
	R(0, 1) = 2.0 * (ee(1) * ee(2) + ee(0) * ee(3));
	R(0, 2) = 2.0 * (ee(1) * ee(3) - ee(0) * ee(2));
	R(1, 0) = 2.0 * (ee(1) * ee(2) - ee(0) * ee(3));
	R(1, 1) = ee(0) * ee(0) - ee(1) * ee(1) + ee(2) * ee(2) - ee(3) * ee(3);
	R(1, 2) = 2.0 * (ee(2) * ee(3) + ee(0) * ee(1));
	R(2, 0) = 2.0 * (ee(1) * ee(3) + ee(0) * ee(2));
	R(2, 1) = 2.0 * (ee(2) * ee(3) - ee(0) * ee(1));
	R(2, 2) = ee(0) * ee(0) - ee(1) * ee(1) - ee(2) * ee(2) + ee(3) * ee(3);
	return R;
}

Eigen::Matrix3d PointParticle::ee2R(const vec4d& ee, vec3d& p1, vec3d& p2, vec3d& p3) {
	Eigen::Matrix3d R = ee2R(ee);
	p1 = R.row(0);
	p2 = R.row(1);
	p3 = R.row(2);
	return R;
}

vec3d PointParticle::vecpf2vec(const vec3d& vecpf, const vec3d& n1, const vec3d& n2, const vec3d& n3) {
	Eigen::Matrix3d temp(3, 3);
	temp.col(0) = n1;
	temp.col(1) = n2;
	temp.col(2) = n3;
	return matmul(temp, vecpf);
}
vec3d PointParticle::vec2vecpf(const vec3d& vec, const vec3d& n1, const vec3d& n2, const vec3d& n3) {
	Eigen::Matrix3d temp(3, 3);
	temp.row(0) = n1;
	temp.row(1) = n2;
	temp.row(2) = n3;
	return matmul(temp, vec);
}

void PointParticle::initOrient2D() {
	//x-y plane
	double c = std::sin(M_PI_4);
	double phi = 0.0;
	for (unsigned i = 0; i < agentnum; i++) {
		phi = rd::randd() * M_PI;
		ee[i](0) = c * std::cos(phi);
		ee[i](1) = c * std::cos(phi);
		ee[i](2) = c * std::sin(phi);
		ee[i](3) = c * std::sin(phi);
		ee2R(ee[i], p1[i], p2[i], p3[i]);
		deedt_new[i] = vec4d::Zero();
		deedt_new[i] = vec4d::Zero();
		_istep1rot = true;
	}
}

//true random
void PointParticle::initOrient3D() {
	for (unsigned i = 0; i < agentnum; i++) {
		vec3d n0;
		double phi = 0;
		double theta = 0;
		double psi = 0;
		double h = rd::randd();

		theta = h * 2.0 * M_PI;
		h = rd::randd();
		phi = asin(h * 2.0 - 1.0);
		n0(0) = cos(theta) * cos(phi);
		n0(1) = sin(theta) * cos(phi);
		n0(2) = sin(phi);

		ee[i] = nn2ee(n0);
		ee2R(ee[i], p1[i], p2[i], p3[i]);
		deedt_new[i] = vec4d::Zero();
		deedt_new[i] = vec4d::Zero();
		_istep1rot = true;
	}
}

void PointParticle::initPos2D(double xmax, double ymax) {
	initPos3D(xmax, ymax, 0);
}

void PointParticle::initPos3D(double xmax, double ymax, double zmax) {
	for (unsigned i = 0; i < agentnum; i++) {
		pos[i][0] = rd::randd() * xmax;
		pos[i][1] = rd::randd() * ymax;
		pos[i][2] = rd::randd() * zmax;
		vp_new[i] = vec3d::Zero();
		vp_old[i] = vec3d::Zero();
		_istep1trans = true;
	}
	return;
}

void PointParticle::initPos2DPoint(double x, double y, double r) {
	std::vector<double> theta = rd::randd(agentnum, 0.0, 2 * M_PI);
	for (unsigned i = 0; i < agentnum; i++) {
		pos[i][0] = cos(theta[i]) * r + x;
		pos[i][1] = sin(theta[i]) * r + y;
		pos[i][2] = 0;
		vp_new[i] = vec3d::Zero();
		vp_old[i] = vec3d::Zero();
		_istep1trans = true;
	}

}
void PointParticle:: resetFlowInfo() {
	for (unsigned i = 0; i < agentnum; i++) {
		uf[i] = vec3d::Zero();
		ufpf[i] = vec3d::Zero();
		gradu[i] = vec3d::Zero();
		gradv[i] = vec3d::Zero();
		gradw[i] = vec3d::Zero();
		gradupf[i] = vec3d::Zero();
		gradvpf[i] = vec3d::Zero();
		gradwpf[i] = vec3d::Zero();
	}
}

void PointParticle::_updateTrans(int i, const vec3d& vppf, double timestep) {
	vp_new[i] = vecpf2vec(vppf, p1[i], p2[i], p3[i]);
	if (_istep1trans) {
		vp_old[i] = vp_new[i];
	}
	pos[i] += timestep * (1.5 * vp_new[i] - 0.5 * vp_old[i]);
	vp_old[i] = vp_new[i];
	//double x = pos[i][0];
	//double y = pos[i][1];
	//std::cout << vp_new[i][2] << std::endl;
	return;
};

void PointParticle::_updateRot(int i, const vec3d& angvel, double timestep) {
	deedt_new[i][0] = -0.5 * (ee[i][1] * angvel[0] + ee[i][2] * angvel[1] + ee[i][3] * angvel[2]);
	deedt_new[i][1] = 0.5 * (ee[i][0] * angvel[0] - ee[i][3] * angvel[1] + ee[i][2] * angvel[2]);
	deedt_new[i][2] = 0.5 * (ee[i][3] * angvel[0] + ee[i][0] * angvel[1] - ee[i][1] * angvel[2]);
	deedt_new[i][3] = 0.5 * (-ee[i][2] * angvel[0] + ee[i][1] * angvel[1] + ee[i][0] * angvel[2]);
	if (_istep1rot) {
		deedt_old[i] = deedt_new[i];
	}
	ee[i] += timestep * (1.5 * deedt_new[i] - 0.5 * deedt_old[i]);
	deedt_old[i] = deedt_new[i];
	double nee = ee[i].norm();
	ee[i] /= nee;
	ee2R(ee[i], p1[i], p2[i], p3[i]); //update particle axis direction


	return;
}


void PointParticle::BoundaryCondition(std::shared_ptr<Environment> env) {
	std::string bc = env->boundaryType();
	//nothing to do for periodic bc 
	double L[3];
	env->getDomainSize(L[0], L[1], L[2]);

	//wall bc
	for (int i = 0; i < agentnum; i++) {
		//loop for three dims
		for (int dim = 0; dim < 3; dim++) {
			if (bc[dim] == 'W') {				
				if (pos[i](dim) < this->a) {
					pos[i](dim) = 2 * this->a - pos[i](dim);
					vp_new[i](dim) *= -1;
				}
				else if (pos[i](dim) > L[dim] - this->a) {
					pos[i](dim) = 2 * (L[dim] - this->a) - pos[i](dim);
					vp_new[i](dim) *= -1;
				}

			}
		}
		
	}
	

}