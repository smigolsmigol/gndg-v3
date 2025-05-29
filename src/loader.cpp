#include "tiny_gltf.h"
#include <iostream>

void loadGLTFModel(const std::string& filename) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    
    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
    if (!err.empty()) std::cerr << "Err: " << err << std::endl;

    if (!ret) {
        std::cerr << "Failed to load .glb: " << filename << std::endl;
        return;
    }

    std::cout << "Model loaded: " << filename << "\n";
    std::cout << "Meshes found: " << model.meshes.size() << std::endl;
}
