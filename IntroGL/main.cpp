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
//"in vec2 position;"
"in vec2 texcoord"
//"in vec3 color;"
"out vec3 Color;"
"out vec2 Texcoord;"
"void main() "
"{"
"Texcoord = texcood;"
//"   Color = color;"
//"	gl_Position = vec4(position, 0.0, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 330\n"
//"uniform vec3 triangleColor;"
"in vec3 Color;"
"out vec4 outColor;"
"void main() "
"{"
//"	outColor = vec4(triangleColor, 1.0);"
"   outColor = vec4(Color, 1.0);"
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

//float vertices[] =
//{
//	0.0f, 0.5f,   1.0f, 0.0f, 0.0f, // Vertex 1 (X, Y) red
//	0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y) green
//	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f // Vertex 3 (X, Y)  blue
//};
float vertices[] =
{
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //top-left
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //top-right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //butt-right
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f //bottom left

	//0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, //butt-right
	//-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //butt-left
	//-0.5f,  0.5f, 1.0f, 0.0f, 0.0f  //top left
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

	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	int width, height;
	unsigned char* image =
		SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);


	//Black/White Checkboard
	float pixels[] =
	{
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);

	/*GLuint elements[] =
	{
		0, 1, 2
	};*/

	GLuint elements[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	

	


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE)
	{
		printf("Vertex shader compiled successfully bruh\n");
		printf("I furted\n");
	}
	else if (status == GL_FALSE)
	{
		printf("Vertex shader error.\n");
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
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	
	
	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	
	
	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);




	while (!glfwWindowShouldClose(window))
	{
		//draw code goes here

		// background color
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		float time = (float)glfwGetTime();
		glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		//swap front and back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}