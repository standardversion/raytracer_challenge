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

void bvh_exercise()
{
    const int dragon_count = 6;
    const double dragon_scale = 0.3;
    const double placement_radius = 6.0;

    // === Load and Place Dragon Meshes ===
    std::vector<std::shared_ptr<Mesh>> dragons;
    for (int i = 0; i < dragon_count; ++i) {
        auto mesh = Mesh::create(".\\assets\\dragon.obj", true);

        // Compute placement in circle
        double angle = i * (2 * PI / dragon_count);
        double x = placement_radius * std::cos(angle);
        double z = placement_radius * std::sin(angle);

        mesh->transform =
            matrix_t::translation(x, 0, z) *
            matrix_t::rotation_y(-angle + PI) *
            matrix_t::scaling(dragon_scale, dragon_scale, dragon_scale);

        // Modify the mesh's existing Phong material
        auto phong = std::dynamic_pointer_cast<Phong>(mesh->material);
        if (phong) {
            switch (i) {
            case 0:
                phong->colour = { 1, 0.2, 0.2 }; // red
                phong->diffuse = 0.7;
                phong->specular = 0.3;
                break;
            case 1:
                phong->colour = { 0.2, 1, 0.2 }; // green
                phong->diffuse = 0.6;
                phong->specular = 0.4;
                break;
            case 2:
                // Softer blue, less reflectivity
                phong->colour = { 0.1, 0.2, 0.6 };
                phong->reflective = 0.5;
                phong->diffuse = 0.4;
                break;
            case 3:
                phong->colour = { 1, 1, 0.2 }; // yellow
                phong->transparency = 0.5;
                phong->refractive_index = 1.3;
                phong->diffuse = 0.2;
                break;
            case 4:
                phong->colour = { 0.7, 0.2, 1 }; // purple
                phong->reflective = 0.5;
                phong->specular = 0.9;
                break;
            case 5:
                // Dimmer cyan, more diffuse to reduce glow
                phong->colour = { 0.1, 0.6, 0.6 };
                phong->transparency = 0.5;
                phong->refractive_index = 1.1;
                phong->diffuse = 0.3;
                break;
            }
        }

        dragons.push_back(mesh);
    }


    // === Ground Plane ===
    auto ground = std::make_shared<Plane>();
    auto ground_mat = std::dynamic_pointer_cast<Phong>(ground->material);
    ground_mat->colour = { 0.1, 0.1, 0.1 };
    ground_mat->specular = 0;

    // === Lights ===
    auto key = std::make_shared<PointLight>(colour_t{ 1.0, 0.95, 0.9 });
    key->transform = matrix_t::translation(-6, 6, -6);

    auto fill = std::make_shared<PointLight>(colour_t{ 0.4, 0.4, 0.5 });
    fill->transform = matrix_t::translation(5, 2, -2);

    auto rim = std::make_shared<PointLight>(colour_t{ 0.6, 0.6, 1.0 });
    rim->transform = matrix_t::translation(0, 4, 6);

    // === World Setup ===
    World world{};
    world.add_object(key);
    world.add_object(fill);
    world.add_object(rim);
    world.add_object(ground);

    for (const auto& dragon : dragons) {
        world.add_object(dragon);
    }

    // === Camera ===
    Camera camera{ 960, 540, PI / 3 };
    camera.transform = matrix_t::view_transform(
        tuple_t::point(0, 5, -14),     // pulled back and up for full view
        tuple_t::point(0, 1, 0),
        tuple_t::vector(0, 1, 0)
    );

    // === Render ===
    canvas_t image{ camera.render(world) };
    const ppm_t ppm{ image };
    ppm.write_to_file("./x64/Release/bvh.ppm");
}
