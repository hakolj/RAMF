#pragma once
#ifndef ENVIRONMENTITF_H
#define ENVIRONMENTITF_H

#include "vectypedef.h"

class InfoAtPointAble {
public:
	virtual void infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) = 0;
};

#endif // !ENVIRONMENTITF_H

