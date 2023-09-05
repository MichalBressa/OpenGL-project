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

in vec2 texCoordsExport;
in vec3 normalExport;

struct Light
{
vec4 ambCols;
vec4 difCols;
vec4 specCols;
vec4 coords;
};

struct Material
{
vec4 ambRefl;
vec4 difRefl;
vec4 specRefl;
vec4 emitCols;
float shininess;
};

uniform Light light0;
uniform Light light1;

uniform vec4 globAmb;
uniform Material sphereFandB;

uniform sampler2D roomTex;
uniform sampler2D tableTex;
uniform sampler2D tableTopTex;
uniform sampler2D whiteballTex;
uniform sampler2D blackballTex;
uniform sampler2D redballTex;
uniform sampler2D yellowballTex;

uniform uint object;

out vec4 colorsOut;

vec4 roomTexColor, tableTexColor, tableTopTexColor,	whiteballTexColor,
	blackballTexColor,
	redballTexColor,
	yellowballTexColor,
	cueTexColor;
vec3 normal, lightDirection;

vec4 fAndBDif0;
vec4 fAndBDif1;
vec4 fAndBSpec,fAndBSpec2;

in vec3 modelExport;
in vec3 eyeExport;

vec3 eyeDirection;
vec3 halfway;

//vec4 fAndBSpec0;
//vec4 fAndBSpec1;


void main(void)
{



roomTexColor = texture(roomTex, texCoordsExport);
tableTexColor = texture(tableTex, texCoordsExport);
tableTopTexColor = texture(tableTopTex, texCoordsExport);
whiteballTexColor = texture(whiteballTex, texCoordsExport);
blackballTexColor = texture(blackballTex, texCoordsExport);
redballTexColor = texture(redballTex, texCoordsExport);
yellowballTexColor = texture(yellowballTex, texCoordsExport);




if (object == SPHERE) {
normal = normalize(normalExport);
lightDirection = normalize(vec3(light0.coords));
fAndBDif0 = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);
colorsOut = vec4(vec3(min(fAndBDif0, vec4(1.0))), 1.0);
}



if (object == ROOMMODEL) {

colorsOut = roomTexColor;
//vec4(1.0,0.0,0.0,1.0);
}
if (object == TABLEMODEL) {
colorsOut = tableTexColor; //vec4(0.0,1.0,0.0,1.0);
}



if (object == TABLETOP){
colorsOut = tableTopTexColor;
}



if (object == WHITEBALL){



lightDirection = normalize(vec3(light0.coords));
fAndBDif0 = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);



eyeDirection = normalize(eyeExport - modelExport);
halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
fAndBSpec = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;



normal = normalize(normalExport);
lightDirection = normalize(vec3(light1.coords));
fAndBDif1 = max(dot(normal, lightDirection), 0.0f) * (light1.difCols * sphereFandB.difRefl);



fAndBSpec2 = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light1.specCols * sphereFandB.specRefl;



colorsOut = vec4(vec3(min(fAndBDif0+fAndBDif1+fAndBSpec+fAndBSpec2, vec4(1.0))), 1.0)*vec4(235.0, 235.0, 235.0, 1.0);

//colorsOut = vec4(235.0, 235.0, 235.0, 1.0);
//colorsOut = whiteballTexColor;
}



if (object == BLACKBALL)
{
lightDirection = normalize(vec3(light0.coords));
fAndBDif0 = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);



eyeDirection = normalize(eyeExport - modelExport);
halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
fAndBSpec = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;



normal = normalize(normalExport);
lightDirection = normalize(vec3(light1.coords));
fAndBDif1 = max(dot(normal, lightDirection), 0.0f) * (light1.difCols * sphereFandB.difRefl);



fAndBSpec2 = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light1.specCols * sphereFandB.specRefl;



colorsOut = vec4(vec3(min(fAndBDif0+fAndBDif1+fAndBSpec+fAndBSpec2, vec4(1.0))), 1.0)*blackballTexColor;




//colorsOut = vec4(13.0, 10.0, 12.0 , 1.0);
//colorsOut = vec4(13.0, 10.0, 12.0 , 1.0);
}



if (object == REDBALL)
{
lightDirection = normalize(vec3(light0.coords));
fAndBDif0 = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);



eyeDirection = normalize(eyeExport - modelExport);
halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
fAndBSpec = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;



normal = normalize(normalExport);
lightDirection = normalize(vec3(light1.coords));
fAndBDif1 = max(dot(normal, lightDirection), 0.0f) * (light1.difCols * sphereFandB.difRefl);



fAndBSpec2 = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light1.specCols * sphereFandB.specRefl;



colorsOut = vec4(vec3(min(fAndBDif0+fAndBDif1+fAndBSpec+fAndBSpec2, vec4(1.0))), 1.0)*redballTexColor;


//colorsOut = redballTexColor;


//colorsOut = vec4(236.0, 19.0, 19.0, 1.0);
}



if (object == YELLOWBALL)
{
lightDirection = normalize(vec3(light0.coords));
fAndBDif0 = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);



eyeDirection = normalize(eyeExport - modelExport);
halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
fAndBSpec = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;



normal = normalize(normalExport);
lightDirection = normalize(vec3(light1.coords));
fAndBDif1 = max(dot(normal, lightDirection), 0.0f) * (light1.difCols * sphereFandB.difRefl);



fAndBSpec2 = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light1.specCols * sphereFandB.specRefl;



colorsOut = vec4(vec3(min(fAndBDif0+fAndBDif1+fAndBSpec+fAndBSpec2, vec4(1.0))), 1.0)*yellowballTexColor;



colorsOut = yellowballTexColor;

//colorsOut = vec4(246.0, 195.0, 12.0, 1.0);
}



if (object == CUE)
{
lightDirection = normalize(vec3(light0.coords));
fAndBDif0 = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);



eyeDirection = normalize(eyeExport - modelExport);
halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
fAndBSpec = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;



normal = normalize(normalExport);
lightDirection = normalize(vec3(light1.coords));
fAndBDif1 = max(dot(normal, lightDirection), 0.0f) * (light1.difCols * sphereFandB.difRefl);



fAndBSpec2 = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light1.specCols * sphereFandB.specRefl;



colorsOut = vec4(vec3(min(fAndBDif0+fAndBDif1+fAndBSpec+fAndBSpec2, vec4(1.0))), 1.0)*cueTexColor;




colorsOut = vec4(77.0, 38.0, 0.0, 1.0);
}



eyeDirection = normalize(eyeExport - modelExport);
halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
fAndBSpec = pow(max(dot(normal, halfway), 0.0), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;



}