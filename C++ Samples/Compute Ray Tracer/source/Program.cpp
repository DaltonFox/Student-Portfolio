#include "Program.h"

#include <utility>

Program::Program() = default;

Program::Program(std::string csfile)
{
    load(std::move(csfile));
}

void Program::load(std::string csfile)
{
    std::ifstream in(csfile.c_str());
    if (!in.good())
    {
        throw std::runtime_error("Cannot load " + csfile);
    }

    std::string src;
    std::getline(in, src, '\0');
    GLuint handle = glCreateShader(GL_COMPUTE_SHADER);
    GLint length = src.length();
    const char* S[] = {src.c_str()};
    glShaderSource(handle, 1, S, &length);
    glCompileShader(handle);
    char infolog[4096];
    glGetShaderInfoLog(handle, sizeof(infolog), &length, infolog);
    if (length > 0)
    {
        std::cout << "When compiling " << csfile << ":\n";
        std::cout << infolog << "\n";
    }
    int status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        throw std::runtime_error("Cannot compile shader " + csfile);
    }
    prog = glCreateProgram();
    glAttachShader(prog, handle);
    glLinkProgram(prog);
    glGetProgramInfoLog(prog, sizeof(infolog), &length, infolog);
    if (length > 0)
    {
        std::cout << "When linking " << csfile << ":\n";
        std::cout << infolog << "\n";
    }
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        throw std::runtime_error("Could not link " + csfile);
    }
}

void Program::use()
{
    glUseProgram(prog);
}

void Program::dispatch(unsigned xsize, unsigned ysize, unsigned zsize)
{
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(xsize, ysize, zsize);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
