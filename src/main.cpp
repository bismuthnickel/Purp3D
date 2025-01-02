#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Vertex {
float x, y, z;
};

class Engine {
public:
Engine() {
window.create(sf::VideoMode(800, 600), "Purp3D");
window.setFramerateLimit(60);
cameraPos = {0.0f, 0.0f, -5.0f};
cameraRotation = {0.0f, 0.0f};
window.setMouseCursorVisible(false);
window.setMouseCursorGrabbed(true);
lastMousePos = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
}

void run() {
while (window.isOpen()) {
handleInput();
window.clear(sf::Color::Black);
std::vector<Vertex> vertices = {
{ -0.5f, -0.5f, 1.0f },
{ 0.5f, -0.5f, 1.0f },
{ 0.0f, 0.5f, 1.0f }
};
std::vector<sf::Vertex> projectedVertices;
for (const auto& vertex : vertices) {
Vertex transformedVertex = applyCameraTransform(vertex);
float scale = 1.0f / (transformedVertex.z - cameraPos.z);
float x2d = transformedVertex.x * scale;
float y2d = transformedVertex.y * scale;
x2d = x2d * 400 + 400;
y2d = -y2d * 300 + 300;
projectedVertices.push_back(sf::Vertex(sf::Vector2f(x2d, y2d), sf::Color::White));
}
window.draw(&projectedVertices[0], projectedVertices.size(), sf::Triangles);
window.display();
}
}

private:
sf::RenderWindow window;
sf::Vector3f cameraPos;
sf::Vector2f cameraRotation;
sf::Vector2i lastMousePos;

void handleInput() {
sf::Event event;
while (window.pollEvent(event)) {
if (event.type == sf::Event::Closed)
window.close();
if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
window.close();
}
sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
sf::Vector2i mouseDelta = currentMousePos - lastMousePos;
cameraRotation.y -= mouseDelta.x * 0.005f;
cameraRotation.x -= mouseDelta.y * 0.005f;
if (cameraRotation.x > 1.5f) cameraRotation.x = 1.5f;
if (cameraRotation.x < -1.5f) cameraRotation.x = -1.5f;
lastMousePos = currentMousePos;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
cameraPos.z += 0.1f;
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
cameraPos.z -= 0.1f;
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
cameraPos.x += 0.1f;
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
cameraPos.x -= 0.1f;
}
}

Vertex applyCameraTransform(const Vertex& vertex) {
float cosY = std::cos(cameraRotation.y);
float sinY = std::sin(cameraRotation.y);
float rotatedX = cosY * vertex.x + sinY * vertex.z;
float rotatedZ = -sinY * vertex.x + cosY * vertex.z;
float cosX = std::cos(cameraRotation.x);
float sinX = std::sin(cameraRotation.x);
float rotatedY = cosX * vertex.y - sinX * rotatedZ;
float finalZ = sinX * vertex.y + cosX * rotatedZ;
return { rotatedX - cameraPos.x, rotatedY - cameraPos.y, finalZ - cameraPos.z };
}
};

int main() {
Engine engine;
engine.run();
return 0;
}
