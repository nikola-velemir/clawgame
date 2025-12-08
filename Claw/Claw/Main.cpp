#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Util.h"
#include "Claw.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <chrono>
#include "Box.h"
#include "Toy.h"
#include "ClawState.h"
#include "Lights.h"
#include "GameData.h"
#include "Overlay.h"
#include "TokenInsert.h"
#include "NameplateHelper.h"

#define REFRESH_RATE 75.0
#define GRAVITY -0.005f
#define BOX_TRANSPARENCY 0.4f

GLuint preprocessTexture(unsigned& texture, const char* filepath);
void onExitCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void adjustClawOpenAngle();
void calculateClawMovement();
float computeStopHeight(const Toy& t, float worldWidth, float dropZoneDepth, float clawDepth, float toyHeight);
int endProgram(std::string message);
void onMouseClick(GLFWwindow* window, int button, int action, int mods);
void checkTokenInsert(float wx, TokenInsert& tokenInsert, float wy, Light* light);
void checkToyCollected(GameData* data, float wx, float wy, Light* light);

const float  worldHeight = 1.0f;
float worldWidth;
int fbWidth, fbHeight;
ClawState clawState = ClawState::Off;

bool close = false;
bool isOpen = false;
const float clawDepth = 0.4f;
const float dropZoneDepth = 0.8f;
const float clawSpeed = 0.01f;
bool blinking = false;
double blinkStartTime = 0.0;
Claw claw;
const double targetFrameTime = 1.0 / REFRESH_RATE;

float uY = 0.0f;
int main()
{
	// Inicijalizacija GLFW i postavljanje na verziju 3 sa programabilnim pajplajnom
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Formiranje prozora za prikaz sa datim dimenzijama i naslovom
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor) {
		std::cerr << "No monitor found!" << std::endl;
		return -1;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Claw", monitor, nullptr);
	if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	float aspect = static_cast<float>(fbWidth) / fbHeight;

	worldWidth = worldHeight * aspect;
	glm::mat4 projection = glm::ortho(-worldWidth, worldWidth, -worldHeight, worldHeight, -1.0f, 1.0f);


	// Inicijalizacija GLEW
	if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetKeyCallback(window, onExitCallback);

	LoadFont("NotoSans_Condensed-Regular.ttf", 48);
	InitTextRenderer();
	InitNameplateShader();

	unsigned int ropeShader = createShader("rope.vert", "basic.frag");
	unsigned int basicShader = createShader("basic.vert", "basic.frag");
	unsigned int fingerShader = createShader("finger.vert", "basic.frag");
	unsigned int texShader = createShader("tex.vert", "tex.frag");

	Overlay overlay = createOverlay(worldWidth, worldHeight);
	Light light = createLight(-0.5f, 0.9f, 0.25f, 0.15f, worldWidth, worldHeight);

	claw = createClaw(worldWidth, worldHeight);

	Box box = createBox(worldWidth, worldHeight);
	Toy toys[] = {
		createToy(-0.5, -0.3, 0.25,0.25, worldWidth, worldHeight, "teddy.png"),
		createToy(0.5, -0.3, 0.25,0.25, worldWidth, worldHeight, "dino.png"),
	};

	TokenInsert tokenInsert = createTokenInsert(0.0f, -0.75f, 0.15f, 0.45f, worldWidth, worldHeight);

	GameData data = { toys, 2 };
	data.light = &light;
	data.tokenInsert = &tokenInsert;

	glfwSetWindowUserPointer(window, &data);
	glfwSetMouseButtonCallback(window, onMouseClick);
	glClearColor(0.3f, 0.1f, 0.6f, 1.0f); // Postavljanje boje pozadine
	double lastFrame = glfwGetTime();

	GLFWcursor* tokenCursor = loadImageToCursor("token.png", 75, 75);
	GLFWcursor* leverCursor = loadImageToCursor("lever.png", 75, 75);

	while (!glfwWindowShouldClose(window) && !close)
	{
		if (clawState == ClawState::Off)
			glfwSetCursor(window, tokenCursor);
		else
			glfwSetCursor(window, leverCursor);
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastFrame;
		if (deltaTime < targetFrameTime) continue;

		calculateClawMovement();
		adjustClawOpenAngle();
		if (clawState != ClawState::Off) {
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				if (clawState != ClawState::Dropping && clawState != ClawState::Retracting)
				{

					float newX = claw.uX + clawSpeed;
					if (newX > -0.8 * worldWidth && newX < 0.8 * worldWidth)
						claw.uX = newX;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				if (clawState != ClawState::Dropping && clawState != ClawState::Retracting)
				{
					float newX = claw.uX - clawSpeed;

					if (newX > -0.8 * worldWidth && newX < 0.8 * worldWidth)
						claw.uX = newX;
				}

			}
		}
		// Grab toy if claw is dropping and reaches the toy
		if (clawState == ClawState::Dropping && !claw.carryingToy) {

			float clawPalmX = claw.x + claw.uX;
			float clawPalmY = claw.y + claw.uY;

			for (Toy& t : toys) {

				float toyX = t.x + t.uX;
				float toyY = t.y + t.uY;

				float dx = fabs(toyX - clawPalmX);
				float dy = fabs(toyY - clawPalmY);

				const				float horizontalRange = 0.15f;
				const float verticalRange = 0.1f;
				if (dx < horizontalRange && dy < verticalRange) {

					claw.carryingToy = true;
					claw.grabbedToy = &t;
					t.state = ToyState::Grabbed;
					isOpen = false;

					break;
				}
			}
		}

		// Gravity for dropped toys
		for (Toy& t : toys) {
			if (t.state != ToyState::Falling) continue;
			t.velocityY += GRAVITY;    // accelerate down
			t.uY += t.velocityY;       // update position
			// --- ADD THIS ---
			float toyWorldY = t.y + t.uY;
			const float dropZoneThreshold = -0.45f;
			if (toyWorldY < dropZoneThreshold) {
				isOpen = false;
			}
			// ----------------

			float stopHeight = computeStopHeight(t, worldWidth, dropZoneDepth, clawDepth, t.height);


			if (t.uY <= stopHeight) {
				t.uY = stopHeight;
				t.state = ToyState::Idle;
				t.velocityY = 0.0f;
			}

		}

		// Move grabbed toy with claw
		if (claw.carryingToy && claw.grabbedToy != nullptr) {
			float palmX = claw.x + claw.uX;
			float palmY = claw.y + claw.uY - 0.15f;

			Toy* t = claw.grabbedToy;

			t->uX = palmX - t->x;
			t->uY = palmY - t->y;
		}
		glClear(GL_COLOR_BUFFER_BIT); // Bojenje pozadine
		claw.render(basicShader, ropeShader, fingerShader, projection, aspect);
		light.render(texShader, projection, aspect);
		box.renderClaimZone(basicShader, projection);

		for (Toy& t : toys) {
			if (t.state == ToyState::Collected) continue;
			t.render(texShader, projection, aspect);
			if (t.x + t.uX >= 0.6f && t.y + t.uY <= -0.7f) {
				if (!blinking) {
					
					blinking = true;
					blinkStartTime = glfwGetTime(); // Start blinking
				}
			}
		}

		box.renderBottom(basicShader, projection);

		box.renderDropZone(basicShader, projection);
		box.renderClaimZoneStrip(basicShader, projection);
		box.renderTop(basicShader, projection);

		box.renderLeft(basicShader, projection);

		box.renderRight(basicShader, projection);
		if (blinking) {
			double elapsed = glfwGetTime() - blinkStartTime;
			// Alternate every 0.5 seconds
			if (fmod(elapsed, 1.0) < 0.5)
				light.currentState = LightState::Red;
			else
				light.currentState = LightState::Green;
		}
		overlay.render(texShader, projection, aspect, BOX_TRANSPARENCY);

		tokenInsert.render(texShader, projection, aspect);
		DrawNameplateWorld("Nikola Velemir SV8/2022", projection, worldWidth, worldHeight);

		glfwSwapBuffers(window); // Zamena bafera - prednji i zadnji bafer se menjaju kao štafeta; dok jedan procesuje, drugi se prikazuje.
		glfwPollEvents(); // Sinhronizacija pristiglih događaja
		lastFrame = currentTime;

	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void calculateClawMovement()
{
	if (clawState == ClawState::Dropping) {
		if (claw.uY + claw.y > -clawDepth)
		{
			claw.uY -= 0.01;
		}
		else {
			clawState = ClawState::Retracting;
		}
	}
	if (clawState == ClawState::Retracting) {
		if (claw.uY < 0) {

			claw.uY += 0.01;
		}
		else {
			clawState = ClawState::Idle;
		}
	}
}

void adjustClawOpenAngle()
{
	if (isOpen) {
		if (claw.openingAngle <= 1.0f)
			claw.openingAngle += 0.02f;
	}
	else {

		if (claw.openingAngle > 0.0f) {
			claw.openingAngle -= 0.02f;
		}
	}
}

void onExitCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		close = true;
	}
	if (clawState == ClawState::Off) return;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {

		// If claw is holding a toy, release it
		if (claw.carryingToy && clawState != ClawState::Retracting) {
			claw.carryingToy = false;
			isOpen = true;
			if (claw.grabbedToy != nullptr) {
				Toy* t = claw.grabbedToy;
				t->state = ToyState::Falling;
				claw.grabbedToy = nullptr;

			}
			clawState = ClawState::Idle;
			return; // Important: don’t start dropping again
		}

		// If claw is free, start dropping
		if (clawState != ClawState::Dropping && clawState != ClawState::Retracting) {
			clawState = ClawState::Dropping;
		}
	}
}


int endProgram(std::string message) {
	std::cout << message << std::endl;
	glfwTerminate();
	return -1;
}


GLuint preprocessTexture(unsigned& texture, const char* filepath) {
	texture = loadImageToTexture(filepath); // Učitavanje teksture
	glBindTexture(GL_TEXTURE_2D, texture); // Vezujemo se za teksturu kako bismo je podesili

	// Generisanje mipmapa - predefinisani različiti formati za lakše skaliranje po potrebi (npr. da postoji 32 x 32 verzija slike, ali i 16 x 16, 256 x 256...)
	glGenerateMipmap(GL_TEXTURE_2D);

	// Podešavanje strategija za wrap-ovanje - šta da radi kada se dimenzije teksture i poligona ne poklapaju
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S - tekseli po x-osi
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T - tekseli po y-osi

	// Podešavanje algoritma za smanjivanje i povećavanje rezolucije: nearest - bira najbliži piksel, linear - usrednjava okolne piksele
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}
float computeStopHeight(const Toy& t, float worldWidth, float dropZoneDepth, float clawDepth, float toyHeight) {
	bool inDropZone = (t.uX + t.x >= 0.5f * worldWidth);

	float computedStopHeight = inDropZone ? -dropZoneDepth : -clawDepth;
	return computedStopHeight + toyHeight / 2 - 0.05f;
}

void onMouseClick(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double mx, my;
		glfwGetCursorPos(window, &mx, &my);

		// Convert to world coordinates (assuming you have worldWidth/worldHeight)
		float wx = (mx / fbWidth) * 2.0f * worldWidth - worldWidth;
		float wy = worldHeight - (my / fbHeight) * 2.0f * worldHeight;

		GameData* data = (GameData*)glfwGetWindowUserPointer(window);

		Light* light = data->light;

		checkToyCollected(data, wx, wy, light);
		TokenInsert& tokenInsert = *(data->tokenInsert);
		checkTokenInsert(wx, tokenInsert, wy, light);

	}
}

void checkTokenInsert(float wx, TokenInsert& tokenInsert, float wy, Light* light)
{
	if (wx >= tokenInsert.x - tokenInsert.width / 2 && wx <= tokenInsert.x + tokenInsert.width / 2 &&
		wy >= tokenInsert.y - tokenInsert.height / 2 && wy <= tokenInsert.y + tokenInsert.height / 2) {
		// Coin inserted!
		blinking = false;            // Stop any blinking
		light->currentState = LightState::Blue; // Turn lamp blue
		clawState = ClawState::Idle;
		isOpen = true;               // Start game mode
	}
}

void checkToyCollected(GameData* data, float wx, float wy, Light* light)
{
	for (int i = 0; i < data->toyCount; i++) {
		Toy& t = data->toys[i];
		if (t.state == ToyState::Collected) continue;

		// Simple bounding-box check
		if (wx >= t.x + t.uX - 0.1f && wx <= t.x + t.uX + 0.1f &&
			wy >= t.y + t.uY - 0.1f && wy <= t.y + t.uY + 0.1f) {

			t.state = ToyState::Collected;  // Mark as collected
			blinking = false;
			isOpen = false;// Stop the lamp
			clawState = ClawState::Off;
			light->currentState = LightState::Empty;
		}
	}
}
