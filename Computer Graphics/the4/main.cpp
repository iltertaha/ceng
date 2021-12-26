#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
// #include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static GLFWwindow* win = NULL;
int widthWindow = 1000, heightWindow = 1000;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;

// Buffers
GLuint idVertexBuffer;
GLuint idIndexBuffer;
GLuint VAO;

GLint locationMV;
GLint locationMVP;
GLint wText;
GLint hText;
GLint heightFactLoc;
GLint lightPosLoc;
GLint cameraPosLoc;
GLint slideLoc, slide = 0;
GLint intensityLoc;
GLint rgbTextureLoc, heightImageLoc;
glm::vec3 lightPos;
glm::vec3 intensity;

GLuint vertex_buffer;

int textureWidth, textureHeight;
float heightFactor = 10.0;

// Camera camera;
// Camera initialCamera;

float camSpeed = 0.0f;
glm::vec3 eye = glm::vec3(500.0f, 100.0f, -250.0f);
glm::vec3 u = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 v = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 modelM;
glm::mat4 projM; 
GLuint depthMapFBO;
GLuint depthCubemap;
bool lightPosFlag = false;

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if(key == GLFW_KEY_R){
        heightFactor += 0.5; 
        glUniform1f(heightFactLoc, heightFactor);
    } else if (key == GLFW_KEY_F){
        heightFactor -= 0.5; 
        glUniform1f(heightFactLoc, heightFactor);
    } else if (key == GLFW_KEY_E){
        slide -= 1;
        glUniform1i(slideLoc, slide);
    } else if (key == GLFW_KEY_Q){
        slide += 1;
        glUniform1i(slideLoc, slide);
    } else if(key == GLFW_KEY_Y){
        camSpeed += 0.01;
    } else if(key == GLFW_KEY_H){
        camSpeed -= 0.01;
    } else if(key == GLFW_KEY_A){
      glm::mat4 rotationMat = glm::rotate(glm::radians(0.05f), v);
      u = rotationMat * glm::vec4(u, 1);
      u = glm::normalize(u);
    } else if(key == GLFW_KEY_D){
      glm::mat4 rotationMat = glm::rotate(glm::radians(-0.05f), v);
      u = rotationMat * glm::vec4(u, 1);
      u = glm::normalize(u);
    } else if(key == GLFW_KEY_S){
      glm::mat4 rotationMat = glm::rotate(glm::radians(0.05f), u);
      v = rotationMat * glm::vec4(v, 1);
      v = glm::normalize(v);
    } else if(key == GLFW_KEY_W){
      glm::mat4 rotationMat = glm::rotate(glm::radians(-0.05f), u);
      v = rotationMat * glm::vec4(v, 1);
      v = glm::normalize(v);
    } else if(key == GLFW_KEY_I){
      eye = glm::vec3(500.0f, 100.0f, -250.0f);
      u = glm::vec3(1.0f, 0.0f, 0.0f);
      v = glm::vec3(0.0f, 1.0f, 0.0f);
      camSpeed = 0.0;
    } else if(key == GLFW_KEY_X){
      camSpeed = 0.0;
    } else if (key == GLFW_KEY_UP){
      lightPos.z += 0.5;
      glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
    } else if (key == GLFW_KEY_DOWN){
      lightPos.z -= 0.5;
      glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
    } else if (key == GLFW_KEY_LEFT){
      lightPos.x += 0.5;
      glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
    } else if (key == GLFW_KEY_RIGHT){
      lightPos.x -= 0.5;
      glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
    } else if (key == GLFW_KEY_T){
      lightPos.y += 0.5;
      glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
    } else if (key == GLFW_KEY_G){
      lightPos.y -= 0.5;
      glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
    } else if (key == GLFW_KEY_P){
      GLFWmonitor *monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode *mode = glfwGetVideoMode(monitor);
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
      glViewport(0, 0, mode->width, mode->height);
    }
}

void reshape(int w, int h)   // Create The Reshape Function (the viewport)
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);


}

void createVertices(std::vector<float> &vertexData, std::vector<float> &textCoord, GLubyte* img){
  for(int i=0; i < textureHeight+1; i++)
    for(int j=0; j < textureWidth+1; j++){
      vertexData.push_back(j); // x
      vertexData.push_back(0); // y
      vertexData.push_back(i); // z

      textCoord.push_back(j); // x
      textCoord.push_back(i); // z
    }
}

void createIndices(std::vector<int> &indicesData){
  for(int i=0; i < textureHeight; i++)
    for(int j=0; j < textureWidth; j++){
      // left triangle indices
      uint leftBotPixel = (textureWidth+1)*i + j;
      indicesData.push_back(leftBotPixel);
      indicesData.push_back(leftBotPixel + 1);
      indicesData.push_back((textureWidth+1) + leftBotPixel);

      // right triangle indices
      indicesData.push_back(leftBotPixel + 1);
      indicesData.push_back((textureWidth+1)+ leftBotPixel + 1);
      indicesData.push_back((textureWidth+1)+ leftBotPixel);
    }
}



int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Please provide height and texture image files!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This might be used for local

  win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);

  if (!win) {
      glfwTerminate();
      exit(-1);
  }
  glfwMakeContextCurrent(win);
  reshape(1000, 1000);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

      glfwTerminate();
      exit(-1);
  }


  std::string shaderShaderString = "shader.vert";
  std::string vertexShaderString = "shader.frag";
  initShaders(idProgramShader, shaderShaderString, vertexShaderString);
  

  glfwSetKeyCallback(win, keyCallback);

  initTexture(argv[1], argv[2], &textureWidth, &textureHeight);

  std::vector<float> vertexData, textCoord; // Make GLfloat
  std::vector<int> indexData; // Make GLfloat
  std::vector<glm::vec3> normalData;

  GLubyte* img = new GLubyte[1000*500*3*sizeof(GLubyte)];
  glBindTexture(GL_TEXTURE_2D, idHeightTexture);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

  createVertices(vertexData, textCoord, img);
  createIndices(indexData);

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indexData.size(), static_cast<void*>(& indexData[0]), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), static_cast<void*>(& vertexData[0]), GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0); 

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0); 


  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  locationMV = (GLint) glGetUniformLocation(idProgramShader, "MV");
  locationMVP = (GLint) glGetUniformLocation(idProgramShader, "MVP");
  wText = (GLint) glGetUniformLocation(idProgramShader, "widthTexture");
  hText = (GLint) glGetUniformLocation(idProgramShader, "heightTexture");
  heightFactLoc = (GLint) glGetUniformLocation(idProgramShader, "heightFactor");
  slideLoc = (GLint) glGetUniformLocation(idProgramShader, "slide");
  lightPosLoc = (GLint) glGetUniformLocation(idProgramShader, "lightPos");
  cameraPosLoc = (GLint) glGetUniformLocation(idProgramShader, "cameraPosition");
  intensityLoc = (GLint) glGetUniformLocation(idProgramShader, "intensity");
  rgbTextureLoc = (GLint) glGetUniformLocation(idProgramShader, "rgbTexture");
  heightImageLoc = (GLint) glGetUniformLocation(idProgramShader, "heightImage");

  glUseProgram(idProgramShader);
  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
 
  modelM = glm::lookAt(eye, eye+glm::cross(u, v), v);
  projM = glm::perspective(45.0, 1.0, 0.1, 1000.0);
  lightPos = glm::vec3(textureWidth/2, 100, textureHeight/2);
  intensity = glm::vec3(1.0f, 1.0f, 1.0f);
  
  // std::cout << glm::to_string(glm::cross(glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0))) << std::endl;

  glUniformMatrix4fv(locationMV, 1, GL_FALSE, glm::value_ptr(modelM));
  glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(projM*modelM));
  glUniform4fv(lightPosLoc, 1, glm::value_ptr(glm::vec4(lightPos, 1.0f)));
  glUniform4fv(cameraPosLoc, 1, glm::value_ptr(glm::vec4(eye, 1.0f)));
  glUniform4fv(intensityLoc, 1, glm::value_ptr(glm::vec4(intensity, 1.0f)));
  glUniform1i(wText, textureWidth);
  glUniform1i(hText, textureHeight);
  glUniform1f(heightFactLoc, heightFactor);
  glUniform1i(slideLoc, slide);
  glUniform1i(rgbTextureLoc, 0);
  glUniform1i(heightImageLoc, 1);

  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  int w, h;
  while (!glfwWindowShouldClose(win))
  {
      glfwGetWindowSize(win, &w, &h);
      glViewport(0, 0, w, h);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClearDepth(1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      eye = eye + camSpeed * glm::cross(u, v);
      modelM = glm::lookAt(eye, eye+glm::cross(u, v), v);

      glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, 0);
      glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(projM*modelM));
      glUniform4fv(cameraPosLoc, 1, glm::value_ptr(glm::vec4(eye, 1.0f)));

      glfwSwapBuffers(win);
      glfwPollEvents();
  }


  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
