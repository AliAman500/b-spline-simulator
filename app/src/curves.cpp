#include "curves.h"
#include "GLFW/glfw3.h"

float stepSize = 0.01f;

std::vector<glm::vec2> splitLine(const glm::vec2& p1, const glm::vec2& p2, int numDivisions) {
    std::vector<glm::vec2> divisionPoints;

    glm::vec2 step = (p2 - p1) / static_cast<float>(numDivisions);

    for (int i = 1; i <= numDivisions - 1; ++i) {
        glm::vec2 divisionPoint = p1 + static_cast<float>(i) * step;
		divisionPoints.push_back(divisionPoint);
    }

	return divisionPoints;
}

void plotPoint(const glm::vec2& p, const glm::vec3& color) {
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x, p.y);
	glEnd();
}

void calculateMidpoints(const glm::vec2& p1, const glm::vec2& p2, std::vector<glm::vec2>& splinePoints, bool debug) {
	std::vector<glm::vec2> b = splitLine(p1, p2, 3);
	splinePoints.insert(splinePoints.end(), b.begin(), b.end());
	
	glPointSize(4);
    for (int i = 0; i < splinePoints.size(); i++)
        if(debug) plotPoint(splinePoints[i], glm::vec3(1.0, 0.65, 0));
}

void bezier(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, bool debug) {
	glLineWidth(2);

	if(debug) {
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0.6, 0.6);
		glVertex2fv(&p1.x);
		glVertex2fv(&p2.x);
		glVertex2fv(&p3.x);
		glVertex2fv(&p4.x);
		glEnd();
	}

	glBegin(GL_LINE_STRIP);
	glColor3f(1.8f, 0.2f, 1.0f);
	for (float t = 0.0; t <= 1.0; t += stepSize) {
		float x = pow((1 - t), 3) * p1.x + 3 * pow((1 - t), 2) * t * p2.x + 3 * (1 - t) * pow(t, 2) * p3.x + pow(t, 3) * p4.x;
		float y = pow((1 - t), 3) * p1.y + 3 * pow((1 - t), 2) * t * p2.y + 3 * (1 - t) * pow(t, 2) * p3.y + pow(t, 3) * p4.y;
		glVertex2f(x, y);
	}
	glEnd();
	if(debug) {
		plotPoint(p1, glm::vec3(0, 1, 1));
		plotPoint(p2, glm::vec3(0, 1, 1));
		plotPoint(p3, glm::vec3(0, 1, 1));
		plotPoint(p4, glm::vec3(0, 1, 1));
	}
}

void bSpline(const std::vector<glm::vec2>& points, bool debug) {
	std::vector<glm::vec2> splinePoints;
	std::vector<glm::vec2> bezierPoints;

	if(debug) {
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0.5f, 0);
		for(int i = 0; i < points.size(); i++) {
			glVertex2fv(&points[i].x);
		}
		glEnd();
	}
	for (size_t i = 0; i < points.size() - 1; i++) {
        const glm::vec2& p1 = points[i];
        const glm::vec2& p2 = points[i + 1];
		calculateMidpoints(p1, p2, splinePoints, debug);
    }

	for(int i = 1; i < splinePoints.size() - 2; i += 2) {
		glm::vec2 bezierPoint = splitLine(splinePoints[i], splinePoints[i+1], 2)[0];
		bezierPoints.push_back(bezierPoint);
		bezierPoints.push_back(splinePoints[i+1]);
		bezierPoints.push_back(splinePoints[i+2]);
	}

	bezierPoints.pop_back();
	bezierPoints.pop_back();

	for(int i = 0; i < bezierPoints.size(); i++) {
		if(debug) plotPoint(bezierPoints[i], glm::vec3(1, 1, 1));
	}

	for(int i = 0; i < bezierPoints.size() - 3; i += 3) {
		bezier(bezierPoints[i], bezierPoints[i+1], bezierPoints[i+2], bezierPoints[i+3], debug);
	}
}

bool isColliding(const glm::vec2& p1, const glm::vec2& p2) {
    float radius = 3.0f;
    float distanceSquared = glm::distance(p1, p2);

    return distanceSquared <= (radius * radius);
}

void scalePoints(std::vector<glm::vec2>& points, const glm::vec2& pivot, float scaleFactor) {
    for (auto& point : points) {
        glm::vec2 vectorToPoint = point - pivot;
        float distance = glm::length(vectorToPoint);
        float scaledDistance = distance * scaleFactor;
        glm::vec2 newPoint = pivot + (scaledDistance / distance) * vectorToPoint;
        point = newPoint;
    }
}
