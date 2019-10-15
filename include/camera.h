// ==------------------------------------------------------------------------------==
// Copyright <2019> <github.com/o-micron>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ==------------------------------------------------------------------------------==

#pragma once

#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

namespace cg {
struct Camera {
  Camera(std::array<float, 2> resolution, float fov, float znear, float zfar,
         std::array<float, 3> position, std::array<float, 3> rotation)
      : resolution(glm::make_vec2(&resolution[0])), fov(fov), znear(znear), zfar(zfar),
        position(glm::make_vec3(&position[0])), rotation(glm::make_vec3(&rotation[0])) {
    update();
  }
  void update() {
    projectionMatrix =
        glm::perspective(glm::radians(fov), resolution[0] / resolution[1], znear, zfar);
    inverseProjectionMatrix = glm::inverse(projectionMatrix);
    glm::quat m_rotation =
        glm::quat(glm::eulerAngleYX(glm::radians(rotation[1]), glm::radians(rotation[0])));
    glm::mat4 positionMatrix = glm::translate(-glm::vec3(position[0], position[1], position[2]));
    glm::mat4 rotationMatrix = glm::transpose(glm::toMat4(m_rotation));
    viewMatrix = rotationMatrix * positionMatrix;
    inverseViewMatrix = glm::inverse(viewMatrix);
  }

  glm::vec2 resolution;
  float fov;
  float znear;
  float zfar;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::mat4 projectionMatrix;
  glm::mat4 inverseProjectionMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 inverseViewMatrix;
};
} // namespace cg
