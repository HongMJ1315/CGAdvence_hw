#include "draw.h"

void GlInit(){
    setGLSLshaders("shader/Phong.vert", "shader/Phong.frag");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 10);
    glGenTextures(10, textName);
    ReadTexture(metal, "texture/metal.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::METAL_TEXTURE]);
    TextureInit(METAL_TEXTURE, textName, metal, 256, 256);

    ReadTexture(earth, "texture/earth.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::EARTH_TEXTURE]);
    TextureInit(EARTH_TEXTURE, textName, earth, 256, 256);

    int texLoc = glGetUniformLocation(ReturnProgramID(), "myTex");
    std::cout << "Texture location: " << texLoc << std::endl;
    glUniform1i(texLoc, 0);



    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(SUN_LIGHT);
    glEnable(HELICOPTER_LIGHT);
}

void Move(float dx, float dy, float dz){
    int hfront = (int) floor(HEIGHT / 2 + (dx / MOVE_SPEED));
    int ffront = (int) floor(WIDTH / 2 + (dz / MOVE_SPEED));

    cameraPos.x += GRAPH_SACLE * dx;
    cameraPos.y += GRAPH_SACLE * dy;
    cameraPos.z += GRAPH_SACLE * dz;
    helicopterPos.x += GRAPH_SACLE * dx;
    helicopterPos.y += GRAPH_SACLE * dy;
    helicopterPos.z += GRAPH_SACLE * dz;
    drawRightDownPoint[0] += dx;
    drawRightDownPoint[2] += dz;
}

void Update(){
    if(keyboardState['W']){
        float dx = MOVE_SPEED * sin(degree * PI / 180.0f);
        float dz = MOVE_SPEED * cos(degree * PI / 180.0f);
        if(helicopterRotate.x < 45.0f){
            helicopterRotate.x += ROTATE_SPEED;
        }
        Move(-dx, 0.0f, dz);

    }
    else{
        if(helicopterRotate.x - ROTATE_SPEED > ESP){
            helicopterRotate.x -= ROTATE_SPEED;
        }
    }
    if(keyboardState['S']){
        float dx = MOVE_SPEED * sin(degree * PI / 180.0f);
        float dz = MOVE_SPEED * cos(degree * PI / 180.0f);
        Move(dx, 0.0f, -dz);
        if(helicopterRotate.x > -45.0f){
            helicopterRotate.x -= ROTATE_SPEED;
        }
    }
    else{
        if(helicopterRotate.x + ROTATE_SPEED < ESP){
            helicopterRotate.x += ROTATE_SPEED;
        }
    }
    if(keyboardState['A']){
        if(helicopterRotate.z > -45.0f){
            helicopterRotate.z -= ROTATE_SPEED;
        }
        float dx = MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        float dz = MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        Move(dx, 0.0f, -dz);
    }
    else{
        if(helicopterRotate.z + ROTATE_SPEED < -ESP){
            helicopterRotate.z += ROTATE_SPEED;
        }
    }
    if(keyboardState['D']){
        if(helicopterRotate.z < 45.0f){
            helicopterRotate.z += ROTATE_SPEED;
        }
        float dx = MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        float dz = MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        Move(-dx, 0.0f, dz);
    }
    else{
        if(helicopterRotate.z - ROTATE_SPEED > ESP){
            helicopterRotate.z -= ROTATE_SPEED;
        }
    }

    // Rotate
    if(keyboardState['I']){
        cameraPos = MoveCameraUD(helicopterPos, cameraPos, ROTATE_SPEED);
    }
    if(keyboardState['K']){
        cameraPos = MoveCameraUD(helicopterPos, cameraPos, -ROTATE_SPEED);
    }
    if(keyboardState['J']){
        cameraPos = MoveCameraLR(helicopterPos, cameraPos, -ROTATE_SPEED);

    }
    if(keyboardState['L']){
        cameraPos = MoveCameraLR(helicopterPos, cameraPos, ROTATE_SPEED);

    }

    if(directionKey[0]){
        cameraPos.y += MOVE_SPEED * GRAPH_SACLE;
        helicopterPos.y += MOVE_SPEED * GRAPH_SACLE;
    }
    if(directionKey[1]){
        cameraPos.y -= MOVE_SPEED * GRAPH_SACLE;
        helicopterPos.y -= MOVE_SPEED * GRAPH_SACLE;
    }
    if(directionKey[2]){
        // Helicopter rotate
        helicopterRotate.y -= ROTATE_SPEED;
        degree -= ROTATE_SPEED;
        if(degree < 0.0f)
            degree += 360.0f;
    }
    if(directionKey[3]){
        // Helicopter rotate
        helicopterRotate.y += ROTATE_SPEED;
        degree += ROTATE_SPEED;

        if(degree > 360.0f)
            degree -= 360.0f;
    }
    GraphInit();
    glfwPostEmptyEvent();
}



void SingleView(GLFWwindow *window){
    glUseProgram(ReturnProgramID());
    // glUseProgram(0);
    glLoadIdentity();
    SetLook(cameraPos);
    DrawView();
}

void Display(GLFWwindow *window){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    DrawCoordinateString(helicopterPos, cameraPos, width, height);
    SingleView(window);

    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        // 在這裡處理錯誤，例如輸出錯誤信息
        std::cerr << "OpenGL error: " << gluErrorString(error) << std::endl;
    }

}


void Reshape(GLFWwindow *window, int w, int h){
    width = w;
    height = h;
    Display(window);
}

void KeyboardEvent(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        keyboardState[key] = true;

        switch(key){
            case GLFW_KEY_MINUS:
            perlinFreq -= 0.001f;
            GraphInit();
            break;
            case GLFW_KEY_EQUAL:
            perlinFreq += 0.001f;
            GraphInit();
            break;
            case GLFW_KEY_UP:
            directionKey[0] = true;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = true;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = true;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = true;
            break;
            // 0
            case GLFW_KEY_0:
            viewMode = !viewMode;
            break;
        }
    }
    else if(action == GLFW_RELEASE){
        keyboardState[key] = false;
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = false;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = false;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = false;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = false;
            break;
        }
    }
}

void bladeRotate(){
    selfRot += 10.0f;
    if(selfRot > 360.0f)
        selfRot -= 360.0f;
}
int main(int argc, char **argv){
    glutInit(&argc, argv);
    if(!glfwInit()){
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(width, height, "Perlin Noise 3D", nullptr, nullptr);
    if(!window){
        glfwTerminate();
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyboardEvent);
    glfwSetWindowSizeCallback(window, Reshape);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    GLenum err = glewInit();

    if(GLEW_OK != err){
        std::cout << "error: " << glewGetErrorString(err) << std::endl;
        exit(-1);
    }
    GraphInit();
    GlInit();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        double currentTime = glfwGetTime();
        if(currentTime - lastTime > 0.01){
            bladeRotate();
            lastTime = currentTime;
        }
        Update();
        Display(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
