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

#include "settings.h"

#include <GLES3/gl3.h>

namespace cg {
#define VS                                                                                         \
  "#version 300 es\n"                                                                              \
  "precision mediump float;\n"                                                                     \
  "layout(location = 0) in vec3 position;\n"                                                       \
  "layout(location = 1) in vec2 texcoord;\n"                                                       \
  "out vec2 _texcoord_;\n"                                                                         \
  "void main() {\n"                                                                                \
  "  _texcoord_ = texcoord;\n"                                                                     \
  "  gl_Position = vec4(position.x, -position.y, 0.0, 1.0);\n"                                     \
  "}\n"

#define FS                                                                                         \
  "#version 300 es\n"                                                                              \
  "precision mediump float;\n"                                                                     \
  "in vec2 _texcoord_;\n"                                                                          \
  "uniform sampler2D tex0;\n"                                                                      \
  "layout(location = 0) out vec4 fragColor;\n"                                                     \
  "void main() {\n"                                                                                \
  "  fragColor = vec4(texture(tex0, _texcoord_).rgb, 1.0);\n"                                      \
  "}\n"

const std::vector<float> verticesTexcoords = {1.0,  -1.0, 0.0, 1.0, 0.0, -1.0, 1.0,  0.0, 0.0, 1.0,
                                              -1.0, -1.0, 0.0, 0.0, 0.0, 1.0,  -1.0, 0.0, 1.0, 0.0,
                                              1.0,  1.0,  0.0, 1.0, 1.0, -1.0, 1.0,  0.0, 0.0, 1.0};

const std::vector<unsigned short> indices = {0, 1, 2, 3, 4, 5};

struct Renderer {
  Renderer(World world, Camera camera) : world(world), camera(camera) {
    pixelX = 0;
    renderedImageIndex = 0;
    glEnable(GL_DEPTH_TEST);
    GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgramId = glCreateProgram();
    std::string VSString(VS);
    std::string FSString(FS);
    std::vector<char> vsSrcCode(VSString.begin(), VSString.end());
    std::vector<char> fsSrcCode(FSString.begin(), FSString.end());
    auto vsSrc = static_cast<const GLchar *>(vsSrcCode.data());
    auto vsSrcLen = static_cast<const GLint>(vsSrcCode.size());
    auto fsSrc = static_cast<const GLchar *>(fsSrcCode.data());
    auto fsSrcLen = static_cast<const GLint>(fsSrcCode.size());
    glShaderSource(vsId, 1, &vsSrc, &vsSrcLen);
    glCompileShader(vsId);
    glShaderSource(fsId, 1, &fsSrc, &fsSrcLen);
    glCompileShader(fsId);
    glAttachShader(shaderProgramId, vsId);
    glAttachShader(shaderProgramId, fsId);
    glLinkProgram(shaderProgramId);
    glDetachShader(shaderProgramId, vsId);
    glDetachShader(shaderProgramId, fsId);
    glDeleteShader(vsId);
    glDeleteShader(fsId);

    glGenTextures(1, &raycastingTextureId);
    glBindTexture(GL_TEXTURE_2D, raycastingTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.resolution[0], camera.resolution[1], 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &raytracingTextureId);
    glBindTexture(GL_TEXTURE_2D, raytracingTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.resolution[0], camera.resolution[1], 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &pathtracingTextureId);
    glBindTexture(GL_TEXTURE_2D, pathtracingTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.resolution[0], camera.resolution[1], 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    const size_t VERTEX_SIZE = 3 * sizeof(float) + 2 * sizeof(float);
    const size_t POSITION_STRIDE = 0;
    const size_t TEXCOORD_STRIDE = POSITION_STRIDE + 3 * sizeof(float);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesTexcoords.size() * sizeof(float), &verticesTexcoords[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void *)(POSITION_STRIDE));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void *)(TEXCOORD_STRIDE));
    glEnableVertexAttribArray(1);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0],
                 GL_STATIC_DRAW);
    glBindVertexArray(0);
  }

  ~Renderer() {
    glDeleteProgram(shaderProgramId);
    glDeleteTextures(1, &raycastingTextureId);
    glDeleteTextures(1, &raytracingTextureId);
    glDeleteTextures(1, &pathtracingTextureId);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
  }

  std::array<unsigned char, 3> raycastingShadingFn(const cg::SHAPE_HIT_INFO &hitInfo) {
    cg::Shape *shape = hitInfo.collider;
    return std::array<unsigned char, 3>{static_cast<unsigned char>(shape->color[0] * 255.0),
                                        static_cast<unsigned char>(shape->color[1] * 255.0),
                                        static_cast<unsigned char>(shape->color[2] * 255.0)};
  }

  std::array<unsigned char, 3> raytracingShadingFn(const cg::SHAPE_HIT_INFO &shapeHitInfo,
                                                   const cg::LIGHT_HIT_INFO &lightHitInfo) {
    cg::Shape *shape = shapeHitInfo.collider;
    cg::Light *light = lightHitInfo.collider;
    glm::vec3 hitPoint = glm::make_vec3(&shapeHitInfo.hitPoint[0]);
    glm::vec3 hitNormal = glm::make_vec3(&shapeHitInfo.hitNormal[0]);
    glm::vec3 lightDirection = glm::normalize(glm::make_vec3(&lightHitInfo.hitPoint[0]) - hitPoint);
    float angle = glm::dot(lightDirection, hitNormal);
    angle = glm::max(angle, 0.0f);
    return std::array<unsigned char, 3>{
        static_cast<unsigned char>(shape->color[0] * 255.0 * angle * light->shape->color[0]),
        static_cast<unsigned char>(shape->color[1] * 255.0 * angle * light->shape->color[1]),
        static_cast<unsigned char>(shape->color[2] * 255.0 * angle * light->shape->color[2])};
  }

  std::array<unsigned char, 3> pathtracingShadingFn(const cg::SHAPE_HIT_INFO &shapeHitInfo,
                                                    const cg::LIGHT_HIT_INFO &lightHitInfo) {
    cg::Shape *shape = shapeHitInfo.collider;
    cg::Light *light = lightHitInfo.collider;
    glm::vec3 hitPoint = glm::make_vec3(&shapeHitInfo.hitPoint[0]);
    glm::vec3 hitNormal = glm::make_vec3(&shapeHitInfo.hitNormal[0]);
    glm::vec3 lightDirection = glm::normalize(glm::make_vec3(&lightHitInfo.hitPoint[0]) - hitPoint);
    float diffuse = glm::max(glm::dot(lightDirection, glm::normalize(hitNormal)), 0.0f);
    return std::array<unsigned char, 3>{
        static_cast<unsigned char>(shape->color[0] * 255.0 * diffuse * light->shape->color[0]),
        static_cast<unsigned char>(shape->color[1] * 255.0 * diffuse * light->shape->color[1]),
        static_cast<unsigned char>(shape->color[2] * 255.0 * diffuse * light->shape->color[2])};
  }

  void renderRaycasting(cg::World world, cg::Camera camera, size_t pixelX, size_t pixelY,
                        size_t renderedImageIndex) {
    const float clearColor = 0.0f;
    auto [ray1, rayInfoCameraShape] = RayHitResponseCache[camera.resolution[0] * pixelX + pixelY];
    if (!rayInfoCameraShape.shapeHitInfo.has_value()) {
      raycastingImage[renderedImageIndex++] = (unsigned char)clearColor;
      raycastingImage[renderedImageIndex++] = (unsigned char)clearColor;
      raycastingImage[renderedImageIndex++] = (unsigned char)clearColor;
    } else {
      auto color = raycastingShadingFn(rayInfoCameraShape.shapeHitInfo.value());
      raycastingImage[renderedImageIndex++] = color[0];
      raycastingImage[renderedImageIndex++] = color[1];
      raycastingImage[renderedImageIndex++] = color[2];
    }
  }

  void renderRaytracing(cg::World world, cg::Camera camera, size_t pixelX, size_t pixelY,
                        size_t renderedImageIndex) {
    const float clearColor = 0.0f;
    auto [ray1, rayInfoCameraShape] = RayHitResponseCache[camera.resolution[0] * pixelX + pixelY];
    if (!rayInfoCameraShape.shapeHitInfo.has_value()) {
      raytracingImage[renderedImageIndex++] = (unsigned char)clearColor;
      raytracingImage[renderedImageIndex++] = (unsigned char)clearColor;
      raytracingImage[renderedImageIndex++] = (unsigned char)clearColor;
    } else {
      glm::vec3 from(glm::make_vec3(&rayInfoCameraShape.shapeHitInfo->hitPoint[0]));
      glm::vec3 to(glm::make_vec3(&world.cubeLights[0].shape->position[0]));
      cg::HIT_RAY ray2 = {from, to};
      cg::HIT_INFO rayInfoShapeLight = cg::raycast(ray2, world);
      if (!rayInfoShapeLight.lightHitInfo.has_value()) {
        raytracingImage[renderedImageIndex++] = (unsigned char)clearColor;
        raytracingImage[renderedImageIndex++] = (unsigned char)clearColor;
        raytracingImage[renderedImageIndex++] = (unsigned char)clearColor;
      } else {
        auto color = raytracingShadingFn(rayInfoCameraShape.shapeHitInfo.value(),
                                         rayInfoShapeLight.lightHitInfo.value());
        raytracingImage[renderedImageIndex++] = color[0];
        raytracingImage[renderedImageIndex++] = color[1];
        raytracingImage[renderedImageIndex++] = color[2];
      }
    }
  }

  void renderPathtracing(cg::World world, cg::Camera camera, size_t pixelX, size_t pixelY,
                         size_t renderedImageIndex) {
    const float clearColor = 0.0f;
    const float shadowColor = 0.0f;
    auto [ray1, rayInfoCameraShape] = RayHitResponseCache[camera.resolution[0] * pixelX + pixelY];
    std::vector<cg::SHAPE_HIT_INFO> orderedRayShapeHits;
    std::vector<cg::LIGHT_HIT_INFO> orderedRayLight;
    if (!rayInfoCameraShape.shapeHitInfo.has_value()) {
      pathtracingImage[renderedImageIndex++] = (unsigned char)clearColor;
      pathtracingImage[renderedImageIndex++] = (unsigned char)clearColor;
      pathtracingImage[renderedImageIndex++] = (unsigned char)clearColor;
    } else {
      glm::vec3 originalColor =
          glm::make_vec3(&rayInfoCameraShape.shapeHitInfo->collider->color[0]);
      glm::vec3 from(glm::make_vec3(&rayInfoCameraShape.shapeHitInfo->hitPoint[0]));
      glm::vec3 to(glm::make_vec3(&world.cubeLights[0].shape->position[0]));
      cg::HIT_RAY ray2 = {from, to};
      cg::HIT_INFO rayInfoShapeLight = cg::raycast(ray2, world);
      if (!rayInfoShapeLight.lightHitInfo.has_value()) {
        pathtracingImage[renderedImageIndex++] = (unsigned char)clearColor;
        pathtracingImage[renderedImageIndex++] = (unsigned char)clearColor;
        pathtracingImage[renderedImageIndex++] = (unsigned char)clearColor;
      } else {
        orderedRayLight.push_back(rayInfoShapeLight.lightHitInfo.value());
        do {
          if (rayInfoCameraShape.lightHitInfo.has_value()) {
            break;
          }
          if (!rayInfoCameraShape.shapeHitInfo.has_value()) {
            break;
          }
          orderedRayShapeHits.push_back(rayInfoCameraShape.shapeHitInfo.value());
          glm::vec3 incidence = (ray1.end - ray1.start);
          glm::vec3 normal = glm::make_vec3(&rayInfoCameraShape.shapeHitInfo->hitNormal[0]);
          glm::vec3 reflectDir = glm::reflect(incidence, normal);
          glm::vec3 from(glm::make_vec3(&rayInfoCameraShape.shapeHitInfo->hitPoint[0]));
          ray1 = {from, from + reflectDir * camera.zfar};
          rayInfoCameraShape = cg::raycast(ray1, world);
        } while (true);
        glm::vec3 color(1.0f, 1.0f, 1.0f);
        float contributionPercentage = 1.0f;
        for (size_t i = 0; i < orderedRayShapeHits.size(); ++i) {
          std::array<unsigned char, 3> shading =
              pathtracingShadingFn(orderedRayShapeHits[i], orderedRayLight[0]);
          color += glm::max(
              (glm::vec3(shading[0], shading[1], shading[2]) *
               glm::vec3(contributionPercentage, contributionPercentage, contributionPercentage)),
              glm::vec3(0.0f, 0.0f, 0.0f));
          contributionPercentage *= 0.75;
        }
        color = glm::clamp(color, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(255.0f, 255.0f, 255.0f));
        pathtracingImage[renderedImageIndex++] = (unsigned char)color[0];
        pathtracingImage[renderedImageIndex++] = (unsigned char)color[1];
        pathtracingImage[renderedImageIndex++] = (unsigned char)color[2];
      }
    }
  }

  void renderAll(int width, int height) {
    for (size_t pixelX = 0; pixelX < camera.resolution[1]; ++pixelX) {
      for (size_t pixelY = 0; pixelY < camera.resolution[0]; ++pixelY) {
        cg::HIT_RAY ray1 = cg::unprojectFromCameraXY(camera, pixelY, pixelX);
        cg::HIT_INFO rayInfoCameraShape = cg::raycast(ray1, world);
        RayHitResponseCache[camera.resolution[0] * pixelX + pixelY] =
            std::make_tuple(ray1, rayInfoCameraShape);
      }
    }
  }

  void renderToScreen(int width, int height) {
    if (pixelX < camera.resolution[1]) {
      for (size_t pixelY = 0; pixelY < camera.resolution[0]; ++pixelY) {
        renderRaycasting(world, camera, pixelX, pixelY, renderedImageIndex);
        renderRaytracing(world, camera, pixelX, pixelY, renderedImageIndex);
        renderPathtracing(world, camera, pixelX, pixelY, renderedImageIndex);
        renderedImageIndex += RENDER_IMAGE_CHANNELS;
      }
      pixelX++;
      glBindTexture(GL_TEXTURE_2D, raycastingTextureId);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, camera.resolution[0], camera.resolution[1], GL_RGB,
                      GL_UNSIGNED_BYTE, raycastingImage.data());
      glBindTexture(GL_TEXTURE_2D, 0);

      glBindTexture(GL_TEXTURE_2D, raytracingTextureId);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, camera.resolution[0], camera.resolution[1], GL_RGB,
                      GL_UNSIGNED_BYTE, raytracingImage.data());
      glBindTexture(GL_TEXTURE_2D, 0);

      glBindTexture(GL_TEXTURE_2D, pathtracingTextureId);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, camera.resolution[0], camera.resolution[1], GL_RGB,
                      GL_UNSIGNED_BYTE, pathtracingImage.data());
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width / 2, height / 2);
    glUseProgram(shaderProgramId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgramId, "tex0"), 0);
    glBindTexture(GL_TEXTURE_2D, raycastingTextureId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

    glViewport(width / 2, 0, width / 2, height / 2);
    glUseProgram(shaderProgramId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgramId, "tex0"), 0);
    glBindTexture(GL_TEXTURE_2D, raytracingTextureId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

    glViewport(0, height / 2, width / 2, height / 2);
    glUseProgram(shaderProgramId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgramId, "tex0"), 0);
    glBindTexture(GL_TEXTURE_2D, pathtracingTextureId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
  }

  size_t pixelX, renderedImageIndex;
  World world;
  Camera camera;
  std::array<unsigned char, RENDER_IMAGE_TOTAL_SIZE> raycastingImage;
  std::array<unsigned char, RENDER_IMAGE_TOTAL_SIZE> raytracingImage;
  std::array<unsigned char, RENDER_IMAGE_TOTAL_SIZE> pathtracingImage;
  GLuint shaderProgramId;
  GLuint raycastingTextureId;
  GLuint raytracingTextureId;
  GLuint pathtracingTextureId;
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  typedef std::tuple<cg::HIT_RAY, cg::HIT_INFO> RayHitResponse;
  std::array<RayHitResponse, RENDER_IMAGE_WIDTH * RENDER_IMAGE_HEIGHT> RayHitResponseCache;
};
#undef VS
#undef FS
} // namespace cg