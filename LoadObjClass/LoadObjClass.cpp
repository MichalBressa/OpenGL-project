#include <cmath>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"
#include "vertex.h"
#include "sphere.h"
#include "Model.h"

#include "soil/SOIL.h"

using namespace std;
using namespace glm;

static enum object { SPHERE, ROOMMODEL, TABLEMODEL, TABLETOP, WHITEBALL, BLACKBALL, REDBALL, YELLOWBALL, CUE}; // VAO ids.

static enum buffer { SPHERE_VERTICES, SPHERE_INDICES, ROOMMODEL_VERTICES, TABLEMODEL_VERTICES, TABLETOP_VERTICIES, 
					WHITEBALL_VERTICES, BLACKBALL_VERTICES, REDBALL_VERTICES, YELLOWBALL_VERTICES, CUE_VERTICES}; // VBO ids.


struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

// Globals.
static Vertex fieldVertices[] =  
{
	{vec4(100.0, 0.0, 100.0, 1.0), vec2(8.0, 0.0)},
	{vec4(100.0, 0.0, -100.0, 1.0), vec2(8.0, 8.0)},
	{vec4(-100.0, 0.0, 100.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-100.0, 0.0, -100.0, 1.0), vec2(0.0, 8.0)}
}; 



// Calculate and update modelview matrix.
vec3 eye = vec3(0.0, 14.0, 20.0);
static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);  //create normal matrix


static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
// Front and back material properties.
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

// first light
static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.5, 1.0, 0.0, 0.0)
};

// second light
static const Light light1 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(-0.5, 1.0, 0.0, 0.0)
};

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   objectLoc,
	tableLoc,
	tableTopLoc,
	roomTexLoc,
	whiteballTexLoc,
	blackballTexLoc,
	redballTexLoc,
	yellowballTexLoc,
	cueTexLoc,
	buffer[10],		// buffer list
	vao[9],			//VAO List  
    texture[8];     //Texture List 

static Sphere testSphere;
vec3 movableBallPos = vec3 (0,0,0); // Z Co-ordinates of the ball.

// models
static Model RoomModel("room.obj");
static Model TableModel("table.obj");
static Model TableTop("tableTop.obj");
static Model WhiteBall("ball.obj");
static Model BlackBall("blackball.obj");
static Model YellowBall("yellowball.obj");
static Model RedBall("redball.obj");
static Model Cue("cueStick.obj");

// camera global variables
float cameraYaw = 90;
float cameraPitch;
float cameraForward = 1.5f;
float cameraPosition; //Camera position


// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 

   //codes for OpenGL lighting
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   // code for 1st light
   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

   // code for 2nd light
   glUniform4fv(glGetUniformLocation(programId, "light1.ambCols"), 1, &light1.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light1.difCols"), 1, &light1.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light1.specCols"), 1, &light1.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light1.coords"), 1, &light1.coords[0]);


   //-----------------VAO and VBO section-------------------------//

   //Create VAO and VBO
   glGenVertexArrays(1, &vao[SPHERE]);
   glGenBuffers(1, &buffer[SPHERE_VERTICES]);
   //Binding VAO and VBO
   testSphere.SetIDs(vao[SPHERE], buffer[SPHERE_VERTICES], buffer[SPHERE_INDICES]);
   testSphere.Setup();

   /////Create room //////
   glGenVertexArrays(1, &vao[ROOMMODEL]);
   glGenBuffers(1, &buffer[ROOMMODEL_VERTICES]);
   //Binding Track VAO and VBO
   RoomModel.SetIDs(vao[ROOMMODEL], buffer[ROOMMODEL_VERTICES], 0);
   RoomModel.Setup();


   /////Create table //////
   glGenVertexArrays(1, &vao[TABLEMODEL]);
   glGenBuffers(1, &buffer[TABLEMODEL_VERTICES]);
   //Binding Track VAO and VBO
   TableModel.SetIDs(vao[TABLEMODEL], buffer[TABLEMODEL_VERTICES], 0);
   TableModel.Setup();


   /////Create tableTop //////
   glGenVertexArrays(1, &vao[TABLETOP]);
   glGenBuffers(1, &buffer[TABLETOP_VERTICIES]);
   //Binding Track VAO and VBO
   TableTop.SetIDs(vao[TABLETOP], buffer[TABLETOP_VERTICIES], 0);
   TableTop.Setup();

   //////	 WHITEBALL
   glGenVertexArrays(1, &vao[WHITEBALL]);
   glGenBuffers(1, &buffer[WHITEBALL_VERTICES]);
   //Binding Track VAO and VBO
   WhiteBall.SetIDs(vao[WHITEBALL], buffer[WHITEBALL_VERTICES], 0);
   WhiteBall.Setup();


   /////	BLACKBALL
   glGenVertexArrays(1, &vao[BLACKBALL]);
   glGenBuffers(1, &buffer[BLACKBALL_VERTICES]);
   //Binding Track VAO and VBO
   BlackBall.SetIDs(vao[BLACKBALL], buffer[BLACKBALL_VERTICES], 0);
   BlackBall.Setup();


   //////	REDBALL
      glGenVertexArrays(1, &vao[REDBALL]);
   glGenBuffers(1, &buffer[REDBALL_VERTICES]);
   //Binding Track VAO and VBO
   RedBall.SetIDs(vao[REDBALL], buffer[REDBALL_VERTICES], 0);
   RedBall.Setup();


   //////	YELLOWBALL
   glGenVertexArrays(1, &vao[YELLOWBALL]);
   glGenBuffers(1, &buffer[YELLOWBALL_VERTICES]);
   //Binding Track VAO and VBO
   YellowBall.SetIDs(vao[YELLOWBALL], buffer[YELLOWBALL_VERTICES], 0);
   YellowBall.Setup();


   //////	CUE
   glGenVertexArrays(1, &vao[CUE]);
   glGenBuffers(1, &buffer[CUE_VERTICES]);
   //Binding Track VAO and VBO
   Cue.SetIDs(vao[CUE], buffer[CUE_VERTICES], 0);
   Cue.Setup();

   


   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat");   //uniform mat4 projMat;
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 3.0, 300.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   
   // Obtain modelview matrix uniform and object uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat");   //uniform mat4 modelViewMat;
   objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;

   // Load the images.
   std::string TexNames[] = {
	    "Textures/darkwood.jpg",
		"Textures/green.png",
		"Textures/roomTex.png",
		"Textures/white.jpg",
		"Textures/black.png",
		"Textures/red.png",
		"Textures/yellow.jpg"
		"Textures/cueTex.jpg"
   };	// last one for the cue

   // Create texture ids.
   glGenTextures(8, texture);

   int width, height;
   unsigned char* data;

   //load image data using SOIL library

   // Table texture
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);

   data = SOIL_load_image(TexNames[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   tableLoc = glGetUniformLocation(programId, "tableTex");
   glUniform1i(tableLoc, 0); //send texture to shader

	// TableTop texture
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture[1]);

   data = SOIL_load_image(TexNames[1].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   tableTopLoc = glGetUniformLocation(programId, "tableTopTex");
   glUniform1i(tableTopLoc, 1); //send texture to shader

	  // Room texture
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texture[2]);

   data = SOIL_load_image(TexNames[2].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   roomTexLoc = glGetUniformLocation(programId, "roomTex");
   glUniform1i(roomTexLoc, 2); //send texture to shader


   // Whiteball texture
   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, texture[3]);

   data = SOIL_load_image(TexNames[3].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   whiteballTexLoc = glGetUniformLocation(programId, "white");
   glUniform1i(whiteballTexLoc, 3); //send texture to shader

   // Blackball texture
   glActiveTexture(GL_TEXTURE4);
   glBindTexture(GL_TEXTURE_2D, texture[4]);

   data = SOIL_load_image(TexNames[4].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   blackballTexLoc = glGetUniformLocation(programId, "black");
   glUniform1i(blackballTexLoc, 4); //send texture to shader



   // Redball texture
   glActiveTexture(GL_TEXTURE5);
   glBindTexture(GL_TEXTURE_2D, texture[5]);

   data = SOIL_load_image(TexNames[5].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   redballTexLoc = glGetUniformLocation(programId, "red");
   glUniform1i(redballTexLoc, 5); //send texture to shader



   // Yellowball texture
   glActiveTexture(GL_TEXTURE6);
   glBindTexture(GL_TEXTURE_2D, texture[6]);

   data = SOIL_load_image(TexNames[6].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   yellowballTexLoc = glGetUniformLocation(programId, "yellow");
   glUniform1i(yellowballTexLoc, 6); //send texture to shader

   /// cue texture
   /*
   glActiveTexture(GL_TEXTURE7);
   glBindTexture(GL_TEXTURE_2D, texture[7]);

   data = SOIL_load_image(TexNames[7].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   cueTexLoc = glGetUniformLocation(programId, "cueTex");
   glUniform1i(cueTexLoc, 7); //send texture to shader
   */
}



// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   glUniform3fv(glGetUniformLocation(programId, "eyePos"), 1, &eye[0]);
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(eye, vec3(0.0 + cameraPosition, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); 

   // Draw sphere
   testSphere.updateModelMatrix(modelViewMatLoc, cameraPosition);
   glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)
   testSphere.Draw();

   // Draw Room
   vec3 roomPos = vec3(0.0, 0.0, 0.0f);
   RoomModel.updateModelMatrix(modelViewMatLoc, cameraPosition, 12.f, roomPos);
   glUniform1ui(objectLoc, ROOMMODEL);  //if (object == ROOMMODEL)
   RoomModel.Draw();
   

   // Draw Table
   vec3 tablePos = vec3(0.0f, 1.0f, 0.0f);
   TableModel.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.0f, vec3(0));
   glUniform1ui(objectLoc, TABLEMODEL); //if (object == TABLEMODEL)
   TableModel.Draw();


   // Draw TableTop
   vec3 tableTopPos = vec3(0.0f, 1.0f, 0.0f);
   TableTop.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, vec3(0));
   glUniform1ui(objectLoc, TABLETOP);  //if (object == TABLETOP)
   TableTop.Draw();

   // WHITEBALL, BLACKBALL, REDBALL, YELLOWBALL, CUE }; // VAO ids.

   // Whiteball
   vec3 whiteballPos = vec3(-0.25f, 0.1f, 0.5f);
   WhiteBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, whiteballPos);
   glUniform1ui(objectLoc, WHITEBALL);  
   WhiteBall.Draw();

   // Blackball
   vec3 blackballPos = vec3(-0.25f, 0.1f, -0.5f);
   BlackBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, blackballPos);
   glUniform1ui(objectLoc, BLACKBALL);  
   BlackBall.Draw();

   // red and yellow - 7 each

   // Redballs  x7s
   vec3 redballPos1 = vec3(-0.2f, 0.1f, -0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos1);
   glUniform1ui(objectLoc, REDBALL);
   RedBall.Draw();

   vec3 redballPos2 = vec3(-0.3f, 0.1f, -0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos2);
   glUniform1ui(objectLoc, REDBALL);
   RedBall.Draw();

   vec3 redballPos3 = vec3(0.5f, 0.1f, 0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos3);
   glUniform1ui(objectLoc, REDBALL);
 //  RedBall.Draw();

   vec3 redballPos4 = vec3(0.5f, 0.2f, 0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos4);
   glUniform1ui(objectLoc, REDBALL);
//   RedBall.Draw();

   vec3 redballPos5 = vec3(0.5f, 1.2f, 0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos5);
   glUniform1ui(objectLoc, REDBALL);
  // RedBall.Draw();

   vec3 redballPos6 = vec3(0.5f, 1.2f, 0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos6);
   glUniform1ui(objectLoc, REDBALL);
 //  RedBall.Draw();

   vec3 redballPos7 = vec3(0.5f, 1.2f, 0.5f);
   RedBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, redballPos7);
   glUniform1ui(objectLoc, REDBALL);
  // RedBall.Draw();



   // Yellowballs x7
   vec3 yellowballPos1 = vec3(-0.22f, 0.2f, -0.45f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos1);
   glUniform1ui(objectLoc, YELLOWBALL);
   YellowBall.Draw();

   vec3 yellowballPos2 = vec3(-0.17f, 0.1f, -0.4f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos2);
   glUniform1ui(objectLoc, YELLOWBALL);
 //  YellowBall.Draw();

   vec3 yellowballPos3 = vec3(0.5f, 1.2f, 0.5f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos3);
   glUniform1ui(objectLoc, YELLOWBALL);
 //  YellowBall.Draw();

   vec3 yellowballPos4 = vec3(0.5f, 1.2f, 0.5f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos4);
   glUniform1ui(objectLoc, YELLOWBALL);
 //  YellowBall.Draw();

   vec3 yellowballPos5 = vec3(0.5f, 1.2f, 0.5f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos5);
   glUniform1ui(objectLoc, YELLOWBALL);
 //  YellowBall.Draw();

   vec3 yellowballPos6 = vec3(0.5f, 1.2f, 0.5f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos6);
   glUniform1ui(objectLoc, YELLOWBALL);
 //  YellowBall.Draw();

   vec3 yellowballPos7 = vec3(0.5f, 1.2f, 0.5f);
   YellowBall.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, yellowballPos7);
   glUniform1ui(objectLoc, YELLOWBALL);
  // YellowBall.Draw();

   // CUE
   vec3 cuePos = vec3(0.4f, 0.3f, 0.5f);
   Cue.updateModelMatrix(modelViewMatLoc, cameraPosition, 8.f, cuePos);
   glUniform1ui(objectLoc, CUE);
   Cue.Draw();


   //Send Camera position to shader for Specular Lighting
   //glUniform3fv(glGetUniformLocation(programId, "eyePos"), 1, &eye[0]);


   glutSwapBuffers();
}

void animate() {;
movableBallPos.z -= 0.01;
	if (movableBallPos.z < -15.0) movableBallPos.z = 0.0;
	testSphere.SetPosition(vec3(-10, 0.2, movableBallPos.z)); //modify sphere's position
	// refresh screen 
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
}

// Keyboard input processing routine.
void KeyInputDown(unsigned char key, int x, int y)
{
	GameObject::keys[key] = true;
	std::cout << "Key pressed: " << key << " : " << GameObject::keys[key] << std::endl;

   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

void KeyInputUp(unsigned char key, int x, int y)
{
	GameObject::keys[key] = false;
	std::cout << "Key de-pressed: " << key << " : " << GameObject::keys[key] << std::endl;

	switch (key)
	{
	case 'w':
	{
		eye.z -= cameraForward;

	}break;
	case 's':
	{
		eye.z += -cameraForward;
	}break;
	case 'a':
	{
		eye.x += -cameraForward;

	}break;
	case 'd':
	{
		eye.x += cameraForward;

	}break;
	case 'e':
	{
		eye += -glm::vec3(0, 1, 0);

	}break;
	case 'q':
	{
		eye += glm::vec3(0, 1, 0);

	}break;
	case 'y':
	{
		Cue.position -= vec3(0, 0, 0.2);
	}break;
	case 'h':
	{
		Cue.position += vec3(0.2, 0, 0);
	}break;
	case 'g':
	{
		Cue.position += vec3(-0.2, 0, 0);
	}break;
	case 'b':
	{
		Cue.position += vec3(0, 0, 0.2);
	}break;
	case 27:
	{
		exit(0);
	}break;
	}
}

// Callback routine for non-ASCII key entry.
void SpecialKeyInputDown(int key, int x, int y)
{
	GameObject::specialKeys[key] = true;
	//std::cout << "Special key pressed: " << key << " : " << GameObject::specialKeys[key] << std::endl;

   if (key == GLUT_KEY_LEFT) 
   {
	   if (cameraPosition > -50.0) cameraPosition -= 0.2;
   }
   if (key == GLUT_KEY_RIGHT) 
   {
	   if (cameraPosition < 15.0) cameraPosition += 0.2;
   }
   if (key == GLUT_KEY_UP) 
   {

   }
   glutPostRedisplay();
}

void SpecialKeyInputUp(int key, int x, int y)
{
	GameObject::specialKeys[key] = false;
	std::cout << "Special key de-pressed: " << key << " : " << GameObject::specialKeys[key] << std::endl;
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left and right arrow keys to move the viewpoint over the field." << endl; 
   cout << "Use y, g, h, b keys to move the cue." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(300, 100); 
   glutCreateWindow("Michal's Pool Game");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize); 
   glutIdleFunc(animate);
   glutKeyboardFunc(KeyInputDown);
   glutKeyboardUpFunc(KeyInputUp);
   glutSpecialFunc(SpecialKeyInputDown);
   glutSpecialUpFunc(SpecialKeyInputUp);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}


