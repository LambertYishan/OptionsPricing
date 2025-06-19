#pragma once
#include <vector>

namespace MeshGenerator {

    // Generate a linearly spaced mesh
    inline std::vector<double> linear_mesh(double start, double end, double h) {
        std::vector<double> mesh;
        for (double x = start; x <= end; x += h) {
            mesh.push_back(x);
        }
        return mesh;
    }
}