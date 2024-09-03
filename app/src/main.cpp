#include <cstdio>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "mouse.h"
#include "curves.h"

bool debug = false;

int main(int argc, char **argv) {
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	unsigned int width = 720;
	unsigned int height = 530;

	GLFWwindow *window = glfwCreateWindow(width, height, "project", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	Mouse mouse(window);

	std::vector<glm::vec2> controlPoints;
	glm::vec2* currentControlPoint;
	bool controlPointSelected = false;
	float scaleFactor = 0;

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_D && action == GLFW_PRESS) debug = !debug; 
	});

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0.0, 25.0f/255.0f, 25.0f/255.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		if(controlPoints.size() > 3) bSpline(controlPoints, debug);
		glPointSize(8);
		for(int i = 0; i < controlPoints.size(); i++) {
			glm::vec3 color(0, 1, 0);

			if(controlPointSelected && controlPoints[i] == *currentControlPoint) {
				color = glm::vec3(0.5, 0, 0.5);
			} else {
				if(isColliding(mouse.getPosition(), controlPoints[i])) color = glm::vec3(1, 0, 1);
				else color = glm::vec3(0, 1, 0);
			}

			plotPoint(controlPoints[i], color);
		}

		for(int i = 0; i < controlPoints.size(); i++) {
			if(isColliding(mouse.getPosition(), controlPoints[i]) && !controlPointSelected && mouse.isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
				currentControlPoint = &controlPoints[i];
				controlPointSelected = true;
				break;
			}
		}

		if(controlPointSelected) {
			currentControlPoint->x += mouse.getDelta().x;
			currentControlPoint->y += mouse.getDelta().y;
			if(mouse.isButtonReleased(GLFW_MOUSE_BUTTON_1)) {
				currentControlPoint = nullptr;
				controlPointSelected = false;
			}
		}

		if(mouse.isButtonPressed(GLFW_MOUSE_BUTTON_1) && !controlPointSelected) controlPoints.push_back(glm::vec2(mouse.getPosition()));

		if(mouse.isButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) && !controlPointSelected) {
			for(int i = 0; i < controlPoints.size(); i++) {
				controlPoints[i].x += mouse.getDelta().x;
				controlPoints[i].y += mouse.getDelta().y;
			}
		}

		if(mouse.isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
			if(controlPoints.size() > 0) controlPoints.pop_back();

		scaleFactor += ((mouse.getDWheel() * 0.1f) - scaleFactor) * 0.1f;

		scalePoints(controlPoints, glm::vec2(width/2.0f, height/2.0f), 1 + scaleFactor);
		
		glfwSwapBuffers(window);
		mouse.update();
	}

	glfwTerminate();
	return 0;
}
