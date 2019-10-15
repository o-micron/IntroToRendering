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
#include <GLFW/glfw3.h>

namespace cg {
struct Window {
  static void glfw_error_callback(int error, const char *description) {
    printf("Window error: %s\n", description);
  }

  static void glfw_window_size_callback(GLFWwindow *handle, int width, int height) {
    Window *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));
    window->width = width;
    window->height = height;
  }

  static void glfw_mouse_btn_callback(GLFWwindow *handle, int button, int action, int mods) {}

  static void glfw_cursor_pos_callback(GLFWwindow *handle, double xpos, double ypos) {}

  static void glfw_scroll_callback(GLFWwindow *handle, double xoffset, double yoffset) {}

  static void glfw_key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) {}

  static void glfw_charCallback(GLFWwindow *handle, unsigned int c) {}

  static void glfw_dropCallback(GLFWwindow *handle, int numFiles, const char **names) {}

  Window() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
      puts("Failed to initialize window");
      return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    width = RENDER_IMAGE_WIDTH * 2;
    height = RENDER_IMAGE_HEIGHT * 2;
    handle = glfwCreateWindow(width, height, "CGExamples", nullptr, nullptr);
    if (!handle) {
      puts("Failed to create glfw window");
      glfwTerminate();
      return;
    }
    glfwSetWindowSizeCallback(handle, glfw_window_size_callback);
    glfwSetFramebufferSizeCallback(handle, glfw_window_size_callback);
    glfwSetMouseButtonCallback(handle, glfw_mouse_btn_callback);
    glfwSetCursorPosCallback(handle, glfw_cursor_pos_callback);
    glfwSetScrollCallback(handle, glfw_scroll_callback);
    glfwSetKeyCallback(handle, glfw_key_callback);
    glfwSetCharCallback(handle, glfw_charCallback);
    glfwSetDropCallback(handle, glfw_dropCallback);
    glfwMakeContextCurrent(handle);
    glfwSetWindowUserPointer(handle, this);
  }

  ~Window() {
    glfwDestroyWindow(handle);
    glfwTerminate();
  }

  void refresh() {
    glfwPollEvents();
    glfwSwapBuffers(handle);
  }

  GLFWwindow *handle;
  int width;
  int height;
};
} // namespace cg