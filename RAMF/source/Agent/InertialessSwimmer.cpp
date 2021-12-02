#include "pch.h"
#include "InertialessSwimmer.h"
#include "MatrixOp.h"


using namespace std;
InertialessSwimmer::InertialessSwimmer(int agentnum, double lda, double a) :
	PointParticle(agentnum), lda(lda), a(a),
	vswim(vector<double>(agentnum)), gyro(vector<double>(agentnum)),
	vsettle(vec3d::Zero()),
	mdisp(vectors3d(agentnum, vec3d::Zero())), eg(0.0, -1.0, 0.0),
	_Lda((lda* lda - 1) / (lda * lda + 1)), _rhop(0), _rhof(0), _nu(0), _gravity(0),
	iFluidInertTorq(true), _Mi(0),
	_swimAngVel(vectors3d(agentnum,vec3d::Zero()))
{
	return;
}

void InertialessSwimmer::initialize(const std::string& path, const Config& config) {
	double vswim = config.Read("vswim", 0.0);
	double B = config.Read("B", INFINITY);
	setMotility(vswim, B);
	double a = config.Read("radius", 0.0);
	double lda = config.Read("lambda", 1.0);
	double gravity = config.Read("gravity", 0.0);
	setShape(a, lda);
	setPhysics(1017, 1000, 1e-6, gravity);
	setVsettle();
	iFluidInertTorq = config.Read<bool>("fluid inertial torque", true);
	setInertialTorqueConst();
	cout << "Vsettle = " << vsettle << endl;
	int dim = config.Read("dimension", 3);
	setDim(dim);
	double initRange = config.Read("initRange", 2.0);
	setInitRange(initRange);
	return;
}

void InertialessSwimmer::reset() {
	if (_dimension == 2) {
		initOrient2D();
		initPos2D(_initRange, _initRange);
		//initPos2DPoint(1.0, 1.0, 2.00);

	}
	else if (_dimension == 3) {
		initOrient3D();
		//initPos3D(2 * M_PI, 2 * M_PI, 2 * M_PI);
		initPos3D(2  , 2 , 2 );
	}
	resetFlowInfo();
	return;
}

void InertialessSwimmer::setMotility(double vswim, double B) {
	for (unsigned i = 0; i < agentnum; i++) {
		const vec3d defalut_mdisp(0.0, 0.0, -1.0);
		this->vswim[i] = vswim;
		this->gyro[i] = 0.5 / B;
		this->mdisp[i] = defalut_mdisp;
	}
}



void InertialessSwimmer::getInfo() {
	envInfoAtPoint->infoAtPoint(pos, uf, gradu, gradv, gradw);
}

// convert fluid vels & gradients to agent frame
// calculate p1, p2, p3 at the same time
void InertialessSwimmer::convertFrame() {
	for (unsigned i = 0; i < agentnum; i++) {
		Eigen::Matrix3d Reul;
		Reul.row(0) = p1[i];
		Reul.row(1) = p2[i];
		Reul.row(2) = p3[i];
		//ee2R(ee[i], p1[i], p2[i], p3[i]);
		Eigen::Matrix3d gradmat(3, 3);
		gradmat.col(0) = gradu[i];
		gradmat.col(1) = gradv[i];
		gradmat.col(2) = gradw[i];
		Eigen::Matrix3d gradmatpf(3, 3);
		gradmatpf = matmul(matmul(Reul, gradmat), Reul.transpose());
		gradupf[i] = gradmatpf.col(0);
		gradvpf[i] = gradmatpf.col(1);
		gradwpf[i] = gradmatpf.col(2);
		ufpf[i] = matmul(Reul, uf[i]);	
	}
	return;
}

void InertialessSwimmer::update(double timestep) {
	
	vec3d vppf(3, 1), egpf(3, 1);
	vec3d t_gyro(3, 1), t_rotate(3, 1), t_deform(3, 1), angvel(3, 1);
	vec3d t_finert(3, 1);
	vec3d vslippf(3, 1);
	for (unsigned i = 0; i < agentnum; i++) {
		egpf = vec2vecpf(eg, p1[i], p2[i], p3[i]);
		// translation		
		vec3d setvel = vsettle.array() * egpf.array();
		vppf = ufpf[i] + setvel;
		
		/*cout << setvel << endl;*/
		vppf[2] += vswim[i];
		_updateTrans(i, vppf, timestep);

		//rotation
		t_gyro = mdisp[i].cross(egpf);
		t_gyro *= gyro[i];

		t_rotate[0] = 0.5 * (gradwpf[i][1] - gradvpf[i][2]);
		t_rotate[1] = 0.5 * (gradupf[i][2] - gradwpf[i][0]);
		t_rotate[2] = 0.5 * (gradvpf[i][0] - gradupf[i][1]);

		
		t_deform[0] = -_Lda * 0.5 * (gradvpf[i][2] + gradwpf[i][1]);
		t_deform[1] = _Lda * 0.5 * (gradupf[i][2] + gradwpf[i][0]);
		t_deform[2] = 0.0;

		vslippf = vppf - ufpf[i];
		t_finert[0] = -_Mi / _nu * vslippf[1] * vslippf[2];
		t_finert[1] = _Mi / _nu * vslippf[0] * vslippf[2];
		t_finert[2] = 0.0;

		angvel = t_gyro + t_rotate + t_deform + t_finert + _swimAngVel[i];

		_updateRot(i, angvel, timestep);

		if (isnan(pos[0](0))) {
			cout << "nan value in inertialess swimmers update" << endl;
		}

	}
	_istep1rot = false;
	_istep1trans = false;


}

// dump particle field
void InertialessSwimmer::dump(const char* path, int step) {
	ofstream os;
	string fullpath = string(path) + "/partfield.txt";
	os.open(fullpath, ios::out | ios::trunc);
	os.precision(8);
	os << scientific;
	for (unsigned i = 0; i < agentnum; i++) {
		os << pos[i][0] << " " << pos[i][1] << " " << pos[i][2] << " " <<
			p3[i][0] << " " << p3[i][1] << " " << p3[i][2] << " " <<
			vp_new[i][0] << " " << vp_new[i][1] << " " << vp_new[i][2] << " " <<
			uf[i][0] << " " << uf[i][1] << " " << uf[i][2];
		os << endl;
	}
	os.close();

	FILE* fp;
	fullpath = string(path) + "/partfield.bin";
	fp = fopen(fullpath.c_str(), "wb");
	for (unsigned i = 0; i < agentnum; i++) {
		fwrite(&pos[i][0], sizeof(double), 3, fp);
		fwrite(&p3[i][0], sizeof(double), 3, fp);
		fwrite(&vp_new[i][0], sizeof(double), 3, fp);
		fwrite(&uf[i][0], sizeof(double), 3, fp);
	}
	fclose(fp);
}

void InertialessSwimmer::setVsettle() {
	double mu = _nu * _rhof;
	double pi = M_PI;
	double kapa, x0, a0, b0, r0;
	if (a == 0.0) {
		vsettle = vec3d::Zero();
	}
	if (lda == 1.0) {
		double vs = 2.0 / 9.0 * (_rhop - _rhof) * pow(a,2) / mu * _gravity;
		vsettle = vec3d(vs, vs, vs);
		cout << vsettle << endl;
		return;
	}
	else if (lda > 1.0) {
		kapa = log((lda - sqrt(pow(lda,2) - 1)) / (lda + sqrt(pow(lda, 2) - 1)));
		x0 = - pow(a, 2) * lda / sqrt(pow(lda, 2) - 1) * kapa;
		a0 = pow(lda, 2) / (pow(lda, 2) - 1) + lda / (2.0 * pow((pow(lda, 2) - 1), 1.5)) * kapa;
		b0 = a0;
		r0 = -2.0 / (pow(lda, 2) - 1) - lda / pow((pow(lda, 2) - 1.0), 1.5) * kapa;
	}
	else if (lda < 1.0) {
		kapa = 2.0 * atan(lda / sqrt(1.0 - pow(lda, 2)));
		x0 = pow(a, 2) * lda / sqrt(1.0 - pow(lda, 2)) * (pi - kapa);
		a0 = -0.5 * lda / pow(sqrt(1.0 - pow(lda, 2)), 3) * (kapa - pi + 2.0 * lda * sqrt(1.0 - pow(lda, 2)));
		b0 = a0;
		r0 = 1.0 / pow(sqrt(1.0 - pow(lda, 2)), 3) * (lda * kapa - pi * lda + 2.0 * sqrt(1.0 - pow(lda, 2)));
	}

	double b = a;
	double c = lda * a;
	vec3d k = vec3d::Zero();
	k[0] = 16.0 * pi * a * b * c * 1.0 / (x0 + pow(a, 2) * a0);
	k[1] = 16.0 * pi * a * b * c * 1.0 / (x0 + pow(b, 2) * b0);
	k[2] = 16.0 * pi * a * b * c * 1.0 / (x0 + pow(c, 2) * r0);
	vsettle[0] = 6.0 * pi * a * lda * (1.0 / ((k[2] - k[0]) * pow(cos(pi / 2.0), 2) + k[0])) * 2.0 / 9.0 * (_rhop - _rhof) * pow(a, 2) / mu * _gravity;
	vsettle[1] = vsettle[0];
	vsettle[2] = 6.0 * pi * a * lda * (1.0 / ((k[2] - k[0]) * pow(cos(0.0), 2) + k[0])) * 2.0 / 9.0 * (_rhop - _rhof) * pow(a, 2) / mu * _gravity;
	return;
}

// return the F_{\beta}, used for calculating fluid inertia constant
double InertialessSwimmer::_Fbeta(double lda) {
	const double eps = 1e-15;
	double pi = M_PI;
	double ksi, e, t1up, t2up, t3up, tdown;
	if (abs(lda - 1.0) < eps) return 0;
	if (lda > 1.0) {
		ksi = lda / sqrt(pow(lda, 2) - 1);
		e = 1.0 / ksi;
		t1up = -pi * pow(e, 2) * (420 * e + 2240 * pow(e, 3) + 4249 * pow(e, 5) - 2152 * pow(e, 7));
		tdown = 315 * pow((pow(e, 2) + 1) * atanh(e) - e, 2) * ((1 - 3 * pow(e, 2)) * atanh(e) - e);
		t2up = +pi * pow(e, 2) * (420 + 3360 * pow(e, 2) + 1890 * pow(e, 4) - 1470 * pow(e, 6)) * atanh(e);
		t3up = -pi * pow(e, 2) * (1260 * e - 1995 * pow(e, 3) + 2730 * pow(e, 5) - 1995 * pow(e, 7)) * pow(atanh(e), 2);
		return (t1up + t2up + t3up) / tdown;
	}
	else if (lda < 1.0) {
		e = sqrt(1 - pow(lda, 2));
		tdown = 315 * sqrt(1 - pow(e, 2)) * (-e * sqrt(1 - pow(e, 2)) + (1 + 2 * pow(e, 2)) * asin(e)) * pow(e * sqrt(1 - pow(e, 2)) + (2 * pow(e, 2) - 1) * asin(e), 2);
		t1up = pi * pow(e, 3) * sqrt(1 - pow(e, 2)) * (-420 + 3500 * pow(e, 2) - 9989 * pow(e, 4) + 4757 * pow(e, 6));
		t2up = 210 * pi * pow(e, 2) * (2 - 24 * pow(e, 2) + 69 * pow(e, 4) - 67 * pow(e, 6) + 20 * pow(e, 8)) * asin(e);
		t3up = 105 * pi * pow(e, 3) * (12 - 17 * pow(e, 2) + 24 * pow(e, 4)) * pow(asin(e), 2) * sqrt(1 - pow(e, 2));
		return (t1up + t2up + t3up) / tdown;
	}
}
// setting fluid inertial torque constant Mi
double InertialessSwimmer::_solveMi(double lda) {
	const double eps = 1e-15;
	if (abs(lda - 1.0)<eps) {
		return 0;
	}
	if (lda < 1.0) {
		cout << "Warning: Mi for oblate particles is not defined!" << endl;
	}
	double a = 1.0;
	double apar = lda * a;
	double aorg = a;
	double F = _Fbeta(lda);
	double Aprime = 5.0 / (6.0 * M_PI) * F / (pow(lda, 2) + 1) * pow(max(lda, 1.0), 3);
	//double beta = log(lda + sqrt(pow(lda, 2) - 1)) / (lda * sqrt(pow(lda, 2) - 1));
	//double Corg = 8.0 * apar * aorg * (pow(lda, 4) - 1) / (9 * pow(lda, 2) * ((2 * pow(lda, 2) - 1) * beta - 1));
	double Corg = 0;
	double Iorg = (1 + pow(lda, 2)) / 5.0 * pow(aorg, 2);
	if (lda > 1.0) {
		double e = sqrt(1 - pow(lda, -2));
		double L = log((1 + e) / (1 - e));
		double YC = 4.0 / 3.0 * pow(e, 3) * (2 - pow(e, 2)) * pow(-2 * e + (1 + pow(e, 2)) * L, -1);
		Corg = 4.0 / 3.0 * pow(a, 2) * pow(lda, 3) * YC;
		//double XC = 4.0 / 3.0 * pow(e, 3) * (1 - pow(e, 2)) * pow(2 * e - (1 - pow(e, 2)) * L, -1);
		//Cpara = 4.0 / 3.0 * pow(a, 2) * pow(lda, 3) * XC;
	} 
	if (lda < 1.0) {
		double e = sqrt(1 - pow(lda, 2));
		double C = M_PI_2 - atan(sqrt(1 - pow(e, 2)) / e);
		double YC = 2.0 / 3.0 * pow(e, 3) * (2 - pow(e, 2)) * pow(e * sqrt(1 - pow(e, 2)) - (1 - 2 * pow(e, 2)) * C, -1);
		Corg = 4.0 / 3.0 * pow(a, 2) * YC;
	}

	double Mi = Aprime * Iorg / Corg;
	return Mi;
}

void InertialessSwimmer::setInertialTorqueConst() {
	this->_Mi = iFluidInertTorq ? _solveMi(this->lda) : 0.0;
	return;
}