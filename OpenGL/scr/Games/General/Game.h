#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Sprite.h"

#include "../../DebugHelper/Printing.h"
#include "../../DebugHelper/Settings.h"

class Game
{
protected:
	int screenWidth = 0;
	int screenHeight = 0;

	const char* screenName = "";

	GLFWwindow* window = nullptr;

	std::vector<Sprite> images = {};

	/*
	* @brief Creates a new image to render on Screen
	* @param Path to the image
	* @return The sprite you have made
	* @author ZaneDevv
	*/
	Sprite CreateImage(const char*);

private:
	GLuint shaderProgram = 0;

	const char* vertexShaderCode = R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;

		void main() {
			gl_Position = vec4(aPos, 1.0);
			TexCoord = aTexCoord;
		}
	)";

	const char* fragmentShaderCode = R"(
		#version 330 core

		out vec4 FragColor;
		in vec2 TexCoord;

		uniform sampler2D texture1;

		void main() {
			FragColor = texture(texture1, TexCoord);
		}
	)";

	/*
	* @brief Compiles a Shader program
	* @param Shader code to be compiled
	* @param Kind of shader (Vertex shader/Fragment shader)
	* @author ZaneDevv
	*/
	GLuint CompileShader(const char*, GLenum);

	/*
	* @brief Creates a shader program
	* @author ZaneDevv
	*/
	GLuint CreateShaderProgram();

public:
	/*
	* @brief Executes when the screen is created
	* @author ZaneDevv
	*/
	virtual void OnScreenCreated() = 0;

	/*
	* @brief Creates a window where the game will render.
	* @author ZaneDevv
	*/
	void CreateWindow();

	/*
	* @brief Updates the whole game
	* @param Delta time
	* @author ZaneDevv
	*/
	virtual void Update(double) = 0;

	/*
	* @brief Updates graphics
	* @author ZaneDevv
	*/
	void UpdateGraphics() ;

	/*
	* @brief Starts the game
	* @author ZaneDevv
	*/
	void StartLoop();

	/*
	* @brief Closes the game
	* @author ZaneDevv
	*/
	void CloseGame();
};