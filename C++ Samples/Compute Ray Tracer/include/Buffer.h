#pragma once

#include "glfuncs.h"

class Buffer
{
	public:
		// Constructor / Deconstructor
		Buffer();
		~Buffer();
		explicit Buffer(int bytesize);
		explicit Buffer(unsigned attachment, int bytesize);
    
		// Methods
		void setup(unsigned attachment, int bytesize);
		void * map();
		void unmap();
		void bind(int slot);
		void unbind(int slot);
		void bindAsVertexBuffer();
		void unbindAsVertexBuffer();
		void update(int size, void * p);
    
		GLuint getID()
		{ 
			return ID; 
		}

	private:
		// Members
		GLuint ID;
		int bytesize;
		unsigned attachment;
};
