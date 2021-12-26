#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "glm/glm.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <math.h>

//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;

static std::vector<int> indices;
static std::vector<glm::vec3> normals;
// static std::vector<parser::Vec3f> normals;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void renderFunction(int k, int count) 
{
    
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, static_cast<void*>(&indices[k]));
}

void reshape(int w, int h, parser::Camera &cam){
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float nearHeight = cam.near_plane.w - cam.near_plane.z;
    float degree = atan2(nearHeight, cam.near_distance);
    gluPerspective((degree/M_PI)*180, cam.image_width/cam.image_height, cam.near_distance, cam.far_distance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam.position.x, cam.position.y, cam.position.z, cam.position.x+cam.gaze.x, cam.position.y+cam.gaze.y, cam.position.z+cam.gaze.z, cam.up.x, cam.up.y, cam.up.z);
}

int main(int argc, char* argv[]) {
    scene.loadFromXml(argv[1]);

    for(int i=0; i < scene.vertex_data.size(); i++){
        normals.push_back(glm::vec3(0.0, 0.0, 0.0));
    }
    for(parser::Mesh mesh : scene.meshes){
        for(parser::Face face : mesh.faces){
            indices.push_back(face.v0_id - 1);
            indices.push_back(face.v1_id - 1);
            indices.push_back(face.v2_id - 1);
            glm::vec3 v0 = glm::vec3(scene.vertex_data[face.v0_id - 1].x, scene.vertex_data[face.v0_id - 1].y, scene.vertex_data[face.v0_id - 1].z);
            glm::vec3 v1 = glm::vec3(scene.vertex_data[face.v1_id - 1].x, scene.vertex_data[face.v1_id - 1].y, scene.vertex_data[face.v1_id - 1].z);
            glm::vec3 v2 = glm::vec3(scene.vertex_data[face.v2_id - 1].x, scene.vertex_data[face.v2_id - 1].y, scene.vertex_data[face.v2_id - 1].z);

            // parser::Vec3f v0 = scene.vertex_data[face.v0_id - 1];
            // parser::Vec3f v1 = scene.vertex_data[face.v1_id - 1];
            // parser::Vec3f v2 = scene.vertex_data[face.v2_id - 1];

            glm::vec3 normal = glm::cross(glm::normalize(v1-v0), glm::normalize(v2-v0));
            normal = glm::normalize(normal);
            // parser::Vec3f normal = glNormal3fv();
            normals[face.v0_id-1] += normal;
            normals[face.v1_id-1] += normal;
            normals[face.v2_id-1] += normal;
        }
    }

    for(int i=0; i < normals.size(); i++){
        normals[i] = glm::normalize(normals[i]);
    }

    glfwSetErrorCallback(errorCallback);
    
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);
    reshape(scene.camera.image_width, scene.camera.image_height, scene.camera);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);
    
    glEnable(GL_DEPTH_TEST);
    if(scene.culling_enabled){
        glEnable(GL_CULL_FACE);
        if(scene.culling_face){
            glCullFace(GL_FRONT);
        }else{
            glCullFace(GL_BACK);
        }
    }
    glShadeModel(GL_SMOOTH);


    glEnable(GL_LIGHTING);
    for(int lights=0; lights < scene.point_lights.size(); lights++){
        glEnable(GL_LIGHT0+lights);

        if(lights == 0){
            GLfloat amb[] = {scene.ambient_light.x, scene.ambient_light.y, scene.ambient_light.z, 1.0f};
            glLightfv(GL_LIGHT0+lights, GL_AMBIENT, amb);
        }
        GLfloat pos[] = {scene.point_lights[lights].position.x, scene.point_lights[lights].position.y, scene.point_lights[lights].position.z, 1.0f};
        GLfloat col[] = {scene.point_lights[lights].intensity.x, scene.point_lights[lights].intensity.y, scene.point_lights[lights].intensity.z, 1.0f};
        glLightfv(GL_LIGHT0+lights, GL_POSITION, pos);
        glLightfv(GL_LIGHT0+lights, GL_DIFFUSE, col);
        glLightfv(GL_LIGHT0+lights, GL_SPECULAR, col);

    }

    // float angle = 1;
    // float change = 0.005;
	static std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    int framesRendered = 0;
    bool isFirst = true;

    glVertexPointer(3, GL_FLOAT, 0, static_cast<void*>(& scene.vertex_data[0]));
    glNormalPointer(GL_FLOAT, 0, static_cast<void*>(& normals[0]));

    while(!glfwWindowShouldClose(win)) {

        glfwPollEvents();
        if(isFirst){
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            isFirst = false;
        }

        int index = 0;
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(parser::Mesh mesh : scene.meshes){
            if(mesh.mesh_type == "Solid"){
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            parser::Material temp = scene.materials[mesh.material_id-1];
            GLfloat amb[4] = {temp.ambient.x, temp.ambient.y, temp.ambient.z, 1.0};
            GLfloat diff[4] = {temp.diffuse.x, temp.diffuse.y, temp.diffuse.z, 1.0};
            GLfloat spec[4] = {temp.specular.x, temp.specular.y, temp.specular.z, 1.0};
            GLfloat specExp[1] = {temp.phong_exponent};

            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
            glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
            glMaterialfv(GL_FRONT, GL_SHININESS, specExp);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            // glRotatef(angle, 0, 1, 0);
            // angle+=change;
            // change += 0.001;
            for(int trans=mesh.transformations.size()-1; trans >= 0; trans--){
                if(mesh.transformations[trans].transformation_type == "Translation"){
                    parser::Vec3f translation = scene.translations[mesh.transformations[trans].id - 1];
                    glTranslatef(translation.x, translation.y, translation.z);
                } else if(mesh.transformations[trans].transformation_type == "Rotation"){
                    parser::Vec4f rotation = scene.rotations[mesh.transformations[trans].id - 1];
                    glRotatef(rotation.x, rotation.y, rotation.z, rotation.w);
                } else if (mesh.transformations[trans].transformation_type == "Scaling"){
                    parser::Vec3f scale = scene.scalings[mesh.transformations[trans].id - 1];
                    glScalef(scale.x, scale.y, scale.z);
                }
            
            }
            // if(angle == 360) angle = 0;
            // if(angle == 360) change = 0.05;
            int count = mesh.faces.size()*3;
            static int framesRendered = 0;
	        renderFunction(index, count);
            index += count;

            glPopMatrix();   
        }
        glfwSwapBuffers(win);

        ++framesRendered;
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsedTime = end - start;
        if (elapsedTime.count() > 1.)
        {
            start = std::chrono::system_clock::now();

            std::stringstream stream;
            stream << "CENG477 - HW3 " << " [" << framesRendered/elapsedTime.count() << " FPS]";
            framesRendered = 0;

            glfwSetWindowTitle(win, stream.str().c_str());
        }
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}