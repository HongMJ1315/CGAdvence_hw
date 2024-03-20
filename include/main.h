#ifndef MAIN_H
#define MAIN_H

#include "PerlinNoise.hpp"
#include "GLSLSetup.h"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <algorithm>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLU
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>

// OpenCV
#include <opencv2/opencv.hpp>


#define DEFAULT_HEIGHT  32 // x
#define DEFAULT_LENGTH  32 // y
#define DEFAULT_WIDTH   32 // z

#define GRAPH_SACLE 1.0f

#define ROTATE_SPEED 1.0f
#define MOVE_SPEED 4.0f

#define PERLIN_FREQ 0.1f
#define PERLIN_OCTAVE 5

#define SUN_LIGHT GL_LIGHT0
#define HELICOPTER_LIGHT GL_LIGHT1
#define TEXTURE_SIZE 256


// Texture
unsigned char wood[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char metal[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char wood2[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char cement[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char earth[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char sky[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned int textName[10];

float perlinFreq = PERLIN_FREQ;
// Window size
int width = (2048 + 1024) / 2;
int height = width / 16 * 9;

// Camera position
glm::vec3 cameraPos = glm::vec3(0.0f, 50.0f, 50.0f);
glm::vec3 helicopterPos = glm::vec3(0.0f, 50.0f, 0.0f);
glm::vec3 helicopterRotate = glm::vec3(0.0f, 0.0f, 0.0f);
float selfRot = 0;
float degree = 180.0f;

float drawRightDownPoint[3] = { 0 - DEFAULT_HEIGHT / 2.0, 0, 0 - DEFAULT_WIDTH / 2.0 };

float points[][3] = { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} };
int faces[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

int graph[DEFAULT_HEIGHT][DEFAULT_LENGTH][DEFAULT_WIDTH];
int tmpGraph[DEFAULT_HEIGHT][DEFAULT_WIDTH];

bool keyboardState[1000];
bool directionKey[4];

bool viewMode = false;

int HEIGHT = DEFAULT_HEIGHT;
int LENGTH = DEFAULT_LENGTH;
int WIDTH = DEFAULT_WIDTH;


GLuint programID; // Shader program ID
GLuint vertexArrayID; // Vertex Array Object ID
GLuint vertexBufferID; // Vertex Buffer Object ID

enum material{
    FLOOR, EARTH, METAL, RUBBER
} MATERIAL;

enum blockType{
    AIR = 0, BEDROCK = 1, STONE, DIRT, GRASS
} BLOCK_TYPE;

enum texture : int32_t{
    EARTH_TEXTURE = 0, METAL_TEXTURE = 1, WOOD_TEXTURE, WOOD2_TEXTURE, CEMENT_TEXTURE, RUBBER_TEXTURE, SKY_TEXTURE, MIRROR_TEXTURE
}TEXTURE;
float colorArray[][3] = {
    // 0
    {0, 0, 0},
    // bedrock
    {45 / 255.0f, 45 / 255.0f, 45 / 255.0f},
    // stone
    {97 / 255.0f, 106 / 255.0f, 107 / 255.0f},
    // dirt
    {142 / 255.0f, 86 / 255.0f, 26 / 255.0f},
    // grass
    {35 / 255.0f, 155 / 255.0f, 86 / 255.0f}
};


void TextureInit(texture textType, unsigned int *textName, unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4], int width, int height){
    glBindTexture(GL_TEXTURE_2D, textName[textType]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT = repeat texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
}

void SetTexture(texture textType, unsigned int *textName){
    float diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    float emission[] = { 0.0, 0.0, 0.0, 1.0 };
    float shininess = 0.0;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textName[textType]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    switch(textType){
        case METAL_TEXTURE:
        shininess = 30;
        specular[0] = 0.708273; specular[1] = 0.708273; specular[2] = 0.308273;
        break;

        case WOOD_TEXTURE: case WOOD2_TEXTURE: case EARTH_TEXTURE:
        shininess = 0.1;
        specular[0] = 0.4; specular[1] = 0.2; specular[2] = 0.0;
        break;

        case RUBBER_TEXTURE:
        shininess = 0.078125;
        break;

        case CEMENT_TEXTURE:
        shininess = 0.08;
        specular[0] = 0.0; specular[1] = 0.0; specular[2] = 0.0;
        break;

        default:
        break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}

void SetMaterial(material materialType, float r = 1, float g = 1, float b = 1){
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 0.0 };
    GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 0.0 };

    switch(materialType){
        case EARTH:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.7; mat_specular[1] = 0.7; mat_specular[2] = 0.7;
        mat_shininess[0] = 30;
        break;

        case FLOOR:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.0; mat_specular[1] = 0.0; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.0;

        case METAL:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.7; mat_specular[1] = 0.7; mat_specular[2] = 0.7;
        mat_shininess[0] = 30;
        break;

        case  RUBBER:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.0; mat_specular[1] = 0.0; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.0;
        break;

        default:
        break;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
}

void Cube(float r, float g, float b){
    glMatrixMode(GL_MODELVIEW);
    SetTexture(METAL_TEXTURE, textName);
    // SetMaterial(METAL, r, g, b);
    glPushMatrix();

    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[faces[i][0]]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(points[faces[i][1]]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(points[faces[i][2]]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(points[faces[i][3]]);
        glEnd();
    }

    for(int i = 0; i < 6; i++){
        // 繪製黑色邊緣
        SetMaterial(EARTH, 0, 0, 0);
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 4; j++){
            glVertex3fv(points[faces[i][j]]);
        }
        glEnd();
    }
    glPopMatrix();
}


void Blade(){
    SetMaterial(METAL, 1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
    glEnd();
}

void Tire(float r, float g, float b){
    glPushMatrix();
    SetMaterial(RUBBER, r, g, b);
    glutSolidTorus(2.0,  /* inner radius */
        2.0,  /* outer radius */
        24,   /* divided into 18 segments */
        12);  /* 12 rings */
    glPopMatrix();
}


void SetLook(glm::vec3 pos){
    glViewport(0, 0, width, height);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
}


void GraphInit(){
    unsigned int seed = 5;
    std::mt19937 mt{ seed };
    siv::PerlinNoise perlin{ mt() };

    for(int h = 0; h < HEIGHT; h++){
        for(int i = 0; i < LENGTH; i++){
            for(int j = 0; j < WIDTH; j++){
                float n = perlin.normalizedOctave3D_01(
                    (h + floor(drawRightDownPoint[0])) * perlinFreq,
                    (i + floor(drawRightDownPoint[1])) * perlinFreq,
                    (j + floor(drawRightDownPoint[2])) * perlinFreq,
                    PERLIN_OCTAVE);
                graph[h][i][j] = (n > 0.5 ? blockType::STONE : blockType::AIR);
            }
        }
    }

    perlin.reseed(mt());
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            float n = perlin.normalizedOctave2D_01(
                (i + floor(drawRightDownPoint[0])) * perlinFreq * 2,
                (j + floor(drawRightDownPoint[2])) * perlinFreq * 2,
                PERLIN_OCTAVE);
            int nn = std::min(n * 20 + LENGTH * 1 / 4, float(LENGTH - 1));
            tmpGraph[i][j] = nn;
        }
    }
    perlin.reseed(mt());
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            float n = perlin.normalizedOctave2D_01(
                (i + floor(drawRightDownPoint[0])) * perlinFreq * 2,
                (j + floor(drawRightDownPoint[2])) * perlinFreq * 2,
                PERLIN_OCTAVE);
            int nn = std::min(n * 20 + LENGTH * 2 / 4, float(LENGTH - 1));
            for(int k = tmpGraph[i][j]; k < nn; k++){
                if(graph[i][k][j] == 0) continue;
                graph[i][k][j] = blockType::DIRT;
            }
            for(int k = nn + 1; k < LENGTH; k++)
                graph[i][k][j] = blockType::AIR;
            if(graph[i][nn][j] == 0) continue;
            graph[i][nn][j] = blockType::GRASS;
        }
    }
    perlin.reseed(mt());
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            for(int k = 1; k < 3; k++){
                graph[i][k][j] = perlin.normalizedOctave3D_01(
                    (i + floor(drawRightDownPoint[0])) * perlinFreq * 5,
                    (k + floor(drawRightDownPoint[1])) * perlinFreq * 10,
                    (j + floor(drawRightDownPoint[2])) * perlinFreq * 5,
                    PERLIN_OCTAVE * 10) > 0.5 ? blockType::AIR : blockType::BEDROCK;
            }
            graph[i][0][j] = blockType::BEDROCK;
        }
    }

}


#endif // MAIN_H