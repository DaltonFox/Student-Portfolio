#include "ray.h"

vec4::vec4(float a, float b, float c, float d)
{
    this->x = a;
    this->y = b;
    this->z = c;
    this->w = d;
}

vec4::vec4()
{
    x = y = z = w = 0;
}

void load_data(char* file_name, vec4& eye, vec4& coi, vec4& up, vec4& lightPos, std::vector<Object>& objects)
{
    std::ifstream fp(file_name);
    for(;;)
    {
        std::string line;
        std::getline(fp, line);
        if (fp.fail())
        {
            break;
        }
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        if (word == "eye")
        {
            float cameraX, cameraY, cameraZ, coiX, coiY, coiZ, upX, upY, upZ;
            iss >> cameraX >> cameraY >> cameraZ;
            iss >> coiX >> coiY >> coiZ;
            iss >> upX >> upY >> upZ;
            eye = vec4(cameraX, cameraY, cameraZ, 1);
            coi = vec4(coiX, coiY, coiZ, 1);
            up = vec4(upX, upY, upZ, 0);
        }
        else if (word == "light")
        {
            float lightX, lightY, lightZ;
            iss >> lightX;
            iss >> lightY;
            iss >> lightZ;
            lightPos = vec4(lightX, lightY, lightZ, 1);
        }
        else if (word == "sphere")
        {
            float cx, cy, cz, radius, r, g, b, refl;
            iss >> cx >> cy >> cz >> radius >> r >> g >> b >> refl;
            vec4 center = vec4(cx, cy, cz, 1);
            Object obj;
            obj.data1 = center;
            obj.data1.w = radius;
            obj.color = vec4(r, g, b, refl);
            obj.type = 0;
            objects.push_back(obj);
        }
        else if (word == "plane")
        {
            float nx, ny, nz, px, py, pz, r, g, b, refl;
            iss >> nx >> ny >> nz >> px >> py >> pz >> r >> g >> b >> refl;
            vec4 normal = vec4(nx, ny, nz, 0);
            vec4 p = vec4(px, py, pz, 1);
            Object obj;
            obj.data1 = normal;
            obj.data2 = p;
            obj.color = vec4(r, g, b, refl);
            obj.type = 1;
            objects.push_back(obj);
        }
    }
}
