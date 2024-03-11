#pragma once
#include <vector>
#include <SDL2/SDL.h>

class Engine
{
public:
    struct point {
        float x, y, z;
    };

    struct triangle {
        point p[3];
    };

    struct mesh {
        std::vector<triangle> tris;
    };

    struct mat4x4 {
        float m[4][4] = {0};
    };

    static void initialize(int sWidth, int sHeight, mesh &meshCube, mat4x4 &matProj);
    static void transform(mesh &meshCube, mat4x4 &matProj, int sWidth, int sHeight, SDL_Renderer &rend, float deltaTime);
    static SDL_Vertex* drawTriangle(triangle tri);

private:
    static float theta;
    static void multiplyMatrixVector(point &i, point &o, mat4x4 &m);
};