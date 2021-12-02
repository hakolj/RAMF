#include "pch.h"
#include "Scalar.h"

using namespace std;

Scalar::Scalar(const Mesh& ms):
	ms(ms),
	Nx(ms.Nx),
	Ny(ms.Ny),
	Nz(ms.Nz)
{
	q_ = new double[(Nx + 1) * (Ny + 1) * (Nz + 1)];
	Set(0.0);
}

Scalar::~Scalar()
{
	delete[] q_;
}

Scalar::Scalar(const Scalar& src) :
	Scalar(src.ms)
{
	Set(src);
}

Scalar& Scalar::operator=(const Scalar& src)
{
	if (&src == this) return *this;
	if (src.ms.Nx != ms.Nx ||
		src.ms.Ny != ms.Ny ||
		src.ms.Nz != ms.Nz)
	{
		cout << "Scalar sizes do not match !" << endl;
		exit(0);
	}
	cout << "Warning: if you just want to assign values, use Set() instead." << endl;
	return Set(src);
}

Scalar& Scalar::Set(const Scalar& src) {
	int length = (Nx + 1) * (Ny + 1) * (Nz + 1);
	for (int i = 0; i < length; i++) {
		this->q_[i] = src.q_[i];
	}
	return *this;
}

Scalar& Scalar::Set(double val) {
	int length = (Nx + 1) * (Ny + 1) * (Nz + 1);
	for (int i = 0; i < length; i++) {
		this->q_[i] = val;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Scalar& scl) {
	for (int j = 0; j <= scl.Ny; j++) {
		os << "j = " << j << " :" << endl;
		for (int k = 0; k <= scl.Nz; k++) {
			for (int i = 0; i <= scl.Nx; i++) {
				os << scl(i, j, k) << " ";
			}
			os << endl;
		}
	}
	return os;
}

/***** interpolate from faces to cell-centers *****/

// way to deal with real-to-virtual booundary interpolation:
// linear extrapolation, without special consideration for periodic scenario

void Scalar::Ugrid2CellCenter(Scalar& dst) const
{
	double c1 = .5 * ms.dx(0) / ms.dx(1);
	double c2 = .5 * ms.dx(Nx) / ms.dx(Nx - 1);

#pragma omp parallel for
	for (int j = 0; j <= Ny; j++) {
		for (int k = 0; k <= Nz; k++) {
			for (int i = 1; i < Nx; i++) {
				int id = ms.idx(i, j, k);
				int ip = ms.idx(ms.ipa(i), j, k);
				dst(i, j, k) = .5 * (q_[id] + q_[ip]);
			}
			dst(0, j, k) = (1 + c1) * q_[ms.idx(1, j, k)] - c1 * q_[ms.idx(2, j, k)];
			dst(Nx, j, k) = (1 + c2) * q_[ms.idx(Nx, j, k)] - c2 * q_[ms.idx(Nx - 1, j, k)];
		}
	}
}
void Scalar::Vgrid2CellCenter(Scalar& dst) const
{
	double c1 = .5 * ms.dy(0) / ms.dy(1);
	double c2 = .5 * ms.dy(Ny) / ms.dy(Ny - 1);

#pragma omp parallel for
	for (int k = 0; k <= Nz; k++) {
		for (int i = 0; i <= Nx; i++) {
			for (int j = 1; j < Ny; j++) {
				int id = ms.idx(i, j, k);
				int jp = ms.idx(i, ms.jpa(j), k);
				dst(i, j, k) = .5 * (q_[id] + q_[jp]);
			}
			dst(i, 0, k) = (1 + c1) * q_[ms.idx(i, 1, k)] - c1 * q_[ms.idx(i, 2, k)];
			dst(i, Ny, k) = (1 + c2) * q_[ms.idx(i, Ny, k)] - c2 * q_[ms.idx(i, Ny - 1, k)];
		}
	}
}
void Scalar::Wgrid2CellCenter(Scalar& dst) const
{
	double c1 = .5 * ms.dz(0) / ms.dz(1);
	double c2 = .5 * ms.dz(Nz) / ms.dz(Nz - 1);

#pragma omp parallel for
	for (int j = 0; j <= Ny; j++) {
		for (int i = 0; i <= Nx; i++) {
			for (int k = 1; k < Nz; k++) {
				int id = ms.idx(i, j, k);
				int kp = ms.idx(i, j, ms.kpa(k));
				dst(i, j, k) = .5 * (q_[id] + q_[kp]);
			}
			dst(i, j, 0) = (1 + c1) * q_[ms.idx(i, j, 1)] - c1 * q_[ms.idx(i, j, 2)];
			dst(i, j, Nz) = (1 + c2) * q_[ms.idx(i, j, Nz)] - c2 * q_[ms.idx(i, j, Nz - 1)];
		}
	}
}


/***** interpolate from cell-centers to edges *****/

void Scalar::CellCenter2EdgeX(Scalar& dst) const
{
#pragma omp parallel for
	for (int j = 1; j <= Ny; j++) {
		double dym, dyp, dyc = ms.dy(j, dym, dyp), hyc = ms.hy(j);
		for (int k = 1; k <= Nz; k++) {
			double dzm, dzp, dzc = ms.dz(k, dzm, dzp), hzc = ms.hz(k);
			for (int i = 0; i <= Nx; i++) {

				int id = ms.idx(i, j, k);
				int jmkm = ms.idx(i, ms.jma(j), ms.kma(k));
				int im, jm, km;       ms.imx(i, j, k, im, jm, km);

				dst[id] = .25 / hyc / hzc * (
					dym * dzm * q_[id] + dyc * dzm * q_[jm] +
					dym * dzc * q_[km] + dyc * dzc * q_[jmkm]);
			}
		}
	}
}
void Scalar::CellCenter2EdgeY(Scalar& dst) const
{
#pragma omp parallel for
	for (int j = 0; j <= Ny; j++) {
		for (int k = 1; k <= Nz; k++) {
			double dzm, dzp, dzc = ms.dz(k, dzm, dzp), hzc = ms.hz(k);
			for (int i = 1; i <= Nx; i++) {
				double dxm, dxp, dxc = ms.dx(i, dxm, dxp), hxc = ms.hx(i);

				int id = ms.idx(i, j, k);
				int imkm = ms.idx(ms.ima(i), j, ms.kma(k));
				int im, jm, km;       ms.imx(i, j, k, im, jm, km);

				dst[id] = .25 / hxc / hzc * (
					dxm * dzm * q_[id] + dxc * dzm * q_[im] +
					dxm * dzc * q_[km] + dxc * dzc * q_[imkm]);
			}
		}
	}
}
void Scalar::CellCenter2EdgeZ(Scalar& dst) const
{
#pragma omp parallel for
	for (int j = 1; j <= Ny; j++) {
		double dym, dyp, dyc = ms.dy(j, dym, dyp), hyc = ms.hy(j);
		for (int k = 0; k <= Nz; k++) {
			for (int i = 1; i <= Nx; i++) {
				double dxm, dxp, dxc = ms.dx(i, dxm, dxp), hxc = ms.hx(i);

				int id = ms.idx(i, j, k);
				int imjm = ms.idx(ms.ima(i), ms.jma(j), k);
				int im, jm, km;       ms.imx(i, j, k, im, jm, km);

				dst[id] = .25 / hxc / hyc * (
					dxm * dym * q_[id] + dxc * dym * q_[im] +
					dxm * dyc * q_[jm] + dxc * dyc * q_[imjm]);
			}
		}
	}
}


/***** differentiation operators *****/

const double* Scalar::Gradient(int i, int j, int k) const
/* compute Gradient of a cell-centered scalar field to corresponding U,V,W grids */

{
	static double grad[3]; // will be overwritten even called from different objects of this class

	int id = ms.idx(i, j, k);
	int im, jm, km; ms.imx(i, j, k, im, jm, km);

	grad[0] = (q_[id] - q_[im]) / ms.hx(i); // [1,Nx], [0,Ny], [0,Nz]
	grad[1] = (q_[id] - q_[jm]) / ms.hy(j); // [0,Nx], [1,NY], [0,Nz]
	grad[2] = (q_[id] - q_[km]) / ms.hz(k); // [0,Nx], [0,NY], [1,Nz]

	return grad; // CAUTION: avoid successive calling to this function, because the static return variable will be overwritten every time
}

//using cell-centered scalar to compute Gradient at center. 2nd order centerd difference. Uniform mesh.
void Scalar::GradientAtCenter_old(Scalar& gradx, Scalar& grady, Scalar& gradz) const {
	//for periodic boundary
	//for (int j = 0; j <= Ny; j++) {
	//	for (int k = 0; k <= Nz; k++)
	//		val(0, j, k) = val(Nx, j, k);
			//q_[ms.idx(0, j, k)] = q_[ms.idx(Nx, j, k)];
		
	//}
	//cout << *this << endl;

#pragma omp parallel for
	for (int j = 1; j <= Ny-1; j++) {
		for (int k = 1; k <= Nz-1; k++) {
			for (int i = 1; i <= Nx-1; i++) {
				int im, jm, km;
				ms.imx(i, j, k, im, jm, km);
				int ip, jp, kp;
				ms.ipx(i, j, k, ip, jp, kp);
				
				gradx(i, j, k) = 0.5 * (q_[ip] - q_[im]) / ms.hx(i);
				grady(i, j, k) = 0.5 * (q_[jp] - q_[jm]) / ms.hy(j);
				gradz(i, j, k) = 0.5 * (q_[kp] - q_[km]) / ms.hz(k);
			}
		}
	}
	return;
}


//using cell-centered scalar to compute Gradient at center. 2nd order centerd difference. Uniform mesh.
void Scalar::GradientAtCenter(Scalar& gradx, Scalar& grady, Scalar& gradz, const char storeType) const {
	//for periodic boundary
	//for (int j = 0; j <= Ny; j++) {
	//	for (int k = 0; k <= Nz; k++)
	//		val(0, j, k) = val(Nx, j, k);
			//q_[ms.idx(0, j, k)] = q_[ms.idx(Nx, j, k)];

	//}
	//cout << *this << endl;

	if (storeType == 'C') {

#pragma omp parallel for
		for (int j = 1; j <= Ny - 1; j++) {
			for (int k = 1; k <= Nz - 1; k++) {
				for (int i = 1; i <= Nx - 1; i++) {
					int im, jm, km;
					ms.imx(i, j, k, im, jm, km);
					int ip, jp, kp;
					ms.ipx(i, j, k, ip, jp, kp);

					gradx(i, j, k) = (q_[ip] - q_[im]) / (ms.hx(i) + ms.hx(i + 1));
					grady(i, j, k) = (q_[jp] - q_[jm]) / (ms.hy(j) + ms.hy(j + 1));
					gradz(i, j, k) = (q_[kp] - q_[km]) / (ms.hz(k) + ms.hz(k + 1));
				}
			}
		}
	}
	else if (storeType == 'E') {
//#pragma omp parallel for
		for (int j = 1; j <= Ny - 1; j++) {
			for (int k = 1; k <= Nz - 1; k++) {
				for (int i = 1; i <= Nx - 1; i++) {
					int im, jm, km;
					ms.imx(i, j, k, im, jm, km);
					int ip, jp, kp;
					ms.ipx(i, j, k, ip, jp, kp);

					gradx(i, j, k) = (q_[ip] - q_[im]) / (ms.hx(i) + ms.hx(i + 1));
					grady(i, j, k) = (q_[jp] - q_[j]) / ms.dy(j);
					gradz(i, j, k) = (q_[kp] - q_[km]) / (ms.hz(k) + ms.hz(k + 1));
#if DEBUG
					if (isinf(grady(i, j, k))) {
						cout<<"nan in makegradient center"<<endl;
					}
#endif
				}
			}
		}

	}
	return;
}


/***** IO *****/

void Scalar::FileIO(const char* path, const char* name, char mode) const
/* read & write field from & to binary files */
{
	FILE* fp;
	char str[1024];

	sprintf(str, "%s%s.bin", path, name);

	fp = fopen(str, mode == 'w' ? "wb" : "rb");
	if (fp == NULL) {
		cout << "File not found: "<< str << endl;
		exit(0);
	}

	// write domain information at the beginning
	int n1 = Nx + 1;
	int n2 = Ny + 1;
	int n3 = Nz + 1;
	if (mode == 'w') {
		fwrite(&n1, sizeof(int), 1, fp);
		fwrite(&n2, sizeof(int), 1, fp);
		fwrite(&n3, sizeof(int), 1, fp);
	}
	// data begin after the info section
	fseek(fp, sizeof(int) * 3, SEEK_SET);

	if (mode == 'w') fwrite(q_, sizeof(double) * (Nx + 1) * (Nz + 1), Ny + 1, fp);
	if (mode == 'r') fread(q_, sizeof(double) * (Nx + 1) * (Nz + 1), Ny + 1, fp);



	fclose(fp);
}

void Scalar::debug_AsciiOutput(const char* path, const char* name, int j1, int j2) const
/* write the fields in ascii files for check */
{
	FILE* fp;
	char str[1024];

	sprintf(str, "%s%s.txt", path, name);

	fp = fopen(str, "w");
	for (int j = j1; j < j2; j++) {
		fputc('\n', fp);
		for (int k = 0; k <= Nz; k++) {
			fputc('\n', fp);
			for (int i = 0; i <= Nx; i++) {
				fprintf(fp, "%.6f\t", q_[ms.idx(i, j, k)]);
			}
		}
	}
	fclose(fp);
}

double Scalar::mean() const {
	double sum = 0;
	int count = 0;

	for (int i = 0; i < Nx + 1; i++) {
		for (int j = 0; j < Ny + 1; j++) {
			for (int k = 0; k < Nz + 1; k++) {
				count++;
				sum += q_[ms.idx(i,j,k)];
				if (abs(q_[ms.idx(i, j, k)]) > 1e15) {
					cout << "?" << endl;
				}
			}
		}
	}
	cout << count;
	//for (int i = 0; i < (Nx + 1) * (Ny + 1) * (Nz + 1); i++) {
	//	sum += q_[i];
	//	if (abs(q_[i]) > 1e15) {
	//		cout << "?" << endl;
	//	}
	//}
	return sum / ((Nx + 1) * (Ny + 1) * (Nz + 1));
}


void Scalar::loadVelFromFortran(const char* path, Scalar& u, int numrank) {
	/*FILE* fp;
	
	fp = fopen(path, "rb");
	int proc_r;
	int proc_c;
	fread(&proc_r, sizeof(int), 1, fp);
	fread(&proc_c, sizeof(int), 1, fp);
	int nodes[3];
	fread(nodes, sizeof(int), 3, fp);
	double gridspace[3];
	fread(gridspace, sizeof(double), 3, fp);
	int *dump_pos = new int[numrank];
	fread(dump_pos, sizeof(int), numrank, fp);

	if ((u.ms.Nx != nodes[0]) || (u.ms.Ny != nodes[1]) || (u.ms.Nz != nodes[2])) {
		cout<<"Mesh shape dismatch when laodVelFromFortran"<<endl;
		fclose(fp);
		return;
	}

	int nx2 = nodes[0] / proc_c;
	int nz2 = nodes[2] / proc_r;
	int ndata = nx2 * nodes[1] * nz2;

	int rank = 0;
	for (int nrow = 1; nrow <= proc_r; nrow++) {
		for (int ncol = 1; ncol <= proc_c; ncol++) {
			rank++;
			fseek(fp, dump_pos[rank - 1] - 1, SEEK_SET);
			int rankcur;
			fread(&rankcur, sizeof(int), 1, fp);
			if (rankcur != rank - 1) {
				cout << "Rank id error when loadVelFromFortran." << endl;
				fclose(fp);
				return;
			}




		}
	}*/
}
