#pragma once
#include "../tuple.h"
#include "../canvas.h"
#include "../colour.h";
#include "../ppm.h"
#include "../intersection.h"
#include "../phong.h"
#include "../world.h"
#include "../camera.h"
#include "../settings.h"
#include "../mesh.h"
#include "../plane.h"
#include "../point_light.h"
#include "../cylinder.h"
#include "../cube.h"
#include "../group.h"

std::shared_ptr<Mesh> make_dragon(colour_t& colour, double ambient, double diffuse, double specular, double shininess)
{
    auto mesh{ Mesh::create(".\\assets\\dragon.obj", false) };
    mesh->transform = matrix_t::translation(0, 0.1217, 0) * matrix_t::scaling(0.268, 0.268, 0.268);
    auto phong = std::dynamic_pointer_cast<Phong>(mesh->material);
    if (phong)
    {
        phong->colour = colour;
        phong->ambient = ambient;
        phong->diffuse = diffuse;
        phong->specular = specular;
        phong->shininess = shininess;
    }

    return mesh;
}

std::shared_ptr<Cylinder> make_pedestal()
{
    auto cyl{ Cylinder::create() };
    cyl->minimum = -0.15;
    cyl->maximum = 0;
    cyl->closed = true;
    auto phong = std::dynamic_pointer_cast<Phong>(cyl->material);
    if (phong)
    {
        phong->colour = { 0.2, 0.2, 0.2 };
        phong->ambient = 0;
        phong->diffuse = 0.8;
        phong->specular = 0;
        phong->reflective = 0.2;
    }
    return cyl;
}

std::shared_ptr<Cube> make_raw_bbox()
{
    auto cube{ Cube::create() };
    cube->cast_shadows = false;
    cube->transform = matrix_t::translation(-3.9863, -0.1217, -1.1820) *
        matrix_t::scaling(3.73335, 2.5845, 1.6283) *
        matrix_t::translation(1, 1, 1);
    return cube;
}

std::shared_ptr<Cube> make_bbox(double ambient, double diffuse, double specular, double transparency, double refractive_index)
{
    auto cube{ make_raw_bbox() };
    auto phong = std::dynamic_pointer_cast<Phong>(cube->material);
    if (phong)
    {
        phong->ambient = ambient;
        phong->diffuse = diffuse;
        phong->specular = specular;
        phong->transparency = transparency;
        phong->refractive_index = refractive_index;
    }
    cube->transform = matrix_t::translation(0, 0.1217, 0) * matrix_t::scaling(0.268, 0.268, 0.268) * cube->transform;
    return cube;
}

void bvh_exercise()
{

    // group1
    auto g1{ std::make_shared<Group>() };
    g1->transform = matrix_t::translation(0, 2, 0);
    std::shared_ptr<Cylinder> p1{ make_pedestal() };
    g1->add(p1);
    auto g1a{ std::make_shared<Group>() };
    colour_t c{ 1, 0, 0.1 };
    std::shared_ptr<Mesh> d1{ make_dragon(c, 0.1, 0.6, 0.3, 15) };
    std::shared_ptr<Cube> bbox1{ make_bbox(0, 0.4, 0, 0.6, 1) };
    g1a->add(d1, bbox1);
    g1->add(g1a);

    // group2
    auto g2{ std::make_shared<Group>() };
    g2->transform = matrix_t::translation(2, 1, -1);
    std::shared_ptr<Cylinder> p2{ make_pedestal() };
    g2->add(p2);
    auto g2a{ std::make_shared<Group>() };
    g2a->transform = matrix_t::scaling(0.75, 0.75, 0.75) * matrix_t::rotation_y(4);
    colour_t c2{ 1, 0.5, 0.1 };
    std::shared_ptr<Mesh> d2{ make_dragon(c2, 0.1, 0.6, 0.3, 15) };
    std::shared_ptr<Cube> bbox2{ make_bbox(0, 0.2, 0, 0.8, 1) };
    g2a->add(d2, bbox2);
    g2->add(g2a);

    // group3
    auto g3{ std::make_shared<Group>() };
    g3->transform = matrix_t::translation(-2, 0.75, -1);
    std::shared_ptr<Cylinder> p3{ make_pedestal() };
    g3->add(p3);
    auto g3a{ std::make_shared<Group>() };
    g3a->transform = matrix_t::scaling(0.75, 0.75, 0.75) * matrix_t::rotation_y(-0.4);
    colour_t c3{ 0.9, 0.5, 0.1 };
    std::shared_ptr<Mesh> d3{ make_dragon(c3, 0.1, 0.6, 0.3, 15) };
    std::shared_ptr<Cube> bbox3{ make_bbox(0, 0.2, 0, 0.8, 1) };
    g3a->add(d3, bbox3);
    g3->add(g3a);

    // group4
    auto g4{ std::make_shared<Group>() };
    g4->transform = matrix_t::translation(-4, 0, -2);
    std::shared_ptr<Cylinder> p4{ make_pedestal() };
    g4->add(p4);
    auto g4a{ std::make_shared<Group>() };
    g4a->transform = matrix_t::scaling(0.5, 0.5, 0.5) * matrix_t::rotation_y(-0.2);
    colour_t c4{ 1, 0.9, 0.1 };
    std::shared_ptr<Mesh> d4{ make_dragon(c4, 0.1, 0.6, 0.3, 15) };
    std::shared_ptr<Cube> bbox4{ make_bbox(0, 0.1, 0, 0.9, 1) };
    g4a->add(d4, bbox4);
    g4->add(g4a);

    // group5
    auto g5{ std::make_shared<Group>() };
    g5->transform = matrix_t::translation(4, 0, -2);
    std::shared_ptr<Cylinder> p5{ make_pedestal() };
    g5->add(p5);
    auto g5a{ std::make_shared<Group>() };
    g5a->transform = matrix_t::scaling(0.5, 0.5, 0.5) * matrix_t::rotation_y(3.3);
    colour_t c5{ 0.9, 1, 0.1 };
    std::shared_ptr<Mesh> d5{ make_dragon(c5, 0.1, 0.6, 0.3, 15) };
    std::shared_ptr<Cube> bbox5{ make_bbox(0, 0.1, 0, 0.9, 1) };
    g5a->add(d5, bbox5);
    g5->add(g5a);

    // group6
    auto g6{ std::make_shared<Group>() };
    g6->transform = matrix_t::translation(0, 0.5, -4);
    std::shared_ptr<Cylinder> p6{ make_pedestal() };
    g6->add(p6);
    colour_t c6{ 1, 1, 1 };
    std::shared_ptr<Mesh> d6{ make_dragon(c6, 0.1, 0.6, 0.3, 15) };
    d6->transform = matrix_t::rotation_y(3.1415) * d6->transform;
    g6->add(d6);


    // === Lights ===
    auto light1 = std::make_shared<PointLight>(colour_t{ 0.8, 0.8, 0.8 });
    light1->transform = matrix_t::translation(-10, 100, -100);


    auto light2 = std::make_shared<PointLight>(colour_t{ 0.2, 0.2, 0.2 });
    light2->transform = matrix_t::translation(100, 10, -25);

    // === World Setup ===
    World world{};
    world.add_object(light1);
    world.add_object(light2);
    world.add_object(g1);
    world.add_object(g2);
    world.add_object(g3);
    world.add_object(g4);
    world.add_object(g5);
    world.add_object(g6);


    // === Camera ===
    Camera camera{ 2000, 800, 1.2 };
    camera.transform = matrix_t::view_transform(
        tuple_t::point(0, 2.5, -10),     // pulled back and up for full view
        tuple_t::point(0, 1, 0),
        tuple_t::vector(0, 1, 0)
    );

    // === Render ===
    canvas_t image{ camera.render(world) };
    const ppm_t ppm{ image };
    ppm.write_to_file("./x64/Release/bvh.ppm");
}
