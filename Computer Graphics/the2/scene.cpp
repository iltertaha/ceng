#include "scene.h"
#include <iostream>
#include <stdlib.h>
#include "ppm.h"
#include "jpeg.h"
#include <string.h>

namespace fst
{
    void Scene::loadFromParser(const parser::Scene& parser)
    {
        for (auto& camera : parser.cameras)
        {
            cameras.push_back(Camera(
                math::Vector3f(camera.position.x, camera.position.y, camera.position.z),
                math::Vector3f(camera.gaze.x, camera.gaze.y, camera.gaze.z),
                math::Vector3f(camera.up.x, camera.up.y, camera.up.z),
                math::Vector4f(camera.near_plane.x, camera.near_plane.y, camera.near_plane.z, camera.near_plane.w),
                math::Vector2f(camera.image_width, camera.image_height),
                camera.image_name,
                camera.near_distance));
        }

        for (auto& pointlight : parser.point_lights)
        {
            point_lights.push_back(PointLight(
                math::Vector3f(pointlight.position.x, pointlight.position.y, pointlight.position.z),
                math::Vector3f(pointlight.intensity.x, pointlight.intensity.y, pointlight.intensity.z)));
        }

        for (auto& material : parser.materials)
        {
            materials.push_back(Material(
                math::Vector3f(material.ambient.x, material.ambient.y, material.ambient.z),
                math::Vector3f(material.diffuse.x, material.diffuse.y, material.diffuse.z),
                math::Vector3f(material.specular.x, material.specular.y, material.specular.z),
                math::Vector3f(material.mirror.x, material.mirror.y, material.mirror.z),
                material.phong_exponent));
        }

        for (auto &translation : parser.translations)
        {
            translations.push_back(
                Translation(translation.x, translation.y, translation.z));
        }

        for (auto &scaling : parser.scalings)
        {
            scalings.push_back(
                Scaling(scaling.x, scaling.y, scaling.z));
        }

        for (auto &rotation : parser.rotations)
        {
            rotations.push_back(
                Rotation(rotation.angle, rotation.x, rotation.y, rotation.z));
        }

        for (auto& vertex : parser.vertex_data)
        {
            vertex_data.push_back(math::Vector3f(vertex.x, vertex.y, vertex.z));
        }

        for (auto& tex_coord : parser.tex_coord_data)
        {
            tex_coord_data.push_back(math::Vector2f(tex_coord.x, tex_coord.y));
        }

        for (auto& mesh : parser.meshes)
        {
            std::vector<Triangle> triangles;
            std::vector<int> faceIds;
            for (auto& face : mesh.faces)
            {   
                faceIds.push_back(face.v0_id);
                faceIds.push_back(face.v1_id);
                faceIds.push_back(face.v2_id);
                triangles.push_back(Triangle(
                    vertex_data[face.v0_id - 1],
                    vertex_data[face.v1_id - 1] - vertex_data[face.v0_id - 1],
                    vertex_data[face.v2_id - 1] - vertex_data[face.v0_id - 1]));
            }

            meshes.push_back(Mesh(std::move(triangles), std::move(faceIds),  mesh.material_id, mesh.texture_id, mesh.transformations));
        }

        for (auto& triangle : parser.triangles)
        {
            std::vector<Triangle> triangles;
            std::vector<int> faceIds;
            
            faceIds.push_back(triangle.indices.v0_id);
            faceIds.push_back(triangle.indices.v1_id);
            faceIds.push_back(triangle.indices.v2_id);
            triangles.push_back(Triangle(
                vertex_data[triangle.indices.v0_id - 1],
                vertex_data[triangle.indices.v1_id - 1] - vertex_data[triangle.indices.v0_id - 1],
                vertex_data[triangle.indices.v2_id - 1] - vertex_data[triangle.indices.v0_id - 1]));

            meshes.push_back(Mesh(std::move(triangles), std::move(faceIds), triangle.material_id, triangle.texture_id, triangle.transformations));
        }

        for (auto& sphere : parser.spheres)
        {
            spheres.push_back(Sphere(vertex_data[sphere.center_vertex_id - 1],
                sphere.radius, sphere.material_id, sphere.texture_id, sphere.transformations));
        }

        for(auto& texture : parser.textures)
        {
            char* cstr = new char[texture.imageName.length() + 1];
            strcpy(cstr, texture.imageName.c_str());
            int w, h;
            read_jpeg_header(cstr, w, h);
            unsigned char* jpegImage = new unsigned char [w * h * 3];
            read_jpeg(cstr, jpegImage, w, h);
            delete [] cstr;

            textures.push_back(Texture(w, h, jpegImage
                , texture.imageName, texture.interpolation
                , texture.decalMode, texture.appearance));
        }

        background_color = math::Vector3f(parser.background_color.x, parser.background_color.y, parser.background_color.z);
        ambient_light = math::Vector3f(parser.ambient_light.x, parser.ambient_light.y, parser.ambient_light.z);
        shadow_ray_epsilon = parser.shadow_ray_epsilon;
        max_recursion_depth = parser.max_recursion_depth;

        Scene::makeTransformations();
    }

    bool Scene::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        HitRecord temp;
        float min_distance = max_distance;
        for (auto& sphere : spheres)
        {
            if (sphere.intersect(ray, temp, min_distance, rotations))
            {
                min_distance = temp.distance;
                hit_record = temp;
            }
        }

        for (auto& mesh : meshes)
        {
            if (mesh.intersect(ray, temp, min_distance))
            {
                min_distance = temp.distance;
                hit_record = temp;
                hit_record.text = mesh.m_texture_id;
                if(mesh.m_texture_id != 0){
                    math::Vector2f a = tex_coord_data[mesh.faceIds[3*hit_record.face_id] -1];
                    math::Vector2f b = tex_coord_data[mesh.faceIds[3*hit_record.face_id + 1] -1];
                    math::Vector2f c = tex_coord_data[mesh.faceIds[3*hit_record.face_id + 2] -1];
                    
                    double u = a.x + hit_record.beta * (b.x-a.x) + hit_record.gamma * (c.x-a.x);
                    double v = a.y + hit_record.beta * (b.y-a.y) + hit_record.gamma * (c.y-a.y);

                    if(textures[mesh.m_texture_id-1].m_appearance == "repeat"){
                        hit_record.beta = u - (int)u;
                        hit_record.gamma = v - (int)v;
                    } else {
                        if(u>1) u = 1;
                        else if(u<0) u = 0;
                        if(v>1) v = 1;
                        else if(v<0) v = 0;                        
                        hit_record.beta = u ;
                        hit_record.gamma = v;
                    }
                }
            }
        }

        return min_distance != max_distance;
    }

    bool Scene::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        for (auto& sphere : spheres)
        {
            if (sphere.intersectShadowRay(ray, max_distance))
            {
                return true;
            }
        }


        for (auto& mesh : meshes)
        {
            if (mesh.intersectShadowRay(ray, max_distance))
            {
		return true;
            }
        }

	return false;
    }

    //Additional Functions
    void Scene::printMat(Matrix &mat){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                std::cout << mat.m_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void Scene::makeTransformations(){
        for(auto& mesh : meshes){
            std::string tf = mesh.getTransformation();
            if(tf.empty())continue;
            std::vector<std::string> tf_splitted;
            size_t pos = 0;

            while((pos = tf.find(" ")) != std::string::npos){
                std::string token = tf.substr(0, pos);
                tf.erase(0, pos+1);
                tf_splitted.push_back(token);
            }
            tf_splitted.push_back(tf);

            Matrix tfMat = Matrix();
            tfMat.makeIdentity();

            for(Triangle& triangle : mesh.m_triangles){
                for(std::string t : tf_splitted){
                    int index = atoi(t.substr(1).c_str());
                    if(t.at(0) == 't')
                    {
                        //make translation
                        Translation tempT = Scene::translations[index-1];
                        triangle.makeTranslation(tempT.x, tempT.y, tempT.z);

                    } else if(t.at(0) == 's')
                    {
                        Scaling tempS = Scene::scalings[index-1];
                        //Matrix sMat = Matrix();
                        triangle.makeScaling(tempS.x, tempS.y, tempS.z);
                    } else if(t.at(0) == 'r')
                    {
                        Rotation tempR = Scene::rotations[index-1];
                        math::Vector3f u(tempR.x, tempR.y, tempR.z);
                        math::Vector3f v;
                        float minAxs = math::min(std::abs(tempR.x), std::abs(tempR.y));
                        minAxs = math::min(std::abs(tempR.z), minAxs);
                        if(std::abs(minAxs-u.x)<0.00001){
                            v.x = 0; v.y = -u.z; v.z=u.y;
                        } else if (std::abs(minAxs-u.y)<0.00001){
                            v.y = 0; v.x = -u.z; v.z = u.x;
                        } else {
                            v.z = 0; v.x = -u.y; v.y = u.x;
                        }

                        math::Vector3f w(math::cross(u, v));
                        v = math::normalize(v);
                        w = math::normalize(w);
                        Matrix mMat = Matrix();
                        Matrix mInvMat = Matrix();
                        Matrix mRot = Matrix();
                        mRot.makeRotation(tempR.angle);
                        
                        mMat.makeIdentity();
                        mInvMat.makeIdentity();
                        for(int clmn=0; clmn<3; clmn++){
                            mMat.m_matrix[0][clmn] = u[clmn];
                            mMat.m_matrix[1][clmn] = v[clmn];
                            mMat.m_matrix[2][clmn] = w[clmn];
                        }

                        for(int row=0; row<3; row++){
                            mInvMat.m_matrix[row][0] = u[row];
                            mInvMat.m_matrix[row][1] = v[row];
                            mInvMat.m_matrix[row][2] = w[row];
                        }
                        tfMat = mInvMat * mRot * mMat;
                        triangle.makeRotation(tfMat);

                    }
                }
            }

            for(Sphere& sphere : spheres){
                std::string tf = sphere.getTransformation();
                if(tf.empty())continue;
                std::vector<std::string> tf_splitted;
                size_t pos = 0;
                
                while((pos = tf.find(" ")) != std::string::npos){
                    std::string token = tf.substr(0, pos);
                    tf.erase(0, pos+1);
                    tf_splitted.push_back(token);
                }
                tf_splitted.push_back(tf);

                Matrix tfMat = Matrix();
                tfMat.makeIdentity();
                for(std::string t : tf_splitted){
                    int index = atoi(t.substr(1).c_str());
                    if(t.at(0) == 't')
                    {
                        //make translation
                        Translation tempT = Scene::translations[index-1];
                        sphere.makeTranslation(tempT.x, tempT.y, tempT.z);

                    } else if(t.at(0) == 's')
                    {
                        Scaling tempS = Scene::scalings[index-1];
                        //Matrix sMat = Matrix();
                        sphere.makeScaling(tempS.x, tempS.y, tempS.z);
                    } else if(t.at(0) == 'r')
                    {
                        Rotation tempR = Scene::rotations[index-1];
                        math::Vector3f u(tempR.x, tempR.y, tempR.z);
                        math::Vector3f v;
                        float minAxs = math::min(std::abs(tempR.x), std::abs(tempR.y));
                        minAxs = math::min(std::abs(tempR.z), minAxs);
                        if(std::abs(minAxs-u.x)<0.00001){
                            v.x = 0; v.y = -u.z; v.z=u.y;
                        } else if (std::abs(minAxs-u.y)<0.00001){
                            v.y = 0; v.x = -u.z; v.z = u.x;
                        } else {
                            v.z = 0; v.x = -u.y; v.y = u.x;
                        }

                        math::Vector3f w(math::cross(u, v));
                        v = math::normalize(v);
                        w = math::normalize(w);
                        Matrix mMat = Matrix();
                        Matrix mInvMat = Matrix();
                        Matrix mRot = Matrix();
                        mRot.makeRotation(tempR.angle);
                        
                        mMat.makeIdentity();
                        mInvMat.makeIdentity();
                        for(int clmn=0; clmn<3; clmn++){
                            mMat.m_matrix[0][clmn] = u[clmn];
                            mMat.m_matrix[1][clmn] = v[clmn];
                            mMat.m_matrix[2][clmn] = w[clmn];
                        }

                        for(int row=0; row<3; row++){
                            mInvMat.m_matrix[row][0] = u[row];
                            mInvMat.m_matrix[row][1] = v[row];
                            mInvMat.m_matrix[row][2] = w[row];
                        }
                        tfMat = mInvMat * mRot * mMat;
                        sphere.makeRotation(tfMat);

                    }
                }
            }
        }
    }
}
