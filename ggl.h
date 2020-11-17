#pragma once
#include <windows.h>
#include "glew.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#define PI 3.141592f
struct Vertex {
	float mPosition[4];
	float mTexcoord[4];
	float mNormal[4];
	float mTangent[4];
};