#include <shader.h>

#include <string>

namespace fs = std::filesystem;

namespace
{
    std::string readFile(std::string filename)
    {
        // Open the file named "example.txt" for reading
        auto currentpath = fs::current_path();
        auto filepath = fs::path(filename);
        std::ifstream inputFile(filepath);

        std::string output = "";

        std::string line;
        // Check if the file was successfully opened
        if (inputFile.is_open())
        {
            // Read lines from the file until the end is reached
            while (std::getline(inputFile, line))
            {
                // Print each line to the console
                output.append(line + "\n");
            }
            // Close the file stream
            inputFile.close();
        }
        else
        {
            // Print an error message if the file couldn't be opened
            std::cerr << "Unable to open file";
        }
        return output;
    }
}

Shader::Shader(const char *vertexPath, const char *fragPath)
{
    std::string vss = readFile(vertexPath);
    std::string fss = readFile(fragPath);
    const char *vertexShaderSource = vss.c_str();
    const char *fragShaderSource = fss.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        ID = -1;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        ID = -1;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        ID = -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

bool Shader::isValid()
{
    return ID != -1;
}

void Shader::setFloat(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
