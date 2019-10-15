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

namespace cg {
struct Shape {
  Shape(std::array<float, 3> position, std::array<float, 3> rotation, std::array<float, 3> scale,
        std::array<float, 3> color) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->color = color;
    this->objectPtr = nullptr;
  }

  std::array<float, 3> position;
  std::array<float, 3> rotation;
  std::array<float, 3> scale;
  std::array<float, 3> color;
  void *objectPtr;
};

struct Point : Shape {
  Point(std::array<float, 3> position, std::array<float, 3> rotation, std::array<float, 3> scale,
        std::array<float, 3> color)
      : Shape(position, rotation, scale, color) {
    this->objectPtr = nullptr;
  }
};

struct Plane : Shape {
  Plane(std::array<float, 3> position, std::array<float, 3> rotation, std::array<float, 3> scale,
        std::array<float, 3> color, float width, float height)
      : Shape(position, rotation, scale, color), width(width), height(height) {
    this->objectPtr = nullptr;
  }

  float width;
  float height;
};

struct Cube : Shape {
  Cube(std::array<float, 3> position, std::array<float, 3> rotation, std::array<float, 3> scale,
       std::array<float, 3> color, float width, float height, float depth)
      : Shape(position, rotation, scale, color), width(width), height(height), depth(depth) {
    this->objectPtr = nullptr;
  }

  float width;
  float height;
  float depth;
};

struct Sphere : Shape {
  Sphere(std::array<float, 3> position, std::array<float, 3> rotation, std::array<float, 3> scale,
         std::array<float, 3> color, float radius)
      : Shape(position, rotation, scale, color), radius(radius) {
    this->objectPtr = nullptr;
  }

  float radius;
};
} // namespace cg
