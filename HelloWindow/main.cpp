#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"
#include <time.h>


#include <iostream>
#include "SkyBox.hpp"
//for sounds
#include "irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") 

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;
glm::mat4 lightRotation;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;
glm::vec3 lightDir1;
glm::vec3 lightColor1;
glm::vec3 lightDir2;
glm::vec3 lightColor2;
glm::vec3 lightDir3;
glm::vec3 lightColor3;
glm::vec3 lightDir4;
glm::vec3 lightColor4;
glm::vec3 lightDir5;
glm::vec3 lightColor5;
glm::vec3 lightDir6;
glm::vec3 lightColor6;
glm::vec3 lightDir7;
glm::vec3 lightColor7;
glm::vec3 lightDir8;
glm::vec3 lightColor8;
glm::vec3 lightCubeColor;

// shader uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;
GLint normalMatrixLoc;
GLint lightDirLoc;
GLint lightColorLoc;
GLint lightDirLoc1;
GLint lightColorLoc1;
GLint lightDirLoc2;
GLint lightColorLoc2;
GLint lightDirLoc3;
GLint lightColorLoc3;
GLint lightDirLoc4;
GLint lightColorLoc4;
GLint lightDirLoc5;
GLint lightColorLoc5;
GLint lightDirLoc6;
GLint lightColorLoc6;
GLint lightDirLoc7;
GLint lightColorLoc7;
GLint lightDirLoc8;
GLint lightColorLoc8;
GLuint lightCubeColorLoc;

GLint turnOnLight;

// camera
gps::Camera myCamera(
    glm::vec3(0.0f, 12.0f, 10.0f),
    glm::vec3(0.0f, 0.0f, -10.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.05f;

GLboolean pressedKeys[1024];

// models
gps::Model3D geralt_spore;
gps::Model3D roata;
gps::Model3D village;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D firstmonster;
gps::Model3D nanosuit;
gps::Model3D teapot;
gps::Model3D ciriSword;
gps::Model3D netflixSword;
gps::Model3D silvan;
gps::Model3D leshen;
gps::Model3D alghoul;

GLfloat angle;
GLfloat lightAngle;
GLfloat lightUpDown;

// shaders
gps::Shader myCustomShader;
gps::Shader skyboxShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;

gps::SkyBox mySkyBox;

bool firstMouse = true;
float lastX = 1860;
float lastY = 1020;
float yaw = -90.0f;
float pitch = 0.0f;
float cameraSensitivity = 0.1f;
float delta = 0.0f;
float delta2 = 0.0f;
float deltaSword1 = 0.0f;
float deltaSword2 = 0.0f;
float deltaSkyBox = 0.0f;
float angleY = 0.0f;
float counter = 0.0f;
float counter2 = 0.0f;
float distance_silvan = 0.0f;
float distance_leshen1 = 0.0f;
float distance_leshen2 = 0.0f;
float distance_leshen3 = 0.0f;
float distance_alghoul1 = 0.0f;
float distance_alghoul2 = 0.0f;
float distance_alghoul3 = 0.0f;
float distance_alghoul4 = 0.0f;
float distance_alghoul5 = 0.0f;

const unsigned int SHADOW_WIDTH = 8192; //32768, 16384, 8192
const unsigned int SHADOW_HEIGHT = 8192; //32768, 16384, 8192

GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;
bool ambientalSounds = false;
bool swordSounds = false;
bool backSwordSounds = false;
bool swordSounds2 = false;
bool backSwordSounds2 = false;
bool turnOnSpotLights = false;
bool transitionNightDay = false;
bool transitionNightDay2 = false;
bool skyBoxTransition = false;
bool presentation = false;
ISoundEngine* engine = createIrrKlangDevice();
ISoundEngine* engine2 = createIrrKlangDevice();
//FILE *recording=fopen("presenatation.txt", "w");
FILE *animPresentation= fopen("presenatation.txt", "r");

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        showDepthMap = !showDepthMap;

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        presentation = not presentation;
        fclose(animPresentation);
        animPresentation= fopen("presenatation.txt", "r");
        //lseek();
    }

	if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //TODO
    if (presentation) {
        return;
    }
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= cameraSensitivity;
    yoffset *= cameraSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    myCamera.rotate(pitch, yaw);
}

void playAmbientalMusic(bool sound) {

    if (!engine)
        return;
    engine->play2D("TheWitcher3.ogg", true);
    if (sound) {
        engine->stopAllSounds();
    }
}

void renderSkyBox() {
    std::vector<const GLchar*> faces;
    if (!skyBoxTransition) {
        faces.push_back("skybox/right.tga");
        faces.push_back("skybox/left.tga");
        faces.push_back("skybox/top.tga");
        faces.push_back("skybox/bottom.tga");
        faces.push_back("skybox/back.tga");
        faces.push_back("skybox/front.tga");
    }
    else {
        faces.push_back("skybox2/right.png");
        faces.push_back("skybox2/left.png");
        faces.push_back("skybox2/top.png");
        faces.push_back("skybox2/bottom.png");
        faces.push_back("skybox2/front.png");
        faces.push_back("skybox2/back.png");
    }

    mySkyBox.Load(faces);
    skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
        glm::value_ptr(view));

    projection = glm::perspective(glm::radians(80.0f), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
        glm::value_ptr(projection));
}

void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {
        //fprintf(recording,"%f,%f,%f\n",myCamera.cameraPosition.x, myCamera.cameraPosition.y, myCamera.cameraPosition.z);
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);

		//update view matrix
        view = myCamera.getViewMatrix();
        myCustomShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_S]) {
       // fprintf(recording, "%f,%f,%f\n", myCamera.cameraPosition.x, myCamera.cameraPosition.y, myCamera.cameraPosition.z);
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myCustomShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_A]) {
       // fprintf(recording, "%f,%f,%f\n", myCamera.cameraPosition.x, myCamera.cameraPosition.y, myCamera.cameraPosition.z);
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myCustomShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_D]) {
       // fprintf(recording, "%f,%f,%f\n", myCamera.cameraPosition.x, myCamera.cameraPosition.y, myCamera.cameraPosition.z);
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myCustomShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

    if (pressedKeys[GLFW_KEY_Q]) {
        angle -= 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    }

    if (pressedKeys[GLFW_KEY_E]) {
        angle += 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    }

    //solid view
    if (pressedKeys[GLFW_KEY_Z]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //wireframe view
    if (pressedKeys[GLFW_KEY_X]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    //pointframe view
    if (pressedKeys[GLFW_KEY_C]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (pressedKeys[GLFW_KEY_RIGHT]) {
        lightAngle += 1.0;
    }

    if (pressedKeys[GLFW_KEY_LEFT]) {
        lightAngle -= 1.0;
    }

    if (pressedKeys[GLFW_KEY_UP]) {
        transitionNightDay = false;
        transitionNightDay2 = true;
        skyBoxTransition = false;
        renderSkyBox();
    }

    if (pressedKeys[GLFW_KEY_DOWN]) {
        transitionNightDay = true;
        transitionNightDay2 = false;
        skyBoxTransition = true;
        renderSkyBox();
    }

    if (pressedKeys[GLFW_KEY_G]) {
        playAmbientalMusic(false);
    }

    if (pressedKeys[GLFW_KEY_H]) {
        playAmbientalMusic(true);
    }

    if (pressedKeys[GLFW_KEY_T]) {
        swordSounds = true;
        swordSounds2 = true;
    }

    if (pressedKeys[GLFW_KEY_K]) {
        turnOnSpotLights = true;
        myCustomShader.useShaderProgram();
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "turnOnLight"), 0);
    }

    if (pressedKeys[GLFW_KEY_L]) {
        turnOnSpotLights = false;
        myCustomShader.useShaderProgram();
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "turnOnLight"), 1);
    }
}

void initOpenGLWindow() {
    myWindow.Create(1920, 1080, "Witcher Scene");
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
    
}

void initModels() {
    village.LoadModel("models/terrain_village/terrain.obj");
    geralt_spore.LoadModel("models/geralt_and_spore/geralt_and_spore.obj");
    firstmonster.LoadModel("models/neker/firstmonster.obj");
    nanosuit.LoadModel("models/nanosuit/nanosuit.obj");
    teapot.LoadModel("models/teapot/teapot20segUT.obj");
    ciriSword.LoadModel("models/cirisword/ciriswordobj.obj");
    netflixSword.LoadModel("models/netflixsword/netflixsword.obj");
    roata.LoadModel("models/roata/roata.obj");
    silvan.LoadModel("models/silvan/silvanobj.obj");
    leshen.LoadModel("models/leshen/leshen.obj");
    alghoul.LoadModel("models/alghoul/alghoul.obj");
    lightCube.LoadModel("models/cube/cube.obj");
    screenQuad.LoadModel("models/quad/quad.obj");
}

void initShaders() {
    myCustomShader.loadShader(
        "shaders/shaderStart.vert",
        "shaders/shaderStart.frag");
    lightShader.loadShader(
        "shaders/lightCube.vert",
        "shaders/lightCube.frag");
    screenQuadShader.loadShader(
        "shaders/screenQuad.vert",
        "shaders/screenQuad.frag");
    depthMapShader.loadShader(
        "shaders/depthMap.vert",
        "shaders/depthMap.frag");
}

void initUniforms() {
    myCustomShader.useShaderProgram();

    model = glm::mat4(1.0f);
    modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    view = myCamera.getViewMatrix();
    viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    projection = glm::perspective(glm::radians(80.0f),
        (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
        0.1f, 80.0f);
    projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //set the light direction (direction towards the light)
    lightDir = glm::vec3(0.0f, 20.0f, -40.0f);
    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

    //set light color
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
    lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    if (!turnOnSpotLights) {
        //second light source
        lightDir1 = glm::vec3(0.0f, 0.0f, 2.0f); 
        lightDirLoc1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir1");
        glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightDir1));

        //second light color
        lightColor1 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor1");
        glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor1));

        //third light source
        lightDir2 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir2");
        glUniform3fv(lightDirLoc2, 1, glm::value_ptr(lightDir2));

        //third light color
        lightColor2 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor2");
        glUniform3fv(lightColorLoc2, 1, glm::value_ptr(lightColor2));  

        //fourth light source
        lightDir3 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir3");
        glUniform3fv(lightDirLoc3, 1, glm::value_ptr(lightDir3));

        //fourth light color
        lightColor3 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor3");
        glUniform3fv(lightColorLoc3, 1, glm::value_ptr(lightColor3));

        //five light source
        lightDir4 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc4 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir4");
        glUniform3fv(lightDirLoc4, 1, glm::value_ptr(lightDir4));

        //five light color
        lightColor4 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc4 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor4");
        glUniform3fv(lightColorLoc4, 1, glm::value_ptr(lightColor4));

        //six light source
        lightDir5 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc5 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir5");
        glUniform3fv(lightDirLoc5, 1, glm::value_ptr(lightDir5));

        //six light color
        lightColor5 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc5 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor5");
        glUniform3fv(lightColorLoc5, 1, glm::value_ptr(lightColor5));

        //seven light source
        lightDir6 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc6 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir6");
        glUniform3fv(lightDirLoc6, 1, glm::value_ptr(lightDir6));

        //seven light color
        lightColor6 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc6 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor6");
        glUniform3fv(lightColorLoc6, 1, glm::value_ptr(lightColor6));

        //eight light source
        lightDir7 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc7 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir7");
        glUniform3fv(lightDirLoc7, 1, glm::value_ptr(lightDir7));

        //eight light color
        lightColor7 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc7 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor7");
        glUniform3fv(lightColorLoc7, 1, glm::value_ptr(lightColor7));

        //nine light source
        lightDir8 = glm::vec3(0.0f, 0.0f, 2.0f);
        lightDirLoc8 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir8");
        glUniform3fv(lightDirLoc8, 1, glm::value_ptr(lightDir8));

        //eight light color
        lightColor8 = glm::vec3(0.21f, 0.33f, 0.6f);
        lightColorLoc8 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor8");
        glUniform3fv(lightColorLoc8, 1, glm::value_ptr(lightColor8));
    }
    lightShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    lightCubeColor = glm::vec3(1.0f, 0.0f, 0.0f);
    lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
    glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));
}

void initFBO() {
    //TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
    //generate FBO ID
    glGenFramebuffers(1, &shadowMapFBO);

    //create depth texture for FBO
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //attach texture to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
    //TODO - Return the light-space transformation matrix
    glm::mat4 lightView = glm::lookAt(glm::mat3(lightRotation) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const GLfloat near_plane = 0.001f, far_plane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
    glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
    return lightSpaceTrMatrix;
}

void renderGeralt(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(-0.7f, 6.9f, 35.0f));
    model = glm::rotate(model, glm::radians(-300.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.1f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    geralt_spore.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderRoata(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    delta += 0.1;
    model = glm::translate(model, glm::vec3(-1.8f, 0.76f, -20.0f));
    model = glm::scale(model, glm::vec3(2.0f));
    model = glm::rotate(model, glm::radians(delta), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    roata.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderMonster(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(31.8f, 11.3f, -47.7f));
    model = glm::scale(model, glm::vec3(0.3f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    firstmonster.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderSilvan(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(-25.84f, 10.0f, 10.2f));
    model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    silvan.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderLesehen(gps::Shader shader, bool depthPass, float x, float y, float z, float radians) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(radians), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    leshen.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderAlghoul(gps::Shader shader, bool depthPass, float x, float y, float z, float radians, float radians2) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(radians), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(radians2), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    alghoul.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderSuit(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(-27.8f, 10.65f, 14.5f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    nanosuit.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderTea(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    delta2 += 0.1;
    model = glm::translate(model, glm::vec3(-28.78f, 10.55f, 13.8f));
    model = glm::rotate(model, glm::radians(delta2), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    teapot.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void playSwordSound(bool sound) {

    if (!engine2)
        return;
    engine2->play2D("swords.ogg", false);
    if (!sound) {
        engine2->stopAllSounds();
    }
}

void renderSword1(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(-27.58f, 10.7f, 13.5f));
    model = glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    if (swordSounds) {
        deltaSword1 += 0.1;
        model = glm::rotate(model, glm::radians(deltaSword1), glm::vec3(0.0f, 1.0f, 0.0f));
        counter++;
        if (counter == 500.0f) {
            playSwordSound(swordSounds);
            swordSounds = false;
            backSwordSounds = true;
            deltaSword1 = 0.0f;
            counter = 0.0f;
        }
    }
    if (!swordSounds && backSwordSounds) {
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (backSwordSounds) {
        deltaSword1 += 0.1;
        model = glm::rotate(model, glm::radians(deltaSword1), glm::vec3(0.0f, -1.0f, 0.0f));
        counter++;
        if (counter == 500.0f) {
            swordSounds = false;
            backSwordSounds = false;
            deltaSword1 = 0.0f;
            counter = 0.0f;
        }
    }
    
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    ciriSword.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderSword2(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    model = glm::translate(model, glm::vec3(-27.72f, 10.63f, 13.41f));
    model = glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    if (swordSounds2) {
        deltaSword2 += 0.1;
        model = glm::rotate(model, glm::radians(deltaSword2), glm::vec3(0.0f, 0.0f, 1.0f));
        counter2++;
        if (counter2 == 500.0f) {
            swordSounds2 = false;
            backSwordSounds2 = true;
            deltaSword2 = 0.0f;
            counter2 = 0.0f;
        }
    }
    if (!swordSounds2 && backSwordSounds2) {
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    if (backSwordSounds2) {
        deltaSword2 += 0.1;
        model = glm::rotate(model, glm::radians(deltaSword2), glm::vec3(0.0f, 0.0f, -1.0f));
        counter2++;
        if (counter2 == 500.0f) {
            swordSounds2 = false;
            backSwordSounds2 = false;
            deltaSword2 = 0.0f;
            counter2 = 0.0f;
        }
    }

    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    netflixSword.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void renderDynamicObjects(gps::Shader shader, bool depthPass) {
    distance_silvan = myCamera.cameraPosition.x - 25.84f;
    distance_leshen1 = myCamera.cameraPosition.x - 45.13f;
    distance_leshen2 = myCamera.cameraPosition.x - 13.77;
    distance_leshen3 = myCamera.cameraPosition.x - 26.69;
    distance_alghoul1 = myCamera.cameraPosition.x + 13.45;
    distance_alghoul2 = myCamera.cameraPosition.x + 13.45;
    distance_alghoul3 = myCamera.cameraPosition.x + 27.76;
    distance_alghoul4 = myCamera.cameraPosition.x + 27.76;
    distance_alghoul5 = myCamera.cameraPosition.x - 15.79;
    if (distance_silvan < -45.0f && skyBoxTransition) {
        renderSilvan(shader, depthPass);
    }
    if ((distance_leshen1 > 1.0f && skyBoxTransition) || (distance_leshen1 < -3.0f && skyBoxTransition)) {
        renderLesehen(shader, depthPass, 45.13, 9.45, -24.14, -90.0);
    }
    if (distance_leshen2 > -3.0f && skyBoxTransition) {
        renderLesehen(shader, depthPass, 13.77, 9.60, 7.28, -180.0);
    }
    if (distance_leshen3 > -3.0f && skyBoxTransition) {
        renderLesehen(shader, depthPass, 26.69, 10.15, 2.14, 240.0);
    }
    if (distance_alghoul1 < 3.0f && skyBoxTransition) {
        renderAlghoul(shader, depthPass, -13.45, 2.37, -40.72, -10.0, 0.0);
    }
    if (distance_alghoul2 < 3.0f && skyBoxTransition) {
        renderAlghoul(shader, depthPass, -12.45, 2.3, -40.72, 2.0, 0.0);
    }
    if (distance_alghoul3 < 3.0f && skyBoxTransition) {
        renderAlghoul(shader, depthPass, -27.76, 3.55, -26.42, -20.0, 90.0);
    }
    if (distance_alghoul4 < 3.0f && skyBoxTransition) {
        renderAlghoul(shader, depthPass, -27.76, 3.55, -25.42, -20.0, 90.0);
    }
    if (distance_alghoul5 < 2.0f && skyBoxTransition) {
        renderAlghoul(shader, depthPass, 15.79, 4.03, -27.62, 18.0, 270.0);
    }
}

void renderVillage(gps::Shader shader, bool depthPass) {
    shader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    village.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void drawObjects(gps::Shader shader, bool depthPass) {

    shader.useShaderProgram();

    model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    renderVillage(shader, depthPass);
    renderGeralt(shader, depthPass);
    renderMonster(shader, depthPass);
    renderSuit(shader, depthPass);
    renderTea(shader, depthPass);
    renderSword1(shader, depthPass);
    renderSword2(shader, depthPass);
    renderDynamicObjects(shader, depthPass);
    renderRoata(shader, depthPass);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    if (skyBoxTransition) {
        deltaSkyBox += 0.0001;
        glm::mat4 skyView = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        skyView = glm::rotate(skyView, (deltaSkyBox), glm::vec3(0.0f, 1.0f, 1.0f));
        mySkyBox.Draw(skyboxShader, skyView, projection);
    }
    else {
        mySkyBox.Draw(skyboxShader, view, projection);
    }
}

void renderScene() {

    // depth maps creation pass
    //TODO - Send the light-space transformation matrix to the depth map creation shader and
    //		 render the scene in the depth map

    //render the scene to the depth buffer
    if (transitionNightDay) {
        lightDir.y  = - 40.0f;
    }
    if (transitionNightDay2) {
        lightDir.y = 20.0f;
    }
    depthMapShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(computeLightSpaceTrMatrix()));
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    drawObjects(depthMapShader, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    // render depth map on screen - toggled with the M key

    if (showDepthMap) {
        glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);

        glClear(GL_COLOR_BUFFER_BIT);

        screenQuadShader.useShaderProgram();

        //bind the depth map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

        glDisable(GL_DEPTH_TEST);
        screenQuad.Draw(screenQuadShader);
        glEnable(GL_DEPTH_TEST);
    }
    else {

        // final scene rendering pass (with shadows)

        glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myCustomShader.useShaderProgram();

        view = myCamera.getViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

        //bind the shadow map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

        glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(computeLightSpaceTrMatrix()));

        drawObjects(myCustomShader, false);

        //draw a white cube around the light

        lightShader.useShaderProgram();

        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //glm::mat4 model;
        model = glm::translate(model, 1.0f * lightDir);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        lightCubeColor = glm::vec3(0.0f, 0.0f, 0.0f);
        lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
        glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

        lightCube.Draw(lightShader);
        if (!turnOnSpotLights) {
            lightShader.useShaderProgram();
            //second
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(1.03f, 1.28f, -32.78f)); 
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //third
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.42f, 1.21f, -22.7f));
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //fourth
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.97f, 1.09f, -26.15f)); 
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //five
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.4f, 1.02f, -30.83f)); 
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //six
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.26f, 1.1f, -37.38f)); 
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //seven
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-16.76f, 1.7f, -21.25f));
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //eight
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-16.79f, 1.5f, -23.75f));
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            lightShader.useShaderProgram();
            //nine
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

            model = glm::translate(glm::mat4(1.0f), glm::vec3(-14.6f, 1.27f, -24.82f));
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            lightCubeColor = glm::vec3(0.0f, 0.0f, 1.0f);
            lightCubeColorLoc = glGetUniformLocation(lightShader.shaderProgram, "lightCubeColor");
            glUniform3fv(lightCubeColorLoc, 1, glm::value_ptr(lightCubeColor));

            lightCube.Draw(lightShader);
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        }  
    }
}

void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}

int main(int argc, const char * argv[]) {

    try {
        initOpenGLWindow();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    float x_presentation = 0.0f;
    float y_presentation = 0.0f;
    float z_presentation = 0.0f;
    renderSkyBox();
    glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    initOpenGLState();
	initModels();
	initShaders();
	initUniforms();
    initFBO();
    setWindowCallbacks();

	glCheckError();
	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {
        std::cout << myCamera.cameraPosition.x << " " << myCamera.cameraPosition.y << " " << myCamera.cameraPosition.z << " " << "\n";
        if (presentation) {
            if (fscanf(animPresentation, "%f,%f,%f", &x_presentation, &y_presentation, &z_presentation) == 3) {
                myCamera.cameraPosition.x = x_presentation;
                myCamera.cameraPosition.y = y_presentation;
                myCamera.cameraPosition.z = z_presentation;
            }
            else {
                presentation = false;
            }
        }
        processMovement();
	    renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}

	cleanup();

    return EXIT_SUCCESS;
}
