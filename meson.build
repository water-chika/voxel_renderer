project('opengl_cube', ['cpp','c'])

cmake = import('cmake')

glfw_proj = cmake.subproject('glfw', required: true)

glfw = glfw_proj.dependency('glfw')

glm_proj = cmake.subproject('glm', required: true)

glm = glm_proj.dependency('glm')

inc = include_directories('./include')

executable('opengl_cube', ['main.cpp', 'src/glad.c'],
  dependencies: [glfw,glm],
  include_directories:inc)
