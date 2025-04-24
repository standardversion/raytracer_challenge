#include "area_light.h"
#include "world.h"
#include "sequence.h"

AreaLight::AreaLight(
    const tuple_t& corner,
    const tuple_t& full_uvec,
    const int usteps,
    const tuple_t& full_vvec,
    const int vsteps,
    Sequence& jitter,
    const colour_t& intensity
)
    :
    Light{ intensity, usteps, vsteps },
    corner{ corner },
    uvec{ full_uvec / (double)usteps },
    vvec{ full_vvec / (double)vsteps },
    jitter{ jitter }
{

}

tuple_t AreaLight::position() const
{
    return tuple_t::point(1, 0, 0.5);
}

tuple_t AreaLight::point_on_light(const double u, const double v)
{
    const tuple_t p{ corner + uvec * (u + jitter.next()) + vvec * (v + jitter.next()) };
    return tuple_t::point(p.x, p.y, p.z);
}

double AreaLight::intensity_at(const tuple_t& point, const World& w)
{
    int samples{ usteps * vsteps };
    double total{ 0.0 };
    for (int v{ 0 }; v < vsteps; v++)
    {
        for (int u{ 0 }; u < usteps; u++)
        {
            const tuple_t light_position{ point_on_light(u, v) };
            if (!w.is_shadowed(point, light_position))
            {
                total += 1.0;
            }
        }
    }
    return total / samples;
}

bool AreaLight::operator==(const Light& l) const
{
    const AreaLight* other = dynamic_cast<const AreaLight*>(&l);
    if (!other) return false; // Not the same type

    return *this == *other;
}

bool AreaLight::operator==(const AreaLight& l) const
{
    return
        this->intensity == l.intensity &&
        this->uvec == l.uvec &&
        this->usteps == l.usteps &&
        this->vvec == l.vvec &&
        this->vsteps == l.vsteps &&
        this->position() == l.position();
}