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

#include "lights.h"
#include "shapes.h"

#include <vector>

struct btRigidBody;
struct btSoftRigidDynamicsWorld;
struct btDispatcher;
struct btCollisionConfiguration;
struct btBroadphaseInterface;
struct btConstraintSolver;
struct btSoftBodySolver;

namespace cg {
struct World {
  World(std::vector<cg::Point> points, std::vector<cg::Plane> planes, std::vector<cg::Cube> cubes,
        std::vector<cg::Sphere> spheres, std::vector<cg::PointLight> pointLights,
        std::vector<cg::PlaneLight> planeLights, std::vector<cg::CubeLight> cubeLights,
        std::vector<cg::SphereLight> sphereLights)
      : points(points), planes(planes), cubes(cubes), spheres(spheres), pointLights(pointLights),
        planeLights(planeLights), cubeLights(cubeLights), sphereLights(sphereLights) {}

  std::vector<cg::Point> points;
  std::vector<cg::Plane> planes;
  std::vector<cg::Cube> cubes;
  std::vector<cg::Sphere> spheres;

  std::vector<cg::PointLight> pointLights;
  std::vector<cg::PlaneLight> planeLights;
  std::vector<cg::CubeLight> cubeLights;
  std::vector<cg::SphereLight> sphereLights;

  btSoftRigidDynamicsWorld *world;
  btDispatcher *dispatcher;
  btCollisionConfiguration *collisionConfig;
  btBroadphaseInterface *broadphase;
  btConstraintSolver *solver;
  btSoftBodySolver *softbodySolver;
  std::vector<btRigidBody *> rigidbodies;
};
} // namespace cg
