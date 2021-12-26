#include "sphere.h"
#include "hit_record.h"
#include "ray.h"
#include <algorithm>

namespace fst
{
    Sphere::Sphere(const math::Vector3f& center, float radius, int material_id, int texture_id, std::string transformations)
        : m_center(center)
        , m_radius(radius)
        , m_material_id(material_id)
        , m_texture_id(texture_id)
        , m_transformations(transformations)
    {}

    bool Sphere::intersect(const Ray& ray, HitRecord& hit_record, float max_distance, std::vector<Rotation> rotations) const
    {
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        if(distance < 0.0f){
            distance = a + sqrtf(x);
        }
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.distance = distance;
            hit_record.normal = math::normalize(ray.getPoint(hit_record.distance) - m_center);
            hit_record.material_id = m_material_id;
            hit_record.text = m_texture_id;


            math::Vector3f hitPoint = ray.getPoint(hit_record.distance) - m_center;
            Matrix tfMat = Matrix();
            Matrix hitMatrix = Matrix();
            hitMatrix.makeTranslation(hitPoint.x, hitPoint.y, hitPoint.z);
    
            //make rotations
            std::string tf = m_transformations;
            if(!tf.empty()){
                std::vector<std::string> tf_splitted;
                size_t pos = 0;

                while((pos = tf.find(" ")) != std::string::npos){
                    std::string token = tf.substr(0, pos);
                    tf.erase(0, pos+1);
                    tf_splitted.push_back(token);
                }
                tf_splitted.push_back(tf);
                std::reverse(tf_splitted.begin(), tf_splitted.end());
                
                for(std::string t : tf_splitted){
                    int index = atoi(t.substr(1).c_str());
                    if(t.at(0) == 'r'){
                        Rotation tempR = rotations[index-1];
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
                        mRot.makeRotation(-tempR.angle);
                        
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
                        if(tempR.angle != 0)
                            hitMatrix = tfMat * hitMatrix;
                    }
                }
            }
            
            hitPoint.x = hitMatrix.m_matrix[0][3];
            hitPoint.y = hitMatrix.m_matrix[1][3];
            hitPoint.z = hitMatrix.m_matrix[2][3];

            double theta = acos(hitPoint.y/m_radius);
            double phi = atan2(hitPoint.z, hitPoint.x);

            hit_record.beta = (-phi + M_PI) / (2*M_PI);
            hit_record.gamma = theta / M_PI;

            return true;
        }
        return false;
    }

    bool Sphere::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        return distance > 0.0f && distance < max_distance;
    }

    void Sphere::makeTranslation(double x, double y, double z){
        m_center.x += x;
        m_center.y += y;
        m_center.z += z;
    }

    void Sphere::makeScaling(double sx, double sy, double sz){
        m_radius *= sx;
        m_center.x *= sx;
        m_center.y *= sx;
        m_center.z *= sx;
    }

    void Sphere::makeRotation(Matrix &tfMat){

    }
}