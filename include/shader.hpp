#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

class Shader {
public:
  // program ID
  unsigned int ID;

  // constructor
  Shader(const char *vertexPath, const char *fragmentPath);

  // use function
  void use();

  // utility/uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4fv(const std::string &name, float* value) const;
  void setVec2i(const std::string &name, int v0, int v1) const;
  void setVec2f(const std::string &name, float v0, float v1) const;
  void setVec4uiv(const std::string &name, bool* value) const; 
};

#endif
