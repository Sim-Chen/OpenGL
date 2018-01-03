#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include<array>
using namespace std;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float sigma = 0.01f;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void CreateGuassColor(float *Posdata, float *Colordata);
void creatGaussainPos(float *data, int pointsNums);
void creatGridVextex(float *data, int x, int y);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to Create a window!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to Initialize Glad" << endl;
    }

// 
//     glEnable(GL_LINE_SMOOTH);
//     glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//     glEnable(GL_POINT_SMOOTH);
//     glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader GaussShader("gauss.vs", "gauss.fs");
    Shader GridShader("grid.vs", "grid.fs");



    //ctreat Grid Vextexcis; 
    float *GridVertexices = new float[240];
    creatGridVextex(GridVertexices, 1, 1);


    const int POINTS_NUMS = 1000000;
    //create Guassian vertex pos
    float *GaussVertexPos = new float[POINTS_NUMS * 2];
    creatGaussainPos(GaussVertexPos, POINTS_NUMS);

    // creat every point's color
    float *GaussColor = new float[POINTS_NUMS * 3];
    CreateGuassColor(GaussVertexPos, GaussColor);

    //set up vertex property
    //create GaussVAO,GRIDVAO,GaussVBO,GRidVBO;
    unsigned int GaussVAO, GaussVBO, ColorVBO;
    unsigned int GridVAO, GridVBO;

    glGenVertexArrays(1, &GaussVAO);
    glGenBuffers(1, &GaussVBO);
    glGenBuffers(1, &ColorVBO);
    glBindVertexArray(GaussVAO);
    glBindBuffer(GL_ARRAY_BUFFER, GaussVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*POINTS_NUMS * 2, GaussVertexPos, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*POINTS_NUMS * 3, GaussColor, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
        (void*)(0));
    glBindVertexArray(0);

    glGenVertexArrays(1, &GridVAO);
    glGenBuffers(1, &GridVBO);
    glBindVertexArray(GridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, GridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 240, GridVertexices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        processInput(window);
        GridShader.use();
        glBindVertexArray(GridVAO);
        glDrawArrays(GL_LINES, 0, 100);

        GaussShader.use();
        sigma += 0.01;
        CreateGuassColor(GaussVertexPos, GaussColor);
        if (sigma > 1.0)
        {
            sigma = 0.01;
        }

        glBindVertexArray(GaussVAO);
        glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
        void *ptr = nullptr;
        ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float)*POINTS_NUMS*3, GL_MAP_WRITE_BIT);
        memcpy(ptr, GaussColor, sizeof(float)*POINTS_NUMS*3);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindVertexArray(0);
        glBindVertexArray(GaussVAO);
        glPointSize(2.0f);
        glDrawArrays(GL_POINTS, 0, POINTS_NUMS);


        glfwSwapBuffers(window);
        glfwPollEvents();
        //  cout << glGetError() << endl;



    }
    glDeleteBuffers(1, &GridVBO);
    glDeleteBuffers(1, &GaussVBO);
    glDeleteVertexArrays(1, &GaussVAO);
    glDeleteVertexArrays(1, &GridVAO);
    glfwTerminate();
    delete[] GridVertexices;
    delete[] GaussVertexPos;
    delete[] GaussColor;
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, WIDTH, HEIGHT);
}




void creatGaussainPos(float *data, int pointsNums)
{
    //data = new float[pointsNums*6];
    const int GRID_X = 1000;
    const int GRID_Y = 1000;
    int index = 0;
    for (int x = -GRID_X / 2; x < GRID_X / 2; x++)
    {
        for (int y = -GRID_Y / 2; y < GRID_Y / 2; y++)
        {
            float x_data = 2.0*x / GRID_X;
            float y_data = 2.0f*y / GRID_Y;

            data[index++] = x_data;
            data[index++] = y_data;
        }
    }
}

void CreateGuassColor(float *Posdata, float *Colordata)
{

    int index = 0;
    float max_value = -999.9f;
    float min_value = 999.9f;
    const int POINTS_NUMS = 1000000;
    float *Z_Data = new float[POINTS_NUMS];
    // int Z_Nums = POINTS_NUMS;

    for (int i = 0; i < POINTS_NUMS * 2 - 1; i++)
    {
        float x_data = Posdata[i++];
        float y_data = Posdata[i];

        float z_data = exp(-0.5f*(x_data*x_data) / (sigma*sigma) - 0.5f*(y_data*y_data) / (sigma*sigma)) / (sigma*sigma*2.0f*M_PI);
        Z_Data[index++] = z_data;

        if (z_data > max_value) {
            max_value = z_data;
        }
        if (z_data < min_value) {
            min_value = z_data;
        }

    }

    const float halfmax = (max_value + min_value) / 2;
    int iColorIndex = 0;
    for (int i = 0; i < POINTS_NUMS; i++)
    {
        float value = Z_Data[i];
        float b = (1.0f - value / halfmax) > 0.0 ? (1.0f - value / halfmax) : 0.0;
        float r = (value / halfmax - 1.0f) > 0.0 ? (1.0f - value / halfmax) : 0.0;
        float g = 1.0 - b - r;
        Colordata[iColorIndex++] = r;
        Colordata[iColorIndex++] = g;
        Colordata[iColorIndex++] = b;


    }


    delete[]Z_Data;

}
void creatGridVextex(float *data, int x, int y)
{
    int index = 0;
    for (float i = y; i > -y; i -= 0.1)
    {
        data[index++] = -x;
        data[index++] = i;
        data[index++] = 0;

        data[index++] = x;
        data[index++] = i;
        data[index++] = 0;


    }


    for (float i = x; i > -x; i -= 0.1)
    {
        data[index++] = i;
        data[index++] = y;
        data[index++] = 0;

        data[index++] = i;
        data[index++] = -y;
        data[index++] = 0;


    }
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
