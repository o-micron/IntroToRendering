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

#include "camera.h"
#include "shapes.h"
#include "world.h"

#include <array>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <btBulletDynamicsCommon.h>

namespace cg {
static void initializeCollisionWorld(cg::World &world) {
  world.collisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
  world.dispatcher = new btCollisionDispatcher(world.collisionConfig);
  world.broadphase = new btDbvtBroadphase();
  world.solver = new btSequentialImpulseConstraintSolver();
  world.softbodySolver = new btDefaultSoftBodySolver();
  world.world = new btSoftRigidDynamicsWorld(world.dispatcher, world.broadphase, world.solver,
                                             world.collisionConfig, world.softbodySolver);
  world.world->setGravity(btVector3(0.0, -9.8, 0.0));
}

static void loadPlaneShape(cg::Plane &plane, cg::World &world) {
  glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0), glm::make_vec3(&plane.position[0]));
  glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                                   glm::radians(glm::make_vec3(&plane.rotation[0])));
  glm::mat4 rotationMatrix = glm::toMat4(rotation);
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::make_vec3(&plane.scale[0]));
  glm::mat4 modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
  btTransform t;
  btScalar openglMatrix[16] = {
      modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2], modelMatrix[0][3],
      modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2], modelMatrix[1][3],
      modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2], modelMatrix[2][3],
      modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2], modelMatrix[3][3]};
  t.setFromOpenGLMatrix(openglMatrix);
  btBoxShape *btBox = new btBoxShape(btVector3(plane.width, plane.height, 0.01));
  btVector3 inertia(0.0, 0.0, 0.0);
  btMotionState *motion = new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(btScalar(0.0), motion, btBox, inertia);
  btRigidBody *rigidbody = new btRigidBody(info);
  rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() |
                               btCollisionObject::CF_STATIC_OBJECT);
  rigidbody->setUserPointer(&plane);
  world.rigidbodies.push_back(rigidbody);
  world.world->addRigidBody(rigidbody);
}

static void loadCubeShape(cg::Cube &cube, cg::World &world) {
  glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0), glm::make_vec3(&cube.position[0]));
  glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                                   glm::radians(glm::make_vec3(&cube.rotation[0])));
  glm::mat4 rotationMatrix = glm::toMat4(rotation);
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::make_vec3(&cube.scale[0]));
  glm::mat4 modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
  btTransform t;
  btScalar openglMatrix[16] = {
      modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2], modelMatrix[0][3],
      modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2], modelMatrix[1][3],
      modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2], modelMatrix[2][3],
      modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2], modelMatrix[3][3]};
  t.setFromOpenGLMatrix(openglMatrix);
  btBoxShape *btBox = new btBoxShape(btVector3(cube.width, cube.height, cube.depth));
  btVector3 inertia(0.0, 0.0, 0.0);
  btMotionState *motion = new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, btBox, inertia);
  btRigidBody *rigidbody = new btRigidBody(info);
  rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() |
                               btCollisionObject::CF_STATIC_OBJECT);
  rigidbody->setUserPointer(&cube);
  world.rigidbodies.push_back(rigidbody);
  world.world->addRigidBody(rigidbody);
}

static void loadSphereShape(cg::Sphere &sphere, cg::World &world) {
  glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0), glm::make_vec3(&sphere.position[0]));
  glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                                   glm::radians(glm::make_vec3(&sphere.rotation[0])));
  glm::mat4 rotationMatrix = glm::toMat4(rotation);
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::make_vec3(&sphere.scale[0]));
  glm::mat4 modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
  btTransform t;
  btScalar openglMatrix[16] = {
      modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2], modelMatrix[0][3],
      modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2], modelMatrix[1][3],
      modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2], modelMatrix[2][3],
      modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2], modelMatrix[3][3]};
  t.setFromOpenGLMatrix(openglMatrix);
  btSphereShape *btSphere = new btSphereShape(sphere.radius);
  btVector3 inertia(0.0, 0.0, 0.0);
  btMotionState *motion = new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, btSphere, inertia);
  btRigidBody *rigidbody = new btRigidBody(info);
  rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() |
                               btCollisionObject::CF_STATIC_OBJECT);
  rigidbody->setUserPointer(&sphere);
  world.rigidbodies.push_back(rigidbody);
  world.world->addRigidBody(rigidbody);
}

struct SHAPE_HIT_INFO {
  Shape *collider;
  std::array<float, 3> hitPoint;
  std::array<float, 3> hitNormal;
};

struct LIGHT_HIT_INFO {
  Light *collider;
  std::array<float, 3> hitPoint;
  std::array<float, 3> hitNormal;
};

struct HIT_INFO {
  std::optional<SHAPE_HIT_INFO> shapeHitInfo;
  std::optional<LIGHT_HIT_INFO> lightHitInfo;
};

struct HIT_RAY {
  glm::vec3 start;
  glm::vec3 end;
};

static HIT_RAY unprojectFromCameraXY(cg::Camera &camera, const size_t pixelX, const size_t pixelY) {
  float x = (2.0f * pixelX) / camera.resolution[0] - 1.0f;
  float y = 1.0f - (2.0f * pixelY) / camera.resolution[1];
  float z = 1.0f;
  glm::vec3 ray_nds = glm::vec3(x, y, z);
  glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
  glm::vec4 ray_eye = camera.inverseProjectionMatrix * ray_clip;
  ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
  glm::vec4 worldEye = camera.inverseViewMatrix * ray_eye;
  glm::vec3 ray_world = glm::vec3(worldEye.x, worldEye.y, worldEye.z);
  ray_world = glm::normalize(ray_world);
  glm::vec3 viewDirection = ray_world * camera.zfar;

  glm::vec3 out_origin(camera.position[0], camera.position[1], camera.position[2]);
  glm::vec3 out_end(camera.position[0] + viewDirection[0], camera.position[1] + viewDirection[1],
                    camera.position[2] + viewDirection[2]);
  return {out_origin, out_end};
}

static HIT_INFO raycast(const HIT_RAY &ray, cg::World &world) {
  btCollisionWorld::ClosestRayResultCallback allResults(
      btVector3(ray.start.x, ray.start.y, ray.start.z), btVector3(ray.end.x, ray.end.y, ray.end.z));
  world.world->rayTest(btVector3(ray.start.x, ray.start.y, ray.start.z),
                       btVector3(ray.end.x, ray.end.y, ray.end.z), allResults);
  if (allResults.hasHit()) {
    const btCollisionObject *collisionObject = allResults.m_collisionObject;
    btVector3 hitPoint = allResults.m_hitPointWorld;
    btVector3 hitNormal = allResults.m_hitNormalWorld;
    Shape *shape = static_cast<Shape *>(collisionObject->getUserPointer());
    if (shape->objectPtr == nullptr) {
      return {SHAPE_HIT_INFO{shape, std::array<float, 3>{hitPoint.x(), hitPoint.y(), hitPoint.z()},
                             std::array<float, 3>{hitNormal.x(), hitNormal.y(), hitNormal.z()}},
              {}};
    } else {
      return {{},
              LIGHT_HIT_INFO{static_cast<Light *>(shape->objectPtr),
                             std::array<float, 3>{hitPoint.x(), hitPoint.y(), hitPoint.z()},
                             std::array<float, 3>{hitNormal.x(), hitNormal.y(), hitNormal.z()}}};
    }
  }
  return {{}, {}};
}

static void deinitializeCollisionWorld(cg::World &world) {
  for (btRigidBody *rigidbody : world.rigidbodies) {
    world.world->removeCollisionObject(rigidbody);
    delete rigidbody->getMotionState();
    delete rigidbody->getCollisionShape();
    delete rigidbody;
  }
  world.rigidbodies.clear();
  delete world.dispatcher;
  delete world.collisionConfig;
  delete world.solver;
  delete world.broadphase;
  delete world.world;
}
} // namespace cg
