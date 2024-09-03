#include "mouse.h"
#include <algorithm>

Mouse* mouseRef;

void Mouse::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS)
		mouseRef->buttonsPressed.push_back(button);
	else if (action == GLFW_RELEASE)
		mouseRef->buttonsReleased.push_back(button);
}

void Mouse::positionCallback(GLFWwindow* window, double x, double y) {
	mouseRef->position.x = (unsigned) x;
	mouseRef->position.y = (unsigned) y;
}

void Mouse::wheelCallback(GLFWwindow* window, double x, double y) {
	mouseRef->dWheel = (float) y;
}

bool Mouse::isButtonPressed(int button) const {
	return std::find(buttonsPressed.begin(), buttonsPressed.end(), button) != buttonsPressed.end();
}

bool Mouse::isButtonReleased(int button) const {
	return std::find(buttonsReleased.begin(), buttonsReleased.end(), button) != buttonsReleased.end();
}

bool Mouse::isButtonDown(int button) const {
	return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Mouse::isButtonUp(int button) const {
	return glfwGetMouseButton(window, button) == GLFW_RELEASE;
}

const glm::uvec2& Mouse::getPosition() const {
	return position;
}

const glm::ivec2& Mouse::getDelta() const {
	return delta;
}

float Mouse::getDWheel() const {
	return dWheel;
}

void Mouse::update() {
	buttonsPressed.clear();
	buttonsReleased.clear();
	dWheel = 0;

	delta = position - previousPosition;
	previousPosition = position;
}

Mouse::Mouse(GLFWwindow* window)
	: window(window), previousPosition(0, 0), position(0, 0), delta(0, 0), dWheel(0) {

	mouseRef = this;

	glfwSetMouseButtonCallback(window, buttonCallback);
	glfwSetCursorPosCallback(window, positionCallback);
	glfwSetScrollCallback(window, wheelCallback);
}