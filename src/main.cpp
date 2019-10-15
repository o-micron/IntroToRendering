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

#include "../include/camera.h"
#include "../include/physics.h"
#include "../include/renderer.h"
#include "../include/settings.h"
#include "../include/window.h"
#include "../include/world.h"

#include <emscripten/emscripten.h>

cg::Window *window;
cg::Renderer *renderer;

void update() {
  window->refresh();
  renderer->renderToScreen(window->width, window->height);
}

int main(int, char **) {
  std::vector<cg::Point> pointShapes = {};
  std::vector<cg::Plane> planeShapes = {
      cg::Plane{{0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {1.0f, 1.0f, 1.0f},
                {1.0f, 0.0f, 0.0f},
                1.0f,
                1.0f},
      cg::Plane{{0.0f, -1.0f, -4.0f},
                {-90.0f, 0.0f, 0.0f},
                {4.5f, 4.5f, 4.5f},
                {1.0f, 1.0f, 1.0f},
                1.0f,
                1.0f},
  };
  std::vector<cg::Cube> cubeShapes = {cg::Cube{{1.5f, 0.0f, -2.5f},
                                               {0.0f, 0.0f, 0.0f},
                                               {1.0f, 1.0f, 1.0f},
                                               {0.0f, 1.0f, 0.0f},
                                               1.0f,
                                               1.0f,
                                               1.0f}};
  std::vector<cg::Sphere> sphereShapes = {cg::Sphere{
      {-1.5f, 0.0f, -2.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, 1.0f}};

  std::vector<cg::PointLight> pointLights = {};
  std::vector<cg::PlaneLight> planeLights = {};
  cg::Cube lightCube0 = cg::Cube{{-2.0f, 4.0f, 5.0f},
                                 {0.0f, 0.0f, 0.0f},
                                 {1.0f, 1.0f, 1.0f},
                                 {1.0f, 1.0f, 1.0f},
                                 1.0f,
                                 1.0f,
                                 1.0f};
  std::vector<cg::CubeLight> cubeLights = {cg::CubeLight{&lightCube0, 10.0f}};
  std::vector<cg::SphereLight> sphereLights = {};

  cg::World world{pointShapes, planeShapes, cubeShapes, sphereShapes,
                  pointLights, planeLights, cubeLights, sphereLights};

  cg::Camera camera{{RENDER_IMAGE_WIDTH, RENDER_IMAGE_HEIGHT},
                    75.0f,
                    0.1f,
                    100.0f,
                    {0.0f, 3.0f, 4.0f},
                    {-20.0f, 0.0f, 0.0f}};

  cg::initializeCollisionWorld(world);
  for (size_t i = 0; i < world.planes.size(); i++) {
    cg::loadPlaneShape(world.planes[i], world);
  }
  for (size_t i = 0; i < world.cubes.size(); i++) {
    cg::loadCubeShape(world.cubes[i], world);
  }
  for (size_t i = 0; i < world.spheres.size(); i++) {
    cg::loadSphereShape(world.spheres[i], world);
  }
  for (size_t i = 0; i < world.planeLights.size(); i++) {
    cg::loadPlaneShape(*static_cast<cg::Plane *>(world.planeLights[i].shape), world);
  }
  for (size_t i = 0; i < world.cubeLights.size(); i++) {
    cg::loadCubeShape(*static_cast<cg::Cube *>(world.cubeLights[i].shape), world);
  }
  for (size_t i = 0; i < world.sphereLights.size(); i++) {
    cg::loadSphereShape(*static_cast<cg::Sphere *>(world.sphereLights[i].shape), world);
  }

  window = new cg::Window();
  renderer = new cg::Renderer(world, camera);
  renderer->renderAll(window->width, window->height);
  emscripten_set_main_loop(update, 0, 1);

  delete renderer;
  delete window;
  cg::deinitializeCollisionWorld(world);
  return 0;
}
