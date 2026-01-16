#pragma once

#include <vector>
#include <string>

class Material;
class Texture;

std::string writeCustomShader(Material material, std::vector<Texture> textures, bool hasUVs);
bool compareFiles(const std::string& file1, const std::string& file2);
bool removeShaderFile(const std::string& file);
std::vector<std::string> getFilesInDirectory(const std::string& directory);
