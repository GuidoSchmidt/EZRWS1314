//will the real SlimShader please stand up

#ifndef SLIMSHADER_H_
#define SLIMSHADER_H_

#include <GL/glew.h>
#include <iostream>

using namespace std;

class SlimShader
{
public:
	SlimShader();
	SlimShader(string root, string vertFileName, string fragFileName);
	SlimShader(string root, string vertFileName, string geoFileName, string fragFileName);
	~SlimShader();

	void printShaderInfoLog(GLuint shader);
	void printProgramInfoLog(GLuint program);
	string readFile(string fileName);

	bool geo;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLuint geometryShader;

	void enable();
	void disable();
};

#endif
