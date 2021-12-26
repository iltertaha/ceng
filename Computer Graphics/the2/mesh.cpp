#include "mesh.h"
#include "hit_record.h"
#include "ray.h"
#include "vector3f.h"

namespace fst
{
    Mesh::Mesh(const std::vector<Triangle>& triangles, const std::vector<int>& faceIds, int material_id, int texture_id, std::string transformations)
	: m_triangles(triangles)
        , faceIds(faceIds)
        , m_material_id(material_id)
        , m_texture_id(texture_id)
        , m_transformations(transformations)
    {}

    Mesh::Mesh(std::vector<Triangle>&& triangles, int material_id, int texture_id, std::string transformations)
	: m_triangles(std::move(triangles))
        , m_material_id(material_id)
        , m_texture_id(texture_id)
        , m_transformations(transformations)
    {}

    bool Mesh::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        HitRecord temp;
        float min_distance = max_distance;
        int triId = 0;
        for (auto& triangle : m_triangles)
        {
            if (triangle.intersect(ray, temp, min_distance) && math::dot(temp.normal, ray.get_direction()) < 0.0f)
            {
                min_distance = temp.distance;
                hit_record = temp;
		        hit_record.material_id = m_material_id;
                hit_record.face_id = triId;
            }
            triId++;
        }

        return min_distance != max_distance;
    }

    bool Mesh::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        for (auto& triangle : m_triangles)
        {
            if (triangle.intersectShadowRay(ray, max_distance))
            {
		return true;
            }
        }

	return false;
    }

    void Mesh::makeTransformations(){
        return;
    }
}
