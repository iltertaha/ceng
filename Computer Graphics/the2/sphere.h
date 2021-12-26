#pragma once

#include "vector3f.h"
#include <string>
#include "matrix.h"
#include "rotation.h"

namespace fst
{
    class Ray;
    struct HitRecord;

    class Sphere
    {
    public:
        Sphere(const math::Vector3f& center, float radius, int material_id, int texture_id, std::string transformations);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance, std::vector<Rotation> rotations) const;
        bool intersectShadowRay(const Ray& ray, float max_distance) const;
        void makeTranslation(double x, double y, double z);
        void makeScaling(double x, double sy, double sz);
        void makeRotation(Matrix &tfMat);
        std::string getTransformation(){return m_transformations;}
        int m_texture_id;

    private:
        math::Vector3f m_center;
        float m_radius;
        int m_material_id;
        std::string m_transformations;
    };
}