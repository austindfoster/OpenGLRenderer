#include <filesystem>
#include <fstream>
#include <iostream>
#include <material.h>
#include <string>
#include <texture.h>

constexpr std::string VERSION = "#version 330 core";
constexpr std::string IN = "in ";
constexpr std::string OUT = "out ";
constexpr std::string FRAG_COLOR = "fragColor";
constexpr std::string FRAG_POS = "fragPos";
constexpr std::string NORMAL = "normal";
constexpr std::string HEIGHT = "height";
constexpr std::string TEX_COORDS = "texCoords";
constexpr std::string STRUCT = "struct ";
constexpr std::string MATERIAL_TYPE = "Material ";
constexpr std::string LIGHT_TYPE = "Light ";
constexpr std::string STRUCT_BEGIN = "{";
constexpr std::string STRUCT_END = "};";
constexpr std::string VEC2 = "vec2 ";
constexpr std::string VEC3 = "vec3 ";
constexpr std::string VEC4 = "vec4 ";
constexpr std::string FLOAT = "float ";
constexpr std::string SAMPLER2D = "sampler2D ";
constexpr std::string MAP = "Map";
constexpr std::string UNIFORM = "uniform ";
constexpr std::string POSITION = "position";
constexpr std::string AMBIENT = "ambient";
constexpr std::string DIFFUSE = "diffuse";
constexpr std::string SPECULAR = "specular";
constexpr std::string SHININESS = "shininess";
constexpr std::string VIEW_POS = "viewPos";
constexpr std::string MATERIAL_INST = "material";
constexpr std::string LIGHT_INST = "light";
constexpr std::string MAIN_FUNC = "void main() {";

static unsigned int n = 0;

std::string writeCustomShader(Material material, std::vector<Texture> textures, bool hasUVs) {
    std::string fileName = "src/shaders/custom" + std::to_string(++n) + ".fs";
    std::ofstream outputFile(fileName);
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr = 0;
    unsigned int heightNr = 0;
    if (outputFile.is_open()) {
        outputFile << VERSION << "\n";
        outputFile << OUT << VEC4 << FRAG_COLOR << ";\n\n";
        outputFile << STRUCT << MATERIAL_TYPE << "{\n";
        for (unsigned int i = 0; i < textures.size(); i++) {
            std::string number;
            std::string name = textures[i].type.substr(MATERIAL_INST.length());
            if (name == DIFFUSE)
                number = std::to_string(++diffuseNr);
            else if (name == SPECULAR)
                number = std::to_string(++specularNr); // transfer unsigned int to string
            else if (name == NORMAL)
                number = std::to_string(++normalNr); // transfer unsigned int to string
            else if (name == HEIGHT)
                number = std::to_string(++heightNr);
            outputFile << "\t" << SAMPLER2D << name << MAP << number << ";\n";
        }
        if (diffuseNr == 0) {
            outputFile << "\t" << VEC3 << DIFFUSE << ";\n";
        }
        if (specularNr == 0) {
            outputFile << "\t" << VEC3 << SPECULAR << ";\n";
        }

        outputFile << "\t" << VEC3 << AMBIENT << ";\n";
        outputFile << "\t" << FLOAT << SHININESS << ";\n";
        outputFile << STRUCT_END << "\n\n";

        outputFile << STRUCT << LIGHT_TYPE << "{\n"
            << "\t" << VEC3 << POSITION << ";\n"
            << "\t" << VEC3 << AMBIENT << ";\n"
            << "\t" << VEC3 << DIFFUSE << ";\n"
            << "\t" << VEC3 << SPECULAR << ";\n"
            << STRUCT_END << "\n\n";

        outputFile << IN << VEC3 << FRAG_POS << ";\n"
            << IN << VEC3 << NORMAL << ";\n";
        if (hasUVs) {
            outputFile << IN << VEC2 << TEX_COORDS << ";\n";
        }

        outputFile << UNIFORM << VEC3 << VIEW_POS << ";\n"
            << UNIFORM << MATERIAL_TYPE << MATERIAL_INST << ";\n"
            << UNIFORM << LIGHT_TYPE << LIGHT_INST << ";\n\n";

        outputFile << MAIN_FUNC << "\n";
        if (normalNr == 0) {

        }
        if (heightNr == 0) {

        }
        outputFile << "\t" << VEC3 << AMBIENT << " = " << LIGHT_INST << "." << AMBIENT << " * ";
        if (hasUVs && diffuseNr > 0) {
            outputFile << "texture(" << MATERIAL_INST << "." << DIFFUSE << MAP << "1, " << TEX_COORDS << ").rgb;\n";
        } else {
            outputFile << MATERIAL_INST << "." << AMBIENT << ";\n";
        }
        outputFile << "\t" << VEC3 << "norm = " << "normalize(" << NORMAL << ");\n"
            << "\t" << VEC3 << "lightDir = normalize(" << LIGHT_INST << "." << POSITION << " - " << FRAG_POS << ");\n"
            << "\t" << FLOAT << "diff = max(dot(norm, lightDir), 0.0);\n"
            << "\t" << VEC3 << DIFFUSE << " = " << LIGHT_INST << "." << DIFFUSE << " * diff * ";
        if (hasUVs && diffuseNr > 0) {
            outputFile << "texture(" << MATERIAL_INST << "." << DIFFUSE << MAP << "1, " << TEX_COORDS << ").rgb;\n";
        } else {
            outputFile << MATERIAL_INST << "." << DIFFUSE << ";\n";
        }

        outputFile << "\t" << VEC3 << "viewDir = normalize(" << VIEW_POS << " - " << FRAG_POS << ");\n"
            << "\t" << VEC3 << "reflectDir = reflect(-lightDir, norm);\n"
            << "\t" << FLOAT << "spec = pow(max(dot(viewDir, reflectDir), 0.0), " << MATERIAL_INST << "." << SHININESS << ");\n"
            << "\t" << VEC3 << SPECULAR << " = " << LIGHT_INST << "." << SPECULAR << " * spec * ";
        if (hasUVs && specularNr > 0) {
            outputFile << "texture(" << MATERIAL_INST << "." << SPECULAR << MAP << "1, " << TEX_COORDS << ").rgb;\n";
        } else {
            outputFile << MATERIAL_INST << "." << SPECULAR << ";\n";
        }
        outputFile << "\t" << VEC3 << "result = " << AMBIENT << " + " << DIFFUSE << " + " << SPECULAR << ";\n"
            << "\t" << FRAG_COLOR << " = vec4(result, 1.0);\n}";
        outputFile.close();
    }
    return fileName;
}

bool compareFiles(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return false; // Indicate failure to open files
    }

    std::string line1;
    std::string line2;
    int lineNumber = 1;
    bool areEqual = true;

    while (std::getline(f1, line1) && std::getline(f2, line2)) {
        if (line1 != line2) {
            std::cout << "Files differ at line " << lineNumber << std::endl;
            std::cout << "  File 1: " << line1 << std::endl;
            std::cout << "  File 2: " << line2 << std::endl;
            areEqual = false;
            break; // Stop immediately on first difference
        }
        lineNumber++;
    }

    // Check if one file is longer than the other
    if (std::getline(f1, line1) || std::getline(f2, line2)) {
        std::cout << "Files have different lengths (number of lines)." << std::endl;
        areEqual = false;
    }

    f1.close();
    f2.close();

    return areEqual;
}

bool removeShaderFile(const std::string& file) {
    if (!std::filesystem::exists(file)) {
        std::cerr << "File does not exist: " << file<< std::endl;
        return false;
    }
    bool success = std::filesystem::remove(file);
    if (success) {
        std::cout << "File successfully deleted: " << file << std::endl;
    } else {
        std::cerr << "Error deleting file: " << file << std::endl;
    }
    return success;
}

std::vector<std::string> getFilesInDirectory(const std::string& directory) {
    std::vector<std::string> fileNames;
    try {
        // Use directory_iterator to iterate over entries in the directory
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            // Check if the current entry is a regular file
            if (std::filesystem::is_regular_file(entry.status())) {
                // Get the file name from the path and convert it to a string
                fileNames.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing directory: " << ex.what() << std::endl;
    }
    return fileNames;
}
