#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <math.h>
#include <algorithm>
#include <thread>
#include <time.h>

typedef unsigned char RGB[3];
parser::Camera camera;
parser::Scene scene;


parser::Vec3f operator+(const parser::Vec3f &first, const parser::Vec3f &second){
    parser::Vec3f result;
    result.x = first.x + second.x;
    result.y = first.y + second.y;
    result.z = first.z + second.z;
    return result;
}

parser::Vec3f operator-(const parser::Vec3f &a, const parser::Vec3f &b){
    parser::Vec3f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

parser::Vec3f operator*(const parser::Vec3f &vec, const float scalar){
    parser::Vec3f result;
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    result.z = vec.z * scalar;
    return result;
}

parser::Vec3f operator*(float scalar, const parser::Vec3f &vec){
    parser::Vec3f result;
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    result.z = vec.z * scalar;
    return result;
}

parser::Vec3f operator/(const parser::Vec3f &vec, const float scalar){
    parser::Vec3f result;
    result.x = vec.x / scalar;
    result.y = vec.y / scalar;
    result.z = vec.z / scalar;
    return result;
}

parser::Vec3f crossVec3f(parser::Vec3f &first, parser::Vec3f &second){
    parser::Vec3f result;
    result.x = (first.y*second.z) - (first.z*second.y);
    result.y = (first.z*second.x) - (first.x*second.z);
    result.z = (first.x*second.y) - (first.y*second.x);
    return result;
}

double operator*(const parser::Vec3f &first, const parser::Vec3f &second){
    double result = 0.0;
    result += first.x * second.x;
    result += first.y * second.y;
    result += first.z * second.z;
    return result;
}

void printVec(parser::Vec3f &vec){
    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
    return;
}

double determinant(float matrix[3][3]){
    double result = matrix[0][0] * (matrix[1][1]*matrix[2][2] - matrix[1][2]*matrix[2][1]) +
                    matrix[1][0] * (matrix[0][2]*matrix[2][1] - matrix[0][1]*matrix[2][2]) +
                    matrix[2][0] * (matrix[0][1]*matrix[1][2] - matrix[1][1]*matrix[0][2]);
    return result;
}

bool sphereIntersect(parser::Vec3f &start, parser::Vec3f &sphereCenter, parser::Vec3f &d, double &t_min, float radius){
    parser::Vec3f oc = (start - sphereCenter);
    double b = (d*2) * oc;
    double discriminant = b*b - 4 * (d*d) * ((oc * oc) - radius*radius);
    double t_1, t_2;

    if (discriminant < 0){
        return false;
    } else {
        t_1 = (-2*(d * oc) - sqrt(discriminant)) / 2*(d*d); 
        t_2 = (-2*(d * oc) + sqrt(discriminant)) / 2*(d*d);
    }
    // https://stackoverflow.com/questions/1632145/use-of-min-and-max-functions-in-c
    bool flag = false;
    if(t_1 >= 0.0 && t_1 < t_min){
        t_min = t_1;
        flag = true;
    } 
    if (t_2 >= 0.0 && t_2 < t_min){
        t_min = t_2;
        flag = true;
    }
    return flag;

    //if(t_2 >= 1 && t_2 < t_min) t_min = t_2; //can be deleted since t_1 is smaller than this
}

bool faceIntersect(parser::Vec3f &start, parser::Vec3f &d, parser::Face &face, double &t_min){
    parser::Vec3f v_0 = scene.vertex_data[face.v0_id-1];
    parser::Vec3f v_1 = scene.vertex_data[face.v1_id-1];
    parser::Vec3f v_2 = scene.vertex_data[face.v2_id-1];
    
    float A[3][3] = {
        {(v_0.x - v_1.x), (v_0.x - v_2.x), d.x},
        {(v_0.y - v_1.y), (v_0.y - v_2.y), d.y},
        {(v_0.z - v_1.z), (v_0.z - v_2.z), d.z}                    
    };

    float Beta[3][3] = {
        {(v_0.x - start.x), (v_0.x - v_2.x), d.x},
        {(v_0.y - start.y), (v_0.y - v_2.y), d.y},
        {(v_0.z - start.z), (v_0.z - v_2.z), d.z}                    
    };

    float Gamma[3][3] = {
        {(v_0.x - v_1.x), (v_0.x - start.x), d.x},
        {(v_0.y - v_1.y), (v_0.y - start.y), d.y},
        {(v_0.z - v_1.z), (v_0.z - start.z), d.z}                    
    };

    float t[3][3] = {
        {(v_0.x - v_1.x), (v_0.x - v_2.x), (v_0.x - start.x)},
        {(v_0.y - v_1.y), (v_0.y - v_2.y), (v_0.y - start.y)},
        {(v_0.z - v_1.z), (v_0.z - v_2.z), (v_0.z - start.z)}
    };

    double detA = determinant(A);
    double detBeta = determinant(Beta);
    double detGamma = determinant(Gamma);
    double detT = determinant(t);
    
    if((detT/detA) >= 0.0 && (detT/detA) < t_min && (detBeta/detA) >= 0 && (detGamma/detA) >= 0 && (detGamma + detBeta)/detA <= 1){
        t_min = detT/detA;
        return true;
    }
    return false;
}

bool faceIntersect(parser::Vec3f &start, parser::Vec3f &d, parser::Vec3f &v_0, parser::Vec3f &v_1, parser::Vec3f &v_2, double &t_min){
    
    float A[3][3] = {
        {(v_0.x - v_1.x), (v_0.x - v_2.x), d.x},
        {(v_0.y - v_1.y), (v_0.y - v_2.y), d.y},
        {(v_0.z - v_1.z), (v_0.z - v_2.z), d.z}                    
    };

    float Beta[3][3] = {
        {(v_0.x - start.x), (v_0.x - v_2.x), d.x},
        {(v_0.y - start.y), (v_0.y - v_2.y), d.y},
        {(v_0.z - start.z), (v_0.z - v_2.z), d.z}                    
    };

    float Gamma[3][3] = {
        {(v_0.x - v_1.x), (v_0.x - start.x), d.x},
        {(v_0.y - v_1.y), (v_0.y - start.y), d.y},
        {(v_0.z - v_1.z), (v_0.z - start.z), d.z}                    
    };

    float t[3][3] = {
        {(v_0.x - v_1.x), (v_0.x - v_2.x), (v_0.x - start.x)},
        {(v_0.y - v_1.y), (v_0.y - v_2.y), (v_0.y - start.y)},
        {(v_0.z - v_1.z), (v_0.z - v_2.z), (v_0.z - start.z)}
    };

    double detA = determinant(A);
    double detBeta = determinant(Beta);
    double detGamma = determinant(Gamma);
    double detT = determinant(t);
    
    if((detT/detA) >= 0.0 && (detT/detA) < t_min && (detBeta/detA) >= 0 && (detGamma/detA) >= 0 && (detGamma + detBeta)/detA <= 1){
        t_min = detT/detA;
        return true;
    }
    return false;
}

bool inShadow(parser::Vec3f &start, parser::Vec3f &d, double &length){
    // --- SPHERE INTERSECTION --- //
    double placeHolder = length;
    for(int ind=0; ind < scene.spheres.size(); ind++){
        parser::Sphere sphere = scene.spheres[ind];
        parser::Vec3f sphereCenter = scene.vertex_data[sphere.center_vertex_id-1];
        if(sphereIntersect(start, sphereCenter, d, placeHolder, sphere.radius)){
            
            return true;
        }
    }
    // --- FACE INTERSECTION --- //
    for(int ind=0; ind < scene.triangles.size(); ind++){
        parser::Triangle triangle = scene.triangles[ind];
        parser::Face face = triangle.indices;
        if(faceIntersect(start, d, face, placeHolder)) {
            return true;
        }
        
    }
    for(int meshIndex=0; meshIndex < scene.meshes.size(); meshIndex++){
        parser::Mesh mesh = scene.meshes[meshIndex];
        // --- BVH LIKE SPEEDING --- //
        if(mesh.faces.size() >= 55){
            double t_min = __DBL_MAX__;
            float x_min = __FLT_MAX__;
            float y_min = __FLT_MAX__;
            float z_min = __FLT_MAX__;
            float x_max = __FLT_MIN__;
            float y_max = __FLT_MIN__;
            float z_max = __FLT_MIN__;
            for(parser::Face face : mesh.faces){
                if(scene.vertex_data[face.v0_id - 1].x < x_min) x_min = scene.vertex_data[face.v0_id - 1].x;
                if(scene.vertex_data[face.v0_id - 1].y < y_min) y_min = scene.vertex_data[face.v0_id - 1].y;
                if(scene.vertex_data[face.v0_id - 1].z < z_min) z_min = scene.vertex_data[face.v0_id - 1].z;
                if(scene.vertex_data[face.v0_id - 1].x > x_max) x_max = scene.vertex_data[face.v0_id - 1].x;
                if(scene.vertex_data[face.v0_id - 1].y > y_max) y_max = scene.vertex_data[face.v0_id - 1].y;
                if(scene.vertex_data[face.v0_id - 1].z > z_max) z_max = scene.vertex_data[face.v0_id - 1].z;

                if(scene.vertex_data[face.v1_id - 1].x < x_min) x_min = scene.vertex_data[face.v1_id - 1].x;
                if(scene.vertex_data[face.v1_id - 1].y < y_min) y_min = scene.vertex_data[face.v1_id - 1].y;
                if(scene.vertex_data[face.v1_id - 1].z < z_min) z_min = scene.vertex_data[face.v1_id - 1].z;
                if(scene.vertex_data[face.v1_id - 1].x > x_max) x_max = scene.vertex_data[face.v1_id - 1].x;
                if(scene.vertex_data[face.v1_id - 1].y > y_max) y_max = scene.vertex_data[face.v1_id - 1].y;
                if(scene.vertex_data[face.v1_id - 1].z > z_max) z_max = scene.vertex_data[face.v1_id - 1].z;

                if(scene.vertex_data[face.v2_id - 1].x < x_min) x_min = scene.vertex_data[face.v2_id - 1].x;
                if(scene.vertex_data[face.v2_id - 1].y < y_min) y_min = scene.vertex_data[face.v2_id - 1].y;
                if(scene.vertex_data[face.v2_id - 1].z < z_min) z_min = scene.vertex_data[face.v2_id - 1].z;
                if(scene.vertex_data[face.v2_id - 1].x > x_max) x_max = scene.vertex_data[face.v2_id - 1].x;
                if(scene.vertex_data[face.v2_id - 1].y > y_max) y_max = scene.vertex_data[face.v2_id - 1].y;
                if(scene.vertex_data[face.v2_id - 1].z > z_max) z_max = scene.vertex_data[face.v2_id - 1].z;
            }

            double radius = sqrt(pow(x_max-x_min, 2) + pow(y_max - y_min, 2) + pow(z_max - z_min, 2));
            parser::Vec3f sphereCenter;
            sphereCenter.x = (x_max + x_min) / 2;
            sphereCenter.y = (y_max + y_min) / 2;
            sphereCenter.z = (z_max + z_min) / 2;
            bool intersect = sphereIntersect(start, sphereCenter, d, t_min, radius);
            if(!intersect)continue;

        }
        // ------------------------- //
        for(int ind=0; ind < mesh.faces.size(); ind++){
            parser::Face face = mesh.faces[ind];
            if(faceIntersect(start, d, face, placeHolder)){
                
                return true;
            } 
        }
    }
    return false;
}

parser::Vec3f colorize(parser::Vec3f &intersectionCoords, parser::Vec3f &endOfRay, int &objectType, int &objectId, int &meshInd, float &red, float &green, float &blue,
            parser::Vec3f &reflect){
    // --- LIGHTING --- //
    parser::Material material;
    parser::Vec3f normal;

    switch (objectType)
    {
    case 0: // Sphere
    {
        parser::Sphere sphere = scene.spheres[objectId];
        material = scene.materials[sphere.material_id - 1];
        normal = (intersectionCoords - scene.vertex_data[sphere.center_vertex_id - 1]) / sphere.radius;
        //std::cout << sqrt(pow(normalVecX, 2) + pow(normalVecY, 2) + pow(normalVecZ, 2)) << std::endl;
        break;
    }
    case 1: // Triangle
    {
        // v_1 - v_0 x v_2 - v_0 / |crossProduct|
        parser::Face face = scene.triangles[objectId].indices;
        parser::Vec3f v_0 = scene.vertex_data[face.v0_id - 1];
        parser::Vec3f v_1 = scene.vertex_data[face.v1_id - 1];
        parser::Vec3f v_2 = scene.vertex_data[face.v2_id - 1];
        
        parser::Vec3f first = v_1-v_0, second = v_2-v_0;
        normal = crossVec3f(first, second);
        normal = normal / sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));

        material = scene.materials[scene.triangles[objectId].material_id - 1];
        break;
    }
    case 2: // Mesh
    {
        parser::Face face = scene.meshes[meshInd].faces[objectId];
        parser::Vec3f v_0 = scene.vertex_data[face.v0_id - 1];
        parser::Vec3f v_1 = scene.vertex_data[face.v1_id - 1];
        parser::Vec3f v_2 = scene.vertex_data[face.v2_id - 1];
        parser::Vec3f first = v_1-v_0, second = v_2-v_0;
        normal = crossVec3f(first, second);
        normal = normal / sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));

        material = scene.materials[scene.meshes[meshInd].material_id - 1];
        break;
    }
    default:
        break;
    }

    // ---- AMBIENT LIGHTING --- //
    parser::Vec3f ambientReflectence = material.ambient;
    
    red += ambientReflectence.x * scene.ambient_light.x * reflect.x;
    green += ambientReflectence.y * scene.ambient_light.y * reflect.y;
    blue += ambientReflectence.z * scene.ambient_light.z * reflect.z;
    
    parser::Vec3f w_o = endOfRay - intersectionCoords;
    w_o = w_o/sqrt(pow(w_o.x, 2) + pow(w_o.y, 2) + pow(w_o.z, 2));
    // --- LIGHT SOURCES LOOP --- //
    for(parser::PointLight lightSource : scene.point_lights){
        // --- SHADOW CONTROL --- //
        parser::Vec3f w_i = (lightSource.position - intersectionCoords);
        

        double r_quad = pow(w_i.x, 2) + pow(w_i.y, 2) + pow(w_i.z, 2);
        double normalizeShadowRay = sqrt(pow(w_i.x, 2) + pow(w_i.y, 2) + pow(w_i.z, 2));
        w_i = w_i / normalizeShadowRay;
        parser::Vec3f startWEpsilon = intersectionCoords + w_i*scene.shadow_ray_epsilon;
        if(inShadow(startWEpsilon, w_i, normalizeShadowRay)){
            continue;
        }
        // --- DIFUSE SHADING --- //
        parser::Vec3f kd = material.diffuse;
        double cosThetaWr = std::max(0.0, w_i*normal) / r_quad;
        red += kd.x * lightSource.intensity.x * cosThetaWr * reflect.x;
        green += kd.y * lightSource.intensity.y * cosThetaWr * reflect.y;
        blue += kd.z * lightSource.intensity.z * cosThetaWr * reflect.z;

        // --- BLINN-PHONG (SPECULAR) SHADING --- //
        parser::Vec3f halfVector = w_i + w_o;
        halfVector = halfVector / sqrt(pow(halfVector.x, 2) + pow(halfVector.y, 2) + pow(halfVector.z, 2));
        parser::Vec3f ks = material.specular;
        double cosSpecularWExp = pow(std::max(0.0, normal*halfVector), material.phong_exponent) / r_quad;
        red += ks.x * cosSpecularWExp * lightSource.intensity.x * reflect.x;
        green += ks.y * cosSpecularWExp * lightSource.intensity.y * reflect.y;
        blue += ks.z * cosSpecularWExp * lightSource.intensity.z * reflect.z;
    }
    parser::Vec3f w_r = w_o * -1 + normal * 2 * (normal * w_o);
    reflect.x *= material.mirror.x;
    reflect.y *= material.mirror.y;
    reflect.z *= material.mirror.z;
    return w_r;
}


 void trace_ray(int width_start,int width_end,int height_start,int height_end,unsigned char* image,int indice){

     //parser::Vec3f cameraPixels[camera.image_height][camera.image_width];
        parser::Vec3f e = camera.position;
        parser::Vec3f v = camera.up;
        parser::Vec3f w = camera.gaze*-1;
        parser::Vec3f u = crossVec3f(v, w);
        parser::Vec3f m = e - w*camera.near_distance;
        parser::Vec3f q = m + u*camera.near_plane.x + v*camera.near_plane.w;
        int ind = indice;
        for(int i=height_start; i < height_end; i++){
            for(int j=width_start; j < width_end; j++){
                float su = (camera.near_plane.y - camera.near_plane.x)/camera.image_width*(j + 0.5);
                float sv = (camera.near_plane.w - camera.near_plane.z)/camera.image_height*(i + 0.5);
                parser::Vec3f s = q + u*su - v*sv;
                float normalize = sqrt(pow(s.x-e.x, 2) + pow(s.y-e.y, 2) + pow(s.z-e.z, 2));
                parser::Vec3f d = (s - e)/normalize;
                // --- INTERSECTION CONTROLS --- //
                // TO DO: object assing when t_min is found
                double t_min = __DBL_MAX__;
                int objectType = -1;
                int objectId = -1;
                int meshInd = -1;
                // --- SPHERE INTERSECTION --- //
                for(int ind=0; ind < scene.spheres.size(); ind++){
                    parser::Sphere sphere = scene.spheres[ind];
                    parser::Vec3f sphereCenter = scene.vertex_data[sphere.center_vertex_id-1];
                    if(sphereIntersect(camera.position, sphereCenter, d, t_min, sphere.radius)){
                        objectType = 0;
                        objectId = ind;
                    }
                }
                
                // --- FACE INTERSECTION --- //
                for(int ind=0; ind < scene.triangles.size(); ind++){
                    parser::Triangle triangle = scene.triangles[ind];
                    parser::Face face = triangle.indices;
                    if(faceIntersect(camera.position, d, face, t_min)){
                        objectType = 1;
                        objectId = ind;
                    }
                }

                for(int meshIndex=0; meshIndex < scene.meshes.size(); meshIndex++){
                    parser::Mesh mesh = scene.meshes[meshIndex];
                    // --- BVH LIKE SPEEDING --- //
                    if(mesh.faces.size() >= 55){
                        double t_min = __DBL_MAX__;
                        float x_min = __FLT_MAX__;
                        float y_min = __FLT_MAX__;
                        float z_min = __FLT_MAX__;
                        float x_max = __FLT_MIN__;
                        float y_max = __FLT_MIN__;
                        float z_max = __FLT_MIN__;
                        for(parser::Face face : mesh.faces){
                            if(scene.vertex_data[face.v0_id - 1].x < x_min) x_min = scene.vertex_data[face.v0_id - 1].x;
                            if(scene.vertex_data[face.v0_id - 1].y < y_min) y_min = scene.vertex_data[face.v0_id - 1].y;
                            if(scene.vertex_data[face.v0_id - 1].z < z_min) z_min = scene.vertex_data[face.v0_id - 1].z;
                            if(scene.vertex_data[face.v0_id - 1].x > x_max) x_max = scene.vertex_data[face.v0_id - 1].x;
                            if(scene.vertex_data[face.v0_id - 1].y > y_max) y_max = scene.vertex_data[face.v0_id - 1].y;
                            if(scene.vertex_data[face.v0_id - 1].z > z_max) z_max = scene.vertex_data[face.v0_id - 1].z;

                            if(scene.vertex_data[face.v1_id - 1].x < x_min) x_min = scene.vertex_data[face.v1_id - 1].x;
                            if(scene.vertex_data[face.v1_id - 1].y < y_min) y_min = scene.vertex_data[face.v1_id - 1].y;
                            if(scene.vertex_data[face.v1_id - 1].z < z_min) z_min = scene.vertex_data[face.v1_id - 1].z;
                            if(scene.vertex_data[face.v1_id - 1].x > x_max) x_max = scene.vertex_data[face.v1_id - 1].x;
                            if(scene.vertex_data[face.v1_id - 1].y > y_max) y_max = scene.vertex_data[face.v1_id - 1].y;
                            if(scene.vertex_data[face.v1_id - 1].z > z_max) z_max = scene.vertex_data[face.v1_id - 1].z;

                            if(scene.vertex_data[face.v2_id - 1].x < x_min) x_min = scene.vertex_data[face.v2_id - 1].x;
                            if(scene.vertex_data[face.v2_id - 1].y < y_min) y_min = scene.vertex_data[face.v2_id - 1].y;
                            if(scene.vertex_data[face.v2_id - 1].z < z_min) z_min = scene.vertex_data[face.v2_id - 1].z;
                            if(scene.vertex_data[face.v2_id - 1].x > x_max) x_max = scene.vertex_data[face.v2_id - 1].x;
                            if(scene.vertex_data[face.v2_id - 1].y > y_max) y_max = scene.vertex_data[face.v2_id - 1].y;
                            if(scene.vertex_data[face.v2_id - 1].z > z_max) z_max = scene.vertex_data[face.v2_id - 1].z;
                        }

                        double radius = sqrt(pow(x_max-x_min, 2) + pow(y_max - y_min, 2) + pow(z_max - z_min, 2));
                        parser::Vec3f sphereCenter;
                        sphereCenter.x = (x_max + x_min) / 2;
                        sphereCenter.y = (y_max + y_min) / 2;
                        sphereCenter.z = (z_max + z_min) / 2;
                        bool intersect = sphereIntersect(camera.position, sphereCenter, d, t_min, radius);
                        if(!intersect)continue;

                    }
                    // ------------------------- //
                    for(int ind=0; ind < mesh.faces.size(); ind++){
                        parser::Face face = mesh.faces[ind];
                        if(faceIntersect(camera.position, d, face, t_min)){
                            meshInd = meshIndex;
                            objectType = 2;
                            objectId = ind;
                        }
                    }
                }

                if(t_min < __DBL_MAX__){
                    parser::Vec3f intersectionCoords = e + d*t_min;
                    float red = 0.0, green=0.0, blue=0.0;
                    parser::Vec3f reflect;
                    reflect.x = 1;
                    reflect.y = 1;
                    reflect.z = 1;

                    parser::Vec3f w_r;
                    w_r = colorize(intersectionCoords, camera.position, objectType, objectId, meshInd, red, green, blue, reflect);
                    
                    for(int mirror=0; mirror < scene.max_recursion_depth; mirror++){
                
                        if(reflect.x == 0 && reflect.y == 0 && reflect.z == 0) break;
                        double t_min = __DBL_MAX__;
                        int objectType = -1;
                        int objectId = -1;
                        int meshInd = -1;
                        intersectionCoords = intersectionCoords + scene.shadow_ray_epsilon * w_r;
                        // --- SPHERE INTERSECTION --- //
                        for(int ind=0; ind < scene.spheres.size(); ind++){
                            parser::Sphere sphere = scene.spheres[ind];
                            parser::Vec3f sphereCenter = scene.vertex_data[sphere.center_vertex_id-1];
                            if(sphereIntersect(intersectionCoords, sphereCenter, w_r, t_min, sphere.radius)){
                                    objectType = 0;
                                    objectId = ind;
                                }
                        }
                            
                        // --- FACE INTERSECTION --- //
                        for(int ind=0; ind < scene.triangles.size(); ind++){
                            parser::Triangle triangle = scene.triangles[ind];
                            parser::Face face = triangle.indices;
                            if(faceIntersect(intersectionCoords, w_r, face, t_min)){
                                objectType = 1;
                                objectId = ind;
                            }
                        }

                        for(int meshIndex=0; meshIndex < scene.meshes.size(); meshIndex++){
                            parser::Mesh mesh = scene.meshes[meshIndex];
                            // --- BVH LIKE SPEEDING --- //
                            if(mesh.faces.size() >= 55){
                                double t_min = __DBL_MAX__;
                                float x_min = __FLT_MAX__;
                                float y_min = __FLT_MAX__;
                                float z_min = __FLT_MAX__;
                                float x_max = __FLT_MIN__;
                                float y_max = __FLT_MIN__;
                                float z_max = __FLT_MIN__;
                                for(parser::Face face : mesh.faces){
                                    if(scene.vertex_data[face.v0_id - 1].x < x_min) x_min = scene.vertex_data[face.v0_id - 1].x;
                                    if(scene.vertex_data[face.v0_id - 1].y < y_min) y_min = scene.vertex_data[face.v0_id - 1].y;
                                    if(scene.vertex_data[face.v0_id - 1].z < z_min) z_min = scene.vertex_data[face.v0_id - 1].z;
                                    if(scene.vertex_data[face.v0_id - 1].x > x_max) x_max = scene.vertex_data[face.v0_id - 1].x;
                                    if(scene.vertex_data[face.v0_id - 1].y > y_max) y_max = scene.vertex_data[face.v0_id - 1].y;
                                    if(scene.vertex_data[face.v0_id - 1].z > z_max) z_max = scene.vertex_data[face.v0_id - 1].z;

                                    if(scene.vertex_data[face.v1_id - 1].x < x_min) x_min = scene.vertex_data[face.v1_id - 1].x;
                                    if(scene.vertex_data[face.v1_id - 1].y < y_min) y_min = scene.vertex_data[face.v1_id - 1].y;
                                    if(scene.vertex_data[face.v1_id - 1].z < z_min) z_min = scene.vertex_data[face.v1_id - 1].z;
                                    if(scene.vertex_data[face.v1_id - 1].x > x_max) x_max = scene.vertex_data[face.v1_id - 1].x;
                                    if(scene.vertex_data[face.v1_id - 1].y > y_max) y_max = scene.vertex_data[face.v1_id - 1].y;
                                    if(scene.vertex_data[face.v1_id - 1].z > z_max) z_max = scene.vertex_data[face.v1_id - 1].z;

                                    if(scene.vertex_data[face.v2_id - 1].x < x_min) x_min = scene.vertex_data[face.v2_id - 1].x;
                                    if(scene.vertex_data[face.v2_id - 1].y < y_min) y_min = scene.vertex_data[face.v2_id - 1].y;
                                    if(scene.vertex_data[face.v2_id - 1].z < z_min) z_min = scene.vertex_data[face.v2_id - 1].z;
                                    if(scene.vertex_data[face.v2_id - 1].x > x_max) x_max = scene.vertex_data[face.v2_id - 1].x;
                                    if(scene.vertex_data[face.v2_id - 1].y > y_max) y_max = scene.vertex_data[face.v2_id - 1].y;
                                    if(scene.vertex_data[face.v2_id - 1].z > z_max) z_max = scene.vertex_data[face.v2_id - 1].z;
                                }

                                double radius = sqrt(pow(x_max-x_min, 2) + pow(y_max - y_min, 2) + pow(z_max - z_min, 2));
                                parser::Vec3f sphereCenter;
                                sphereCenter.x = (x_max + x_min) / 2;
                                sphereCenter.y = (y_max + y_min) / 2;
                                sphereCenter.z = (z_max + z_min) / 2;
                                bool intersect = sphereIntersect(intersectionCoords, sphereCenter, w_r, t_min, radius);
                                if(!intersect)continue;

                            }
                            // ------------------------- //
                            for(int ind=0; ind < mesh.faces.size(); ind++){
                                parser::Face face = mesh.faces[ind];
                                if(faceIntersect(intersectionCoords, w_r, face, t_min)){
                                    meshInd = meshIndex;
                                    objectType = 2;
                                    objectId = ind;
                                }
                            }
                        }

                        if(t_min < __DBL_MAX__){
                            parser::Vec3f newIntersectionCoords = intersectionCoords + w_r*t_min;
                            w_r = colorize(newIntersectionCoords, intersectionCoords, objectType, objectId, meshInd, red, green, blue, reflect);
                            intersectionCoords = newIntersectionCoords;
                        }
                        else {
                            red += reflect.x * scene.background_color.x;
                            green += reflect.y * scene.background_color.y;
                            blue += reflect.z * scene.background_color.z;
                            break;
                        }

                    }


                // --- PIXEL COLOR ASSIGNMENT --- //
                    image[ind++] = round(std::min(red, 255.0f));
                    image[ind++] = round(std::min(green, 255.0f));
                    image[ind++] = round(std::min(blue, 255.0f));
                } else {
                    image[ind++] = scene.background_color.x;
                    image[ind++] = scene.background_color.y;
                    image[ind++] = scene.background_color.z;
                }
            }
        }

        
    }
    //left-right-bottom-top

    // r(t) = e + (s-e)t
    // r(t) = Vec3f direction 
    //Camera = 800*800
    //Vector<Vec3f> RayArray[800][800] = {[0][0] = Vec3f<direction>, .., }

    //Sphere intersection, Face Intersection
    //CameraPixels[800][800] if intersects(CameraPixels[i][j]): CameraPixels[i][j] = 1, else = 0

    // --- PSUEDO CODE FOR RAY TRACING --- //
    // foreach pixel s:
    //     compute the viewing ray r (from e to s)
    //     t min = ∞, obj = NULL
    //     foreach object o:
    //         if r intersects o at point x:
    //             if t < t min :
    //                 t min = t, obj = o
    //     if obj not NULL: // viewing ray intersected with an object
    //         pixel_color = L_a // ambient shading is not affected by shadows
    //         foreach light l:
    //             compute the shadow ray s from x to l
    //             foreach object p:
    //                 if s intersects p before the light source:
    //                     continue the light loop; // point is in shadow – no contribution from this light
    //             pixel_color += L_d + L_s + L_m // add diffuse and specular components for this light source
    //     else:
    //         pixel color = color of background (or black)

    // --- SIMPLE PPM WRITER USAGE --- //

    // const RGB BAR_COLOR[8] =
    // {
    //     { 255, 255, 255 },  // 100% White
    //     { 255, 255,   0 },  // Yellow
    //     {   0, 255, 255 },  // Cyan
    //     {   0, 255,   0 },  // Green
    //     { 255,   0, 255 },  // Magenta
    //     { 255,   0,   0 },  // Red
    //     {   0,   0, 255 },  // Blue
    //     {   0,   0,   0 },  // Black
    // };

    // int width = 640, height = 480;
    // int columnWidth = width / 8;

    // unsigned char* image = new unsigned char [width * height * 3];

    // int i = 0;
    // for (int y = 0; y < height; ++y)
    // {
    //     for (int x = 0; x < width; ++x)
    //     {
    //         int colIdx = x / columnWidth;
    //         image[i++] = BAR_COLOR[colIdx][0];
    //         image[i++] = BAR_COLOR[colIdx][1];
    //         image[i++] = BAR_COLOR[colIdx][2];
    //     }
    // }

    // write_ppm("test.ppm", image, width, height);
        
    


int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    scene.loadFromXml(argv[1]);
    
    int face_count = 0;
    for(parser::Mesh mesh : scene.meshes){
        //std::cout << mesh.faces.size() << std::endl;
        face_count += mesh.faces.size();
    }
    //std::cout << "Total face number: " << std::endl;
    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.
    
    // --- FACE NORMALS --- //
    // std::vector<float> triangle_normals;
    // std::vector<std::vector<float>> mesh_normals;

    // // v_1 - v_0 x v_2 - v_0 / |crossProduct|
    // for(parser::Triangle triangle : scene.triangles){
    //     parser::Vec3f v_0 = scene.vertex_data[triangle.indices.v0_id - 1];
    //     parser::Vec3f v_1 = scene.vertex_data[triangle.indices.v1_id - 1];
    //     parser::Vec3f v_2 = scene.vertex_data[triangle.indices.v2_id - 1];


    // }
    
    // for(parser::Mesh mesh : scene.meshes){
    //     for(parser::Face face : mesh.faces){
        
    //     }    
    // }
    

   
    //time_t start = time(0);
    //char* dt = ctime(&start);
    //std::cout << "The local date and time is: " << dt << std::endl;

    for(int cam=0; cam < scene.cameras.size(); cam++){
        camera = scene.cameras[cam];
        int height = camera.image_height;
        int width = camera.image_width;
        unsigned char* image = new unsigned char [width * height * 3];
        
        // thread params width_start,width_end,height_start,height_end
        std::thread tracer1 = std::thread(trace_ray,0,width,0,height/4,image,0);
        std::thread tracer2 = std::thread(trace_ray,0,width,height/4,height/2,image,width *3*(height/4));
        std::thread tracer3 = std::thread(trace_ray,0,width,height/2,(height*3)/4,image,width*3*(height/2));
        std::thread tracer4 = std::thread(trace_ray,0,width,(height*3)/4,height,image,width*3*((height*3)/4));

        // std::thread tracer1 = std::thread(trace_ray,0,width,0,2*height/5,image,0);
        // std::thread tracer2 = std::thread(trace_ray,0,width,2*height/5,3*height/5,image,width *3*(height/5*2));
        // std::thread tracer3 = std::thread(trace_ray,0,width,3*height/5,(height*4)/5,image,width*3*(height/5*3));
        // std::thread tracer4 = std::thread(trace_ray,0,width,(height*4)/5,height,image,width*3*((height*4)/5));
        
        tracer1.join();
        tracer2.join();
        tracer3.join();
        tracer4.join();

        const char* image_name = camera.image_name.c_str();
        write_ppm(image_name, image, camera.image_width, camera.image_height);
    }
    
    //std::cout << time(NULL) - start << std::endl;

}
