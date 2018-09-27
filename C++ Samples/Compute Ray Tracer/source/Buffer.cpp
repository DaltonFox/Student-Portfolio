#include "Buffer.h"

Buffer::Buffer() = default;

Buffer::~Buffer()
{

}

Buffer::Buffer(unsigned attachment, int bytesize)
{
    setup(attachment, bytesize);
}

Buffer::Buffer(int bytesize)
{
    setup(GL_SHADER_STORAGE_BUFFER, bytesize);
}

void Buffer::setup(unsigned attachment, int bytesize)
{
    this->bytesize = bytesize;
    this->attachment = attachment;
    glGenBuffers(1, &ID);
    glBindBuffer(this->attachment, ID);
    glBufferData(attachment, bytesize, NULL, GL_STREAM_READ);
    glBindBuffer(this->attachment, 0);
}

void* Buffer::map()
{
    glBindBuffer(attachment, ID);
    return glMapBuffer(attachment, GL_READ_WRITE);
}

void Buffer::unmap()
{
    glBindBuffer(attachment, ID);
    glUnmapBuffer(attachment);
    glBindBuffer(attachment, 0);
}

void Buffer::bind(int slot)
{
    if (slot == -1)
    { 
		glBindBuffer(attachment, ID); 
	}
    else
    { 
		glBindBufferBase(attachment, (GLuint) slot, ID); 
	}
}

void Buffer::unbind(int slot)
{
    if (slot != -1)
    { 
		glBindBufferBase(attachment, (GLuint) slot, 0); 
	}
    glBindBuffer(attachment, 0);
}

void Buffer::bindAsVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void Buffer::unbindAsVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::update(int size, void * p)
{
    bind(-1);
    glBufferSubData(attachment, 0, size, p);
}
