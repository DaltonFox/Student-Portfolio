#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "pngops.h"

struct vec4
{
    float x, y, z, w;

    vec4();
    vec4(float a, float b, float c, float d);
};

struct mat4
{
    float data[4][4];
    
    mat4();
    mat4(vec4 row1, vec4 row2, vec4 row3, vec4 row4);
};

struct Object
{
    vec4 data1;
    vec4 data2;
    vec4 color;
    uint32_t type;
};

void load_data(char* file_name, vec4& eye, vec4& coi, vec4& up, vec4& lightPos, std::vector<Object>& objects);
