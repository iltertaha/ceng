#include "integrator.h"
#include "image.h"
#include "ctpl_stl.h"
#include <iostream>

namespace fst
{
    Integrator::Integrator(const parser::Scene& parser)
    {
        m_scene.loadFromParser(parser);
    }

    math::Vector3f Integrator::renderPixel(const Ray& ray, int depth) const
    {
        if (depth > m_scene.max_recursion_depth)
        {
            return math::Vector3f(0.0f, 0.0f, 0.0f);
        }

        HitRecord hit_record;
        auto result = m_scene.intersect(ray, hit_record, std::numeric_limits<float>::max());

        if (!result)
        {
            return m_scene.background_color;
        }

        auto& material = m_scene.materials[hit_record.material_id - 1];
        auto color = material.get_ambient() * m_scene.ambient_light;
        auto intersection_point = ray.getPoint(hit_record.distance);

        for (auto& light : m_scene.point_lights)
        {
            auto to_light = light.get_position() - intersection_point;
            auto light_pos_distance = math::length(to_light);
            to_light = to_light / light_pos_distance;

            Ray shadow_ray(intersection_point + m_scene.shadow_ray_epsilon * to_light, to_light);

            if (!m_scene.intersectShadowRay(shadow_ray, light_pos_distance))
            {
                if(hit_record.text != 0){
                    Texture temp = m_scene.textures[hit_record.text -1];
                    double hInd = hit_record.gamma * temp.m_height;
                    double wInd = hit_record.beta * temp.m_width;
                    int imgH, imgW;
                    
                    float r, g, b;
                    
                    if(temp.m_interpolation == "nearest"){
                        imgH = std::round(hInd);
                        imgW = std::round(wInd);

                        int imgInd = 3 * (imgH*temp.m_width + imgW);
                        r = temp.m_image[imgInd++];
                        g = temp.m_image[imgInd++];
                        b = temp.m_image[imgInd++];    
                    
                    } else if(temp.m_interpolation == "bilinear"){
                        int imgInd = 3 * (((int)hInd)*temp.m_width + (int)wInd);
                        float tempR = temp.m_image[imgInd++];
                        float tempG = temp.m_image[imgInd++];
                        float tempB = temp.m_image[imgInd++];
                        math::Vector3f pixel00(tempR, tempG, tempB);
                        tempR = temp.m_image[imgInd++];
                        tempG = temp.m_image[imgInd++];
                        tempB = temp.m_image[imgInd++];
                        math::Vector3f pixel10(tempR, tempG, tempB);
                        imgInd = 3 * (((int)hInd + 1)*temp.m_width + (int)wInd);
                        tempR = temp.m_image[imgInd++];
                        tempG = temp.m_image[imgInd++];
                        tempB = temp.m_image[imgInd++];
                        math::Vector3f pixel01(tempR, tempG, tempB);
                        tempR = temp.m_image[imgInd++];
                        tempG = temp.m_image[imgInd++];
                        tempB = temp.m_image[imgInd++];
                        math::Vector3f pixel11(tempR, tempG, tempB);
                        
                        float dx = wInd - (int)wInd, dy = hInd - (int)hInd;
                        r = pixel00.x*(1-dx)*(1-dy)
                            + pixel10.x*(dx)*(1-dy)
                            + pixel01.x*(1-dx)*(dy)
                            + pixel11.x*(dx)*(dy);

                        g = pixel00.y*(1-dx)*(1-dy)
                            + pixel10.y*(dx)*(1-dy)
                            + pixel01.y*(1-dx)*(dy)
                            + pixel11.y*(dx)*(dy);

                        b = pixel00.z*(1-dx)*(1-dy)
                            + pixel10.z*(dx)*(1-dy)
                            + pixel01.z*(1-dx)*(dy)
                            + pixel11.z*(dx)*(dy);
                        //if(dx > 0.5 || dy > 0.5)std::cout << "error" << std::endl;

                    }
                    
                    math::Vector3f text(r,g,b);
                    
                    if(temp.m_decalMode == "replace_kd")
                        color = color + light.computeRadiance(light_pos_distance) * material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal, 0, text);
                    else if(temp.m_decalMode == "blend_kd")
                        color = color + light.computeRadiance(light_pos_distance) * material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal, 1, text);
                    else if(temp.m_decalMode == "replace_all")
                        color = color + text + light.computeRadiance(light_pos_distance) * material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal, 2, text);
                }
                else color = color + light.computeRadiance(light_pos_distance) * material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal);
            }
        }

        auto& mirror = material.get_mirror();
        if (mirror.x + mirror.y + mirror.z > 0.0f)
        {
            auto new_direction = math::reflect(ray.get_direction(), hit_record.normal);
            Ray secondary_ray(intersection_point + m_scene.shadow_ray_epsilon * new_direction, new_direction);

            return color + mirror * renderPixel(secondary_ray, depth + 1);
        }
        else
        {
            return color;
        }
    }

    void Integrator::integrate() const
    {
        for (auto& camera : m_scene.cameras)
        {
            auto& resolution = camera.get_screen_resolution();
            Image image(resolution.x, resolution.y);

            ctpl::thread_pool pool(128);
            for (int i = 0; i < resolution.x; ++i)
            {
                pool.push([i, &resolution, &camera, &image, this](int id) {
                    for (int j = 0; j < resolution.y; ++j)
                    {
                        auto ray = camera.castPrimayRay(static_cast<float>(i), static_cast<float>(j));
                        auto color = renderPixel(ray, 0);
                        image.setPixel(i, j, math::clamp(color, 0.0f, 255.0f));
                    }
                });
            }
            pool.stop(true);

            image.save(camera.get_image_name());
        }
    }
}
