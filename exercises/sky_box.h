#pragma once
#include "../camera.h"
#include "../ppm.h"
#include "../point_light.h"
#include "../sphere.h"
#include "../phong.h"
#include "../cube.h"
#include "../cube_map.h"
#include "../pattern_file.h"

// magick convert .\negx.jpg - compress none negx.ppm
void sky_box_exercise()
{
    // === Camera ===
    Camera camera{ 1600, 800, 1.2 };
    camera.transform = matrix_t::view_transform(
        tuple_t::point(0, 0, 0),
        tuple_t::point(0, 0, 5),
        tuple_t::vector(0, 1, 0)
    );

    // === Lights ===
    auto light1{ std::make_shared<PointLight>(colour_t{ 1, 1, 1 }) };
    light1->transform = matrix_t::translation(0, 100, 0);

    // === Sphere ===
    auto sphere{ Sphere::create() };
    sphere->transform = matrix_t::translation(0, 0, 5) * matrix_t::scaling(0.75, 0.75, 0.75);
    auto phong = std::dynamic_pointer_cast<Phong>(sphere->material);
    if (phong)
    {
        phong->ambient = 0;
        phong->diffuse = 0.4;
        phong->specular = 0.6;
        phong->reflective = 0.6;
        phong->shininess = 20;
    }

    // === Cube ===
    auto cube{ Cube::create() };
    cube->transform = matrix_t::scaling(1000, 1000, 1000);
    auto negx{ std::make_shared<PatternFile>(".\\assets\\negx.ppm") };
    auto negy{ std::make_shared<PatternFile>(".\\assets\\negy.ppm") };
    auto negz{ std::make_shared<PatternFile>(".\\assets\\negz.ppm") };
    auto posx{ std::make_shared<PatternFile>(".\\assets\\posx.ppm") };
    auto posy{ std::make_shared<PatternFile>(".\\assets\\posy.ppm") };
    auto posz{ std::make_shared<PatternFile>(".\\assets\\posz.ppm") };
    auto cube_map{ std::make_shared<CubeMap>(negx, posx, negz, posz, posy, negy) };
    auto phong2 = std::dynamic_pointer_cast<Phong>(cube->material);
    if (phong2)
    {
        phong2->pattern = cube_map;
        phong2->ambient = 1;
        phong2->diffuse = 0;
        phong2->specular = 0;
    }

    // === World Setup ===
    World world{};
    world.add_object(sphere);
    world.add_object(cube);
    world.add_object(light1);

    // === Render ===
    canvas_t image{ camera.render(world) };
    const ppm_t ppm{ image };
    ppm.write_to_file("./x64/Release/sky_box.ppm");
}
