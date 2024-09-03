#pragma once

#include <stdlib.h>
#include <glm/glm.hpp>

extern float stepSize;

std::vector<glm::vec2> splitLine(const glm::vec2& p1, const glm::vec2& p2, int numDivisions);
void plotPoint(const glm::vec2& p, const glm::vec3& color);
void calculateMidpoints(const glm::vec2& p1, const glm::vec2& p2, std::vector<glm::vec2>& splinePoints);
void bezier(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, bool debug);
void bSpline(const std::vector<glm::vec2>& points, bool debug);
bool isColliding(const glm::vec2& p1, const glm::vec2& p2);
void scalePoints(std::vector<glm::vec2>& points, const glm::vec2& pivot, float scaleFactor);