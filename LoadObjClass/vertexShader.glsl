#version 430 core

#define SPHERE 0
#define ROOMMODEL 1
#define TABLEMODEL 2
#define TABLETOP 3
#define WHITEBALL 4
#define BLACKBALL 5
#define REDBALL 6
#define YELLOWBALL 7
#define CUE 8


layout(location=0) in vec4 Coords;
layout(location=1) in vec2 TexCoords;
layout(location=2) in vec4 sphereCoords;
layout(location=3) in vec3 sphereNormals;
layout(location=4) in vec3 objCoords;
layout(location=5) in vec3 objNormals;
layout(location=6) in vec2 objTexCoords;


uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

out vec2 texCoordsExport;
out vec3 normalExport;

vec4 coords;


uniform vec3 eyePos;

out vec3 eyeExport;
out vec3 modelExport;


void main(void)
{   

   if (object == SPHERE)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }

   	if (object == ROOMMODEL)
    {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
      modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));
    }

    if (object == TABLEMODEL)
    {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
      modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));
    }

	    if (object == TABLETOP)
    {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
      modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));
    }

           if (object == WHITEBALL)
   {
      coords =  vec4(objCoords, 1.0f);
      texCoordsExport = objTexCoords;
	  normalExport = objNormals;
	 modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));

   }

          if (object == BLACKBALL)
   {
      coords =  vec4(objCoords, 1.0f);
      texCoordsExport = objTexCoords;
	  normalExport = objNormals;
	  modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));

   }

          if (object == REDBALL)
   {
      coords =  vec4(objCoords, 1.0f);
      texCoordsExport = objTexCoords;
	  normalExport = objNormals;
	  modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));

   }

          if (object == YELLOWBALL)
   {
      coords =  vec4(objCoords, 1.0f);
      texCoordsExport = objTexCoords;
	  normalExport = objNormals;
	  modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));

   }

          if (object == CUE)
   {
      coords =  vec4(objCoords, 1.0f);
      texCoordsExport = objTexCoords;
	  normalExport = objNormals;
	  modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));

   }

	//modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));
	eyeExport = eyePos;
   
   gl_Position = projMat * modelViewMat * coords;
}
