#pragma once

#include "triangle.h"

#include <vector>
#include <string>
#include "matrix.h"

namespace fst
{
    class Mesh : public Triangular
    {
    public:
        Mesh(const std::vector<Triangle>& triangles, const std::vector<int>& faceIds, int material_id, int texture_id, std::string transformations);
        Mesh(std::vector<Triangle>&& triangles, int material_id, int texture_id, std::string transformations);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const override;
        bool intersectShadowRay(const Ray& ray, float max_distance) const override;
        void makeTransformations();
        std::string getTransformation(){return m_transformations;}
        std::vector<Triangle>& getTriangles(){return m_triangles;}
        std::vector<Triangle> m_triangles;
        int m_texture_id;
        std::vector<int> faceIds;

    private:
        int m_material_id;
        std::string m_transformations;
    };
}
