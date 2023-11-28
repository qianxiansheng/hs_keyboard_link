
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include "model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0, 0, 3.0f));
glm::vec2 mouse_pos(0.0f, 0.0f);

float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

static glm::vec3 GetIntersectWithLineAndPlane(glm::vec3 point, glm::vec3 direct, glm::vec3 planeNormal, glm::vec3 planePoint)
{
    float d = glm::dot(planePoint - point, planeNormal) / glm::dot(direct, planeNormal);
    return d * direct + point;
}

bool isPointInsideTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P) {
    // Calculate vectors
    glm::vec3 AB = B - A;
    glm::vec3 BC = C - B;
    glm::vec3 CA = A - C;

    glm::vec3 AP = P - A;
    glm::vec3 BP = P - B;
    glm::vec3 CP = P - C;

    // Calculate cross products
    glm::vec3 cross_AB_AP = glm::cross(AB, AP);
    glm::vec3 cross_BC_BP = glm::cross(BC, BP);
    glm::vec3 cross_CA_CP = glm::cross(CA, CP);

    // Check if the vectors have the same direction
    if (glm::dot(cross_AB_AP, cross_BC_BP) >= 0 &&
        glm::dot(cross_BC_BP, cross_CA_CP) >= 0 &&
        glm::dot(cross_CA_CP, cross_AB_AP) >= 0) {
        return true;
    }
    else {
        return false;
    }
}

bool wmouse_in_quad(glm::vec3 mouse_pos, glm::vec3 camera_pos, float* quad)
{
    glm::vec3 dir = glm::normalize(mouse_pos - camera_pos);

    bool intri1 = false;
    bool intri2 = false;
    {
        glm::vec4 p1(quad[0], quad[1], quad[2], 1.0f);
        glm::vec4 p2(quad[3], quad[4], quad[5], 1.0f);
        glm::vec4 p3(quad[6], quad[7], quad[8], 1.0f);

        glm::vec3 d1 = p1 - p2;
        glm::vec3 d2 = p3 - p2;
        glm::vec3 n = glm::cross(d1, d2);
        n = glm::normalize(n);

        glm::vec3 a = GetIntersectWithLineAndPlane(camera_pos, dir, n, p1);

        intri1 = isPointInsideTriangle(p1, p2, p3, a);
    }
    {
        glm::vec4 p1(quad[6], quad[7] , quad[8] , 1.0f);
        glm::vec4 p2(quad[9], quad[10], quad[11], 1.0f);
        glm::vec4 p3(quad[0], quad[1] , quad[2] , 1.0f);

        glm::vec3 d1 = p1 - p2;
        glm::vec3 d2 = p3 - p2;
        glm::vec3 n = glm::cross(d1, d2);
        n = glm::normalize(n);

        glm::vec3 a = GetIntersectWithLineAndPlane(camera_pos, dir, n, p1);

        intri2 = isPointInsideTriangle(p1, p2, p3, a);
    }

    return intri1 || intri2;
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader shader("3.2.blending.vs", "3.2.blending.fs");

    RenderModelInit();

    std::vector<float> vertices = RenderModelUpdate();

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // shader configuration
    // --------------------
    shader.use();

    glViewport(00, 000, 800, 600);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        shader.use();
        //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f)); // a smaller cube
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 modelViewMatrix = view * model;

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        glm::vec3 mousePos_NDC(mouse_pos.x, SCR_HEIGHT - mouse_pos.y, 0.1f);
        glm::vec3 mousePos_3DC = glm::unProject(
            mousePos_NDC,
            modelViewMatrix,
            projection,
            glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3])
        );

        for (int i = 0; i < keyBtnViewSize; ++i)
        {
            if (wmouse_in_quad(mousePos_3DC, camera.Position, keyBtnViewList[i].face_vertex))
            {
                keyBtnViewList[i].hover = true;
            }
            else
            {
                keyBtnViewList[i].hover = false;
            }
        }

        // render the cube
        std::vector<float> vertices = RenderModelUpdate();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 10);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    mouse_pos.x = xposIn;
    mouse_pos.y = yposIn;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}