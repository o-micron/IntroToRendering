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

#include "shapes.h"

namespace cg {
struct Light {
  Light(Shape *shape, float intensity) : shape(shape), intensity(intensity) {
    this->shape->objectPtr = this;
  }

  cg::Shape *shape;
  float intensity;
};

struct PointLight : Light {
  PointLight(cg::Point *point, float intensity) : Light(point, intensity) {
    this->shape->objectPtr = this;
  }
};

struct PlaneLight : Light {
  PlaneLight(cg::Plane *plane, float intensity) : Light(plane, intensity) {
    this->shape->objectPtr = this;
  }
};

struct CubeLight : Light {
  CubeLight(cg::Cube *cube, float intensity) : Light(cube, intensity) {
    this->shape->objectPtr = this;
  }
};

struct SphereLight : Light {
  SphereLight(cg::Sphere *sphere, float intensity) : Light(sphere, intensity) {
    this->shape->objectPtr = this;
  }
};
} // namespace cg
