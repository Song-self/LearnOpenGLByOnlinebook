#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.f, 1.0f);\n"
"}\n\0";

unsigned int CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

int main(int argc, char** argv)
{
	// glfw: Initialize and configure
	// -----------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw window creation
	// ----------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
		"LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" <<
			std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD"
			<< std::endl;
		return -1;
	}

	unsigned int shaderProgram1 = CreateShaderProgram(vertexShaderSource, fragmentShaderSource1);
	unsigned int shaderProgram2 = CreateShaderProgram(vertexShaderSource, fragmentShaderSource2);

	

	float vertices1[] = {
	-1, 0, 0,	0, 0, 0,  -0.5, 1, 0,
	};
	float vertices2[] = {
	0, 0, 0,   1, 0, 0,   0.5, 1, 0
	};


	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO); glGenBuffers(2, VBO);
	glBindVertexArray(VAO[0]); glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *
		sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]); glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *
		sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// cull back
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);




	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// ------
		processInput(window);



		// render
		// -------
		// 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		for (size_t i = 0; i < 2; i++)
		{
			switch (i)
			{
			case 0:
				glUseProgram(shaderProgram1); break;
			case 1:
				glUseProgram(shaderProgram2); break;
			}
			glBindVertexArray(VAO[i]);  // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		    glDrawArrays(GL_TRIANGLES, 0, 3);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// glfw: swap buffers and poll IO events (key pressed/released, mouse moved etc.)
		// -----------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram1); glDeleteProgram(shaderProgram2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// -----------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	std::cout << "resize" << std::endl;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ----------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

unsigned int CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog
			<< std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog
			<< std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader); glDeleteShader(fragmentShader);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog
			<< std::endl;
	}

	return shaderProgram;
}