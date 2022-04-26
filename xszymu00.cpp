/**
*	MPC-MPG 2022 - Zaverecny projekt
*
*	Autor:		xszymu00
*	Student ID:	211273
*	Email:		xszymu00@vutbr.cz
*
*	Seznam vypracovanych ukolu:
*
*   Neco vlastniho co by mohlo byt take hodnoceno:
*
*	Ocekavany pocet bodu:	fakt hodne
*
**/
#include "imageLoad.h"
#include <cmath>
#include <GL/glut.h>
#include "OBJ_Loader.h"

// konstanty a makra pridejte zde
// PI/180
#define PIover180 0.017453292519943f

#define MENU_RESET  1001
#define MENU_EXIT_OK 1002
#define MENU_EXIT_NO 1003 
#define MENU_TIMER_ON 1004
#define MENU_TIMER_OFF 1005
#define MENU_TIMER_RESET 1006
#define MENU_TEXTURES_ON 1007
#define MENU_TEXTURES_OFF 1008
#define MENU_LIGHTING_OFF 1009
#define MENU_LIGHTING_ON 1010
#define MENU_FLYING_OFF 1011
#define MENU_FLYING_ON 1012

#define TEXTURE_WIDTH  64
#define TEXTURE_HEIGHT 64


// globalni promenne pro transformace
int xnew = 0, ynew = 0;
int xold = 0, yold = 0;
int xx, yy, zz;
int state;
float angle = 0;
float throwAngle;
float tranZ = -12.0;
float tranX = 1.0;
float tranY = 0.0;
bool timerOn = false;
float xScale = 1;
bool throwing = false;
float teapotZ = -100.0;
float teapotX = 0;
bool mipmap = false;
bool texturesEnabled = true;
unsigned char texture[TEXTURE_HEIGHT][TEXTURE_WIDTH][4];
GLuint textury[2];
bool lightingEnabled = true;
bool flashlightEnabled = false;
bool walk = false;
float walkingAngle = 0;
float flyAngle = 0;
bool loaded;
bool flying = false;
float tempX = 0; float tempY = 0; float tempZ = 0;

objl::Loader loader;

// nastaveni projekce
float fov = 60.0;
float nearPlane = 0.1;
float farPlane = 150.0;

GLfloat lightPosition[] = { 0, 0, -20, 0 };
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightSpecular[] = { .0f, .0f, .0f, 1.0f };
GLfloat lightDirection[] = { .0f, .0f, -1.0f };

GLfloat flashlightAmbient[] = { .5f, .5f, .5f, 1 };
GLfloat flashlightDiffuse[] = { .9f, .9f, .9f, 1.0f };
GLfloat flashlightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat flashlightPosition[] = { tranX,tranY,tranZ,0.0 };
GLfloat flashlightDirection[] = { 0.0f,0.0f,-1.0f };

GLfloat materialAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat materialSpecular[] = { .0f, .0f, .0f, 1.0f };
GLfloat materialShininess = 60;

// zde vlastnosti svetel materialu, menu, popripade jinych vlastnosti
// ...



void InitTexture(void)
{
	int i, j, c;
	unsigned char* P;

	for (j = 0; j < TEXTURE_HEIGHT; j++) {
		P = texture[j][0];
		for (i = 0; i < TEXTURE_WIDTH; i++) {
			*P++ = 100;
			*P++ = 20;
			*P++ = 20;
			*P++ = 255;
		}
	}
}
// funkce volana pri zmene velikosti sceny
void OnReshape(int w, int h)            // event handler pro zmenu velikosti okna
{
	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fov, (double)w / h, nearPlane, farPlane); // perspektivni projekce
}

// funkce pro inicializaci
// do teto funkce vlozit vsechny vlastnosti a inicializace, ktere se nebudou menit pri behu aplikace
void OnInit(void)
{
	// steny
	glFrontFace(GL_CW);					// nastaveni front == clockwise
	glPolygonMode(GL_FRONT, GL_FILL);   // mod predni steny fill
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// dalsi stavy a vlastnosti (napr. nastaveni svetel, materialu, ...)

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	
	glLightfv(GL_LIGHT1, GL_AMBIENT, flashlightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, flashlightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, flashlightSpecular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, flashlightDirection);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10);
	

	InitTexture();

	glGenTextures(1, &textury[0]);
	glBindTexture(GL_TEXTURE_2D, textury[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		TEXTURE_WIDTH, TEXTURE_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	setTexture("drevo.bmp", &textury[1], mipmap);
	glBindTexture(GL_TEXTURE_2D, textury[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);

	loaded = loader.LoadFile("block.obj");


}

void jiggle() {
	walkingAngle += 170;
	tranY += sin(walkingAngle);

}

bool collisionWithCube() {
	if (-22 <= tranX && tranX <= 2 && -2 <= tranZ && tranZ <= 22 && -11 <= tranY && tranY <= 11)
		return true;
	return false;
}

void drawTetrahedron() {

	glPushMatrix();
	if (texturesEnabled) {
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, textury[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0, 2, 0);
	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, -1.4);
	glVertex3f(0, 2, 0);
	glVertex3f(-1, 0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void DrawCube(int x, int y, int z, int size)
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);



	// predni stena
	glVertex3i(x - size, y - size, z + size);
	glVertex3i(x - size, y + size, z + size);
	glVertex3i(x + size, y + size, z + size);
	glVertex3i(x + size, y - size, z + size);


	// zadni stena
	glVertex3i(x + size, y - size, z - size);
	glVertex3i(x + size, y + size, z - size);
	glVertex3i(x - size, y + size, z - size);
	glVertex3i(x - size, y - size, z - size);

	// leva stena
	glVertex3i(x - size, y - size, z + size);
	glVertex3i(x - size, y - size, z - size);
	glVertex3i(x - size, y + size, z - size);
	glVertex3i(x - size, y + size, z + size);

	// prava stena
	glVertex3i(x + size, y + size, z + size);
	glVertex3i(x + size, y + size, z - size);
	glVertex3i(x + size, y - size, z - size);
	glVertex3i(x + size, y - size, z + size);

	// horni stena
	glVertex3i(x + size, y + size, z + size);
	glVertex3i(x - size, y + size, z + size);
	glVertex3i(x - size, y + size, z - size);
	glVertex3i(x + size, y + size, z - size);

	// spodni stena
	glVertex3i(x + size, y - size, z - size);
	glVertex3i(x - size, y - size, z - size);
	glVertex3i(x - size, y - size, z + size);
	glVertex3i(x + size, y - size, z + size);

	glEnd();

	// hrany
	glColor3f(0.1, 0.1, 0.8);
	glLineWidth(2);

	// predni stena
	glBegin(GL_LINE_STRIP);
	glVertex3i(x - size, y - size, z + size);
	glVertex3i(x - size, y + size, z + size);
	glVertex3i(x + size, y + size, z + size);
	glVertex3i(x + size, y - size, z + size);
	glEnd();

	// zadni stena
	glBegin(GL_LINE_STRIP);
	glVertex3i(x + size, y - size, z - size);
	glVertex3i(x + size, y + size, z - size);
	glVertex3i(x - size, y + size, z - size);
	glVertex3i(x - size, y - size, z - size);
	glEnd();

	// leva stena
	glBegin(GL_LINE_STRIP);
	glVertex3i(x - size, y - size, z + size);
	glVertex3i(x - size, y - size, z - size);
	glVertex3i(x - size, y + size, z - size);
	glVertex3i(x - size, y + size, z + size);
	glEnd();

	// prava stena
	glBegin(GL_LINE_STRIP);
	glVertex3i(x + size, y + size, z + size);
	glVertex3i(x + size, y + size, z - size);
	glVertex3i(x + size, y - size, z - size);
	glVertex3i(x + size, y - size, z + size);
	glEnd();

	// horni stena
	glBegin(GL_LINE_STRIP);
	glVertex3i(x + size, y + size, z + size);
	glVertex3i(x - size, y + size, z + size);
	glVertex3i(x - size, y + size, z - size);
	glVertex3i(x + size, y + size, z - size);
	glEnd();

	// spodni stena
	glBegin(GL_LINE_STRIP);
	glVertex3i(x + size, y - size, z - size);
	glVertex3i(x - size, y - size, z - size);
	glVertex3i(x - size, y - size, z + size);
	glVertex3i(x + size, y - size, z + size);
	glEnd();
	glPopMatrix();

}

// doporucene vykresleni objektu 
// (1) pouzijte PUSH/POP pro GL_MODELVIEW matice, abyste si nerozhodili transformace cele sceny
// (2) vykreslujte vlastni objekty okolo pocatku (0,0,0) pro snazsi provadeni transformaci a umisteni ve scene
// (3) pro ziskaji aktulanich stavovych promennych v OpenGL muzete pouzit funkce glGetBooleanv(...)
void DrawPlane(int size)
{
	// (1)
	glPushMatrix();

	// nastaveni transformace (pokud je treba)
	glTranslatef(0, -15, 0);

	// (3) nastaveni vlastnosti OpenGL
	GLboolean isLightingEnabled;
	glGetBooleanv(GL_LIGHTING, &isLightingEnabled);

	if (isLightingEnabled)
		glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.7, 0.0);

	// (2) vykresleni objektu
	glBegin(GL_QUADS);
	glVertex3i(-size, 0, size);
	glVertex3i(-size, 0, -size);
	glVertex3i(size, 0, -size);
	glVertex3i(size, 0, size);
	glEnd();

	// v pripade, ze jste vypnuli/zapnuli nejaky stav, vratte ho do puvodniho
	if (isLightingEnabled)
		glEnable(GL_LIGHTING);

	glPopMatrix();
}

void drawSphereWithTexture() {
	glPushMatrix();
	GLUquadric* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	if (texturesEnabled) {
		gluQuadricTexture(quadric, GLU_TRUE);
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, textury[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluSphere(quadric, 5.0, 50, 50);
	glDisable(GL_TEXTURE_2D);

	gluDeleteQuadric(quadric);
	glPopMatrix();
}

void drawBlenderModel() {
	glPushMatrix();
	if (texturesEnabled) {
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, textury[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (loaded)
	{
		for (int i = 0; i < loader.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = loader.LoadedMeshes[i];

			glBegin(GL_TRIANGLES);
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				glNormal3f(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z);
				glVertex3f(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
			}
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void throwTeapot() {
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(teapotX, 0, teapotZ);
	glutSolidTeapot(2);
	glutPostRedisplay();
	glPopMatrix();
}

// funkce volana pri prekresleni sceny
void OnDisplay(void)
{
	// nastaveni parametru pro defaultni mazani rovin
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glClearDepth(1.0f);

	// mazani rovin
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// inicializace matice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// provedeni transformaci sceny (pro rotaci mysi a pohyb klavesami)
	glRotatef(ynew, 1.0f, 0.0f, 0.0f);		// rotace podle osy x
	glRotatef(xnew, 0.0f, 1.0f, 0.0f);		// rotace podle osy y
	glTranslatef(tranX, tranY, tranZ);		// relativni zmena v z

	// zde vykreslovani dalsich objektu ...
	if (throwing) {
		throwTeapot();
	}
	// (priklad) vykresli podlahu


	DrawPlane(100);

	glPushMatrix();
	glTranslatef(-30, 0, 0);
	glColor3f(1, 1, 1);
	glDisable(GL_LIGHTING);
	drawSphereWithTexture();
	if (lightingEnabled) {
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (flashlightEnabled) {
		
	}

	glPushMatrix();
	glScalef(xScale, 1, 1);
	DrawCube(10, 0, -10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, 0);
	glScalef(7, 7, 7);
	glDisable(GL_LIGHTING);
	drawTetrahedron();
	if (lightingEnabled) {
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// pruhledna stena
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);

	glColor4f(0, 1, 1, 0.8);
	glVertex3i(-5, -5, 5);
	glVertex3i(-5, 5, 5);
	glVertex3i(5, 5, 5);
	glVertex3i(5, -5, 5);

	glEnd();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);


	glPushMatrix();
	glDisable(GL_LIGHTING);
	glScalef(5, 5, 5);
	glTranslatef(15, 0, 0);
	glColor3f(1, 1, 1);
	drawBlenderModel();
	glPopMatrix();

	if (!lightingEnabled) {
		glDisable(GL_LIGHTING);
	}

	glFlush();
	glutSwapBuffers();
}

// funkce volana pri stisku tlacitka
void OnMouseButton(int button, int state, int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			xx = x;
			yy = y;
		}
		else {
			state = 0;
			xold = xnew;
			yold = ynew;
		}
	}

	// pripadne test pro prave tlacitko pro vyvolani menu
	// ... GLUT_RIGHT_BUTTON

	glutPostRedisplay();
}

// funkce volana pri pohybu mysi a stistenem tlacitku mysi
void OnMouseMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	if (state == 0) {
		xnew = xold + x - xx;
		ynew = -(yold + y - yy);
		angle = xnew * PIover180;
		flyAngle = ynew * PIover180;
		glutPostRedisplay();
	}
}

// funkce volana pri stisku specialnich klaves
void OnSpecial(int key, int mx, int my)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
	{
		if (collisionWithCube()) {
			tranZ = tempZ;
			tranY = tempY;
			tranX = tempX;
			break;
		}
		tempX = tranX;
		tempY = tranY;
		tempZ = tranZ;
		tranY -= 2;
	}
	break;
	case GLUT_KEY_PAGE_DOWN:
	{
		if (collisionWithCube()) {
			tranZ = tempZ;
			tranY = tempY;
			tranX = tempX;
			break;
		}
		tempX = tranX;
		tempY = tranY;
		tempZ = tranZ;
		if (tranY >= 0) {
			break;
		}
		tranY += 2;
	}

	default:
		break;
	}

	glutPostRedisplay();
}



inline void createMenu(void(*func)(int value))
{
	int idSub = glutCreateMenu(func);
	glutAddMenuEntry("Ano", MENU_EXIT_OK);
	glutAddMenuEntry("Ne", MENU_EXIT_NO);

	int idTimer = glutCreateMenu(func);
	glutAddMenuEntry("Spustit", MENU_TIMER_ON);
	glutAddMenuEntry("Zastavit", MENU_TIMER_OFF);
	glutAddMenuEntry("Resetovat velikost", MENU_TIMER_RESET);

	int idTextures = glutCreateMenu(func);
	glutAddMenuEntry("Vypnout", MENU_TEXTURES_OFF);
	glutAddMenuEntry("Zapnout", MENU_TEXTURES_ON);

	int idLighting = glutCreateMenu(func);
	glutAddMenuEntry("Vypnout", MENU_LIGHTING_OFF);
	glutAddMenuEntry("Zapnout", MENU_LIGHTING_ON);

	int idFlying = glutCreateMenu(func);
	glutAddMenuEntry("Vypnout", MENU_FLYING_OFF);
	glutAddMenuEntry("Zapnout", MENU_FLYING_ON);

	glutCreateMenu(func);
	glutPostRedisplay();
	glutAddSubMenu("Animace", idTimer);
	glutAddMenuEntry("Reset pozice ", MENU_RESET);
	glutAddSubMenu("Textury", idTextures);
	glutAddSubMenu("Osvetleni", idLighting);
	glutAddSubMenu("Letani", idFlying);
	glutAddSubMenu("Konec", idSub);
	glutPostRedisplay();

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void onTimer(int value)
{
	if (timerOn)
	{
		xScale += 0.008;
		glutTimerFunc(15, onTimer, 10);
	}
	if (throwing) {
		glutTimerFunc(15, onTimer, 10);
		teapotZ -= 2 * cos(throwAngle);
		teapotX += 2 * sin(throwAngle);
	}
	glutPostRedisplay();
}

void onMenu(int value)
{
	switch (value)
	{
	case MENU_RESET:
		tranY = 0.0;
		tranZ = -100.0f;
		tranX = 0.0f;
		break;
	case MENU_EXIT_OK:
		exit(1);
		break;
	case MENU_EXIT_NO:
		break;
	case MENU_TIMER_ON:
		if (!timerOn)
		{
			glutTimerFunc(15, onTimer, value);
		}
		timerOn = true;
		break;
	case MENU_TIMER_OFF:
		timerOn = false;
		break;

	case MENU_TIMER_RESET:
	{
		xScale = 1;
	}
	break;
	case MENU_TEXTURES_OFF:
	{
		texturesEnabled = false;
	}
	break;
	case MENU_TEXTURES_ON:
	{
		texturesEnabled = true;
	}
	break;
	case MENU_LIGHTING_OFF:
	{
		lightingEnabled = false;
	}
	break;
	case MENU_LIGHTING_ON:
	{
		lightingEnabled = true;
	}
	break;
	case MENU_FLYING_OFF:
		flying = false;
		tranY = 0;
		break;
	case MENU_FLYING_ON:
		flying = true;
		break;
		glutPostRedisplay();
	}
}

void OnKey(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'w':
	{
		if (collisionWithCube()) {
			tranZ = tempZ;
			tranY = tempY;
			tranX = tempX;
			break;
		}
		tempX = tranX;
		tempY = tranY;
		tempZ = tranZ;
		tranZ += 2 * cos(angle);
		tranX -= 2 * sin(angle);
		if (flying) {
			tranY += 2 * sin(flyAngle);
			break;
		}
		jiggle();

	}
	break;
	case 's':
	{
		if (collisionWithCube()) {
			tranZ = tempZ;
			tranY = tempY;
			tranX = tempX;
			break;
		}
		tempX = tranX;
		tempY = tranY;
		tempZ = tranZ;
		tranZ -= 2 * cos(angle);
		tranX += 2 * sin(angle);
		if (flying) {
			break;
		}
		jiggle();
	}
	break;
	case 'd':
	{
		if (collisionWithCube()) {
			tranZ = tempZ;
			tranY = tempY;
			tranX = tempX;
			break;
		}
		tempX = tranX;
		tempY = tranY;
		tempZ = tranZ;
		tranZ -= 2 * sin(angle);
		tranX -= 2 * cos(angle);
		if (flying) {
			break;
		}
		jiggle();

	}
	break;
	case 'a':
	{
		if (collisionWithCube()) {
			tranZ = tempZ;
			tranY = tempY;
			tranX = tempX;
			break;
		}
		tempX = tranX;
		tempY = tranY;
		tempZ = tranZ;
		tranZ += 2 * sin(angle);
		tranX += 2 * cos(angle);
		if (flying) {
			break;
		}
		jiggle();
	}
	break;
	case 'f':
	{
		glLightfv(GL_LIGHT1, GL_POSITION, flashlightPosition);
		flashlightEnabled = !flashlightEnabled;
		if (flashlightEnabled)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
	}
	break;
	case 't':
	{
		teapotX = -1 * tranX;
		teapotZ = -1 * tranZ;
		throwAngle = angle;
		throwing = !throwing;
		glutTimerFunc(15, onTimer, 1);
	}
	break;
	default:
		break;
	}
	glutPostRedisplay();
}


// funkce main
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);					// inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // init double buffer

	glutInitWindowSize(640, 480);			// nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 200);		// nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("Projekt 2022 – Autor: xszymu00");	// vytvoreni okna

	// registracni funkce
	glutDisplayFunc(OnDisplay);				// registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(OnReshape);				// registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(OnMouseButton);			// registrace funkce pro stisk tlacitka mysi
	glutMotionFunc(OnMouseMotion);			// registrace funkce pro pohyb mysi pri stisknutem tlacitku
	glutSpecialFunc(OnSpecial);				// registrace funkce pro zachytavani specialnich klaves
	glutKeyboardFunc(OnKey);
	createMenu(onMenu);

	// pripadne dalsi udalosti...

	// inicializace sceny
	OnInit();

	glutMainLoop();

	return 0;
}
