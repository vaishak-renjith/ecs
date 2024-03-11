#include "Engine.h"
#include <cmath>
#include <SDL2/SDL.h>
#include <stdio.h>

float Engine::theta = 0.00001f;

void Engine::initialize(int sWidth, int sHeight, mesh &meshCube, mat4x4 &matProj)
{
    meshCube.tris = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	};
    float near = 0.1f;
    float far = 1000.0f;
    float fov = 90.0f;
    float aspectRatio = (float)sHeight / (float)sWidth;
    float fovRad = 1.0f/tanf(fov * 0.5f / 180.0f * 3.14159f); // degress to radians

    matProj.m[0][0] = aspectRatio * fovRad;
    matProj.m[1][1] = fovRad;
    matProj.m[2][2] = far / (far-near);
    matProj.m[3][2] = (-far * near) / (far - near);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;
}

void Engine::transform(mesh &meshCube, mat4x4 &matProj, int sWidth, int sHeight, SDL_Renderer &rend, float deltaTime)
{
    SDL_RenderClear(&rend);

    SDL_Vertex* triangleVertices = nullptr;

    mat4x4 matRotZ, matRotX;
    theta += 1.0f * deltaTime;

    //rotation z
    matRotZ.m[0][0] = cosf(theta);
    matRotZ.m[0][1] = sinf(theta);
    matRotZ.m[1][0] = -sinf(theta);
    matRotZ.m[1][1] = cosf(theta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    //rotation x
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(theta * 0.5f);
    matRotX.m[1][2] = sinf(theta * 0.5f);
    matRotX.m[2][1] = -sinf(theta * 0.5f);
    matRotX.m[2][2] = cosf(theta * 0.5f);
    matRotX.m[3][3] = 1;


    for (auto tri: meshCube.tris) {
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        multiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
        multiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
        multiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);        

        multiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        multiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        multiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        multiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
        multiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
        multiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

        triProjected.p[0].x *= 0.5f * (float)sWidth; 
        triProjected.p[0].y *= 0.5f * (float)sHeight; 
        triProjected.p[1].x *= 0.5f * (float)sWidth; 
        triProjected.p[1].y *= 0.5f * (float)sHeight; 
        triProjected.p[2].x *= 0.5f * (float)sWidth; 
        triProjected.p[2].y *= 0.5f * (float)sHeight; 

        SDL_SetRenderDrawColor(&rend, 0, 0, 0, 255);
        SDL_RenderDrawLine(&rend, triProjected.p[0].x, triProjected.p[0].y,
                           triProjected.p[1].x, triProjected.p[1].y);
        SDL_RenderDrawLine(&rend, triProjected.p[1].x, triProjected.p[1].y,
                           triProjected.p[2].x, triProjected.p[2].y);
        SDL_RenderDrawLine(&rend, triProjected.p[2].x, triProjected.p[2].y,
                           triProjected.p[0].x, triProjected.p[0].y);
    }

    SDL_RenderPresent(&rend);
}

void Engine::multiplyMatrixVector(point &i, point &o, mat4x4 &m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f) {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

SDL_Vertex* Engine::drawTriangle(triangle tri) {
    SDL_Vertex* triangleVertices = new SDL_Vertex[3] 
    {
        {
            {tri.p[0].x, tri.p[0].y},
            {0, 0, 0, 0xFF},
            {0.0f, 0.0f}
        },
        {
            {tri.p[1].x, tri.p[1].y},
            {0, 0, 0, 0xFF},
            {0.0f, 0.0f}
        },
        {
            {tri.p[2].x, tri.p[2].y},
            {0, 0, 0, 0xFF},
            {0.0f, 0.0f}
        }
    };
    
    return triangleVertices;
}
