// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <controls.hpp>

#include <iterator>
#include <vector>
#include <iostream>
#include <math.h>

//for random
#include <cstdlib>
#include <ctime>
#include <random>

std::vector<glm::vec3> onecubevector() {
	return {

	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),

	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),


	glm::vec3(1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),

	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),

	glm::vec3(1.0f, 1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),

	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),

	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),

	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f),

	 glm::vec3(1.0f, 1.0f, 1.0f),
	 glm::vec3(1.0f,-1.0f,-1.0f),
	 glm::vec3(1.0f, 1.0f,-1.0f),



	 glm::vec3(1.0f, 1.0f, 1.0f),
	 glm::vec3(1.0f, 1.0f,-1.0f),
	 glm::vec3(-1.0f, 1.0f,-1.0f),

	 glm::vec3(1.0f, 1.0f, 1.0f),
	 glm::vec3(-1.0f, 1.0f,-1.0f),
	 glm::vec3(-1.0f, 1.0f, 1.0f),

	 glm::vec3(1.0f, 1.0f, 1.0f),
	 glm::vec3(-1.0f, 1.0f, 1.0f),
	 glm::vec3(1.0f,-1.0f, 1.0f)
	};
}

constexpr int MIN = 1;
constexpr int MAX = 100;


float getRandom(int LO, int HI) {
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

	return r3;
}

float getRandom01() {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return r;
}
    
//void printRandoms(int lower, int upper,
//	int count)
//{
//	int i;
//	for (i = 0; i < count; i++) {
//	
//
//		printf("%f ", r2);
//	}
//}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	printf("\n %s", xoffset);
	printf("%s", yoffset);

	//if (fov >= 1.0f && fov <= 45.0f)
	//	fov -= yoffset;
	//if (fov <= 1.0f)
	//	fov = 1.0f;
	//if (fov >= 45.0f)
	//	fov = 45.0f;
}


int main(void)
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024 , 768, "Cube", NULL, NULL);
	if (window == NULL) {
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


	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glfwSetScrollCallback(window, scroll_callback);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("cube.v.glsl", "cube.f.glsl");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	int lower = 5, upper = 7, count = 20;


	// Use current time as 
	// seed for random generator
	srand(time(0));

	//printRandoms(lower, upper, count);

	const int cubesCount = 400000;
	const int triCount = 3;
	const int cubVertCount = triCount * 12;

	printf("VVOVVA");

	//glm::vec3 g_vertex_buffer_data[cubVertCount * triCount * cubesCount];

	glm::vec3* g_vertex_buffer_data = new glm::vec3[cubVertCount * triCount * cubesCount];
	glm::vec3* g_color_buffer_data = new glm::vec3[cubVertCount * triCount * cubesCount];

	//GLfloat g_vertex_buffer_data[cubVertCount * triCount * cubesCount];
	//GLfloat g_color_buffer_data[cubVertCount * triCount * cubesCount];

	std::vector<glm::vec3> cubeVec = onecubevector();

	int intend = triCount * cubVertCount;
	int counterr = 0;

	while (counterr < cubesCount)
	{
		int l_b = -100;
		int u_b = 100;
		float n = getRandom(l_b, u_b);
		float n1 = getRandom(l_b, u_b);
		float n2 = getRandom(l_b, u_b);

		int scale = 10;

		for (int y = 0; y < cubVertCount; y++) {
			int index = counterr * intend + y;// * 3;
			g_vertex_buffer_data[index] = glm::vec3(cubeVec[y][0]/ scale + n, cubeVec[y][1] / scale + n1, cubeVec[y][2] / scale + n2);


			g_color_buffer_data[index] = glm::vec3(getRandom01(), getRandom01(), getRandom01());
		}

		counterr += 1;
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * cubVertCount * triCount * cubesCount, &g_vertex_buffer_data[0], GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * cubVertCount * triCount * cubesCount, &g_color_buffer_data[0], GL_STATIC_DRAW);


	printf("VOVAAAAA");
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
				// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		 //2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, cubVertCount* cubesCount); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

