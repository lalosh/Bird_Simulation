#pragma once
#include <iostream>
#include <stack>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MatrixStack
{
public:
	MatrixStack(void);
	~MatrixStack(void);
	
	const glm::mat4 &Top();

	void RotateX(float fAngDeg);

	void RotateY(float fAngDeg);

	void RotateZ(float fAngDeg);

	void RotateAxis(float theta , glm::vec3 inAxis);

	void Scale(const glm::vec3 &scaleVec);

	void Translate(const glm::vec3 &offsetVec);

	void Push();

	void Pop();

private:
	glm::mat4 m_currMat;
	std::stack<glm::mat4> m_matrices;
};

