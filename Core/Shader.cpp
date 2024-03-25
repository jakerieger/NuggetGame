#include "Shader.h"

#include "Utilities.inl"

#include <fstream>
#include <string>
#include <exception>
#include <regex>

#include "glad/glad.h"

static std::string CheckCompileErrors(u32 shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            printf("ERR::SHADER::COMPILE - %s\n", infoLog);
            return infoLog;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            printf("ERR::SHADER::LINK - %s\n", infoLog);
            return infoLog;
        }
    }

    return "";
}

static std::optional<const FShaderSource> ReadShaderFromFile(const char* path) noexcept {
    const auto VERTEX_START   = "#define VERTEX\n";
    const auto VERTEX_END     = "#undef VERTEX\n";
    const auto FRAGMENT_START = "#define FRAGMENT\n";
    const auto FRAGMENT_END   = "#undef FRAGMENT\n";

    const auto readShaderResult = Utilities::ReadFileToString(path);
    if (!readShaderResult.has_value()) {
        return {};
    }
    const std::string& strSource = readShaderResult.value();

    // Check syntax is correct
    // =======================
    size_t vertStart = strSource.find(VERTEX_START);
    if (vertStart == std::string::npos) {
        return {};
    }
    vertStart += strlen(VERTEX_START);

    size_t vertEnd = strSource.find(VERTEX_END);
    if (vertEnd == std::string::npos) {
        return {};
    }
    vertEnd = vertEnd - vertStart;

    size_t fragStart = strSource.find(FRAGMENT_START);
    if (fragStart == std::string::npos) {
        return {};
    }
    fragStart += strlen(FRAGMENT_START);

    size_t fragEnd = strSource.find(FRAGMENT_END);
    if (fragEnd == std::string::npos) {
        return {};
    }
    fragEnd = fragEnd - fragStart;

    const std::string vertexCode   = strSource.substr(vertStart, vertEnd);
    const std::string fragmentCode = strSource.substr(fragStart, fragEnd);

    return FShaderSource {
      vertexCode,
      fragmentCode,
    };
}

static std::optional<const FShaderSource> ReadShaderFromSource(const std::string& source) noexcept {
    constexpr auto VERTEX_START   = "#define VERTEX\n";
    constexpr auto VERTEX_END     = "#undef VERTEX\n";
    constexpr auto FRAGMENT_START = "#define FRAGMENT\n";
    constexpr auto FRAGMENT_END   = "#undef FRAGMENT\n";
    constexpr auto GEOMETRY_START = "#define GEOMETRY\n";
    constexpr auto GEOMETRY_END   = "#undef GEOMETRY\n";
    FShaderSource sources;

    // Check syntax is correct
    // =======================
    size_t vertStart = source.find(VERTEX_START);
    if (vertStart == std::string::npos) {
        return {};
    }
    vertStart += strlen(VERTEX_START);

    size_t vertEnd = source.find(VERTEX_END);
    if (vertEnd == std::string::npos) {
        return {};
    }
    vertEnd = vertEnd - vertStart;

    size_t fragStart = source.find(FRAGMENT_START);
    if (fragStart == std::string::npos) {
        return {};
    }
    fragStart += strlen(FRAGMENT_START);

    size_t fragEnd = source.find(FRAGMENT_END);
    if (fragEnd == std::string::npos) {
        return {};
    }
    fragEnd = fragEnd - fragStart;

    const std::string vertexCode   = source.substr(vertStart, vertEnd);
    const std::string fragmentCode = source.substr(fragStart, fragEnd);
    sources.m_Vertex               = vertexCode;
    sources.m_Fragment             = fragmentCode;
    sources.m_Geometry             = "";

    size_t geoStart = source.find(GEOMETRY_START);
    if (geoStart == std::string::npos) {
        return sources;
    }
    geoStart += strlen(GEOMETRY_START);

    size_t geoEnd = source.find(GEOMETRY_END);
    if (geoEnd == std::string::npos) {
        return {};
    }
    geoEnd = geoEnd - geoStart;

    const std::string geometryCode = source.substr(geoStart, geoEnd);
    sources.m_Geometry             = geometryCode;

    return sources;
}

void AShader::CompileShaders(const FShaderSource& sources) {
    const auto vertexCode   = sources.m_Vertex.c_str();
    const auto fragmentCode = sources.m_Fragment.c_str();

    const u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    if (const auto error = CheckCompileErrors(vertexShader, "VS"); !error.empty()) {
        fprintf(stderr, "ERROR::SHADER::VS::COMPILE - %s\n", error.c_str());
        return;
    }

    const u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    if (const auto error = CheckCompileErrors(fragmentShader, "FS"); !error.empty()) {
        fprintf(stderr, "ERROR::SHADER::FS::COMPILE - %s\n", error.c_str());
        return;
    }

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);

    glLinkProgram(m_ShaderProgram);
    if (const auto error = CheckCompileErrors(m_ShaderProgram, "PROGRAM"); !error.empty()) {
        fprintf(stderr, "ERROR::SHADER::LINK - %s\n", error.c_str());
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

AShader::AShader(const char* path) {
    const auto sources = ReadShaderFromFile(path);
    if (!sources.has_value()) {
        // fprintf(stderr, "ERROR::SHADER::READ - %d\n", static_cast<int>(sources.error()));
        throw std::exception();
    }

    const auto& processedSources = sources.value(); /*Preprocess(sources.value());*/
    CompileShaders(processedSources);
}

AShader::AShader(const std::string& source) {
    const auto sources = ReadShaderFromSource(source);
    if (!sources.has_value()) {
        // fprintf(stderr, "ERROR::SHADER::READ - %d\n", static_cast<int>(sources.error()));
        throw std::exception();
    }

    const auto& processedSources = sources.value(); /*Preprocess(sources.value());*/
    CompileShaders(processedSources);
}

AShader::AShader(const FShaderSource& sources) { CompileShaders(sources); }

void AShader::Use() const { glUseProgram(m_ShaderProgram); }

void AShader::Destroy() const { glDeleteProgram(m_ShaderProgram); }

void AShader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), static_cast<int>(value));
}

void AShader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
}

void AShader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
}

void AShader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, &value[0]);
}

void AShader::SetVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y);
}

void AShader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, &value[0]);
}

void AShader::SetVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z);
}

void AShader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, &value[0]);
}

void AShader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z, w);
}

void AShader::SetMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}

void AShader::SetMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}

void AShader::SetMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()),
                       1,
                       GL_FALSE,
                       &mat[0][0]);
}