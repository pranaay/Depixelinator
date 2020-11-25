#include "utils.h"
#include "Image.h"
#include "Pixels.h"
#include<math.h>
#include<stdio.h>
#include <unistd.h>

// GLobal variables
int width = 1060, height = 1060;


int main(int, char *argv[]) {
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO &io = ImGui::GetIO(); // Create IO object

    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);

    Image pepe = Image("./PixelArtImages/smw2_yoshi_01_input.png", 0);
    std::cout << "height : " << pepe.getHeight() << " width : " << pepe.getWidth() << std::endl;
    glBindTexture(GL_TEXTURE_2D, pepe.getTexId());

    // Create VBOs, VAOs
    const GLfloat vertexdata[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f,  1.0f, 0.0f,
            -0.5f, 0.5f,  0.0f, 1.0f,
            0.5f, 0.5f,  1.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            1, 2, 3
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), indices, GL_STATIC_DRAW);
    unsigned int graphPoints;
    glGenBuffers(1, &graphPoints);
    glBindBuffer(GL_ARRAY_BUFFER, graphPoints);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertexdata, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *) 0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *) 2);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    int button_status = 0;
    //Display loop
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        ImGui::Render();

        // Add a new point on mouse click
        float x, y;
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);

        int location =  glGetUniformLocation(shaderProgram,"u_Texture");
        glUniform1i(location, 0);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

        glUseProgram(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }

    // Delete VBO buffers

    // Cleanup
    cleanup(window);
    return 0;
}
