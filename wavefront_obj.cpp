#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <exception>
#include "utils.h"
#include "tuple.h"
#include "wavefront_obj.h"

wavefront_t::wavefront_t(const char* obj_filename)
{
	const std::filesystem::path obj_filepath{ std::filesystem::absolute(obj_filename) };
	if (!std::filesystem::exists(obj_filepath))
	{
		throw std::invalid_argument("Obj file does not exist");
	}
	else
	{
		std::ifstream file(obj_filepath);
		std::vector<std::vector<double>> added_normal_indices;
		std::vector<tuple_t> obj_normals{};
		std::string line{};
		while (std::getline(file, line))
		{
			std::vector<std::string> parts{ split(line, " ") };
			if (parts.size()) {
				if (parts[0] == "v")
				{
					// vertex line in obj: v 1.000000 1.000000 -1.000000
					//vertices.emplace_back(tuple_t::vector(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3])));
					vertices.emplace_back(tuple_t::point(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3])));
					// create an empty array for each vertex to store it's normals so we can index it when we add the vtx normals
					vertex_normals.emplace_back(std::vector<tuple_t>{});
					added_normal_indices.push_back(std::vector<double>{});
				}
				if (parts[0] == "vn")
				{
					// vertex normal line in obj vn -0.0000 1.0000 - 0.0000
					obj_normals.emplace_back(tuple_t::vector(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3])));
				}
				if (parts[0] == "vt")
				{
					// vertex uv line in obj vt 0.875000 0.500000
					uvs.emplace_back(std::stod(parts[1]), std::stod(parts[2]));
				}
				else if (parts[0] == "f")
				{
					// face line in obj: f 5/1/1 3/2/1 1/3/1, 5, 3, 1 are the vertex indices
					std::vector<std::string> face_parts1{ split(parts[1], "/") };
					std::vector<std::string> face_parts2{ split(parts[2], "/") };
					std::vector<std::string> face_parts3{ split(parts[3], "/") };
					faces.emplace_back(
						std::stoi(face_parts1[0]),
						std::stoi(face_parts2[0]),
						std::stoi(face_parts3[0]),
						std::stoi(face_parts1[1]),
						std::stoi(face_parts2[1]),
						std::stoi(face_parts3[1]),
						std::stoi(face_parts1[2]),
						std::stoi(face_parts2[2]),
						std::stoi(face_parts3[2])
					);
				}
			}
		}
		std::size_t counter{ 0 };
		for (const auto& face : faces)
		{
			int vtx_a_index{ face.a - 1 };
			int vtx_a_normal_index{ face.a_normal - 1 };
			if (std::find(added_normal_indices[vtx_a_index].begin(), added_normal_indices[vtx_a_index].end(), vtx_a_normal_index) == added_normal_indices[vtx_a_index].end())
			{
				vertex_normals[vtx_a_index].push_back(obj_normals[vtx_a_normal_index]);
				added_normal_indices[vtx_a_index].push_back(vtx_a_normal_index);
			}
			int vtx_b_index{ face.b - 1 };
			int vtx_b_normal_index{ face.b_normal - 1 };
			if (std::find(added_normal_indices[vtx_b_index].begin(), added_normal_indices[vtx_b_index].end(), vtx_b_normal_index) == added_normal_indices[vtx_b_index].end())
			{
				vertex_normals[vtx_b_index].push_back(obj_normals[vtx_b_normal_index]);
				added_normal_indices[vtx_b_index].push_back(vtx_b_normal_index);
			}
			int vtx_c_index{ face.c - 1 };
			int vtx_c_normal_index{ face.c_normal - 1 };
			if (std::find(added_normal_indices[vtx_c_index].begin(), added_normal_indices[vtx_c_index].end(), vtx_c_normal_index) == added_normal_indices[vtx_c_index].end())
			{
				vertex_normals[vtx_c_index].push_back(obj_normals[vtx_c_normal_index]);
				added_normal_indices[vtx_c_index].push_back(vtx_c_normal_index);
			}
			counter++;
		}
		for (const std::vector<tuple_t>& vtx_normals : vertex_normals)
		{
			tuple_t normal_avg{ tuple_t::vector(0.0, 0.0, 0.0) };
			for (const auto& normal : vtx_normals)
			{
				normal_avg += normal;
			}
			normal_avg /= vtx_normals.size();
			normal_avg.normalize();
			vertex_normals_avg.push_back(normal_avg);
		}
	}

}