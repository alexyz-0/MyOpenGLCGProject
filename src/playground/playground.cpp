#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <cmath>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <iostream>
#include <random>


glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

std::vector<glm::vec3> vertices[3][3][3];
std::vector<glm::vec3> normals[3][3][3];
std::vector<glm::vec3> colors[3][3][3];

bool isRotating = false;
glm::vec3 rotateDirection;
glm::vec3 translateDirection;
std::vector<glm::vec3> rotateIndices;
float anglesTillNow = 0;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(0, 8);

void initiateCube() {
		float f = 2.1;
		for (int k = 0; k < 3; k++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					vertices[k][i][j] = {
						//k=0 face
						glm::vec3(-3.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						//i=0 face
						glm::vec3(-1.1 + k * f,-3.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						//j=0 face
						glm::vec3(-1.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f,-3.1 + j * f),
						//k=2 face
						glm::vec3(-1.1 + k * f, -3.1 + i * f, -1.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						glm::vec3(-1.1 + k * f,-1.1 + i * f,-1.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f,-1.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f, -3.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f, -3.1 + j * f),
						//i=2 face
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f,-1.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f,-3.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -3.1 + j * f),
						//j=2 face
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f,-3.1 + i * f, -1.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f, -1.1 + j * f),
						glm::vec3(-1.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-3.1 + k * f, -1.1 + i * f, -1.1 + j * f),
						glm::vec3(-1.1 + k * f,-3.1 + i * f, -1.1 + j * f)
					};
				}
			}
		}

		//If we would represent the cube with 8 vertices instead of seeing the faces independently,
		//the normals would actually be a combination of 1/sqrt(3) and -1/sqrt(3)
		//This doesn't make much sense though as it would result in spherical representation in the shader
		//That's why we use the normals of the triangles/faces, resulting in basis vectors when the cube isn't rotated
		for (int k = 0; k < 3; k++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					normals[k][i][j] = {
						//k=0 face
						glm::vec3(-1,0,0),
						glm::vec3(-1,0,0),
						glm::vec3(-1,0,0),
						glm::vec3(-1,0,0),
						glm::vec3(-1,0,0),
						glm::vec3(-1,0,0),
						//i=0 face
						glm::vec3(0,-1,0),
						glm::vec3(0,-1,0),
						glm::vec3(0,-1,0),
						glm::vec3(0,-1,0),
						glm::vec3(0,-1,0),
						glm::vec3(0,-1,0),
						//j=0 face
						glm::vec3(0,0,1),
						glm::vec3(0,0,1),
						glm::vec3(0,0,1),
						glm::vec3(0,0,1),
						glm::vec3(0,0,1),
						glm::vec3(0,0,1),
						//k=2 face
						glm::vec3(1,0,0),
						glm::vec3(1,0,0),
						glm::vec3(1,0,0),
						glm::vec3(1,0,0),
						glm::vec3(1,0,0),
						glm::vec3(1,0,0),
						//i=2 face
						glm::vec3(0,1,0),
						glm::vec3(0,1,0),
						glm::vec3(0,1,0),
						glm::vec3(0,1,0),
						glm::vec3(0,1,0),
						glm::vec3(0,1,0),
						//j=2 face
						glm::vec3(0,0,-1),
						glm::vec3(0,0,-1),
						glm::vec3(0,0,-1),
						glm::vec3(0,0,-1),
						glm::vec3(0,0,-1),
						glm::vec3(0,0,-1)
					};
				}
			}
		}

		float g = 0.450;
		for (int k = 0; k < 3; k++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					colors[k][i][j] = {
						//k=0 face
						glm::vec3(g,g,0),
						glm::vec3(g,g,0),
						glm::vec3(g,g,0),
						glm::vec3(g,g,0),
						glm::vec3(g,g,0),
						glm::vec3(g,g,0),
						//i=0 face
						glm::vec3(g,g,g),
						glm::vec3(g,g,g),
						glm::vec3(g,g,g),
						glm::vec3(g,g,g),
						glm::vec3(g,g,g),
						glm::vec3(g,g,g),
						//j=0 face
						glm::vec3(0,g,0),
						glm::vec3(0,g,0),
						glm::vec3(0,g,0),
						glm::vec3(0,g,0),
						glm::vec3(0,g,0),
						glm::vec3(0,g,0),
						//k=2 face
						glm::vec3(g,0,0),
						glm::vec3(g,0,0),
						glm::vec3(g,0,0),
						glm::vec3(g,0,0),
						glm::vec3(g,0,0),
						glm::vec3(g,0,0),
						//i=2 face
						glm::vec3(0,0,g),
						glm::vec3(0,0,g),
						glm::vec3(0,0,g),
						glm::vec3(0,0,g),
						glm::vec3(0,0,g),
						glm::vec3(0,0,g),
						//j=2 face
						glm::vec3(0,g,g),
						glm::vec3(0,g,g),
						glm::vec3(0,g,g),
						glm::vec3(0,g,g),
						glm::vec3(0,g,g),
						glm::vec3(0,g,g)
					};
				}
			}
		}
		/*
			for (int k = 0; k < 3; k++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					double L = k / 9.0 + i / 9.0 + j / 9.0;
					colors[k][i][j] = {
						//k=0 face
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						//i=0 face
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						//j=0 face
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						//k=2 face
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						//i=2 face
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						//j=2 face
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0),
						glm::vec3(k/3.0,i/3.0,j/3.0)
					};
				}
			}
		}
		*/
	}

void chooseRandomRotation(){
	if (isRotating) return;

	int r = distr(gen);

	rotateIndices.clear();
	if (r < 3) {
		rotateDirection = glm::vec3(1, 0, 0);
		for (int n = 0; n < 3; n++)
			for (int m = 0; m < 3; m++)
				rotateIndices.push_back(glm::vec3(r % 3, n, m));
		std::vector<glm::vec3> temp = vertices[r % 3][1][0];
		vertices[r % 3][1][0] = vertices[r % 3][0][1];
		vertices[r % 3][0][1] = vertices[r % 3][1][2];
		vertices[r % 3][1][2] = vertices[r % 3][2][1];
		vertices[r % 3][2][1] = temp;
		temp = vertices[r % 3][0][0];
		vertices[r % 3][0][0] = vertices[r % 3][0][2];
		vertices[r % 3][0][2] = vertices[r % 3][2][2];
		vertices[r % 3][2][2] = vertices[r % 3][2][0];
		vertices[r % 3][2][0] = temp;

		temp = normals[r % 3][1][0];
		normals[r % 3][1][0] = normals[r % 3][0][1];
		normals[r % 3][0][1] = normals[r % 3][1][2];
		normals[r % 3][1][2] = normals[r % 3][2][1];
		normals[r % 3][2][1] = temp;
		temp = normals[r % 3][0][0];
		normals[r % 3][0][0] = normals[r % 3][0][2];
		normals[r % 3][0][2] = normals[r % 3][2][2];
		normals[r % 3][2][2] = normals[r % 3][2][0];
		normals[r % 3][2][0] = temp;
	}
	else if (r < 6) {
		rotateDirection = glm::vec3(0, 1, 0);
		for (int n = 0; n < 3; n++)
			for (int m = 0; m < 3; m++)
				rotateIndices.push_back(glm::vec3(n, r % 3, m));
		std::vector<glm::vec3> temp = vertices[0][r % 3][1];
		vertices[0][r % 3][1] = vertices[1][r % 3][0];
		vertices[1][r % 3][0] = vertices[2][r % 3][1];
		vertices[2][r % 3][1] = vertices[1][r % 3][2];
		vertices[1][r % 3][2] = temp;
		temp = vertices[0][r % 3][0];
		vertices[0][r % 3][0] = vertices[2][r % 3][0];
		vertices[2][r % 3][0] = vertices[2][r % 3][2];
		vertices[2][r % 3][2] = vertices[0][r % 3][2];
		vertices[0][r % 3][2] = temp;

		temp = normals[0][r % 3][1];
		normals[0][r % 3][1] = normals[1][r % 3][0];
		normals[1][r % 3][0] = normals[2][r % 3][1];
		normals[2][r % 3][1] = normals[1][r % 3][2];
		normals[1][r % 3][2] = temp;
		temp = normals[0][r % 3][0];
		normals[0][r % 3][0] = normals[2][r % 3][0];
		normals[2][r % 3][0] = normals[2][r % 3][2];
		normals[2][r % 3][2] = normals[0][r % 3][2];
		normals[0][r % 3][2] = temp;
	}
	else {
		rotateDirection = glm::vec3(0, 0, 1);
		for (int n = 0; n < 3; n++)
			for (int m = 0; m < 3; m++)
				rotateIndices.push_back(glm::vec3(n, m, r % 3));

		std::vector<glm::vec3> temp = vertices[1][0][r % 3];
		vertices[1][0][r % 3] = vertices[0][1][r % 3];
		vertices[0][1][r % 3] = vertices[1][2][r % 3];
		vertices[1][2][r % 3] = vertices[2][1][r % 3];
		vertices[2][1][r % 3] = temp;
		temp = vertices[0][0][r % 3];
		vertices[0][0][r % 3] = vertices[0][2][r % 3];
		vertices[0][2][r % 3] = vertices[2][2][r % 3];
		vertices[2][2][r % 3] = vertices[2][0][r % 3];
		vertices[2][0][r % 3] = temp;

		temp = normals[1][0][r % 3];
		normals[1][0][r % 3] = normals[0][1][r % 3];
		normals[0][1][r % 3] = normals[1][2][r % 3];
		normals[1][2][r % 3] = normals[2][1][r % 3];
		normals[2][1][r % 3] = temp;
		temp = normals[0][0][r % 3];
		normals[0][0][r % 3] = normals[0][2][r % 3];
		normals[0][2][r % 3] = normals[2][2][r % 3];
		normals[2][2][r % 3] = normals[2][0][r % 3];
		normals[2][0][r % 3] = temp;
	}
	isRotating = true;
	anglesTillNow = 0;
}

void myRotate(float angle) {
	bool flag = false;
	if (anglesTillNow + angle >= M_PI_2){
		angle = M_PI_2 - anglesTillNow;
		isRotating = false;
		anglesTillNow = 0;
		flag = true;
	}
	else {
		anglesTillNow += angle;
	}

	Model = glm::rotate(angle, rotateDirection);
	for (int m = 0; m < rotateIndices.size(); m++) {
		int k = rotateIndices[m].x;
		int i = rotateIndices[m].y;
		int j = rotateIndices[m].z;
		for (int n = 0; n < vertices[k][i][j].size(); n++) {
			glm::vec4 point = glm::vec4(vertices[k][i][j][n], 1);
			point = Model * point;
			vertices[k][i][j][n].x = point.x;
			vertices[k][i][j][n].y = point.y;
			vertices[k][i][j][n].z = point.z;
		}
		for (int n = 0; n < normals[k][i][j].size(); n++) {
			glm::vec4 point = glm::vec4(normals[k][i][j][n], 1);
			point = Model * point;
			normals[k][i][j][n].x = point.x;
			normals[k][i][j][n].y = point.y;
			normals[k][i][j][n].z = point.z;
		}
	}
	
	if (flag) {
		for (int m = 0; m < rotateIndices.size(); m++) {
			int k = rotateIndices[m].x;
			int i = rotateIndices[m].y;
			int j = rotateIndices[m].z; 
			for (int n = 0; n < normals[k][i][j].size(); n++) {
				//std::cout << "normals: " << normals[k][i][j][n].x << " : " <<
				//	normals[k][i][j][n].y << " : " << normals[k][i][j][n].z << "\n";
				//fix x normals
				if (normals[k][i][j][n].x < -0.5)
					normals[k][i][j][n].x = -1;
				else if (normals[k][i][j][n].x > 0.5)
					normals[k][i][j][n].x = 1;
				else
					normals[k][i][j][n].x = 0;
				//fix y normals
				if (normals[k][i][j][n].y < -0.5)
					normals[k][i][j][n].y = -1;
				else if (normals[k][i][j][n].y > 0.5)
					normals[k][i][j][n].y = 1;
				else
					normals[k][i][j][n].y = 0;
				//fix z normals
				if (normals[k][i][j][n].z < -0.5)
					normals[k][i][j][n].z = -1;
				else if (normals[k][i][j][n].z > 0.5)
					normals[k][i][j][n].z = 1;
				else
					normals[k][i][j][n].z = 0;
			}
		}
		
	}
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Dark blue background
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "AVertexShader.vertexshader", "AFragmentShader.fragmentshader" );


	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


	initiateCube();
	

	GLuint vertexbuffer[3][3][3][2];
	GLuint colorbuffer[3][3][3];

	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				glGenBuffers(1, &vertexbuffer[k][i][j][0]);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[k][i][j][0]);
				glBufferData(GL_ARRAY_BUFFER, vertices[k][i][j].size() * sizeof(glm::vec3), &vertices[k][i][j][0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glGenBuffers(1, &vertexbuffer[k][i][j][1]);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[k][i][j][1]);
				glBufferData(GL_ARRAY_BUFFER, normals[k][i][j].size() * sizeof(glm::vec3), &normals[k][i][j][0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glGenBuffers(1, &colorbuffer[k][i][j]);
				glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[k][i][j]);
				glBufferData(GL_ARRAY_BUFFER, colors[k][i][j].size() * sizeof(glm::vec3), &colors[k][i][j][0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
	}

	double lastTime = glfwGetTime();
	double timeToRotate = 0;
	do{

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = getPosition();
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glUseProgram(programID);
		computeMatricesFromInputs();
		Projection = getProjectionMatrix();
		View = getViewMatrix();
		Model = glm::mat4(1.0);
		MVP = Projection * View * Model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		if(!isRotating)
			timeToRotate += deltaTime;
		if (!isRotating && timeToRotate >= 1.5) {
			timeToRotate = 0;
			chooseRandomRotation();
		}
		if (isRotating) {
			myRotate(0.01f);
		}
		for (int k = 0; k < 3; k++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[k][i][j][0]);
					glBufferData(GL_ARRAY_BUFFER, vertices[k][i][j].size() * sizeof(glm::vec3), &vertices[k][i][j][0], GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[k][i][j][1]);
					glBufferData(GL_ARRAY_BUFFER, normals[k][i][j].size() * sizeof(glm::vec3), &normals[k][i][j][0], GL_STATIC_DRAW);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[k][i][j]);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

					glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

				}
			}
		}


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				glDeleteBuffers(1, &vertexbuffer[k][i][j][0]);
				glDeleteBuffers(1, &vertexbuffer[k][i][j][1]);
				glDeleteBuffers(1, &colorbuffer[k][i][j]);
			}
		}
	}
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


/*
		for (int k = 0; k < 3; k++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int n = 0; n < normals[k][i][j].size(); n++) {

					}
				}
			}
		}
		*/