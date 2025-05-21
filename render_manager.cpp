#include <chrono>
#include <iostream>
#include <future>
#include "render_manager.h"
#include "settings.h"

RenderManager::RenderManager(const Camera& camera, const int tile_size)
	: render_camera{ camera }, tile_size{ tile_size }
{
	int full_tiles_x{ render_camera.hsize / tile_size };
	int left_over_x{ render_camera.hsize % tile_size };
	int full_tiles_y{ render_camera.vsize / tile_size };
	int left_over_y{ render_camera.vsize % tile_size };
	int x_max{ left_over_x ? full_tiles_x + 1 : full_tiles_x };
	int y_max{ left_over_y ? full_tiles_y + 1 : full_tiles_y };
	render_tiles.reserve(x_max * y_max);
	for (int y{ 0 }; y < y_max; y++)
	{
		int y_start{ y * tile_size };
		int y_end{ y_start + tile_size };
		if (left_over_y && y == full_tiles_y) y_end = y_start + left_over_y;
		y_end = y_end <= render_camera.vsize ? y_end : render_camera.vsize;
		for (int x{ 0 }; x < x_max; x++)
		{
			int x_start{ x * tile_size };
			int x_end{ x_start + tile_size };
			if (left_over_x && x == full_tiles_x) x_end = x_start + left_over_x;
			x_end = x_end <= render_camera.hsize ? x_end : render_camera.hsize;
			render_tiles.emplace_back(x_start, x_end, y_start, y_end);
		}
	}
}

canvas_t RenderManager::render(const World& world)
{
	canvas_t image{ render_camera.hsize, render_camera.vsize };
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::future<void>> futures;

	for (auto& tile : render_tiles)
	{
		std::packaged_task<void()> task{ [this, &tile, &world, &image]() {
			for (int y{ tile.y_start }; y < tile.y_end; y++)
			{
				for (int x{ tile.x_start }; x < tile.x_end; x++)
				{
						const ray_t ray{ render_camera.ray_for_pixel(x, y) };
						const colour_t colour{ world.colour_at(ray, MAX_REFLECTION_DEPTH) };
						image.write_pixel(x, y, colour);
				}
			}
		} };
		futures.push_back(task.get_future());
		thread_pool.submit(std::move(task));
	}
	for (auto& future : futures)
	{
		future.get();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;

	std::cout << "Render time: " << duration.count() << " seconds\n";
	return image;
}