#include <GL/glew.h>
#include <GL/wglew.h>
#include <SOIL.h>

//GLEW includes must come before anything else
#include <vector>
#include <string>
#include <fstream>
#include <GLFW/glfw3.h>

#include <iostream>


//Shader Sources BRUH
const GLchar* vertexSource =
"#version 330\n"
"in vec2 position;"
"void main() {"
"	gl_Position = vec4(position, 0.0, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 330\n"
"uniform vec3 triangleColor;"
"out vec4 outColor;"
"void main() {"
"	outColor = vec4(triangleColor, 1.0);"
"}";

GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile)
{
	std::string strShaderCode;
	//open shader file
	std::ifstream shaderStream(a_strShaderFile);
	//if that worked ok, load file line by line
	if (shaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(shaderStream, Line))
		{
			strShaderCode += "\n" + Line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const *szShaderSourcePointer = strShaderCode.c_str();

	//create shader ID
	GLuint uiShader = glCreateShader(a_eShaderType);
	
	//load source code
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);
	
	//compile shader
	glCompileShader(uiShader);
	

	//Check for compiliation errors and output them
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if (iStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (a_eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return uiShader;
}
GLuint CreateProgram(const char *a_vertex, const char *a_frag)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

	//create shader program ID
	GLuint uiProgram = glCreateProgram();

	//attach shaders
	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
		glAttachShader(uiProgram, *shader);

	//link program
	glLinkProgram(uiProgram);

	//check for link errors and output them
	GLint status;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(uiProgram, *shader);
		glDeleteShader(*shader);
	}

	return uiProgram;
}

float vertices[] =
{
	0.0f, 0.5f,  // Vertex 1 (X, Y)
	0.5f, -0.5f, // Vertex 2 (X, Y)
	-0.5f, -0.5f // Vertex 3 (X, Y)
};


int main()
{
	//Initilization of GLFW
	if (!glfwInit())
	{
		return -1;
	}

	//create a windowed mode winow and it's OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(window);

	//.............................................................................
	//START GLEW BRUH
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code bruh
		glfwTerminate();
		return -1;
	}
	//..............................................................................
	//looooppppooop unitl user closes windooe
	GLuint vao;
	glGenVertexArrays(1, &vao);
	//Start using by bindiing;sehti
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE)
	{
		printf("Vertex shader compiled successfully bruh\n");
		printf("I furted\n");
	}

	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

	printf(buffer);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint SHIT;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &SHIT);

	if (SHIT == GL_TRUE)
	{
		printf("SHIT IS TRUE BITCH\n");
	}
	else if (SHIT == GL_FALSE)
	{
		printf("SHIT AINT REAL BRUH\n");

	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//glBindFragDataLocation(shaderProgram, 0, "outColor");
	//Linksshit
	glLinkProgram(shaderProgram);
	//start using shit
	glUseProgram(shaderProgram);
	
	GLint statusProgram;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &statusProgram);
	if (statusProgram == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	//time to link mofer shitea
	//Retrieving Postion
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");




	while (!glfwWindowShouldClose(window))
	{
		//draw code goes here
		glClearColor(0.1f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap front and back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}