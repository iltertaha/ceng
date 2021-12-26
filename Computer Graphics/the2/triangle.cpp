#include "hit_record.h"
#include "triangle.h"
#include "matrixInverse.h"
#include "ray.h"

namespace fst
{
    Triangle::Triangle(const math::Vector3f& v0, const math::Vector3f& edge1, const math::Vector3f& edge2)
        : m_v0(v0)
        , m_edge1(edge1)
        , m_edge2(edge2)
        , m_normal(math::normalize(math::cross(edge1, edge2)))
    {}

    bool Triangle::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        //M�ller-Trumbore algorithm
        auto pvec = math::cross(ray.get_direction(), m_edge2);
        auto inv_det = 1.0f / math::dot(m_edge1, pvec);

        auto tvec = ray.get_origin() - m_v0;
        auto w1 = math::dot(tvec, pvec) * inv_det;

        if (w1 < 0.0f || w1 > 1.0f)
        {
            return false;
        }

        auto qvec = math::cross(tvec, m_edge1);
        auto w2 = math::dot(ray.get_direction(), qvec) * inv_det;

        if (w2 < 0.0f || (w1 + w2) > 1.0f)
        {
            return false;
        }

        auto distance = math::dot(m_edge2, qvec) * inv_det;
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.normal = m_normal;
            hit_record.distance = distance;
            hit_record.beta = w1;
            hit_record.gamma = w2;
            return true;
        }
        return false;
    }

    bool Triangle::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        //M�ller-Trumbore algorithm
        auto pvec = math::cross(ray.get_direction(), m_edge2);
        auto inv_det = 1.0f / math::dot(m_edge1, pvec);

        auto tvec = ray.get_origin() - m_v0;
        auto w1 = math::dot(tvec, pvec) * inv_det;

        if (w1 < 0.0f || w1 > 1.0f)
        {
            return false;
        }

        auto qvec = math::cross(tvec, m_edge1);
        auto w2 = math::dot(ray.get_direction(), qvec) * inv_det;

        if (w2 < 0.0f || (w1 + w2) > 1.0f)
        {
            return false;
        }

        auto distance = math::dot(m_edge2, qvec) * inv_det;
        return distance > 0.0f && distance < max_distance;
    }

    void Triangle::makeTranslation(double x, double y, double z){
        m_v0.x += x;
        m_v0.y += y;
        m_v0.z += z;
    }

    void Triangle::makeScaling(double sx, double sy, double sz){
        double ix, iy, iz;
        math::Vector3f v1, v2;

        v1 = m_v0 + m_edge1;
        v2 = m_v0 + m_edge2;

        // ix = (3*m_v0.x + m_edge1.x + m_edge2.x)/3;
        // iy = (3*m_v0.y + m_edge1.y + m_edge2.y)/3;
        // iz = (3*m_v0.z + m_edge1.z + m_edge2.z)/3;
        
        v1.y *= sy;
        v1.z *= sz;
        v1.x *= sx;
        v2.x *= sx;
        v2.y *= sy;
        v2.z *= sz;

        m_v0.x *= sx;
        m_v0.y *= sy;
        m_v0.z *= sz;

        m_edge1 = v1 - m_v0;
        m_edge2 = v2 - m_v0;
        //makeTranslation(-m_edge1.x/(2*sx), -m_edge1.y/(2*sy), -m_edge1.z/(2*sz));
    }

    void Triangle::makeRotation(Matrix &tfMat){
        Matrix e1, e2, v0, normal;
        Matrix inv;
        e1.makeTranslation(m_edge1.x, m_edge1.y, m_edge1.z);
        e2.makeTranslation(m_edge2.x, m_edge2.y, m_edge2.z);
        v0.makeTranslation(m_v0.x, m_v0.y, m_v0.z);
        normal.makeTranslation(m_normal.x, m_normal.y, m_normal.z);

        e1 = tfMat * e1;
        e2 = tfMat * e2;
        v0 = tfMat * v0;
        m_edge1.x = e1.m_matrix[0][3];
        m_edge1.y = e1.m_matrix[1][3];
        m_edge1.z = e1.m_matrix[2][3];
        m_edge2.x = e2.m_matrix[0][3];
        m_edge2.y = e2.m_matrix[1][3];
        m_edge2.z = e2.m_matrix[2][3];
        m_v0.x = v0.m_matrix[0][3];
        m_v0.y = v0.m_matrix[1][3];
        m_v0.z = v0.m_matrix[2][3];


        math::Vector3f unit1(math::normalize(m_edge1)), unit2(math::normalize(m_edge2));
        m_normal = math::normalize(math::cross(unit1, unit2));
    }
}
