#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shaders.hh"
#include "math.hh"

extern int main();
static void handleInput (GLFWwindow * window);


static float gTestVA[] {
    0.f, 0.f, 0.f,
    4.f, 0.f, 0.f,
    0.f, 4.f, 0.f,
    0.f, 0.f, 4.f,
    2.f, 0.f, 1.f,
    -2.f, 0.f, 1.f,
    2.f, 1.f, 0.f,
    -2.f, 1.f, 0.f,
};

static int gTestIA[] {
    0, 1,
    0, 2,
    0, 3,
    4, 5,
    6, 7,
};

static float gPrismVA[] {
    1.0f, 0.f, 0.f,
};

static int gPrismIA[] {
    1
};

static struct Camera {
    Vec3 pos;
} gCamera {{3, 4, 6}};

Transform calcViewTransform (Camera cam) {
    // 1..3
    auto viewTransform = Transform {}
        .translate(-cam.pos)
        .scale({-1.f, 1.f, 1.f})
        .rotateX(.25f * TAU)
    ;

    // 4
    float d; {
        Transform R {};
        d = hypotf(cam.pos.x, cam.pos.y);
        float cosu;
        float sinu;

        if (d < EPSILON) {
            cosu = 1.f;
            sinu = 0.f;
        }

        cosu = cam.pos.y / d;
        sinu = cam.pos.x / d;
        
        R.mat[0] = cosu;
        R.mat[10] = cosu;
        R.mat[2] = sinu;
        R.mat[8] = -sinu;

        viewTransform *= R;
    }

    // 5
    {
        Transform R {};
        float s = hypotf(d, cam.pos.z);
        float cosw;
        float sinw;

        if (s < EPSILON) {
            cosw = 1.f;
            sinw = 0.f;
        }

        cosw = d / s;
        sinw = cam.pos.z / s;

        R.mat[5] = cosw;
        R.mat[10] = cosw;
        R.mat[6] = -sinw;
        R.mat[9] = sinw;

        viewTransform *= R;
    }

    return viewTransform;
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window; {
        if (!(window = glfwCreateWindow(800, 600, "opengl", NULL, NULL))) {
            exit(-1);
        }
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(-1);
    }

    /****************************************************************/





    /****************************************************************/
    // and add the thingy to move camera on key press

    unsigned shaderProgram; {
        char const * vertexShaderSrc = gVertexShaderSrc;
        char const * fragShaderSrc = gFragmentShaderSrc;

        unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
        glCompileShader(vertexShader);

        unsigned fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
        glCompileShader(fragShader);

        {
            int frag, vertex;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex);
            glGetShaderiv(fragShader, GL_COMPILE_STATUS, &frag);
            if (frag == GL_FALSE || vertex == GL_FALSE) {
                exit(-1);
            }
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);

        {
            int ok;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);
            if (ok == GL_FALSE) {
                exit(-1);
            }
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
    }
    unsigned vao, vbo; {
        float * vertices = gPrismVA;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // TODO correct the number of vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }



    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        handleInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, 3);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}

void handleInput (GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}
