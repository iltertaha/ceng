#include "material.h"
#include "vector3f.h"

namespace fst
{
    Material::Material(const math::Vector3f& ambient, const math::Vector3f& diffuse, const math::Vector3f& specular, const math::Vector3f& mirror, float phong_exponent)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_mirror(mirror)
        , m_phong_exponent(phong_exponent)
    {}

    math::Vector3f Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal) const
    {
        auto diffuse = math::max(math::dot(normal, wi), 0.0f);
        auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

        return m_specular * specular + m_diffuse * diffuse;
    }

    math::Vector3f Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal, int decalMode, const math::Vector3f& C) const
    {
        auto diffuse = math::max(math::dot(normal, wi), 0.0f);
        auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

        if(decalMode == 0){
            return m_specular * specular + C/255 * diffuse;
        } else if (decalMode == 1){
            return m_specular * specular + ((C/255) + m_diffuse)/2 * diffuse;
        }else
            return m_specular * specular;
    }
}