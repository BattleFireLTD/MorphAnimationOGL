#pragma once
#include "ggl.h"
#include "Geometry.h"

class Model : public Geometry {
public:
	void SetModelPath(const char*path);
};