#define _CRT_SECURE_NO_WARNINGS
#define GLM_SWIZZLE

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <gl/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <gtc/matrix_transform.hpp>
#include <gtx/euler_angles.hpp>

#include "blockGroup.h"

GLfloat cube_positions[] = 
{
	 0.0f, 0.0f, 0.0f, 1.0f, // triangle 1 : begin
	 0.0f, 0.0f, 1.0f, 1.0f,
	 0.0f, 1.0f, 1.0f, 1.0f, // triangle 1 : end

	 1.0f, 1.0f, 0.0f, 1.0f, // triangle 2 : begin
	 0.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f, // triangle 2 : end

	 1.0f, 0.0f, 1.0f, 1.0f,
	 0.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 0.0f, 1.0f,

	 1.0f, 1.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 0.0f, 1.0f,

	 0.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 1.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f,

	 1.0f, 0.0f, 1.0f, 1.0f,
	 0.0f, 0.0f, 1.0f, 1.0f,
	 0.0f, 0.0f, 0.0f, 1.0f,

	 0.0f, 1.0f, 1.0f, 1.0f,
	 0.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 1.0f, 1.0f,

	 1.0f, 1.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 1.0f,

	 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 1.0f, 1.0f,

	 1.0f, 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f,

	 1.0f, 1.0f, 1.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 1.0f, 1.0f,

	 1.0f, 1.0f, 1.0f, 1.0f,
	 0.0f, 1.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 1.0f, 1.0f
};

GLfloat cube_normals[] =
{
    -1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	 0.0f, 0.0f,-1.0f,
	 0.0f, 0.0f,-1.0f,
	 0.0f, 0.0f,-1.0f,

	 0.0f,-1.0f, 0.0f,
	 0.0f,-1.0f, 0.0f,
	 0.0f,-1.0f, 0.0f,

	 0.0f, 0.0f,-1.0f,
	 0.0f, 0.0f,-1.0f,
	 0.0f, 0.0f,-1.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	 0.0f,-1.0f, 0.0f,
	 0.0f,-1.0f, 0.0f,
	 0.0f,-1.0f, 0.0f,

	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,

	 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f,

	 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f,

	 0.0f, 1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,

	 0.0f, 1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,

	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,
};

GLuint vao_cube, vbo_position, vbo_normal, vs, fs, cs, shader_programme, compute_programme;
glm::vec3 cameraPos;
glm::mat4 cameraRot;

bool keyState[256] = {};

float cameraArc = 60.0f, aspectRatio = 16.0f / 9.0f, cameraNear = 0.3f, cameraFar = 1000.0f;
float prevTime, nowTime;

std::vector<blockGroup *> blockGroupList;

int groupCountX = 10, groupCountZ = 10;
float lambdax = 20, lambdaz = 20, ax = 5, az = 5, px = 3, pz = 12;

int mousePrevX = -1, mousePrevY = -1;
float mouse_dx, mouse_dy;
float cameraRotY = 0.0f, cameraRotX = 0.0f;
float scalarSpeed = 5.0f;

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}

void setVSync(bool sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	bool flag = false;

	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (i = 0; i < n; i++) 
	{
		char* extensions = (char *)glGetStringi(GL_EXTENSIONS, i);
		printf("%s\n", extensions);

		if (strstr(extensions, "WGL_EXT_swap_control") != 0)
		{
			flag = true;
			break;
		}
	}

	if (flag == false)
	{
		return;
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(sync);
	}
}

//init glew & glut
void initGL(int *argc, char **argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitContextVersion(4, 4);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Hello triangle!");

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	setVSync(false);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.25, 0.25, 0.25, 1.0);

	glDisable(GL_CULL_FACE);

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glutReportErrors();
}

void initBlockGroups()
{
	glUseProgram(compute_programme);
	for (int x = 0; x < groupCountX; x++)
	{
		for (int z = 0; z < groupCountZ; z++)
		{
			blockGroup* grp = new blockGroup();
			grp->Init_sinXsinY(lambdax, lambdaz, px, pz, ax, az, x * 32.0f, z * 32.0f);
			grp->InitBuffers(compute_programme);
			grp->GenerateBuffer();

			blockGroupList.push_back(grp);
		}
	}
}

//init buffers
void initApp()
{
	//Camera position
	cameraPos.x = 0.0f;
	cameraPos.y = 16.0f;
	cameraPos.z = 32.0f;

	//VAO Creation

	vao_cube = 0;
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);

	//VBO Creation

	vbo_position = 0;
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions), cube_positions, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	vbo_normal = 0;
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	/*
	Before using the shaders we have to load the strings into a GL shader, and compile them.
	*/

	const char* vertex_shader = filetobuf("simpleInstance.vert");
	const char* fragment_shader = filetobuf("simpleInstance.frag");

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	/*
	Now, these compiled shaders must be combined into a single, executable GPU shader programme.
	We create an empty "program", attach the shaders, then link them together.
	*/
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	GLint testval = 0;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &testval);
	if (testval == GL_FALSE)
	{
		char infolog[1024];
		glGetShaderInfoLog(vs, 1024, NULL, infolog);
		printf("The vertex shader failed to compile with the error: %s \n", infolog);
	}

	glGetShaderiv(fs, GL_COMPILE_STATUS, &testval);
	if (testval == GL_FALSE)
	{
		char infolog[1024];
		glGetShaderInfoLog(fs, 1024, NULL, infolog);
		printf("The fragment shader failed to compile with the error: %s \n", infolog);
	}

	//==Compute shader==

	const char* compute_shader = filetobuf("rawIdToCubePos.compute");
	cs = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(cs, 1, &compute_shader, NULL);
	glCompileShader(cs);

	compute_programme = glCreateProgram();
	glAttachShader(compute_programme, cs);
	glLinkProgram(compute_programme);

	glGetShaderiv(cs, GL_COMPILE_STATUS, &testval);
	if (testval == GL_FALSE)
	{
		char infolog[1024];
		glGetShaderInfoLog(cs, 1024, NULL, infolog);
		printf("The compute shader failed to compile with the error: %s \n", infolog);
	}

	initBlockGroups();
}

//rendering stuff
void render()
{
	/*
	We draw in a loop. Each iteration draws the screen once;
	a "frame" of rendering. The loop finishes if the window is closed.
	Later we can also ask GLFW is the escape key has been pressed.
	*/

	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shader_programme);
	glBindVertexArray(vao_cube);

	//camera Pos
	glm::mat4 view = glm::inverse(glm::translate(glm::mat4(1.0f), cameraPos) * cameraRot);
	glm::mat4 proj = glm::perspective(cameraArc, 1.0f * aspectRatio, cameraNear, cameraFar);

	glUniformMatrix4fv(glGetUniformLocation(shader_programme, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader_programme, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	GLint modelUniformIndex = glGetUniformLocation(shader_programme, "model_group");

	for each (auto grp in blockGroupList)
	{
		grp->Draw(36, 2, modelUniformIndex);
	}

	//put the stuff we've been drawing onto the display
	glutSwapBuffers();
	glutPostRedisplay();
}

int frameCount = 0;

//update
void update()
{
	nowTime = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	float dTime = nowTime - prevTime;
	prevTime = nowTime;

	//cameraRot = glm::rotate(cameraRot, -mouse_dx * 180.0f * dTime, glm::vec3(0.0f, 1.0f, 0.0f));
	cameraRotY += -mouse_dx * 2.0f * dTime;
	cameraRotX += -mouse_dy * 2.0f * dTime;
	cameraRot = glm::eulerAngleYX(cameraRotY, cameraRotX);

	mouse_dx = 0;
	mouse_dy = 0;
	
	/*if (cameraRotY < 0)
		cameraRotY += 360.0f;
	else if (cameraRotY > 360.0f)
		cameraRotY -= 360.0f;

	if (cameraRotX < -180)
		cameraRotX = -180.0f;
	else if (cameraRotX > 180.0f)
		cameraRotX = 180.0f;*/

	glm::vec4 camMoveSpeed;

	camMoveSpeed.z += (float)(keyState['s'] - keyState['w']) * scalarSpeed * dTime;
	camMoveSpeed.x += (float)(keyState['d'] - keyState['a']) * scalarSpeed * dTime;
	camMoveSpeed.y += (float)(keyState['q'] - keyState['e']) * scalarSpeed * dTime;
	camMoveSpeed.w = 1.0f;

	cameraPos += (cameraRot * camMoveSpeed).xyz();

	glUseProgram(compute_programme);

	lambdax = 32.0f + 24.0f * sinf(nowTime * 3.0f);
	//lambdaz = 32.0f + 10.0f * sinf(nowTime * 1.0f);

	int start = glutGet(GLUT_ELAPSED_TIME);

	//for each (auto grp in blockGroupList)
	//{
	//	if ((rand() % 100 <= 100))
	//	{
	//		grp->Init_sinXsinY(lambdax, lambdaz, px, pz, ax, az, grp->blockGroupPos.x, grp->blockGroupPos.z);
	//	}
	//}

#pragma omp parallel for num_threads(8)
	for (int i = 0; i < blockGroupList.size(); i++)
	{
		blockGroupList.at(i)->Init_sinXsinY(
			lambdax, lambdaz, px, pz, ax, az, 
			blockGroupList.at(i)->blockGroupPos.x, blockGroupList.at(i)->blockGroupPos.z);
	}

	int dur = glutGet(GLUT_ELAPSED_TIME) - start;
	
	for each (auto grp in blockGroupList)
	{
		if (grp->bufferUpdated == false)
		{
			grp->GenerateBuffer(true);
		}
	}

	if (frameCount % 60 == 0)
	{
		char windowTitle[50];
		sprintf(windowTitle, "Voxel test - fps: %.2f; updateBlockId: %d ms", 1.0f / dTime, dur);
		glutSetWindowTitle(windowTitle);
	}

	frameCount++;
}

//clean up
void cleanup()
{

}

//reshape the viewport
void reshape(int w, int h)
{
	aspectRatio = (float)w / (float)h;
}

//mouse click (state change)?
void mouse(int button, int state, int x, int y)
{
	mousePrevX = x;
	mousePrevY = y;
}

//mouse move (with button clicked)
void motion(int x, int y)
{
	mouse_dx = (float)(x - mousePrevX);
	mouse_dy = (float)(y - mousePrevY);

	mousePrevX = x;
	mousePrevY = y;
}

//keyboard
void key(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		keyState['w'] = true;
	}
	if (key == 's' || key == 'S')
	{
		keyState['s'] = true;
	}
	if (key == 'd' || key == 'D')
	{
		keyState['d'] = true;
	}
	if (key == 'a' || key == 'A')
	{
		keyState['a'] = true;
	}
	if (key == 'q' || key == 'Q')
	{
		keyState['q'] = true;
	}
	if (key == 'e' || key == 'E')
	{
		keyState['e'] = true;
	}

	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
	{
		scalarSpeed = 12.0f;
	}
	else
	{
		scalarSpeed = 5.0f;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		keyState['w'] = false;
	}
	if (key == 's' || key == 'S')
	{
		keyState['s'] = false;
	}
	if (key == 'd' || key == 'D')
	{
		keyState['d'] = false;
	}
	if (key == 'a' || key == 'A')
	{
		keyState['a'] = false;
	}
	if (key == 'q' || key == 'Q')
	{
		keyState['q'] = false;
	}
	if (key == 'e' || key == 'E')
	{
		keyState['e'] = false;
	}
}

//For special keys like F1, arrow keys etc.
void special(int k, int x, int y)
{

}

int main(int argc, char **argv)
{
	initGL(&argc, argv);
	initApp();

	glutDisplayFunc(render);
	glutIdleFunc(update);

	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);

	glutSpecialFunc(special);

	glutCloseFunc(cleanup);

	glutMainLoop();

	return 0;
}