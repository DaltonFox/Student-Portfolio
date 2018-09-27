#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "glfuncs.h"

class Program
{
public:
    Program();
    explicit Program(std::string csfile);

    void load(std::string csfile);
    void use();
    void dispatch(unsigned xsize, unsigned ysize, unsigned zsize);

private:
    GLuint prog;
};
