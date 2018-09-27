// Lab 2: SIMD2 ~ Dalton Fox

#include <chrono>
#include <iostream>
#include <bitset>
#include "immintrin.h"
#include "pngops.h"
#include <math.h>

void scale(Image & image, Image & out_image)
{
	if (image.sixteenBit) 
	{
		uint64_t * data = (uint64_t *)image.data;
		uint64_t * out_data = (uint64_t *)out_image.data;
		uint64_t pixel = 0;
		for (int y = 0; y < image.h; y++) 
		{
			for (int x = 0; x < image.w; x++) 
			{
				int y2 = y * 2;
				int x2 = x * 2;
				// Get pixel of origin image and copy it four times
				pixel = data[(image.w * y) + x];
				out_data[(out_image.w * y2) + x2] = pixel;
				out_data[(out_image.w * (y2 + 1)) + x2] = pixel;
				out_data[(out_image.w * (y2 + 1)) + x2 + 1] = pixel;
				out_data[(out_image.w * y2) + x2 + 1] = pixel;
			}
		}
	}
	else 
	{
		uint32_t * data = (uint32_t *)image.data;
		uint32_t * out_data = (uint32_t *)out_image.data;
		uint32_t pixel = 0;
		for (int y = 0; y < image.h; y++) 
		{
			for (int x = 0; x < image.w; x++) 
			{
				int y2 = y * 2;
				int x2 = x * 2;
				// Get pixel of origin image and copy it four times
				pixel = data[(image.w * y) + x];
				out_data[(out_image.w * y2) + x2] = pixel;
				out_data[(out_image.w * (y2 + 1)) + x2] = pixel;
				out_data[(out_image.w * (y2 + 1)) + x2 + 1] = pixel;
				out_data[(out_image.w * y2) + x2 + 1] = pixel;
			}
		}
	}
}

void scaleSIMD(Image & image, Image & out_image)
{
	/*couldn't continue testing this past the white board because Ryzen 7 1700x doesn't seem to like any mm256 instructions
	the program basically quits as soon as it runs one, no errors or warnings, just moves on to the return of main.*/

	alignas(8) uint32_t r1[8];
	alignas(8) uint32_t r2[8];
	__m256i b1, b2, s1, s2, idx_r1, idx_r2;
	const uint32_t * data = (const uint32_t *) image.data;
	const uint32_t * outdata = (const uint32_t *) out_image.data;

	// For ever row in our image split it up into sections of 32 and copy the data 4 times by shuffling it around
	for (int y = 0; y < image.h; y++)
	{
		for (int x = 0; x < image.w / 32; x++)
		{
			for (int i = 0; i < 4; i++)
			{
				r1[i] = i + (1 * x); r1[i + 1] = i + (1 * x);
				r2[i] = (i + (1 * x) * 2); r2[i + 1] = (i + (1 * x) * 2);
			}
			b1 = _mm256_load_si256((__m256i const *) (data + x));
			b2 = _mm256_load_si256((__m256i const *) (data + x));
			idx_r1 = _mm256_load_si256((__m256i const *) r1);
			idx_r2 = _mm256_load_si256((__m256i const *) r2);
			s1 = _mm256_permutevar8x32_epi32(b1, idx_r1);
			s2 = _mm256_permutevar8x32_epi32(b2, idx_r2);
			_mm256_store_si256((__m256i *) (outdata + x), s1);
			_mm256_store_si256((__m256i *) (outdata + x), s2);
		}
	}
}

int main(int argc, char *argv[])
{
	int count;
	for (count = 0; count < argc; count++)
	{
		std::string inputpath(argv[count]);

		if (inputpath.substr(inputpath.length() - 4) == ".png")
		{
			std::cout << "Scaling Image: \"" << inputpath << "\"\n" << std::endl;

			// Input specified image for a non SIMD conversion
			Image inputimg(inputpath);
			Image outimg;
			outimg.allocate(inputimg.w * 2, inputimg.h * 2, false);

			// Start timing and the non AVX conversion
			auto start = std::chrono::high_resolution_clock::now();
			scale(inputimg, outimg);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;

			// Print time taken and write scaled image
			std::cout << "Time to scale with CPU instructions: " << diff.count() * 1000 << "ms\n" << std::endl;
			outimg.write("output/n.png");

			// Start timing and the AVX conversion
			start = std::chrono::high_resolution_clock::now();
			scaleSIMD(inputimg, outimg);
			end = std::chrono::high_resolution_clock::now();
			diff = end - start;

			// Print time taken and write scaled image
			std::cout << "Time to scale with AVX instructions: " << diff.count() * 1000 << "ms\n" << std::endl;
			outimg.write("output/s.png");
		}
	}
	return 0;
}