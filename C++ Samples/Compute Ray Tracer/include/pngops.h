#pragma once

#include <png.h>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>

class Image
{
	public:
		// Members
		unsigned w;
		unsigned h;
		bool sixteenBit;
		uint8_t * data;
    
		// Constructor Overloads
		Image();
		Image(std::string infile);
		Image(unsigned w, unsigned h, bool sixteenBit);
		Image(const Image& i2);
		Image(Image&& i2);

		// Methods
		void allocate(unsigned w, unsigned h, bool sixteenBit);
		void read(std::string infile);
		void write(std::string fname) const;
    
	private:
		// Members
		std::vector<uint8_t> datav;
		void makePointer();
		struct FileHandle
		{
			FILE * fp;
			FileHandle(FILE * fp);
			~FileHandle();
		};
};
