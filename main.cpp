#include <iostream>
#include <stdlib.h>
#include<math.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<vector>
#include "imageloader.h"
#include "md2model.h"
#include<cstring>
#include<sstream>
#include "SOIL/SOIL.h"
#include "glm.h"
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)


using namespace std;


void drawBitmapText(string str,float x,float y,float z) 
{  
	string c;
	glRasterPos3f(x, y,z);
	for (int i=0;i<str.length();i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}



vector < pair<int,int> >hurdle1;
vector < pair<int,int> >fossil;
const float FLOOR_TEXTURE_SIZE = 15.0f; //The size of each floor "tile"
static int spin = 0;
float _angle = 30.0f;
int game = 0;
MD2Model* _model;
int Ttime = 0;
char view = 'n';
int _textureId;
int type = 1;//sleep
//The forward position of the guy relative to an arbitrary floor "tile"
float _guyPos = 0;

float jump = 0;
float move = 0,walk = 0;
float movex=0,movey=-3.5;
char ca1;
char ca = 'w';
int f1 = 0;
int score = 0;
string text = "Score : ";
int ta[1000];
int timer = 0;

GLfloat road[] = { 0.2f, 0.2f, 0.2f };
GLuint roadt;


void cleanup() {
	delete _model;
}

void convert()
{
	ostringstream s;
	s << score;
	text = "Score : ";
	string ss;
	ss = s.str();
	text += ss;
}

void texParams() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

GLuint load_image(string fileName) {
	GLuint tex_ID;

	tex_ID = SOIL_load_OGL_texture(
			fileName.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_POWER_OF_TWO
			| SOIL_FLAG_MIPMAPS
			| SOIL_FLAG_MULTIPLY_ALPHA
			| SOIL_FLAG_COMPRESS_TO_DXT
			| SOIL_FLAG_DDS_LOAD_DIRECT
			| SOIL_FLAG_INVERT_Y
			);

	if( tex_ID > 0 ) {
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, tex_ID );

		return tex_ID;
	}
	else {
		cout << "Bad image" << endl;
		return 0;
	}
}


void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			cleanup();
			exit(0);

		case 'a':
			walk = 1;
			move -= 0.25f;
			break;
		case 'z':
			walk = 1;
			move += 0.25f;
			break;
		case 'f':
			view = 'f';
			break;
		case 'n':
			view = 'n';
			break;

		
	}
	switch(key)
	{
		case 'w':
			ca = 'w';
			break;
		case 's':
			if(timer == 0)
			{
				walk = 0;
				timer = 150;
				ca = 's';
			}
			break;
	}

	switch(key)
	{
		case 'j':
			if (movex <= 0)
			{
				ca1 = 'j';
				movex += 10;
			}
			break;
		case 'k':
			//walk = 0;
			//ca1 = 'k';
			if (movex >= 0)
			{
				ca1 = 'k';
				movex -= 10;
				//break;
			}
			break;
		case ' ' :
			if(jump == 0)
			{
				jump = 1;
			}
			break;
	}
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
	//Load the model
	_model = MD2Model::load("tallguy.md2");
	if (_model != NULL) {
		_model->setAnimation("run");
	}
	
	//Load the floor texture
	Image* image = loadBMP("vtr.bmp");
	_textureId = loadTexture(image);
	delete image;
	roadt = load_image(string("road.jpg"));
	
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -20.0f);
	//glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	
	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);


	//glColor3f(1,0,1);
	//drawBitmapText("LEVEL 1",0,35,-100);
	/*drawBitmapText("Score ::",50,30,-100);
	drawBitmapText("buffer",67,30,-100);
	drawBitmapText("time :",47,20,-100);

	drawBitmapText("b1",60,20,-100);
	drawBitmapText(":",63,20,-100);
	drawBitmapText("b",66,20,-100);*/


	if(game == 0)
	{
	
	//sun
	
	if ( Ttime < 400 )
	{
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
			GLfloat lightColor[] = {1, 1, 0, 1.0f};
			GLfloat lightPos[] = {-0.2f, 0.3f, -1, 0.0f};
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
			glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
	}
	else
	{
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
		    GLfloat lightColor0[] = {0, 0, 0, 1.0f};
		    GLfloat lightPos0[] = {-0.23f, 0.2f, -1.f, 0.0f};
		    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor0);
		    glLightfv(GL_LIGHT1, GL_POSITION, lightPos0);
	}
	
	//normal
	switch(view)
	{
		case 'n': 
			gluLookAt(
					0,0,-10+move,
					0,-5,10+move,
					0,0,1
					);
			break;
	
		case 'f':
			gluLookAt(
					0,0,move,
					0,1,-10+move,
					0,0,1
					);
			break;
		
	}
	//std::cout<<move<<"\t";
	//Draw the guy
	if (_model != NULL) {

		
		glPushMatrix();
		switch( ca )
		{
			case 'w':
				
				glTranslatef(movex, movey , move);
		//glRotatef(-90, 0 ,0 , 1);
		//glRotatef(-90.0f, 0, 1, 0);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				
				
				glScalef(0.0815f, 0.15f, 0.5f);
				break;
			case 's':
				glTranslatef(movex, movey-0.5 , move-5);
				glRotatef(-90, 0 ,0 , 1);
				glRotatef(-90.0f, 0, 1, 0);
				glColor3f(1,1,1);
		//		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				glScalef(0.15f, 0.15f, 0.15f);
				break;
				
		}
		_model->draw();
		glPopMatrix();
	}
	
	//Draw the floor
	glTranslatef(0.0f, -5.4f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBegin(GL_QUADS);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, _guyPos / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,
				 (2000 + _guyPos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, (2000 + _guyPos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, _guyPos / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, -1000.0f);
	
	glEnd();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roadt);
	texParams();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0,0);
	glVertex3f(-1.5, 0.2, -500);
	glTexCoord2f(10,0);
	glVertex3f(1.5, 0.2, -500);
	glTexCoord2f(10,10);
	glVertex3f(1.5, 0.2, 5000);
	glTexCoord2f(0,10);
	glVertex3f(-1.5,0.2, 5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();	

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roadt);
	texParams();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0,0);
	glVertex3f(-1.5-10, 0.2, -500);
	glTexCoord2f(10,0);
	glVertex3f(1.5-10, 0.2, -500);
	glTexCoord2f(10,10);
	glVertex3f(1.5-10, 0.2, 5000);
	glTexCoord2f(0,10);
	glVertex3f(-1.5-10,0.2, 5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();	

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roadt);
	texParams();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0,0);
	glVertex3f(-1.5+10, 0.2, -500);
	glTexCoord2f(10,0);
	glVertex3f(1.5+10, 0.2, -500);
	glTexCoord2f(10,10);
	glVertex3f(1.5+10, 0.2, 5000);
	glTexCoord2f(0,10);
	glVertex3f(-1.5+10,0.2, 5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();	

	for(size_t k=0;k<fossil.size();k++)
	{
		if (ta[k] == 0)
		{
		glPushMatrix();
		glTranslatef(fossil[k].first,1.5,fossil[k].second);
		//glColor3f(0.858824, 0.858824, 0.439216);
		glColor3f(1,1,1);
			glBegin(GL_TRIANGLE_FAN);
			for(int j=0 ; j<360 ; j++) 
			{
				glVertex2f(0.3 * cos(DEG2RAD(j)), 0.3 * sin(DEG2RAD(j)));
			}
			glEnd();
		glPopMatrix();
		}
	}

	for(size_t k=0;k<hurdle1.size();k++)
    	{

        glPushMatrix();
        glTranslatef(hurdle1[k].first,0,hurdle1[k].second);
	glScalef(0.75,1.0,0.5);
	glPushMatrix();
	glTranslatef(1.4,0,0);
	glScalef(0.1,1.0,0.1);

	
       
	glColor3f(0.0f,0.0f,0.0f);
	// +XZ	
	glBegin(GL_POLYGON);
		glVertex3f(-1,-2.5,1);
		glVertex3f(1,-2.5,1);
		glVertex3f(1,2.5,1);
		glVertex3f(-1,2.5,1);
	glEnd();
	// +XY
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(1,2.5,1);
		glVertex3f(1,-2.5,1);
		glVertex3f(1,-2.5,-1);
		glVertex3f(1,2.5,-1);
	glEnd();
	// -XZ
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(1,2.5,-1);
		glVertex3f(1,-2.5,-1);
		glVertex3f(-1,-2.5,-1);
		glVertex3f(-1,2.5,-1);
	glEnd();

	// -XY
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-1,2.5,-1);
		glVertex3f(-1,-2.5,-1);
		glVertex3f(-1,-2.5,1);
		glVertex3f(-1,2.5,1);
	glEnd();

	// +YZ

	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-1,2.5,-1);
		glVertex3f(1,2.5,-1);
		glVertex3f(1,2.5,1);
		glVertex3f(-1,2.5,1);
	glEnd();
	// -YZ

	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);

		glVertex3f(-1,-2.5,1);
		glVertex3f(1,-2.5,1);
		glVertex3f(1,-2.5,-1);
		glVertex3f(-1,-2.5,-1);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0,2.3,0.0);
	glScalef(1.5,0.75,0.5);
	
	glColor3f(0.0f,0.0f,0.0f);
	// +XZ	
	glBegin(GL_POLYGON);
		glVertex3f(-1,-0.5,1);
		glVertex3f(1,-0.5,1);
		glVertex3f(1,0.5,1);
		glVertex3f(-1,0.5,1);
	glEnd();
	// +XY
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(1,0.5,1);
		glVertex3f(1,-0.5,1);
		glVertex3f(1,-0.5,-1);
		glVertex3f(1,0.5,-1);
	glEnd();
	// -XZ
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(1,0.5,-1);
		glVertex3f(1,-0.5,-1);
		glVertex3f(-1,-0.5,-1);
		glVertex3f(-1,0.5,-1);
	glEnd();

	// -XY
	glColor3f(0.0f,0.0f,0.0f);;
	glBegin(GL_POLYGON);
		glVertex3f(-1,0.5,-1);
		glVertex3f(-1,-0.5,-1);
		glVertex3f(-1,-0.5,1);
		glVertex3f(-1,0.5,1);
	glEnd();

	glColor3f(0.0f,0.0f,0.0f);
	// +YZ
	glBegin(GL_POLYGON);
		glVertex3f(-1,0.5,-1);
		glVertex3f(1,0.5,-1);
		glVertex3f(1,0.5,1);
		glVertex3f(-1,0.5,1);
	glEnd();
	// -YZ
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-1,-0.5,1);
		glVertex3f(1,-0.5,1);
		glVertex3f(1,-0.5,-1);
		glVertex3f(-1,-0.5,-1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4,0.0,0.0);
//	glScalef(1,2,10);
	glScalef(0.1,1.0,0.1);
	glColor3f(0.0f,0.0f,0.0f);
	// +XZ	
	glBegin(GL_POLYGON);
		glVertex3f(-1,-1,1);
		glVertex3f(1,-1,1);
		glVertex3f(1,1,1);
		glVertex3f(-1,1,1);
	glEnd();
	// +XY
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(1,1,1);
		glVertex3f(1,-1,1);
		glVertex3f(1,-1,-1);
		glVertex3f(1,1,-1);
	glEnd();
	// -XZ
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(1,2.5,-1);
		glVertex3f(1,-2.5,-1);
		glVertex3f(-1,-2.5,-1);
		glVertex3f(-1,2.5,-1);
	glEnd();

	// -XY
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-1,2.5,-1);
		glVertex3f(-1,-2.5,-1);
		glVertex3f(-1,-2.5,1);
		glVertex3f(-1,2.5,1);
	glEnd();

	// +YZ
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-1,2.5,-1);
		glVertex3f(1,2.5,-1);
		glVertex3f(1,2.5,1);
		glVertex3f(-1,2.5,1);
	glEnd();
	// -YZ
	glColor3f(0.0f,0.0f,0.0f);
	//glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-1,-2.5,1);
		glVertex3f(1,-2.5,1);
		glVertex3f(1,-2.5,-1);
		glVertex3f(-1,-2.5,-1);
	glEnd();

	glPopMatrix();
	
	glPopMatrix();
	glColor3f(1.0f,1.0f,0.0f);
	
    }
	glutSwapBuffers();
    }
 

	else 
	{
	glPushMatrix();
		glRasterPos3f(-0.5f, 0.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		string S= "Game Over";
		for (int i=0;i<S.length();i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, S[i]);
		glPopMatrix();

		glPushMatrix();
		glRasterPos3f(-0.3f, -0.5f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		convert();
		for(int i=0;i<text.length();i++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
		glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
	}

}

void update(int value) {

	if(game == 0)
	{
	if (timer>0)
	{
		timer--;
	}
	if (timer == 0 && ca=='s')
	{
		ca = 'w';
	}
		

	//Advance the animation
	if (_model != NULL) {
		if ( ca == 'w' )
		{
			if ( walk == 1 )	
				_model->advance(0.125f);
			
			else
				_model->advance(0.0f);
		}
		else
		{
			_model->advance(0.0f);
		}
	}
	
	//Update _guyPos
	if ( Ttime > 800 )
		Ttime = 0;
	else
		Ttime++;
	_guyPos += 0.0f;
	while (_guyPos > FLOOR_TEXTURE_SIZE) {
		_guyPos -= FLOOR_TEXTURE_SIZE;
	}

	if (jump == 1)
	{
		if (f1 == 0)
		{
			movey += 0.1;
			move += 0.15;
		}
			
		if (movey >= 0.7)
		{
			f1 = 1;
		}
		if (f1 == 1)
		{
			movey -= 0.1;
			move += 0.15;
		}
		if (movey <= -3.5)
		{
			jump = 0;
			movey = -3.5f;
			f1 = 0;
		}
	}

	for(size_t k=0;k<hurdle1.size();k++)
    	{
		if (move >= hurdle1[k].second - 0.5 && move <= hurdle1[k].second + 0.5 && movex <= hurdle1[k].first + 0.5 && movex >= hurdle1[k].first - 0.5 && movey <= -1.0  && walk == 1 && ca == 'w')//&& movey >= -3.5)
		{
			//cout<<move<<endl;
			//cout<<hurdle1[k].second<<endl;
			game = 1;
		}
		//else if(movey <= 5.0 && move == hurdle1[k].second && movex == hurdle1[k].first )
	}
	}



	for(size_t k=0;k<fossil.size();k++)
	{
		if(move <= fossil[k].second + 0.25 && move >= fossil[k].second - 0.25 && ta[k] == 0 && movex == fossil[k].first)
		{
			score++;
			ta[k] = 1;
		}
	}
	//cout<<score<<endl;
	
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}





void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            spin = (spin + 30) % 360;
            glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}
int main(int argc, char** argv) {
	int i; 
	for(int i=0;i<1000;i++)
	{
		ta[i] = 0;
	}
	for(i=-500;i<500;i++){
		if(i%80==0 && i!=0){
			hurdle1.push_back(make_pair(0,i));		
		}

	}
	for(i = 0;i < 100;i++)
	{
		int n = (rand()%(500+500))-500;
		fossil.push_back(make_pair(0,n));
	}	
	for(i=-500;i<500;i++){
		if(i%60==0 && i!=0){
			hurdle1.push_back(make_pair(10,i));		
		}
	}
	for(i = 0;i < 100;i++)
	{
		int n = (rand()%(500+500))-500;
		fossil.push_back(make_pair(10,n));
	}	
	for(int i=-500;i<500;i++){
		if(i%50==0 && i!=0){
			hurdle1.push_back(make_pair(-10,i));		
		}
	}
	for(int i = 0;i < 100;i++)
	{
		int n = (rand()%(500+500))-500;
		fossil.push_back(make_pair(-10,n));
	}	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("Animation");
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMouseFunc(mouse);
       	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}
