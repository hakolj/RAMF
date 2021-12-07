#pragma once
#ifndef TASKINTTF_H
#define TASKINTTF_H

#include "vectypedef.h"

// to obtain the target of navigation for a task
class GetTargetable {
public:
	virtual vec3d getTarget() = 0;
};


#endif // !TASKINTTF_H

