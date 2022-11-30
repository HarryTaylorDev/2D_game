#include <windows.h>		// Header File For Windows

/*	nvImage.h contains all of the function prototypes needed for image loading.
	nvImage.h includes glew.h and so we don't need to include gl and glu header files for OpenGL
*/
#include "Image_Loading/nvImage.h"
#include <chrono>
#include <stdio.h>
#include <iostream>
#include "GameObject.h"
#include <vector>
#include "OBB.h"
#include "Level.h"
#include "GameCharacter.h"
#include "GameEnemy.h"
#include "TextBox.h"
#include "TriggerBox.h"
#include "FadeEffect.h"
#include "Arrow.h"


#define CDS_FULLSCREEN 4

/**********************************************************************************************/

//initialise primitive variables
double deltaTime = 0.0;
float timePast = 0.0;
int screenWidth = 1280, screenHeight = 720;
bool keys[256];
bool debug = false;
int stage = 0;
int exitX = 97;
int exitY = 97;
int selection = 0;
int levelCount = 0;
bool restart = false;

//initialise objects
std::vector<Level*> levels;
Level				menuLevel;
Level				levelList[3];
Level				customLevel;
GameCharacter		player;
GameEnemy			enemy;
GameCharacter*		winner;
TextBox				mainMenu;
TextBox				end;
TextBox				winnerText;
TextBox				first;
TextBox				second;
TextBox				player_head;
TextBox				enemy_head;
FadeEffect			fade;
Arrow				directionArrow;
TextBox				go;



std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::steady_clock::now();
void reshape(int width, int height);   //called when the window is resized

void init()
{
	//set background colour
	glClearColor(0.0755, 0.0755, 0.0755, 1.0);

	//play the game music
	PlaySound("game_song.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

	//make and open the consol for testing
	//AllocConsole();
	//std::freopen("conin$", "r", stdin);
	//std::freopen("conout$", "w", stdout);
	//std::freopen("conout$", "w", stderr);

	//create player and level objects

	menuLevel		=	Level();
	levelList[0]	=	Level("level_one.csv",	 exitX, exitY);
	levelList[1]	=	Level("level_two.csv",	 exitX, exitY);
	levelList[2]	=	Level("level_three.csv", exitX, exitY);
	customLevel		=	Level("custom.csv", exitX, exitY);
	player			=	GameCharacter(1.0f, 1.0f, 2.0f, "player.png");
	mainMenu		=	TextBox(2.75, 1.2, 6, 7.5,		"MainMenu.png");
	end				=	TextBox(6, 2, 6, 7,			"endScreen.png");
	go				=	TextBox(7.6, 6.5, 3, 3,			"go.png",3);
	first			=	TextBox(0.25, 8.5, 1.5, 1.5,	"1st.png");
	second			=	TextBox(0.25, 7, 1.5, 1.5,		"2nd.png");
	player_head		=	TextBox(1.65, 8.5, 1.8, 1.8,	"player_hud.png");
	enemy_head		=	TextBox(1.65, 7.5, 1.8, 1.8,	"player_two_hud.png");
	
	//set the players velocity to 1 to animate it for the menu screen
	player.Xvelocity = 1;
}

void processKeys()
{
	//update the player and enemys movement
	player.updateMovment(enemy, *levels[levelCount], keys[0x57], keys[0x53], keys[0x41], keys[0x44], deltaTime);
	enemy.updateMovment(player, *levels[levelCount], deltaTime);

} 

float count = 0.0;
int menuFrame(double deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//draw the menu text
	glPushMatrix();
		mainMenu.drawText();
	glPopMatrix();

	//draw the level tiles
	glPushMatrix();
		//translate the level tiles up 
		glTranslatef(11.0, count, 0.0);
		menuLevel.drawLevel();
	glPopMatrix();

	//draw the player
	glPushMatrix();
		glTranslatef(4.4, 0.0, 0.0);
		player.draw(deltaTime);
	glPopMatrix();

	//update the shift in level tiles
	count += 0.8 * deltaTime;
	//if the shift is greater than the size of a tile reset to 0
	if (count > 0) {
		count = -1;
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//return the input from the user, -1 if no input, 1 for defualt level and 2 for custom level
	return selection;
}

GameCharacter* gameFrame(double deltaTime) {
	//Set the background colour
	glClear(GL_COLOR_BUFFER_BIT);

	//Load identity matrix, enable textures and blending
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//draw the level tiles
	glPushMatrix();
		//Translate the level by the offset of the character  
		//and then by the opposite of the movement of the character
		glTranslatef(-player.worldPosX + player.spritePosX + (player.spriteSize - player.worldSizeX) / 2, -player.worldPosY + player.spritePosY + player.worldSizeY, 0.0);
		levels[levelCount]->drawLevel(player.worldPosX, player.worldPosY);
	glPopMatrix();

	//draw player and the enemy
	glPushMatrix();
		//check who is higher up in the y direction so the lower character is drawn on top

		//draw the enemy on top
		if (player.worldPosY > enemy.worldPosY) {
			glPushMatrix();
				player.draw(deltaTime);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-player.worldPosX + player.spritePosX + (player.spriteSize - player.worldSizeX) / 2, -player.worldPosY + player.spritePosY + player.worldSizeY, 0.0);
				enemy.draw(deltaTime);
			glPopMatrix();
		}
		//draw the player on top
		else {
			glPushMatrix();
			glTranslatef(-player.worldPosX + player.spritePosX + (player.spriteSize - player.worldSizeX) / 2, -player.worldPosY + player.spritePosY + player.worldSizeY, 0.0);
				enemy.draw(deltaTime);
			glPopMatrix();

			glPushMatrix();
				player.draw(deltaTime);
			glPopMatrix();
		}
	glPopMatrix();

	//draw the direction arrow in the top right
	glPushMatrix();
		//update the angle between the player and the end goal
		directionArrow.updateTurn(player.worldPosX+player.worldSizeX/2, player.worldPosY+ player.worldSizeY/2, levels[levelCount]->levelExit.posX+0.5, levels[levelCount]->levelExit.posY+0.5);
		directionArrow.draw();
	glPopMatrix();

	//draw the leader board
	glPushMatrix();
		//draw the places
		first.drawText();
		second.drawText();
	
		//player is closest
		if (player.distance(*levels[levelCount]) < enemy.distance(*levels[levelCount])) {			
			player_head.posY = 8.35;
			enemy_head.posY = 6.9;
		}
		//enemy is closest
		else {
			player_head.posY = 6.9;
			enemy_head.posY = 8.35;
		}
		//draw the player heads
		player_head.drawText();
		enemy_head.drawText();
	glPopMatrix();


	//disable textures and blending
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//return a pointer to a player is they are colliding with the level exit
	if (player.colliding(levels[levelCount]->levelExit,0,0))
	{
		return &player;
	}
	if (enemy.colliding(levels[levelCount]->levelExit, 0, 0))
	{
		return &enemy;
	}
	//return null if there is no winner
	return nullptr;
}

bool endFrame(double deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//draw the end screen text and the winning character
	glPushMatrix();
		end.drawText();
		winner->draw(deltaTime);
		winnerText.drawText();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//return true if the user press 1
	if (restart) {
		return true;
	}

	//return flase otherwise
	return false;
}

float alpha = 1.0;
bool startGame;
int levelStage = 0;

void display()
{
	
	//work out time since last frame
	auto start = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = start - oldTime;
	deltaTime = elapsed.count();
	oldTime = start;

	//calculate time passed
	timePast += deltaTime;

	//Stages of the game
	//0 is the menu screen
	//1 is running a level
	//2 is fading out from the level
	//3 is fading into the end screen
	//4 is the endscreen
	if (stage == 0) {
		//display the menu screen and get the users input
		int menuReply = menuFrame(deltaTime);

		//if there is an input set up levels
		if (menuReply!=0) {
			//remove any previous levels
			levels.clear();
			//add the defualt levels if the users pressed one
			if (menuReply == 1) {
				levels.push_back(&levelList[0]);
				levels.push_back(&levelList[1]);
				levels.push_back(&levelList[2]);
			}
			//add the custom level if the users pressed two
			if (menuReply == 2) {
				levels.push_back(&customLevel);
			}

			//reset game values/ objects
			levelCount = 0;
			levelStage = 0;
			stage = 1;

			player = GameCharacter(2.0f, 3.0f, 2.0f, "player.png");
			enemy = GameEnemy(3.0f, 2.0f, 2.0f, "player_two.png", *levels[levelCount]);
			fade = FadeEffect(true);
			directionArrow = Arrow(2, 2, 0);
			selection = 0;
		}
	}
	else if (stage == 1) {
		//set the debug mode on and off for the players and level
		levels[levelCount]->setDebug(debug);
		player.debug = debug;
		enemy.debug = debug;

		//Level stages control the loop of levels and the fades
		//0 is fading into a level
		//1 is playing the level
		//2 is fading out from the level
		if (levelStage == 0) {
			//draw the game frame
			gameFrame(deltaTime);
			//draw the fade and only entre if the fade is done
			if (fade.drawFade(deltaTime)) {
				//reset level values
				go.dead = false;
				go.timer = 3;
				levelStage = 1;
			}
		}
		else if (levelStage == 1) {
			//proccess player input and ai movement
			processKeys();
			//display the game frame and get if there is a winner
			winner = gameFrame(deltaTime);

			glPushMatrix();
			//draw the go text as long as it is not dead
			if (!go.dead) {
				go.drawText(deltaTime);
			}
			glPopMatrix();

			//if there is a winner move on
			if (winner!=nullptr) {
				//if the winner is the player move on to the next level stage
				if (winner == &player) {
					levelStage = 2;
					fade = FadeEffect(false);
				}
				//else skip to the end screen fade
				else {
					stage = 2;
					fade = FadeEffect(false);
				}
			}
		}
		else if (levelStage == 2) {
			//draw the game frame
			gameFrame(deltaTime);
			//continue if the fade has finished
			if (fade.drawFade(deltaTime)) {
				//move to the first level stage
				levelStage = 0;
				//increment the level counter
				levelCount++;
				//reset the fade
				fade = FadeEffect(true);

				//set the players values for the winning screen
				player.spritePosX = 8.1;
				player.spritePosY = 4.0;
				player.vertical = true;
				player.Xvelocity = 0.0;
				player.Yvelocity = 0.0;
				winnerText = TextBox(6.1, 5.5, 6, 2, "you_win.png");
				
				//go to the end screen if there is no levels left
				if (levelCount > levels.size()-1) {
					stage = 3;
					restart = false;	
				} 
				//reset the players for the next level
				else{
					player.reset(2,3);
					enemy.reset(3, 2, *levels[levelCount]);
				}
			}
		}
	}
	else if (stage == 2) {
		//draw the gaaem frame
		gameFrame(deltaTime);
		//continue if the fade has finished
		if (fade.drawFade(deltaTime)) {
			//set the enemys values for the winning screen
			enemy.spritePosX = 8.1;
			enemy.spritePosY = 4.0;
			enemy.vertical = true;
			enemy.Xvelocity = 0.0;
			enemy.Yvelocity = 0.0;
			winnerText = TextBox(6.1, 5.5, 6, 2, "blue_Win.png");

			//reset the fade
			fade = FadeEffect(true);
			//set the next stage
			stage = 3;
			//reset the end screen input
			restart = false;
		}
	}
	else if (stage == 3) {
		//draw the end screen
		endFrame(deltaTime);
		//draw the fade and update the stage when it is finished
		if (fade.drawFade(deltaTime)) {
			stage = 4;
		}

	}
	else if (stage == 4) {
		//draw the end screen, continue if the user presses 1
		if (endFrame(deltaTime)) {
			//reset the game stage and set the menu player as the winner
			stage = 0;
			player = *winner;
			//give them 1 velocity to animate them as running
			player.Xvelocity = 1;
			selection = 0;
			
		}
	}

	//print test info to console
	if (timePast > 2)
	{
		std::cout << "fps = " << 1 / deltaTime << std::endl;
		std::cout << "velocity = " << player.Xvelocity << std::endl;
		std::cout << "x = " << player.worldPosX << std::endl;
		std::cout << "y = " << player.worldPosY << std::endl;
		timePast = 0.0;
	}
}


/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height, int bits, bool fullscreen); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
bool fullscreen = true;
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	//{
	fullscreen = FALSE;                       // Windowed Mode
//}

// Create Our OpenGL Window
	if (!CreateGLWindow("Racing Game", 1280/*GetSystemMetrics(SM_CXSCREEN)*/, 720/*GetSystemMetrics(SM_CYSCREEN)*/, GetDeviceCaps(GetWindowDC(GetDesktopWindow()), BITSPIXEL), fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{

			display();								// Draw The Scene
			SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;
	}
	break;

	case WM_LBUTTONDOWN:
	{
	}
	break;

	case WM_LBUTTONUP:
	{
	}
	break;

	case WM_MOUSEMOVE:
	{
	}
	break;
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = true;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = false;					// If So, Mark It As FALSE
		if (wParam == 0x42) {
			if (debug) {
				debug = false;
			}
			else {
				debug = true;
			}
		}
		if (wParam == 0x31) {
			selection = 1;
			restart = true;
		}
		if (wParam == 0x32) {
			selection = 2;
			
		}
		//close the program if the user hits escape
		if (wParam== VK_ESCAPE) {
			PostQuitMessage(0);
		}
		return 0;								// Jump Back
	}
	break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/

bool CreateGLWindow(char* title, int width, int height, int bits, bool fullscreen)
{
	GLuint		PixelFormat;				// Holds The Results After Searching For A Match
	WNDCLASS	wc;							// Windows Class Structure
	DWORD		dwExStyle;					// Window Extended Style
	DWORD		dwStyle;					// Window Style
	RECT		WindowRect;					// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;				// Set Left Value To 0
	WindowRect.right = (long)width;			// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);							// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;							// WndProc Handles Messages
	wc.cbClsExtra = 0;											// No Extra Window Data
	wc.cbWndExtra = 0;											// No Extra Window Data
	wc.hInstance = hInstance;									// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);						// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;										// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	if (fullscreen)														// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;										// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));			// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);				// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;							// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;							// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;							// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;


		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "Racing game", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;               // Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;                   // Exit And Return FALSE
			}
		}
	}

	if (fullscreen)							// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;		// Window Extended Style
		dwStyle = WS_POPUP;					// Windows Style
		ShowCursor(FALSE);					// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;				// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;								// Windows Style
	}


	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		//below is 16 
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window

	init();

	return true;									// Success
}

void reshape(int width, int height)							// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;				// to ensure the mouse coordinates match 
															// we will use these values to set the coordinate system

	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Modelview Matrix


	glViewport(0, 0, width, height);						// Reset The Current Viewport


	glLoadIdentity();										// Reset The Projection Matrix

	float aspect_ratio = static_cast<float>(width) / height;
	if (width <= height) {
		gluOrtho2D(0, 10.0f, 0.0f / aspect_ratio, 10.0f / aspect_ratio);
	}
	else {
		gluOrtho2D(0 * aspect_ratio, 10.0f * aspect_ratio, 0.0, 10.0f);
	}														// set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix

}

