// Dalton Fox ~ Lab 6: CS2 (Raytracer)

#include <map>
#include "glfuncs.h"
#include "SDL.h"
#include "Program.h"
#include "Buffer.h"
#include "ray.h"

#define SHADER_LOCAL_SIZE (128)
#define IMAGE_SIZE (10752)

// GLOBALS
unsigned item_count = 0;
Buffer data1_buffer;
Buffer data2_buffer;
Buffer color_buffer;
Buffer type_buffer;
Buffer misc_buffer;
Buffer image_buffer;

// Send to GPU
void map_data(std::vector<Object>& objects, vec4& lightPos)
{
    {
        unsigned index = 0;
        float * fdata = (float *) data1_buffer.map();
        for (auto& object : objects)
        {
            fdata[index] = object.data1.x;
            fdata[index + 1] = object.data1.y;
            fdata[index + 2] = object.data1.z;
            fdata[index + 3] = object.data1.w;
            index += 4;
        }
        data1_buffer.unmap();
    }

    {
        unsigned index = 0;
        float * fdata = (float *) data2_buffer.map();
        for (auto& object : objects)
        {
            fdata[index] = object.data2.x;
            fdata[index + 1] = object.data2.y;
            fdata[index + 2] = object.data2.z;
            fdata[index + 3] = object.data2.w;
            index += 4;
        }
        data2_buffer.unmap();
    }

    {
        unsigned index = 0;
        float * fdata = (float *) color_buffer.map();
        for (auto& object : objects)
        {
            fdata[index] = object.color.x;
            fdata[index + 1] = object.color.y;
            fdata[index + 2] = object.color.z;
            fdata[index + 3] = object.color.w;
            index += 4;
        }
        color_buffer.unmap();
    }

    {
        unsigned index = 0;
		uint32_t * idata = (uint32_t *) type_buffer.map();
        for (auto& object : objects)
        {
            idata[index] = object.type;
            index++;
        }
        type_buffer.unmap();
    }

    {
        void * ptr = misc_buffer.map();
        float * fdata = (float *) ptr;
        fdata[0] = IMAGE_SIZE;
        fdata[1] = IMAGE_SIZE;
        fdata[2] = item_count;
        fdata[3] = lightPos.x;
        fdata[4] = lightPos.y;
        fdata[5] = lightPos.z;
        for (unsigned i = 0; i < 9; ++i)
            fdata[i + 6] = 0;
        misc_buffer.unmap();
    }

    data1_buffer.bind(0);
    data2_buffer.bind(1);
    color_buffer.bind(2);
    type_buffer.bind(3);
	misc_buffer.bind(4);
    image_buffer.bind(5);
}

// Program Main
int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage: foo.exe bar.txt" << std::endl;
        return 1;
    }

	// SDL / GL Initialization
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * win = SDL_CreateWindow("ETGG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, IMAGE_SIZE, IMAGE_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	// Error Handle
	SDL_GLContext glContext = SDL_GL_CreateContext(win);
	if (0 == SDL_GL_CreateContext(win))
	{
		std::cout << "Cannot create GL context\n";
		exit(-1);
	}

	// Debug Setup
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, (GLboolean)true);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEBUG_OUTPUT);

    // Initialization
	Image raytrace(IMAGE_SIZE, IMAGE_SIZE, false);
    std::vector<Object> objects;
    vec4 eye, coi, up, lightPos;
    load_data(argv[1], eye, coi, up, lightPos, objects);
    item_count = objects.size();
	
	// Start clock
	auto start = std::chrono::high_resolution_clock::now();

	// Load and Use Compute Shader
	Program main_compute("shader//main.shader");
	main_compute.use();

	// Initialize data buffers
	data1_buffer.setup(GL_SHADER_STORAGE_BUFFER, item_count * sizeof(float) * 4);
	data2_buffer.setup(GL_SHADER_STORAGE_BUFFER, item_count * sizeof(float) * 4);
	color_buffer.setup(GL_SHADER_STORAGE_BUFFER, item_count * sizeof(float) * 4);
	type_buffer.setup(GL_SHADER_STORAGE_BUFFER, item_count * sizeof(uint32_t));
	misc_buffer.setup(GL_SHADER_STORAGE_BUFFER, (4 * sizeof(float) * 3) + (sizeof(uint32_t) * 3));
	image_buffer.setup(GL_SHADER_STORAGE_BUFFER, IMAGE_SIZE * IMAGE_SIZE * 4);

	// Map object data to the GPU
	map_data(objects, lightPos);
	
	// Map miscellaneous data to GPU
	float * fdata = (float *) misc_buffer.map();
	fdata[6] = eye.x;
	fdata[7] = eye.y;
	fdata[8] = eye.z;
	fdata[9] = coi.x;
	fdata[10] = coi.y;
	fdata[11] = coi.z;
	fdata[12] = up.x;
	fdata[13] = up.y;
	fdata[14] = up.z;
	misc_buffer.unmap();

	// Run Shader
	main_compute.dispatch(IMAGE_SIZE / SHADER_LOCAL_SIZE, IMAGE_SIZE, 1);
	
	// Get Data From GPU
	uint8_t * colors = (uint8_t *)image_buffer.map();
	memcpy(raytrace.data, colors, IMAGE_SIZE * IMAGE_SIZE * 4);
	image_buffer.unmap();

	// Save image
	raytrace.write("output//raytrace_x" + std::to_string(IMAGE_SIZE) + ".png");
    
	// End clock
	auto end = std::chrono::high_resolution_clock::now();
	auto time_taken = std::chrono::duration<double>(end - start);
	std::cout << "Time to render with GPU: " << time_taken.count() << " seconds\n";

	// Finalize
	SDL_Quit();
    return 0;
}
