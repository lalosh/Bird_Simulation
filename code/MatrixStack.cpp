#include "MatrixStack.h"

inline float DegToRad_(float fAngDeg){
	const float fDegToRad_ = 3.14159f * 2.0f / 360.0f;
	return fAngDeg * fDegToRad_;
}

glm::mat3 RotateAxis(float theta , glm::vec3 inAxis){

	float fAngRad = DegToRad_(theta);

	float fCos = cosf(fAngRad);
	float fInvCos = 1.0f - fCos;
	float fSin = sinf(fAngRad);
	float fInvSin = 1.0f - fSin;

	glm::vec3 axis = inAxis;
    axis = glm::normalize(axis);

	glm::mat3 theMat(1.0f);
	theMat[0].x = (axis.x * axis.x) + ((1 - axis.x * axis.x) * fCos);
	theMat[1].x = axis.x * axis.y * (fInvCos) - (axis.z * fSin);
	theMat[2].x = axis.x * axis.z * (fInvCos) + (axis.y * fSin);

	theMat[0].y = axis.x * axis.y * (fInvCos) + (axis.z * fSin);
	theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
	theMat[2].y = axis.y * axis.z * (fInvCos) - (axis.x * fSin);

	theMat[0].z = axis.x * axis.z * (fInvCos) - (axis.y * fSin);
	theMat[1].z = axis.y * axis.z * (fInvCos) + (axis.x * fSin);
	theMat[2].z = (axis.z * axis.z) + ((1 - axis.z * axis.z) * fCos);
	return theMat;
}

glm::mat3 RotateX(float fAngDeg){
	
	float fAngRad = DegToRad_(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[1].y = fCos; theMat[2].y = -fSin;
	theMat[1].z = fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateY(float fAngDeg){

	float fAngRad = DegToRad_(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[2].x = fSin;
	theMat[0].z = -fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateZ(float fAngDeg){
	
	float fAngRad = DegToRad_(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[1].x = -fSin;
	theMat[0].y = fSin; theMat[1].y = fCos;
	return theMat;
}

const glm::mat4& MatrixStack::Top(){
		return m_currMat;
}

void MatrixStack::RotateX(float fAngDeg){
		m_currMat = m_currMat * glm::mat4(::RotateX(fAngDeg));
	}

void MatrixStack::RotateY(float fAngDeg){
		m_currMat = m_currMat * glm::mat4(::RotateY(fAngDeg));
	}

void MatrixStack::RotateZ(float fAngDeg){
		m_currMat = m_currMat * glm::mat4(::RotateZ(fAngDeg));
	}

void MatrixStack::RotateAxis(float theta , glm::vec3 inAxis){
		m_currMat = m_currMat * glm::mat4(::RotateAxis(theta,inAxis));
	}

void MatrixStack::Scale(const glm::vec3 &scaleVec){
		glm::mat4 scaleMat(1.0f);
		scaleMat[0].x = scaleVec.x;
		scaleMat[1].y = scaleVec.y;
		scaleMat[2].z = scaleVec.z;

		m_currMat = m_currMat * scaleMat;
	}

void MatrixStack::Translate(const glm::vec3 &offsetVec){
		glm::mat4 translateMat(1.0f);
		translateMat[3] = glm::vec4(offsetVec, 1.0f);

		m_currMat = m_currMat * translateMat;
	}

void MatrixStack::Push(){
		m_matrices.push(m_currMat);
	}

void MatrixStack::Pop(){
		m_currMat = m_matrices.top();
		m_matrices.pop();
	}

MatrixStack::MatrixStack(void):m_currMat(1.0f){
}

MatrixStack::~MatrixStack(void){
}