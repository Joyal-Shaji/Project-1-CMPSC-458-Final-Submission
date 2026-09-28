/*
	Project 1 Submission for CMPSC458
    Name: Joyal Shaji
	psu id: jxs7202
*/

#include <Project1.hpp>
#include <cmath>
#include <string>

namespace {
std::string projectAssetPath(const char* relativePath)
{
	return std::string(PROJECT_SOURCE_DIR) + "/" + relativePath;
}
// Transformation variables
const float rotationRateStep = 90.0f;
float xRotationRate = 0.0f;
float yRotationRate = 0.0f;
float zRotationRate = 0.0f;
float xRotation = 0.0f;
float yRotation = 0.0f;
float zRotation = 0.0f;
float xScale = 1.0f;
float yScale = 1.0f;
float zScale = 1.0f;
float xPosition = 0.0f;
float yPosition = 0.0f;
float zPosition = 0.0f;
}

// globals 
	// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

	// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


std::string preamble =
"Project 1 code \n\n"
"Press the U,I,O to increase transformations \n"
"Press the J,K,L to decrease transformations \n"
"\tKey alone will alter rotation rate\n "
"\tShift+Key will alter scale\n "
"\tControl+Key will alter translation\n "
"Pressing R will reset transformations\n ";

int main(int argc, char **argv)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X (left here as legacy, more would need to change)
	#endif

	 // Print Preamble
	std:printf(preamble.c_str());

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project 1: Heightmap", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// build and compile our shader program (defined in shader.hpp)
	// ------------------------------------
	Shader ourShader(projectAssetPath("Project_1/Shaders/shader.vert").c_str(),
		projectAssetPath("Project_1/Shaders/shader.frag").c_str());

	// set up vertex data (and buffer(s)) and configure vertex attributes for boxes
	// ------------------------------------------------------------------

	//   3D Coordinates   | Texture Coordinates
	//    x     y     z   |  s     t  
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	//  1.  Create ID / Generate Buffers and for Vertex Buffer Object (VBO), 
	//      Vertex Array Buffer (VAO), and the Element Buffer Objects (EBO)
	unsigned int VBO, VAO, EBO;

	// 2. Bind Vertex Array Object
	glGenVertexArrays(1, &VAO);

	//  Bind the Vertex Buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	// 3. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. Copy our indices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 5.  Position attribute for the 3D Position Coordinates and link to position 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 6.  TexCoord attribute for the 2d Texture Coordinates and link to position 2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Give the skybox its own vertex data so its UV rotation does not affect the boxes.
	float skyboxVertices[sizeof(vertices) / sizeof(vertices[0])];
	for (unsigned int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); ++i)
		skyboxVertices[i] = vertices[i];

	// Face order: back, front, left, right, bottom, top
	const float skyboxFaceRotationDegrees[6] = { 0.0f, 0.0f, 90.0f, 90.0f, 90.0f, 90.0f };	//Had to rotate some of the faces to make the skybox loook normal
	const bool skyboxFaceMirrored[6] = { true, false, false, false, true, false };
	for (unsigned int face = 0; face < 6; ++face)	// Rotating the UV coordinates of each face of the skybox to match the orientation of the texture images
	{
		const float angle = glm::radians(skyboxFaceRotationDegrees[face]);
		const float c = std::cos(angle);
		const float s = std::sin(angle);
		for (unsigned int vertex = 0; vertex < 6; ++vertex)
		{
			const unsigned int uv = (face * 6 + vertex) * 5 + 3;
			const float originalU = skyboxVertices[uv];
			const float v = skyboxVertices[uv + 1];
			const float u = skyboxFaceMirrored[face] ? 1.0f - originalU : originalU;
			const float x = u - 0.5f;
			const float y = 0.5f - v;
			skyboxVertices[uv] = c * x + s * y + 0.5f;
			skyboxVertices[uv + 1] = 0.5f + s * x - c * y;
		}
	}

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	unsigned int box_texture = loadTexture(projectAssetPath("Project_1/Media/textures/container.jpg").c_str());
	unsigned int smile_texture = loadTexture(projectAssetPath("Project_1/Media/textures/awesomeface.png").c_str());
	unsigned int front_texture = loadTexture(projectAssetPath("Project_1/Media/skybox/front.jpg").c_str());
	unsigned int back_texture = loadTexture(projectAssetPath("Project_1/Media/skybox/back.jpg").c_str());
	unsigned int bottom_texture = loadTexture(projectAssetPath("Project_1/Media/skybox/bottom.jpg").c_str());
	unsigned int right_texture = loadTexture(projectAssetPath("Project_1/Media/skybox/left.jpg").c_str());
	unsigned int left_texture = loadTexture(projectAssetPath("Project_1/Media/skybox/right.jpg").c_str());
	unsigned int top_texture = loadTexture(projectAssetPath("Project_1/Media/skybox/top.jpg").c_str());


	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
					 // either set it manually like so:
	//Setting it using the texture class
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// Choose the heightmap image and terrain texture here. Place a custom terrain
	// texture in Project_1/Media/textures and update terrainTexturePath.
	//hflab4.jpg, spiral.jpg,heightmap1.jpeg
	const char* heightmapImagePath = "Project_1/Media/heightmaps/hflab4.jpg";
	//PrototypeHeightMapTexture.png, Texture1.png, dirtTexture.jpg
	const char* terrainTexturePath = "Project_1/Media/textures/Texture1.png";
	unsigned int terrainTexture = loadTexture(projectAssetPath(terrainTexturePath).c_str());
	Heightmap heightmap(projectAssetPath(heightmapImagePath).c_str(), 80.0f, 12.0f, -48.0f);
	
	

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		xRotation += xRotationRate * deltaTime;
		yRotation += yRotationRate * deltaTime;
		zRotation += zRotationRate * deltaTime;

		// render
		// ------
		// Set background color (shouldn't need it in the end b/c the box should fully cover everything
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units in fragment shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, box_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, smile_texture);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		
		// render boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model(1.0f);
			// Translate the model to the cube starting position
			model = glm::translate(model, cubePositions[i] + glm::vec3(xPosition, yPosition, zPosition));
			// Keep each box's original X rotation and add the user-controlled rotations.
			model = glm::rotate(model, glm::radians(20.0f * i + xRotation), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(xScale, yScale, zScale));

			// Set model in shader
			ourShader.setMat4("model", model);

			// Draw the box with triangles
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw the six skybox faces
		const unsigned int skyboxTextures[] = {
			back_texture, front_texture, left_texture,
			right_texture, bottom_texture, top_texture
		};
		// Set the model matrix for the skybox to be large and centered around the camera
		glm::mat4 skyboxModel(1.0f);
		skyboxModel = glm::translate(skyboxModel, glm::vec3(0.0f, 0.0f, 1.0f));
		skyboxModel = glm::scale(skyboxModel, glm::vec3(100.0f));
		ourShader.setMat4("model", skyboxModel);
		const glm::mat4 skyboxView = glm::mat4(glm::mat3(view));	//so the skybox doesnt translate when the camera translates, only rotates
		ourShader.setMat4("view", skyboxView);

		glBindVertexArray(skyboxVAO);
		for (unsigned int face = 0; face < 6; ++face)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, skyboxTextures[face]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, skyboxTextures[face]);
			glDrawArrays(GL_TRIANGLES, face * 6, 6);
		}
		ourShader.setMat4("view", view);	// Restore the view matrix for the rest of the scene

		// Draw the textured terrain below the skybox center.
		heightmap.Draw(ourShader, terrainTexture);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &skyboxVBO);
	heightmap.Cleanup();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Add other key operations here.  
	// Controls for transformations
	bool scaleable = false;
	bool translation = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			scaleable = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		{
			translation = true;
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)	//U
		{
			if (scaleable)
			{
				xScale += 0.5f * deltaTime;
				//std::cout << "Scaleable = true" << std::endl;
			}
			else if (translation)
			{
				xPosition += 0.5f * deltaTime;
				//std::cout << "Translation = true" << std::endl;
			}
			else
			{
				xRotationRate += rotationRateStep * deltaTime;
				//std::cout << "Increase Rotation Rate in X axis " << std::endl;
			}
			
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)	//J
		{
			if (scaleable)
			{
				xScale = std::max(0.0f, xScale - 0.5f * deltaTime);
				//std::cout << "Scaleable = true" << std::endl;
			}
			else if (translation)
			{
				xPosition -= 0.5f * deltaTime;
				//std::cout << "Translation = true" << std::endl;
			}
			else
			{
			xRotationRate = std::max(0.0f, xRotationRate - rotationRateStep * deltaTime);
			//std::cout << "Decrease Rotation Rate in X axis" << std::endl;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)	//I
		{
			if (scaleable)
			{
				yScale += 0.5f * deltaTime;
				//std::cout << "Scaleable = true" << std::endl;
			}
			else if (translation)
			{
				yPosition += 0.5f * deltaTime;
				//std::cout << "Translation = true" << std::endl;
			}
			else
			{
			yRotationRate += rotationRateStep * deltaTime;
			//std::cout << "Increase Rotation Rate in Y axis" << std::endl;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)	//K
		{
			if (scaleable)
			{
				yScale = std::max(0.0f, yScale - 0.5f * deltaTime);
				//std::cout << "Scaleable = true" << std::endl;
			}
			else if (translation)
			{
				yPosition -= 0.5f * deltaTime;
				//std::cout << "Translation = true" << std::endl;
			}
			else
			{
			yRotationRate = std::max(0.0f, yRotationRate - rotationRateStep * deltaTime);
			//std::cout << "Decrease Rotation Rate in Y axis" << std::endl;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)	//O
		{
			if (scaleable)
			{
				zScale += 0.5f * deltaTime;
				//std::cout << "Scaleable = true" << std::endl;
			}
			else if (translation)
			{
				zPosition += 0.5f * deltaTime;
				//std::cout << "Translation = true" << std::endl;
			}
			else
			{
			zRotationRate += rotationRateStep * deltaTime;
			//std::cout << "Increase Rotation Rate in Z axis" << std::endl;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)	//L
		{
			if (scaleable)
			{
				zScale = std::max(0.0f, zScale - 0.5f * deltaTime);
				//std::cout << "Scaleable = true" << std::endl;
			}
			else if (translation)
			{
				zPosition -= 0.5f * deltaTime;
				//std::cout << "Translation = true" << std::endl;
			}
			else
			{
			zRotationRate = std::max(0.0f, zRotationRate - rotationRateStep * deltaTime);
			//std::cout << "Decrease Rotation Rate in Z axis" << std::endl;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)	//R
		{
			// Reset all rotation rates to 1.0f
			xRotationRate = 0.0f;
			yRotationRate = 0.0f;
			zRotationRate = 0.0f;
			xRotation = 0.0f;
			yRotation = 0.0f;
			zRotation = 0.0f;
			// Reset scale
			xScale = 1.0f;
			yScale = 1.0f;
			zScale = 1.0f;
			//Reset positions
			xPosition = 0.0f;
			yPosition = 0.0f;
			zPosition = 0.0f;
			//std::cout << "Reset all transformations" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)	//P
		{
			xScale += 0.5f * deltaTime;
			yScale += 0.5f * deltaTime;
			zScale += 0.5f * deltaTime;
			//std::cout << "Scaleable = true" << std::endl;
		}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



// utility function for loading a 2D texture from file - Note, you might want to create another function for this with GL_CLAMP_TO_EDGE for the texture wrap param
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


