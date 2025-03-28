project('opengl_cube', ['cpp','c'])

cmake = import('cmake')

glfw_proj = cmake.subproject('glfw', required: true)

glfw = glfw_proj.dependency('glfw')

inc = include_directories('./include')

executable('opengl_cube', ['main.cpp', 'src/glad.c'],
  dependencies:glfw,
  include_directories:inc)
