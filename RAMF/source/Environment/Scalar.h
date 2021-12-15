#pragma once
#ifndef SCALAR_H
#define SCALAR_H


#include "pch.h"
#include "Geometry.h"

enum FieldStoreType {
	UDF = 0, //undefined
	CCC = 1, //Center-Center-Center: uvw stored at the center of grid
	CYC = 2 // Center-Edge-Center: uw stored at the center of grid, v at the center of surface in y direction
};


//
class Scalar
{
public:
	const Mesh ms;
	Scalar(const Mesh& ms);
	~Scalar();
	Scalar(const Scalar& src);
	Scalar& operator=(const Scalar& src);

	Scalar& Set(const Scalar& src);
	Scalar& Set(double val);
	// memory access
	double& operator()(int i, int j, int k) { return q_[ms.idx(i, j, k)]; };
	double  operator()(int i, int j, int k) const { return q_[ms.idx(i, j, k)]; };
	friend std::ostream& operator<<(std::ostream& os, const Scalar& scl);

	double& operator[](int id) { return q_[id]; };
	double  operator[](int id) const { return q_[id]; };
	double& val(int i, int j, int k) { return q_[ms.idx(i, j, k)]; };
	double val(int i, int j, int k) const { return q_[ms.idx(i, j, k)]; }


	double* GetLyr(int j = 0) { return &q_[ms.idx(0, j, 0)]; };
	const double* SeeLyr(int j = 0) const { return &q_[ms.idx(0, j, 0)]; };

	double* GetBlk() { return &q_[ms.idx(0, 0, 0)]; };
	const double* SeeBlk() const { return &q_[ms.idx(0, 0, 0)]; };

	// interpolation (among faces, edges and cell-centers)
	void Ugrid2CellCenter(Scalar& dst) const;
	void Vgrid2CellCenter(Scalar& dst) const;
	void Wgrid2CellCenter(Scalar& dst) const;
	void CellCenter2EdgeX(Scalar& dst) const;
	void CellCenter2EdgeY(Scalar& dst) const;
	void CellCenter2EdgeZ(Scalar& dst) const;

	// differential operators (operating on cell-centered quantities)
	const double* Gradient(int i, int j, int k) const;
	void GradientAtCenter_old(Scalar& gradx, Scalar& grady, Scalar& gradz) const;

	void GradientAtCenter(Scalar& gradx, Scalar& grady, Scalar& gradz, const char storeType) const;
	
	static void loadVelFromFortran(const char* path, Scalar& u, int numrank);


	// IO functions
	void FileIO(const char* path, const char* name, char mode) const;
	void debug_AsciiOutput(const char* path, const char* name, int j1, int j2) const;
	double mean() const;
private:
	const int Nx, Ny, Nz;
	// pointer to the bulk memory
	double* q_;

};


#endif // !SCALAR_H