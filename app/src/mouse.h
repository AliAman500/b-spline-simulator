#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window;

class Mouse {
	private:
		static void buttonCallback(GLFWwindow* window, int button, int action, int mods);
		static void positionCallback(GLFWwindow* window, double x, double y);
		static void wheelCallback(GLFWwindow* window, double x, double y);
	private:
		GLFWwindow* window;

		std::vector<int> buttonsPressed;
		std::vector<int> buttonsReleased;

		glm::uvec2 previousPosition;
		glm::uvec2 position;
		glm::ivec2 delta;

		float dWheel;
	public:
		bool isButtonPressed(int button) const;
		bool isButtonReleased(int button) const;
		bool isButtonDown(int button) const;
		bool isButtonUp(int button) const;

		const glm::uvec2& getPosition() const;
		const glm::ivec2& getDelta() const;

		float getDWheel() const;
		void update();
	public:
		Mouse(GLFWwindow* window);
};