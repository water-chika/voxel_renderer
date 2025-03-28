#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

struct Vertex {
    vec3 pos;
    vec3 col;
};


static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec3 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

auto generate_cube_vertices(vec3 center={}) {
    std::vector<Vertex> res{};
    for (float sign = -1; sign < 2; sign+=2) {
        for (int i = 0; i < 3; i++) {
            auto s = Vertex{ center - sign*vec3{0.5,0.5,0.5}, {0,0,0}};
            s.col = s.pos;
            auto s0 = s;
            s0.pos[i] = center[i]+sign*0.5;
            s0.col = s0.pos;
            auto s1 = s0;
            s1.pos[(i+1)%3] = center[(i+1)%3]+sign*0.5;
            s1.col = s1.pos;
            auto s2 = s;
            s2.pos[(i+1)%3] = center[(i+1)%3]+sign*0.5;
            s2.col = s2.pos;
            res.push_back(s);
            res.push_back(s0);
            res.push_back(s1);
            res.push_back(s);
            res.push_back(s1);
            res.push_back(s2);
        }
    }
    return res;
}

int main() {
    if (!glfwInit()) {
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(
        [](int error, const char* desc) {
            std::cerr << "Error: " << error << ": " << desc << std::endl;
        }
    );

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Cube", NULL, NULL);
    if (!window) {
        std::cerr << "create window failed" << std::endl;
    }
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "glewInit failed" << std::endl;
    }

    auto vertices = generate_cube_vertices({});

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, col));

    glClearColor(255,255,255,255);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4 m{}, v, p, mvp;
        m = mat4x4{
                1,0   ,0,0,
                0,1   ,0,0.0,
                0,0   ,1,0,
                0,0,0.7,1,
        };
        float t = glfwGetTime();
        m = m * mat4x4{
            cos(t),0.0,sin(t),0.0,
            0.0   ,1.0,0.0,0.0,
            -sin(t),0.0,cos(t), 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        v = mat4x4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 1, 0, 1,
        } * mat4x4{
            cos(1), sin(1), 0, 0,
            -sin(1), cos(1), 0, 0,
            0,            0, 1, 0,
            0,            0, 0, 1,
        };
        p = mat4x4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 1-(1/1000.0),
            0, 0, 0, 1,
        };
        mvp = p * v * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const float*)&mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}
