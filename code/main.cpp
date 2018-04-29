#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexData.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Environment.h"
#include "Power.h"
#include "Bird_physic.h"

using namespace std;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

const int WIDTH  = 1280;
const int HEIGHT = 960;

const int numberOfVertices_leftWing   = 214;
const int numberOfVertices_rightWing  = 214;
const int numberOfVertices_body       = 155;
const int numberOfVertices_head       = 127;
const int numberOfVertices_tail       = 197;
const int numberOfVertices_claws      = 127;
const int numberOfVertices_new		  =  8;

#define MULT 100

GLuint  theProgram;

GLuint  vao_leftWing,					
		vao_rightWing,					
		vao_body,					
		vao_head,					
		vao_tail,
		vao_claws,
		vao_new;

GLuint  vertexBufferObject_leftWing,
		vertexBufferObject_rightWing,
		vertexBufferObject_body,
		vertexBufferObject_head,
		vertexBufferObject_tail,
		vertexBufferObject_claws,
		vertexBufferObject_new;

GLuint  indexBufferObject_leftWing,
		indexBufferObject_rightWing,
		indexBufferObject_body,
		indexBufferObject_head,
		indexBufferObject_tail,
		indexBufferObject_claws,
		indexBufferObject_new;

GLuint modelToCameraMatrixUnif;

GLuint cameraToClipMatrixUnif;
glm::mat4 cameraToClipMatrix(0.0f);

GLuint translateUnif;
glm::mat4 translate(1.0f);

const std::string strVertexShader(
		"#version 330\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"smooth out vec4 theColor;\n"
		"uniform mat4 cameraToClipMatrix;\n"
		"uniform mat4 modelToCameraMatrix;\n"
		"uniform mat4 translate;\n"
		"void main()\n"
		"{\n"
			"vec4 cameraPos = modelToCameraMatrix * position;\n"
			"gl_Position = (   cameraToClipMatrix *translate ) * cameraPos;\n"
			"theColor = color;\n"
		"}\n"
);

const std::string strFragmentShader(
		"#version 330\n"
		"smooth in vec4 theColor;\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
			"outputColor = theColor;\n"
		"}\n"
);

inline float DegToRad(float fAngDeg){
	const float fDegToRad = 3.14159f * 2.0f / 360.0f;
	return fAngDeg * fDegToRad;
}

 float RadToDeg(float fAngDeg){
	fAngDeg = fAngDeg *  360.0f  ;
	fAngDeg = fAngDeg / 6.28318f ;
	return fAngDeg;
}

inline double Clamp(double fValue, double fMinValue, double fMaxValue){

	if(fValue < fMinValue)
		return fMinValue;

	if(fValue > fMaxValue)
		return fMaxValue;

	return fValue;
}

	float glidThetaR(0.0f);
	float glidThetaL(0.0f);

Bird_physic BaldEagle_physic;

class Bird_openGL
{
public:
	#define STANDARD_ANGLE_INCREMENT 0.5f
	#define SMALL_ANGLE_INCREMENT 0.5f
	#define FAST_ANGEL_INCREMENT 3.0
	#define WING_FREQ BaldEagle_physic.wingbeatfrq

	Bird_openGL()
		:posBody(glm::vec3(0.0f, 0.0f, 0.0f))
		,rotateBodyY(0.0f)
		,rotateBodyX(0.0f)
		,rotateBodyZ(0.0f)
		,angelRightWing(0.0f)
		,angelLeftWing(0.0f)
		,bodyX(0)
		,bodyY(0)
		,bodyZ(0)
		,zValue(0)
		,temp(0)
	{}
	
	void Draw()
	{
		MatrixStack modelToCameraStack;
		
		//#ground
		modelToCameraStack.Push();
		glUseProgram(theProgram);
		glBindVertexArray(vao_new);		
		modelToCameraStack.Translate(glm::vec3(0, -20 , -60));
		modelToCameraStack.Scale(glm::vec3(800,10,800));
		glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_new), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		modelToCameraStack.Pop();
		//#end ground
		
		//#Bird
		modelToCameraStack.Push();
		glUseProgram(theProgram);
								
			modelToCameraStack.Translate(glm::vec3(0, 0 , -60));
			modelToCameraStack.Translate(glm::vec3(bodyX, bodyY , bodyZ));

			modelToCameraStack.RotateX(rotateBodyX);
			modelToCameraStack.RotateY(rotateBodyY);
			modelToCameraStack.RotateZ(rotateBodyZ);
		
		modelToCameraStack.RotateY(-180);

		DrawBody(modelToCameraStack);
		DrawHead(modelToCameraStack);
		DrawTail(modelToCameraStack);
		DrawClaws(modelToCameraStack);

			modelToCameraStack.Push();
						modelToCameraStack.Translate(glm::vec3(-5.50,0,0)); // those numbers are from what you've edit in 3DMax
						DrawRightWing(modelToCameraStack);
			modelToCameraStack.Pop();

			modelToCameraStack.Push();
						modelToCameraStack.Translate(glm::vec3(5.50,0,0));  // those numbers are from what you've edit in 3DMax
						DrawLeftWing(modelToCameraStack);
			modelToCameraStack.Pop();


		float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
				if(fmod(currentTime,1.0)==0.0){
						WritePose();
				}

		//update values		
				BaldEagle_physic.UpdateShape();
				BaldEagle_physic.UpdateEnvironment();
				BaldEagle_physic.UpdatePower();

		//adjust values
				bodyY = MULT*  BaldEagle_physic.yRes;
				bodyZ = MULT* -BaldEagle_physic.xRes;
				rotateBodyX = BaldEagle_physic.angelOfAttack;

		glUseProgram(0);
		modelToCameraStack.Pop();
		//#end Bird
	}

	void frontalCamera(bool ok){
		if(ok){
			rotateBodyY = 180;
		}
		else rotateBodyY=0;
	}

	void glidRight(){

		glidThetaR++;
		glidThetaR = fmod(glidThetaR,360);
		bodyX += -(cos(DegToRad(glidThetaR))-1.0); 
		bodyZ += -1*sin(DegToRad(glidThetaR));
	}

	void glidLeft(){

		glidThetaL++;
		glidThetaL = fmod(glidThetaL,360);
		bodyX += (cos(DegToRad(glidThetaL))-1.0); 
		bodyZ += -1*sin(DegToRad(glidThetaL));
	}

	void oneQuarter(float &theta){
		if(theta>90 && theta<=180)
			theta = 90 - theta;
		if(theta>180 && theta<=270)
			theta = 180 - theta;
		if(theta>270 && theta <=360)
			theta = 270 - theta;

	}

	void AdjBodyRotateX(bool bIncrement){
		rotateBodyX += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		rotateBodyX = fmodf(rotateBodyX, 360.0f);
		rotateBodyX = Clamp (rotateBodyX , -10.0f , +10.0f) ;
	}

	void AdjBodyRotateY(bool bIncrement){
		rotateBodyY += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		rotateBodyY = fmodf(rotateBodyY, 360.0f);
	}

	void AdjBodyRotateY(float t){
		rotateBodyY += t;
		rotateBodyY = fmodf(rotateBodyY, 360.0f);
	}

	void AdjBodyRotateZ(bool bIncrement){
		rotateBodyZ += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		rotateBodyZ = fmodf(rotateBodyZ, 360.0f);
		if(rotateBodyZ<0)
			rotateBodyZ = - Clamp(fabs(rotateBodyZ), 0.0f, +20.0f);
		else
			rotateBodyZ =   Clamp(rotateBodyZ, 0.0f, +20.0f);

	}
	
	void AdjBodyRotateZBack(){
		if(rotateBodyZ > 0){
			rotateBodyZ-=0.01;
		}
		if(rotateBodyZ < 0){
			rotateBodyZ+=0.01;
		}
	
	}
	
	void AdjRightWingRotateZ(bool bIncrement){
		angelRightWing += bIncrement ? WING_FREQ*5 : -WING_FREQ*5;
		angelRightWing = Clamp(angelRightWing,-30.0f, +30.0f);
	}

	void AdjLeftWingRotateZ(bool bIncrement){
		angelLeftWing += bIncrement ? WING_FREQ*5 : -WING_FREQ*5;
		angelLeftWing = Clamp(angelLeftWing, -30.0f, +30.0f);
	}

	void DownStrockRight(bool &flag){

		if(angelRightWing < +30.0 && !flag )
				AdjRightWingRotateZ(true);
		
		else { 
				flag=true;
				if(angelRightWing > -30.0 )
						AdjRightWingRotateZ(false); 
				else
						flag=false;
			}  // first else
	} // Down strock Right function

	void DownStrockLeft(bool &flag){

		if(angelLeftWing > -30.0 && !flag )
				AdjLeftWingRotateZ(false); 
		
		else {  flag=true;
				if(angelLeftWing < +30.0 )
						AdjLeftWingRotateZ(true); 
				else
						flag=false;
			} // first else
	}  // Down strock Left function
	
	void AdjBodyX(bool bIncrement){
		bodyX += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;	
	}

	void AdjBodyY(bool bIncrement){
		bodyY += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;	
	}

	void AdjBodyY(float y){
		bodyY += y;
	}

	void AdjBodyZ(bool bIncrement){
		bodyZ += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;	
	}

	void AdjBodyX(float x){
		bodyX+=x;
	}

	void AdjBodyZ(float z){
		bodyZ+=z;
	}

	float getRotateBodyX(){
		return rotateBodyX;
	}

	float getAngelRightWing(){
		return angelRightWing;
	}

	float getAngelLeftWing(){
		return angelLeftWing;
	}
	
	void  fixWing(){
			if(angelRightWing>0)
				angelRightWing-=0.5;
			if(angelRightWing<0)
				angelRightWing+=0.5;

			if(angelLeftWing>0)
				angelLeftWing-=0.5;
			if(angelLeftWing<0)
				angelLeftWing+=0.5;
	}

	void updateCamera(){
					glUseProgram(theProgram);		

					translate[3].x = -bodyX  ;
					translate[3].y = -bodyY  ;
					translate[3].z = -bodyZ  ;

					glUniformMatrix4fv(translateUnif, 1, GL_FALSE, glm::value_ptr(translate));
					glUseProgram(0);
	}

	void WritePose(){	
				
		printf("\n \n \n \n \n");

		printf("\n-------------- Bird Info ---------------\n");

		printf("\nPositions:\n");
				printf("bodyX:\t%f\n", bodyX);
				printf("bodyY:\t%f\n", bodyY);
				printf("bodyZ:\t%f\n", bodyZ);
				
		printf("\nRotations:\n");
				printf("rotateBodyX:\t%f\n", rotateBodyX);
				printf("rotateBodyY:\t%f\n", rotateBodyY);
				printf("rotateBodyZ:\t%f\n", rotateBodyZ);

		//printf("\nCamera Positions:\n");
			//	printf("camera pos x = %f\n",cameraToClipMatrix[3].x);
			//  printf("camera pos y = %f\n",cameraToClipMatrix[3].y);
			//	printf("camera pos z = %f\n",cameraToClipMatrix[3].z);
		BaldEagle_physic.printEnvironment();
		BaldEagle_physic.printPower();
		
		printf("\n \n \n \n \n \n \n \n");

		printf("-----------------------------------------------------\n");
	}
	
private:

	void DrawBody(MatrixStack &modelToCameraStack){
			
			//Draw body
			glBindVertexArray(vao_body);

			modelToCameraStack.Push();
			modelToCameraStack.Scale(glm::vec3(10,10,10));

			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_body), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
			
			glBindVertexArray(0);

		}

	void DrawRightWing(MatrixStack &modelToCameraStack){
		
		glBindVertexArray(vao_rightWing);

		modelToCameraStack.Push();
		modelToCameraStack.RotateZ(angelRightWing);
		
		{
			modelToCameraStack.Push();
			modelToCameraStack.Scale(glm::vec3(10,10,10));
			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_rightWing), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		modelToCameraStack.Pop();
		glBindVertexArray(0);

	}
	
	void DrawLeftWing(MatrixStack &modelToCameraStack){
		
		glBindVertexArray(vao_leftWing);

		modelToCameraStack.Push();
		modelToCameraStack.RotateZ(angelLeftWing);
		
		{
			modelToCameraStack.Push();
			modelToCameraStack.Scale(glm::vec3(10,10,10));
			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_leftWing), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}
		modelToCameraStack.Pop();
		glBindVertexArray(0);

	}
	
	void DrawHead(MatrixStack &modelToCameraStack){
		
		glBindVertexArray(vao_head);

		modelToCameraStack.Push();
		
		{
			modelToCameraStack.Push();
			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_head), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		modelToCameraStack.Pop();
		glBindVertexArray(0);

	}
	
	void DrawTail(MatrixStack &modelToCameraStack){
		
		glBindVertexArray(vao_tail);

		modelToCameraStack.Push();
		
		{
			modelToCameraStack.Push();
			modelToCameraStack.Scale(glm::vec3(10,10,10));
			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_tail), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		modelToCameraStack.Pop();
		glBindVertexArray(0);

	}
	
	void DrawClaws(MatrixStack &modelToCameraStack){
		
		glBindVertexArray(vao_claws);

		modelToCameraStack.Push();
		
		{
			modelToCameraStack.Push();
			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData_claws), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		modelToCameraStack.Pop();
		
		glBindVertexArray(0);

	}

	glm::vec3		posBody;
	float			rotateBodyY;
	float			rotateBodyX;
	float			rotateBodyZ;
	float			bodyY;
	float			bodyX;
	float			bodyZ;
	float			angelRightWing;
	float			angelLeftWing;
	float			glidTheta;
	float           zValue;
	float			temp;
};

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile){
	
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList){
	
	GLuint program = glCreateProgram();

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

float CalcFrustumScale(float fFovDeg){
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}

const float fFrustumScale = CalcFrustumScale(45.0f);

float mFar=0,mNear=0;

void InitializeProgram(){
	
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = CreateProgram(shaderList);

	modelToCameraMatrixUnif = glGetUniformLocation(theProgram, "modelToCameraMatrix");
	cameraToClipMatrixUnif = glGetUniformLocation(theProgram, "cameraToClipMatrix");
	translateUnif = glGetUniformLocation(theProgram, "translate");

	float fzNear = 1.0f ; float fzFar = 1000.0f;

	cameraToClipMatrix[0].x = fFrustumScale ;
	cameraToClipMatrix[1].y = fFrustumScale;
	cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
	cameraToClipMatrix[2].w = -1.0f;
	cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar); 

	glUseProgram(theProgram);
	glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
	glUniformMatrix4fv(translateUnif, 1, GL_FALSE, glm::value_ptr(translate));
	glUseProgram(0);
}

void InitializeVertexBuffer(){
	//new
	glGenBuffers(1, &vertexBufferObject_new);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_new);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_new), vertexData_new, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_new);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_new);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_new), indexData_new, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// left wing
	glGenBuffers(1, &vertexBufferObject_leftWing);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_leftWing);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_leftWing), vertexData_leftWing, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_leftWing);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_leftWing);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_leftWing), indexData_leftWing, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// right wing
	glGenBuffers(1, &vertexBufferObject_rightWing);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_rightWing);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_rightWing), vertexData_rightWing, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_rightWing);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_rightWing);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_rightWing), indexData_rightWing, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// body
	glGenBuffers(1, &vertexBufferObject_body);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_body);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_body), vertexData_body, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_body);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_body);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_body), indexData_body, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// head
	glGenBuffers(1, &vertexBufferObject_head);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_head);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_head), vertexData_head, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_head);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_head);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_head), indexData_head, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// tail
	glGenBuffers(1, &vertexBufferObject_tail);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_tail);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_tail), vertexData_tail, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_tail);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_tail);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_tail), indexData_tail, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// claws
	glGenBuffers(1, &vertexBufferObject_claws);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_claws);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData_claws), vertexData_claws, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject_claws);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_claws);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData_claws), indexData_claws, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init(){
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	InitializeProgram();
	InitializeVertexBuffer();
	size_t colorDataOffset;

	//vertex buffer (vao) left wing
	glGenVertexArrays(1, &vao_leftWing);
	glBindVertexArray(vao_leftWing);

	colorDataOffset = sizeof(float) * 3 * numberOfVertices_leftWing;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_leftWing);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_leftWing);
	glBindVertexArray(0);

	//vertex buffer (vao) right wing
	glGenVertexArrays(1, &vao_rightWing);
	glBindVertexArray(vao_rightWing);

	colorDataOffset = sizeof(float) * 3 * numberOfVertices_rightWing;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_rightWing);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_rightWing);

	glBindVertexArray(0);

	//vertex buffer (vao) body
	glGenVertexArrays(1, &vao_body);
	glBindVertexArray(vao_body);

	colorDataOffset = sizeof(float) * 3 * numberOfVertices_body;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_body);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_body);

	glBindVertexArray(0);

	//vertex buffer (vao) head
	glGenVertexArrays(1, &vao_head);
	glBindVertexArray(vao_head);

	colorDataOffset = sizeof(float) * 3 * numberOfVertices_head;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_head);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_head);

	glBindVertexArray(0);

	//vertex buffer (vao) tail
	glGenVertexArrays(1, &vao_tail);
	glBindVertexArray(vao_tail);

	colorDataOffset = sizeof(float) * 3 * numberOfVertices_tail;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_tail);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_tail);

	glBindVertexArray(0);

	//vertex buffer (vao) claws
	glGenVertexArrays(1, &vao_claws);
	glBindVertexArray(vao_claws);

	colorDataOffset = sizeof(float) * 3 * numberOfVertices_claws;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_claws);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_claws);

	glBindVertexArray(0);
	
	//new
	glGenVertexArrays(1, &vao_new);
	glBindVertexArray(vao_new);

	size_t colorDataOffset1 = sizeof(float) * 3 * numberOfVertices_new;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_new);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_new);

	glBindVertexArray(0);
	//finish working with vertex Buffer binding

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);	//cull the back face
	//glFrontFace(GL_CW);		//clock wise winding order
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

bool flagR=false,flagL=false;
Bird_openGL BaldEagle_openGL;

void display(){
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(theProgram);
	
	BaldEagle_openGL.Draw();
	
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape (int w, int h){

	cameraToClipMatrix[0].x = fFrustumScale * (h / (float)w);
	cameraToClipMatrix[1].y = fFrustumScale;

	glUseProgram(theProgram);
	glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
	glUseProgram(0);

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y){
	
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		return;

	// body Rotations
	case 'g': BaldEagle_openGL.AdjBodyRotateY(true); break;
	case 'h': BaldEagle_openGL.AdjBodyRotateY(false); break;
	case 'j': BaldEagle_openGL.AdjBodyRotateX(true);  break;
	case 'k': BaldEagle_openGL.AdjBodyRotateX(false); break;
	case 'o': BaldEagle_openGL.AdjBodyRotateZ(true); break;
	case 'p': BaldEagle_openGL.AdjBodyRotateZ(false); break;
	
	case 'c':{BaldEagle_openGL.DownStrockRight(flagR); BaldEagle_openGL.DownStrockLeft(flagL); break;}

	// body positions
	case 'w': BaldEagle_openGL.AdjBodyY(true); break;
	case 's': BaldEagle_openGL.AdjBodyY(false); break;
	case 'd': BaldEagle_openGL.AdjBodyX(true); break;
	case 'a': BaldEagle_openGL.AdjBodyX(false); break;
	
	case 'q': BaldEagle_openGL.AdjBodyZ(true); break;		  
	case 'e':BaldEagle_openGL.AdjBodyZ(false);break;
	
	case 'x': BaldEagle_openGL.updateCamera();
			 
	case 32: BaldEagle_openGL.WritePose(); break;
	}
}

bool    myFlag=true;
float   StationTime=0;

/*
void joystick(unsigned int buttonmask, int x, int y, int z){
	
   if (buttonmask & 0x0100) glutLeaveMainLoop();
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x01) { 
	 
	   BaldEagle_openGL.AdjBodyRotateX(true);
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x04) {

	   BaldEagle_openGL.AdjBodyRotateX(false);
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x02){
	
						   BaldEagle_openGL.DownStrockRight(flagR); 
						   BaldEagle_openGL.DownStrockLeft(flagL);

						  BaldEagle_openGL.AdjBodyZ(false);  	
						  BaldEagle_openGL.AdjBodyY(true); 
						   
						   myFlag=false;
						   BaldEagle_openGL.updateCamera();
   }
   else{

	   float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

		   if(!myFlag){
					StationTime = currentTime  + 10;
					myFlag=true;
				}

			  if(currentTime > StationTime){
								  BaldEagle_openGL.AdjBodyY(false); 
			  }

	  
	  BaldEagle_openGL.fixWing();
	  BaldEagle_openGL.updateCamera();
}
    //-----------------------------------------------------------------------------
   //if (buttonmask & 0x08) {}
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x10) { 

		   BaldEagle_openGL.AdjBodyRotateZ(true);
		   BaldEagle_openGL.glidLeft();
		   BaldEagle_openGL.updateCamera();
   }
   else{

		    BaldEagle_openGL.AdjBodyRotateZBack();
			BaldEagle_openGL.updateCamera();
		    glidThetaL=0;
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x20) {

		   BaldEagle_openGL.AdjBodyRotateZ(false);
		   BaldEagle_openGL.glidRight();
		   BaldEagle_openGL.updateCamera();
   }
   else{

		   BaldEagle_openGL.AdjBodyRotateZBack();
		   BaldEagle_openGL.updateCamera();	  
		   glidThetaR=0;
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x40)  BaldEagle_openGL.AdjBodyZ(true); 
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x80)  BaldEagle_openGL.AdjBodyZ(false);
   //-----------------------------------------------------------------------------

} 
*/

void joystick(unsigned int buttonmask, int x, int y, int z){
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x0100) glutLeaveMainLoop();
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x01) { 
	   BaldEagle_physic.angelOfAttack+=0.1; 
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x04) {   	
	   BaldEagle_physic.angelOfAttack-=0.1;  
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x02){
	   BaldEagle_physic._ishovering=true;
	   BaldEagle_openGL.DownStrockRight(flagR); 
	   BaldEagle_openGL.DownStrockLeft(flagL);		 
	   BaldEagle_openGL.updateCamera();
			   
   }
   else{
	   	   BaldEagle_physic._ishovering=false;
		   BaldEagle_openGL.fixWing();	 
		   BaldEagle_openGL.updateCamera();

	   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x08) {	  
	   BaldEagle_physic._isforward=true;
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x10) { 
	   if(BaldEagle_physic.height > 0 ){
		   BaldEagle_openGL.AdjBodyRotateZ(true);
		   BaldEagle_openGL.glidLeft();	
		   BaldEagle_openGL.updateCamera();
	   }
   }
   else{
		    BaldEagle_openGL.AdjBodyRotateZBack();
		    BaldEagle_openGL.updateCamera();
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x20) {
	   if(BaldEagle_physic.height > 0 ){
		   BaldEagle_openGL.AdjBodyRotateZ(false);
		   BaldEagle_openGL.glidRight();
		   BaldEagle_openGL.updateCamera();
	   }
   }
   else{
		   BaldEagle_openGL.AdjBodyRotateZBack();	  
		   BaldEagle_openGL.updateCamera();
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x40) {
	   if(BaldEagle_physic.height > 0 ){
		   BaldEagle_openGL.AdjBodyRotateZ(true);
		   BaldEagle_openGL.glidLeft();	
		   BaldEagle_openGL.updateCamera();
	   }
   }
   else{
		    BaldEagle_openGL.AdjBodyRotateZBack();
		    BaldEagle_openGL.updateCamera();
   }
   //-----------------------------------------------------------------------------
   if (buttonmask & 0x80) {
	   if(BaldEagle_physic.height > 0 ){
		   BaldEagle_openGL.AdjBodyRotateZ(false);
		   BaldEagle_openGL.glidRight();
		   BaldEagle_openGL.updateCamera();
	   }
   }
   else{
		   BaldEagle_openGL.AdjBodyRotateZBack();	  
		   BaldEagle_openGL.updateCamera();
   }
   //-----------------------------------------------------------------------------
}   

void OnShutdown(){
	cout<<"shutdown"<<endl;
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |GLUT_RGBA);
	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frozen Ground");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		cerr<<"Error: "<<glewGetErrorString(err)<<endl;
	} else {
			if (GLEW_VERSION_3_3){
				cout<<"Driver supports OpenGL 3.3\nDetails:"<<endl;}
			}

	cout<<"\tUsing glew  "	 <<glewGetString(GLEW_VERSION)				 <<endl;
	cout<<"\tVendor:     "   <<glGetString (GL_VENDOR)					 <<endl;
	cout<<"\tRenderer:   "   <<glGetString (GL_RENDERER)				 <<endl;
	cout<<"\tVersion:    "   <<glGetString (GL_VERSION)					 <<endl;
	cout<<"\tGLSL:       "   <<glGetString(GL_SHADING_LANGUAGE_VERSION)  <<endl;



//Shape
BaldEagle_physic.mass = 4.2;
BaldEagle_physic.wingSpan = 1.9;
BaldEagle_physic.refernceLength = 0.80;
BaldEagle_physic.wingArea = 0.8;
BaldEagle_physic.meanChord = 0.35;
BaldEagle_physic.aspectRatio = 4;
BaldEagle_physic.cropMass = 0.0;
BaldEagle_physic.emptyMass = 0.0;
BaldEagle_physic.fatMass = 0.5;
BaldEagle_physic.frameMass = 3.0;
BaldEagle_physic.musclesMass = 0.7;
BaldEagle_physic.UpdateShape();

//Environment
BaldEagle_physic.height = 0;
BaldEagle_physic.latitude = 0;
BaldEagle_physic.UpdateEnvironment();

//Power
BaldEagle_physic.liftCoefficient = 1;
BaldEagle_physic.db=0.2;
BaldEagle_physic.angelOfAttack=0;
BaldEagle_physic.UpdatePower();


	init();
	glutCloseFunc(OnShutdown);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
    glutJoystickFunc(joystick, 25);

	glutMainLoop();

	return 0;
}