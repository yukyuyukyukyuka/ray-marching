#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
    Shader();
    ~Shader();

    void load(const char* vertex_path, const char* fragment_path);

    inline void set_bool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); }
    inline void set_int(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }
    inline void set_float(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }
    inline void set_double(const std::string& name, double value) const { glUniform1d(glGetUniformLocation(id, name.c_str()), value); }
    inline void set_ivec2(const std::string& name, const glm::ivec2& value) const { glUniform2iv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    inline void set_dvec2(const std::string& name, const glm::dvec2& value) const { glUniform2dv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    inline void set_vec3(const std::string& name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    inline void set_mat4(const std::string& name, const glm::mat4& value) const { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); }

    inline void use() const { glUseProgram(id); }

private:
    unsigned int id;
};