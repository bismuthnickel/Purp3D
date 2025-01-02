#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

std::string readFile(const std::string& filepath) {
std::ifstream file(filepath);
if (!file.is_open()) {
std::cerr << "Failed to open file: " << filepath << "\n";
exit(EXIT_FAILURE);
}
std::stringstream buffer;
buffer << file.rdbuf();
return buffer.str();
}

GLuint createShaderProgram(const std::string& vertexFile, const std::string& fragmentFile) {
std::string vertexSource = readFile(vertexFile);
std::string fragmentSource = readFile(fragmentFile);
GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
const char* vertexSourceCStr = vertexSource.c_str();
glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
glCompileShader(vertexShader);
GLint success;
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if (!success) {
char infoLog[512];
glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << "\n";
}
GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
const char* fragmentSourceCStr = fragmentSource.c_str();
glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
glCompileShader(fragmentShader);
glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
if (!success) {
char infoLog[512];
glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << "\n";
}
GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success) {
char infoLog[512];
glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
std::cerr << "ERROR: Shader program linking failed\n" << infoLog << "\n";
}
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
return shaderProgram;
}

void generateCube(GLfloat* vertices, GLuint* indices) {
vertices[0] = -0.5f; vertices[1] = -0.5f; vertices[2] = -0.5f; vertices[3] = 1.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
vertices[6] = 0.5f; vertices[7] = -0.5f; vertices[8] = -0.5f; vertices[9] = 0.0f; vertices[10] = 1.0f; vertices[11] = 0.0f;
vertices[12] = 0.5f; vertices[13] = 0.5f; vertices[14] = -0.5f; vertices[15] = 0.0f; vertices[16] = 0.0f; vertices[17] = 1.0f;
vertices[18] = -0.5f; vertices[19] = 0.5f; vertices[20] = -0.5f; vertices[21] = 1.0f; vertices[22] = 1.0f; vertices[23] = 0.0f;
vertices[24] = -0.5f; vertices[25] = -0.5f; vertices[26] = 0.5f; vertices[27] = 1.0f; vertices[28] = 0.0f; vertices[29] = 1.0f;
vertices[30] = 0.5f; vertices[31] = -0.5f; vertices[32] = 0.5f; vertices[33] = 0.0f; vertices[34] = 1.0f; vertices[35] = 1.0f;
vertices[36] = 0.5f; vertices[37] = 0.5f; vertices[38] = 0.5f; vertices[39] = 0.0f; vertices[40] = 0.0f; vertices[41] = 1.0f;
vertices[42] = -0.5f; vertices[43] = 0.5f; vertices[44] = 0.5f; vertices[45] = 1.0f; vertices[46] = 1.0f; vertices[47] = 1.0f;

indices[0] = 0; indices[1] = 1; indices[2] = 2;
indices[3] = 0; indices[4] = 2; indices[5] = 3;
indices[6] = 4; indices[7] = 5; indices[8] = 6;
indices[9] = 4; indices[10] = 6; indices[11] = 7;
indices[12] = 0; indices[13] = 1; indices[14] = 5;
indices[15] = 0; indices[16] = 5; indices[17] = 4;
indices[18] = 1; indices[19] = 2; indices[20] = 6;
indices[21] = 1; indices[22] = 6; indices[23] = 5;
indices[24] = 2; indices[25] = 3; indices[26] = 7;
indices[27] = 2; indices[28] = 7; indices[29] = 6;
indices[30] = 3; indices[31] = 0; indices[32] = 4;
indices[33] = 3; indices[34] = 4; indices[35] = 7;
}

void renderCube(GLuint shaderProgram, GLfloat* vertices, GLuint* indices, float rotation) {
GLuint VAO, VBO, EBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f, 2.0f, -3.0f));

glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
}

GLFWwindow* initWindow(int width, int height, const char* title) {
if (!glfwInit()) {
std::cerr << "Failed to initialize GLFW\n";
exit(EXIT_FAILURE);
}
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
if (!window) {
std::cerr << "Failed to create GLFW window\n";
glfwTerminate();
exit(EXIT_FAILURE);
}
glfwMakeContextCurrent(window);
glfwSwapInterval(1);
if (glewInit() != GLEW_OK) {
std::cerr << "Failed to initialize GLEW\n";
exit(EXIT_FAILURE);
}
glEnable(GL_DEPTH_TEST);
return window;
}

int main() {
GLFWwindow* window = initWindow(800, 600, "Purp3D");
GLuint shaderProgram = createShaderProgram("shaders/vert.glsl", "shaders/frag.glsl");

GLfloat vertices[48];
GLuint indices[36];
generateCube(vertices, indices);
float rotation = 0;

while (!glfwWindowShouldClose(window)) {
glfwPollEvents();
renderCube(shaderProgram, vertices, indices, rotation);
glfwSwapBuffers(window);
rotation += 0.1f;
}
glfwTerminate();
return 0;
}
