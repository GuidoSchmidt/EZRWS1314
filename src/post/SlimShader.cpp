#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <GL/glew.h>
#include "SlimShader.h"
using namespace std;

SlimShader::SlimShader(string root, string vertFileName, string fragFileName)
{
	geo = false;

	if(glewInit() != GLEW_OK)
		   cout << "GLEW init failed!" << endl;


	cout << "new Shader: " << fragFileName << endl;
	// Create empty shader object (vertex shader)
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Read vertex shader source
	string shaderSource = readFile(root+vertFileName);
	const char* sourcePtr = shaderSource.c_str();

	// Attach shader code
	glShaderSource(vertexShader, 1, &sourcePtr, NULL);

	// Compile
	glCompileShader(vertexShader);
	printShaderInfoLog(vertexShader);

	// Create empty shader object (fragment shader)
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read fragment shader source
	shaderSource = readFile(root+fragFileName);
	sourcePtr = shaderSource.c_str();


	// Attach shader code
	glShaderSource(fragmentShader, 1, &sourcePtr, NULL);

	// Compile
	glCompileShader(fragmentShader);
	printShaderInfoLog(fragmentShader);

	// Create shader program
	shaderProgram = glCreateProgram();

	// Attach shader
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);

	// Link program
	glLinkProgram(shaderProgram);
	printProgramInfoLog(shaderProgram);
}

SlimShader::SlimShader(string root, string vertFileName, string geoFileName, string fragFileName)
{
	geo=true;

	if(glewInit() != GLEW_OK)
		   cout << "GLEW init failed!" << endl;


	cout << "new Shader: " << fragFileName << endl;
	// Create empty shader object (vertex shader)
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Read vertex shader source
	string shaderSource = readFile(root+vertFileName);
	const char* sourcePtr = shaderSource.c_str();

	// Attach shader code
	glShaderSource(vertexShader, 1, &sourcePtr, NULL);

	// Compile
	glCompileShader(vertexShader);
	printShaderInfoLog(vertexShader);



	// Create empty shader object (geometry shader)
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	// Read vertex shader source
	shaderSource = readFile(root+geoFileName);
	sourcePtr = shaderSource.c_str();

	// Attach shader code
	glShaderSource(geometryShader, 1, &sourcePtr, NULL);

	// Compile
	glCompileShader(geometryShader);
	printShaderInfoLog(geometryShader);




	// Create empty shader object (fragment shader)
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read fragment shader source
	shaderSource = readFile(root+fragFileName);
	sourcePtr = shaderSource.c_str();


	// Attach shader code
	glShaderSource(fragmentShader, 1, &sourcePtr, NULL);

	// Compile
	glCompileShader(fragmentShader);
	printShaderInfoLog(fragmentShader);

	// Create shader program
	shaderProgram = glCreateProgram();

	// Attach shader
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,geometryShader);
	glAttachShader(shaderProgram,fragmentShader);

	// Link program
	glLinkProgram(shaderProgram);
	printProgramInfoLog(shaderProgram);
}


SlimShader::~SlimShader()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	if (geo) glDeleteShader(geometryShader);
}



// Print information about the compiling step
void SlimShader::printShaderInfoLog(GLuint shader)
{
    GLint infologLength = 0;
    GLsizei charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);
	infoLog = (char *)malloc(infologLength);
	glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
	printf("%s\n",infoLog);
	free(infoLog);
}

// Print information about the linking step
void SlimShader::printProgramInfoLog(GLuint program)
{
	GLint infoLogLength = 0;
	GLsizei charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH,&infoLogLength);
	infoLog = (char *)malloc(infoLogLength);
	glGetProgramInfoLog(program, infoLogLength, &charsWritten, infoLog);
	printf("%s\n",infoLog);
	free(infoLog);
}

// Reads a file and returns the content as a string
string SlimShader::readFile(string fileName)
{
	string fileContent;
	string line;

	ifstream file(fileName.c_str());
	if (file.is_open()) {
		while (!file.eof()){
			getline (file,line);
			line += "\n";
			fileContent += line;
		}
		file.close();
	}
	else
		cout << "ERROR: Unable to open file " << fileName << endl;

	return fileContent;
}

void SlimShader::enable()
{
	glUseProgram(shaderProgram);
}

void SlimShader::disable()
{
	glUseProgram(0);
}

