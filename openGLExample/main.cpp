#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include <time.h>

GLuint h_ShaderProgram;

// for Phong Shading shaders
GLint loc_ModelViewProjectionMatrix;
GLint loc_base_texture;

#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;
glm::mat4 ViewMatrix, ProjectionMatrix;

#include "ImageLoader.h"

GLuint texture_name;

//quad object
GLuint quad_VBO, quad_VAO;
GLfloat quad_vertices[6][8] = {  // vertices enumerated counterclockwise
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }
};

void prepare_quad(void) { // Draw coordinate axes.
						   // Initialize vertex buffer object.
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

	glGenBuffers(1, &quad_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &quad_VAO);
	glBindVertexArray(quad_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#define IMGSIZE 600.0f

void draw_texture() {
	glm::mat4 ModelMatrix;	

	glUseProgram(h_ShaderProgram);
	glUniform1i(loc_base_texture, 0);

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-IMGSIZE/2, -IMGSIZE/2, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(IMGSIZE, IMGSIZE, IMGSIZE));
	ModelViewMatrix = ViewMatrix * ModelMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	
	glBindVertexArray(quad_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glUseProgram(0);
}

void display(void) {

	glClearColor(1, 0, 0, 0);
	glViewport(0, 0, (int)IMGSIZE, (int)IMGSIZE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_texture();
	
	glutSwapBuffers();
}

void prepare_shader_program(void) {

	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/shader.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/shader.frag" },
	{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_base_texture = glGetUniformLocation(h_ShaderProgram, "u_base_texture");
}


void loadTexture(const char* filename) {

	glGenTextures(1, &texture_name);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_name);

	//load_DDS_image(filename);
	load_unpack_image(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void initialize_renderer(void) {
	prepare_shader_program();

#define TO_RADIAN 0.01745329252f  
	ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, 1.0f, 1.0f, IMGSIZE*1.6f);
	ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f - IMGSIZE * 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	prepare_quad();
	//loadTexture("Data/img1_JPG_DXT5.DDS");
	loadTexture("Data/img1.jpg");

	glutDisplayFunc(display);
}

int main(int argc, char *argv[]) {
	char program_name[64] = "openGL Example";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize((int)IMGSIZE, (int)IMGSIZE);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	glewInit();
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();

	return 0;
}