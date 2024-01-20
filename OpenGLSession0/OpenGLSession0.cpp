#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resources/Shaders/shaderClass.h"
#include "Resources/Shaders/VAO.h"
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/EBO.h"


const unsigned int width = 800;
const unsigned int height = 800;

struct Vertex {
    float x, y, z;
    float r, g, b;
};

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS     
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,
};

// Indices for vertices order, irrelevant to spiral
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};
void writeToFile(const char* fileName, double x, double y, double z, double r, double g, double b) {
	std::ofstream outputFile(fileName, std::ios::app);  // Open the file in append mode
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(3);
		outputFile << x << " , " << y << " , " << z << " , " << r << " , " << g << " , " << b;


	}
	else {
		std::cerr << "Unable to open the output file for writing." << std::endl;
	}
	//if (y > 0) {
	//	outputFile << " green";  // Use green for positive y
	//}
	//else if (y < 0) {
	//	outputFile << " red";    // Use red for negative y
	//}
	//else {
	//	outputFile << " white";  // Use white for y = 0
	//}
	outputFile << std::endl;
	outputFile.close();
}
void Readfile(const char* fileName, std::vector<Vertex>& verticesSpiral) {
	std::ifstream inputFile(fileName);
	if (inputFile.is_open()) {
		Vertex vertex;
		char comma; // to capture the commas in the file

		while (inputFile >> vertex.x >> comma >> vertex.y >> comma >> vertex.z >> comma >> vertex.r >> comma >> vertex.g >> comma >> vertex.b) {
			verticesSpiral.push_back(vertex);
		}

		inputFile.close();
	}
	else {
		std::cerr << "Unable to open the input file for reading." << std::endl;
	} 
	
	
	
}
void CreateArchimedeanSpiral(std::vector<Vertex>& verticesSpiral, float a, float b, float c, float angularFrequency, float iterations, const char* filename) {
	for (int i = 0; i <= iterations; ++i) {
		float t = static_cast<float>(i);
		float x = a * std::cos(angularFrequency * t);
		float y = b * std::sin(angularFrequency * t);
		float z = c * t;

		Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.z = z;
		vertex.r = 0.0f;
		vertex.g = 0.0f;
		vertex.b = std::abs(z) / (c * iterations);  // Adjust for coloring effect

		verticesSpiral.push_back(vertex);
		//writeToFile(filename, vertex.x, vertex.y, vertex.z,vertex.r, vertex.g, vertex.b);
		
	}
}

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);
	std::vector<Vertex> verticesSpiral;
	Readfile("spiraldata.txt", verticesSpiral);
	float a = 0.1f;  // Adjust these parameters accordingly
	float b = 1.1f;
	float c = 0.10f;
	float angularFrequency = 3.1f;
	int iterations = 50;

	const char* outputFileName = "spiraldata.txt";
	

	//CreateArchimedeanSpiral(verticesSpiral, a, b, c, angularFrequency, iterations,outputFileName);



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	std::vector<GLfloat> flattenedVertices;
	for (const Vertex& vertex : verticesSpiral) {
		flattenedVertices.push_back(vertex.x);
		flattenedVertices.push_back(vertex.y);
		flattenedVertices.push_back(vertex.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}

	
	

	// Generates Vertex Buffer Object and links it to spiral vertices
	VBO VBO_Spiral(flattenedVertices.data(), verticesSpiral.size() * sizeof(Vertex));
	VAO1.LinkAttrib(VBO_Spiral, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO_Spiral, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	
	
	
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO_Spiral.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float scaleValue = 1200.0f;
	
	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			rotation += 0.09f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1f(uniID, scaleValue);
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of vertices, starting index
		glDrawArrays(GL_LINE_STRIP, 0, verticesSpiral.size());
		glLineWidth(20.0f);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	/*std::ofstream clearFile(outputFileName);
	clearFile.close();*/
	
	// Delete all the objects we've created
	VAO1.Delete();
	VBO_Spiral.Delete();
	
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}