#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float x_mod = 0;
float y_mod = 0;
float z_mod = -2.f;
float theta_mod = 0;
float scale_mod = 1;

void Key_CallBack(GLFWwindow* window,
    int key, int scancode,
    int action, int mods) {
    if (key == GLFW_KEY_W) { //&& action == GLFW_PRESS
        y_mod += 0.01f;
    }
    if (key == GLFW_KEY_A) { //&& action == GLFW_PRESS
        x_mod -= 0.01f;
    }
    if (key == GLFW_KEY_S) { //&& action == GLFW_PRESS
        y_mod -= 0.01f;
    }
    if (key == GLFW_KEY_D) { //&& action == GLFW_PRESS
        x_mod += 0.01f;
    }
    if (key == GLFW_KEY_RIGHT) { //&& action == GLFW_PRESS
        theta_mod += 2.0f;
    }
    if (key == GLFW_KEY_LEFT) { //&& action == GLFW_PRESS
        theta_mod -= 2.0f;
    }
    if (key == GLFW_KEY_Q) { //&& action == GLFW_PRESS
        scale_mod += 2.0f;
    }
    if (key == GLFW_KEY_E) { //&& action == GLFW_PRESS
        scale_mod -= 2.0f;
    }
    if (key == GLFW_KEY_Z) { //&& action == GLFW_PRESS
        z_mod += 0.3f;
    }
    if (key == GLFW_KEY_X ) { //&& action == GLFW_PRESS
        z_mod -= 0.3f;
    }


}
int main(void)
{
    std::cout << " test";

    //creating a 3x3 and a 4x4 matrix        
    //glm::mat3 identity_matrix3 = glm::mat3(1.0f);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float window_height = 600.0f;
    float window_width = 600.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Nikos Railey Bumanglag", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

   glViewport(0, 0, window_width, window_height);

    glfwSetKeyCallback(window, Key_CallBack);
    
    std::fstream vertSrc("Shaders/sample.vert"); //loading shader file into a string stream
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf(); //adding the file stream to the string stream
    
    std::string vertS = vertBuff.str(); 
    const char* v = vertS.c_str();  //converting the stream to a character array

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();

    const char* f = fragS.c_str();

    //creating vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    //Compile the Vertex Shader
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    //Creating shader program
    GLuint shaderProg = glCreateProgram();
    //Attaching compiled vertex shader
    glAttachShader(shaderProg, vertexShader);
    //Atatching compiled fragment shader
    glAttachShader(shaderProg, fragShader);
    
    //Finalizing compilation process
    glLinkProgram(shaderProg);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t>material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );
    
    //getting EBO indices array
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f //2
      // x    y     z
    };

    GLuint indices[]{
        0, 1, 2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //vao = null
    glBindVertexArray(VAO);
    //vbo = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //vbo = vbo
    
    glBufferData(GL_ARRAY_BUFFER, 
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        attributes.vertices.data(), // == &attributes.vertices[0]
        GL_STATIC_DRAW);
    

    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT, 
        GL_FALSE,
        3 * sizeof(float),
        (void*)0 
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(), // == &mesh_indices[0]
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float x = 0.f, y = 0.f, z = 0.f;
    float scale_x = 1.f, scale_y = 1.f, scale_z = 1.f;
    float theta = 0.f;
    float axis_x = 0.f, axis_y = 1.f, axis_z = 0.f;

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    //glm::mat4 projectionMatrix = glm::ortho(
    //    -2.f, //left
    //    2.f, //right
    //    - 2.f, //bot
    //    2.f, //top
    //    -1.f, //z near
    //    1.f);  //z far
    
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f),//FOV
        window_height / window_width, //aspect ratio
        0.1f, //znear > 0
        100.f //zfar
    );

    z = -2.f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        x = x_mod;
        y = y_mod;
        z = z_mod;
        theta = theta_mod;
        scale_x = scale_mod;
        
        //start with translation matrix
        glm::mat4 transformation_matrix = glm::translate(identity_matrix,
            glm::vec3(x, y, z));

        //multiply the resulting matrix with the scale matrix
        transformation_matrix = glm::scale(transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z));

        //finally multiply it with the rotation matrix
        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        //getting the variable named transform from one of the shaders
        //attached to the shaderProg
        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
       
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

        glUniformMatrix4fv(transformLoc, //address of the transform variable
            1, //how many matrices to assign
            GL_FALSE,
            glm::value_ptr(transformation_matrix)); //poiner to the matrix

        //creating a 3d translation matrix
        //glm::mat4 translation = glm::translate(identity_matrix4, glm::vec3(x, y, z)); //move by x,y,z units from the center
        //glm::mat4 scale = glm::scale(identity_matrix4, glm::vec3(x, y, z));
        //glm::mat4 rotation = glm::rotate(identity_matrix4, glm::radians(theta), glm::vec3(x, y, z));


        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

     /*   glBegin(GL_POLYGON);      //pentagon
        glVertex2f(0.0f, 0.55f);
        glVertex2f(0.5f, 0.0f);
        glVertex2f(0.3f, -0.7f);
        glVertex2f(-0.3f, -0.7f);
        glVertex2f(-0.5f, 0.0f); 

        glEnd();
     */


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}