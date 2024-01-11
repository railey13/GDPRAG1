#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Nikos Railey Bumanglag", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
   
        //glBegin(GL_QUADS);          //square
        //glVertex2f(-0.5f, -0.5f);
        //glVertex2f(-0.5f, 0.5f);
        //glVertex2f(0.5f, 0.5f);
        //glVertex2f(0.5f, -0.5f);


        glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.55f);
        glVertex2f(0.5f, 0.0f);
        glVertex2f(0.3f, -0.7f);
        glVertex2f(-0.3f, -0.7f);
        glVertex2f(-0.5f, 0.0f);
            
        glEnd();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}