#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name){
            GLint location = glGetUniformLocation(program, (GLchar*)name.c_str());
            
            return (GLuint)location;
        }

        void set(const std::string &uniform, GLfloat value) {
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            glUniform2fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec3 value) {
            glUniform3fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec4 value) {
            glUniform4fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }
        ShaderProgram(const ShaderProgram&) = delete; 
        ShaderProgram& operator=(const ShaderProgram&) = delete; 

        //Question: Why do we delete the copy constructor and assignment operator?

        //To prevent objects of a class from being copied or assigned to each other         // completeeee

    };

}

#endif