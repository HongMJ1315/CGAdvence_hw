#ifndef DRAW_H
#define DRAW_H

#include "main.h"


void ReadTexture(unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4], char *filename, int width = TEXTURE_SIZE, int height = TEXTURE_SIZE){
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if(img.empty()){
        std::cout << "Could not read the image: " << filename << std::endl;
        return;
    }
    cv::resize(img, img, cv::Size(width, height));

    int index = 0;
    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(j, i));
            texture[i][j][0] = color[2];
            texture[i][j][1] = color[1];
            texture[i][j][2] = color[0];
            texture[i][j][3] = 1.0;
        }
    }
    for(int i = 0; i < img.rows / 2; i++){
        for(int j = 0; j < img.cols; j++){
            std::swap(texture[i][j][0], texture[img.rows - i - 1][j][0]);
            std::swap(texture[i][j][1], texture[img.rows - i - 1][j][1]);
            std::swap(texture[i][j][2], texture[img.rows - i - 1][j][2]);
            std::swap(texture[i][j][3], texture[img.rows - i - 1][j][3]);
        }
    }
}

void HelicopterLight(glm::vec3 color, glm::vec3 dir, float cutoff, float intensity, bool isOn = true){
    // glEnable(HELICOPTER_LIGHT);

    float r = color.x * intensity;
    float g = color.y * intensity;
    float b = color.z * intensity;
    r = std::min(r, 1.0f);
    g = std::min(g, 1.0f);
    b = std::min(b, 1.0f);
    r = std::max(r, 0.0f);
    g = std::max(g, 0.0f);
    b = std::max(b, 0.0f);
    GLfloat lightPosition[] = { 0, -1.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2 * intensity * r, 0.2 * intensity * g, 0.2 * intensity * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)

    glLightfv(HELICOPTER_LIGHT, GL_POSITION, lightPosition);
    glLightfv(HELICOPTER_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(HELICOPTER_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(HELICOPTER_LIGHT, GL_SPECULAR, lightSpecular);


    GLfloat spot_direction[] = { dir.x, dir.y, dir.z };
    glLightfv(HELICOPTER_LIGHT, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HELICOPTER_LIGHT, GL_SPOT_CUTOFF, cutoff);
    glLightf(HELICOPTER_LIGHT, GL_SPOT_EXPONENT, 5.0);
    glLightf(HELICOPTER_LIGHT, GL_CONSTANT_ATTENUATION, 2.0);
}

void DrawHelicopter(glm::vec3 helicopterLocation, glm::vec3 helicopterRotate, float self_ang){
    glMatrixMode(GL_MODELVIEW);
    float helicopterX = helicopterLocation.x;
    float helicopterY = helicopterLocation.y;
    float helicopterZ = helicopterLocation.z;
    float helicopterRotateX = helicopterRotate.x;
    float helicopterRotateY = helicopterRotate.y;
    float helicopterRotateZ = helicopterRotate.z;

    // std::cout << helicopterX << " " << helicopterY << " " << helicopterZ << std::endl;
    glPushMatrix();
    glTranslatef(helicopterX, helicopterY, helicopterZ + 12);
    glRotatef(-helicopterRotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(-helicopterRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(-helicopterRotateZ, 0.0f, 0.0f, 1.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    // glPushMatrix();

    // glScalef(10.0f, 10.0f, 10.0f);

    HelicopterLight(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(1, 1, 0)), 45.0f, 1.0f);


    //Main body
    glPushMatrix();
    glTranslatef(-6.0f, 0, 0.0f);
    glScalef(12, 10, 12);
    Cube(0.10f, 0.44f, 0.24f);
    glPopMatrix();

    //Tail
    glPushMatrix();
    glTranslatef(-1.5, 0, -12);
    glScalef(3, 3, 12);
    Cube(0.67f, 0.70f, 0.73f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.5, 3, -12);
    glScalef(3, 5, 3);
    Cube(115.0 / 255.0, 198.0 / 255.0, 182.0 / 255.0);
    glPopMatrix();

    //Tail rotor support
    glPushMatrix();
    glTranslatef(1, 5, -12);
    glScalef(1, 3, 3);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire support back
    glPushMatrix();
    glTranslatef(-0.5, -1, -13);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glScalef(1, 3, 1);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire back
    glPushMatrix();
    glTranslatef(0, -1, -13);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 0.5, 0.5);
    Tire(0, 0, 0);
    glPopMatrix();

    // Main rotor support
    glPushMatrix();
    glTranslatef(-1.5, 10, 5);
    glScalef(3, 3, 3);
    Cube(174.0 / 255.0, 214.0 / 255.0, 241.0 / 255.0);
    glPopMatrix();

    //Tire support right
    glPushMatrix();
    glTranslatef(7, -1, 3.5);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1, 3, 1);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire right
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-4, -1, -7.5);
    glScalef(0.5, 0.5, 0.5);
    Tire(0, 0, 0);
    glPopMatrix();

    //Tier support left
    glPushMatrix();
    glTranslatef(-8, -1, 3.5);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1, 3, 1);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire left
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-4, -1, 7.5);
    glScalef(0.5, 0.5, 0.5);
    Tire(0, 0, 0);
    glPopMatrix();


    /*
    glColor3f(0.10f, 0.44f, 0.24f);
    // helicopterBody.draw();

    glColor3f(0.67f, 0.70f, 0.73f);
    // helicopterBackSupport.draw();

    glColor3f(0.0f, 0.0f, 0.0f);
    // helicopterBackTire.draw();
    // helicopterLeftTire.draw();
    // helicopterRightTire.draw();
    */

    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslated(0, 13.1, 7);
    glRotated(self_ang, 0.0, 1.0, 0.0);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslated(1, 0, -38);
        glScalef(1, 1, 5);
        glRotated(90.0, 1.0, 0.0, 0.0);
        Blade();

        glPopMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    }
    glPopMatrix();
    glPushMatrix();
    glTranslated(2.1, 6.5, -10.5);
    glRotated(self_ang, 1.0, 0.0, 0.0);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslated(0, -8, 0.5);
        glScalef(1, 1, 0.5);
        glRotated(90.0, 0, 1.0, 0.0);
        Blade();

        glPopMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPopMatrix();
}


void DrawCoordinateString(glm::vec3 helicopterPos, glm::vec3 cameraPos, int width, int height){
    glUseProgram(0);

    std::string coordinateStr = "Helicopter x: " + std::to_string(helicopterPos.x).substr(0, 5) + " y: " + std::to_string(helicopterPos.y).substr(0, 5) + " z: " + std::to_string(helicopterPos.z).substr(0, 5);
    std::string cameraPosSt = "Camera: x: " + std::to_string(cameraPos.x).substr(0, 5) + " y: " + std::to_string(cameraPos.y).substr(0, 5) + " z: " + std::to_string(cameraPos.z).substr(0, 5);
    std::string helicopterRotateStr = "Helicopter Rotate: x: " + std::to_string(helicopterRotate.x).substr(0, 5) + " y: " + std::to_string(helicopterRotate.y).substr(0, 5) + " z: " + std::to_string(helicopterRotate.z).substr(0, 5);

    glMatrixMode(GL_PROJECTION);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(10, height - 30);
    for(int i = 0; i < coordinateStr.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, coordinateStr[i]);
    }
    glRasterPos2f(10, height - 60);
    for(int i = 0; i < cameraPosSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cameraPosSt[i]);
    }
    glRasterPos2f(10, height - 90);
    for(int i = 0; i < helicopterRotateStr.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, helicopterRotateStr[i]);
    }


    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}



void DrawGraph(){
    glMatrixMode(GL_MODELVIEW);
    glScalef(GRAPH_SACLE, GRAPH_SACLE, GRAPH_SACLE);
    glPushMatrix();
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < LENGTH; j++){
            for(int k = 0; k < WIDTH; k++){
                if(!graph[i][j][k])continue;
                glPushMatrix();
                glTranslatef(
                    (floor(drawRightDownPoint[0]) + i),
                    (floor(drawRightDownPoint[1]) + j),
                    (floor(drawRightDownPoint[2]) + k));
                int type = graph[i][j][k];
                // if(type == 1)
                Cube(colorArray[type][0], colorArray[type][1], colorArray[type][2]);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}
/*
void DrawHeadLight(glm::vec3 color, glm::vec3 loc, glm::vec3 dir, float cutoff, float intensity, bool isOn = true){
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    loc = loc + dir * 0.1f;
    glTranslatef(loc.x, loc.y, loc.z);

    float r = color.x * intensity;
    float g = color.y * intensity;
    float b = color.z * intensity;
    r = std::min(r, 1.0f);
    g = std::min(g, 1.0f);
    b = std::min(b, 1.0f);
    r = std::max(r, 0.0f);
    g = std::max(g, 0.0f);
    b = std::max(b, 0.0f);
    GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.3 * intensity * r, 0.3 * intensity * g, 0.3 * intensity * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)

    glLightfv(HEAD_LIGHT, GL_POSITION, lightPosition);
    glLightfv(HEAD_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(HEAD_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(HEAD_LIGHT, GL_SPECULAR, lightSpecular);

    GLfloat spot_direction[] = { dir.x, dir.y, dir.z };
    glLightfv(HEAD_LIGHT, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HEAD_LIGHT, GL_SPOT_CUTOFF, cutoff);
    glPopMatrix();
}
// */

void DrawSunLight(glm::vec3 color, float instance){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // glTranslatef(HEIGHT / 2, LENGTH + 10, WIDTH / 2);
    float r = color.x, g = color.y, b = color.z;
    GLfloat lightAmbient[] = { r * 0.7 * instance, g * 0.7 * instance, b * 0.7 * instance, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    GLfloat lightDirection[] = { 0, 1, 0 , 0.0 };  // Light direction for directional light
    glLightfv(SUN_LIGHT, GL_POSITION, lightDirection);
    glLightfv(SUN_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, lightSpecular);
    glPopMatrix();
}


void DrawFloor(int len = 25){
    int i, j;
    for(i = 0; i < len; i++)
        for(j = 0; j < len; j++){
            // if((i + j) % 2 == 0){
            //     // glColor3f(0.7, 0.7, 0.7);
            //     // SetMaterial(FLOOR, 0.7, 0.7, 0.7);
            //     SetTexture(WOOD2_TEXTURE, textName);
            // }
            // else{
            //     // glColor3f(0.1, 0.1, 0.7);
            //     // SetMaterial(FLOOR, 0.1, 0.1, 0.7);
            //     SetTexture(WOOD_TEXTURE, textName);
            // }
            SetTexture(EARTH_TEXTURE, textName);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f((i - 5.0) * 10.0, 0, (j - 5.0) * 10.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f((i - 4.0) * 10.0, 0, (j - 5.0) * 10.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f((i - 4.0) * 10.0, 0, (j - 4.0) * 10.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f((i - 5.0) * 10.0, 0, (j - 4.0) * 10.0);
            glEnd();

        }
}

void DrawView(){
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, helicopterPos.x, helicopterPos.y, helicopterPos.z, 0.0f, 1.0f, 0.0f);
    glUniform3f(glGetUniformLocation(ReturnProgramID(), "EP"), cameraPos.x, cameraPos.y, cameraPos.z);
    // DrawGraph();
    DrawSunLight({ 1.0f, 1.0f, 1.0f }, 1.0f);
    DrawHelicopter(helicopterPos, helicopterRotate, selfRot);
    DrawFloor();
    // DrawHeadLight(glm::vec3(0.5, 0.5, 0.5), helicopterPos, helicopterPos - cameraPos, 30.0f, 1.0f);
}


#endif // DRAW_H