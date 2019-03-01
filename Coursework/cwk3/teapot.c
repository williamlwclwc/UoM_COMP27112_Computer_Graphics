#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "bitmap.c"
#include "math.h"

/* 
This is for COMP27112 Course Assignment 3.

Author: Toby Howard, 5/04/12. 
 */

GLfloat xRotation, yRotation= 0.0;
GLint mouseX, mouseY;

GLfloat white_light[] =     { 0.5, 0.8, 0.4, 1.0 };
GLfloat another_light[] =   { 0.6, 0.7, 0.8, 1.0 };
GLfloat light_position0[] = { 3.0, 4.0, 5.0, 0.0 };
GLfloat light_position1[] = { 5.0, 5.0, 5.0, 0.0 };
GLfloat matSpecular[] =     { 1.0, 1.0, 1.0, 0.0 };
GLfloat matShininess[] =    { 80.0 };
GLfloat matSurface[] =      { 0.0, 1.0, 0.0, 0.0 };

BITMAPINFO *TexInfo; // Texture bitmap information
GLubyte    *TexBits; // Texture bitmap pixel bits

GLint shademodel = 1;

void initialise(void) {
   // TexBits = LoadDIBitmap("splodge.bmp", &TexInfo);
   TexBits = LoadDIBitmap("coyote.bmp", &TexInfo);

   glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
   glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

   // another light
   glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, another_light);
   glLightfv(GL_LIGHT1, GL_SPECULAR, another_light);
 
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   // Once enabled OpenGL automatically stores fragments their z-values in the depth buffer if they passed the depth test 
   // and discards fragments if they failed the depth test accordingly. 
   // If you have depth testing enabled you should also clear the depth buffer before each render iteration using the GL_DEPTH_BUFFER_BIT, 
   // otherwise you're stuck with the written depth values from last render iteration:
   glEnable(GL_DEPTH_TEST); // z buffer enabled
   glShadeModel(GL_SMOOTH);
   // glShadeModel(GL_FLAT);
}

float dist (float x, float y, float z) {
// returns distance to origin from point (x,y,z)
return(fabs(sqrt(x*x + y*y + z*z)));
} // dist()

int within(float x, float y, float z, float inner, float outer) {
// check if (x,y,z) lies between "inner" and "outer"
float d;
d= dist(x,y,z);
if ((d > inner) && (d < outer)) {return(1);}
else {return(0);}
} // within()

void energy(float ringMin, float ringMax, float r, float g, float b) {
// Draw a ring of energy, comprising a swarm of particles in random positions, but 
// bounded to lie withing ringMin-ringMax, the space between two concentric spheres
int i;
float R= 10.0; // to get rands in range 0-R
float x,y,z;

glDisable(GL_LIGHTING); // don't want lighting on
glEnable(GL_POINT_SMOOTH);
glPointSize(2.0);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i= 0; i<10001; i++) {
   // get rand p in range -R/2 < p < +R/2
   x= R* rand() / (((double)RAND_MAX+1)) -R/2.0;
   y= R* rand() / (((double)RAND_MAX+1)) -R/2.0;
   z= 0.0; // to make it a flat ring
   if (within(x,y,z, ringMin, ringMax)) glVertex3f(x,y,z);
}
glEnd();
glEnable(GL_LIGHTING);
} // energy()

void display(void) {  
   glClearColor(70/255.0, 130/255.0, 180/255.0, 1.0); // dodger blue
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// define material properties
   glMaterialfv(GL_FRONT, GL_SPECULAR,  matSpecular);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
   glMaterialfv(GL_FRONT, GL_AMBIENT,   matSurface);

   // code to set up texturing...

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo->bmiHeader.biWidth,
                TexInfo->bmiHeader.biHeight, 0, GL_BGR_EXT,
                GL_UNSIGNED_BYTE, TexBits);
   // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_INTERPOLATE);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glEnable(GL_TEXTURE_2D);

 // draw the teapot
   glPushMatrix();
      glRotatef(xRotation, 1.0, 0.0, 0.0);
      glRotatef(yRotation, 0.0, 1.0, 0.0);
      glutSolidTeapot(0.7);
      for (int j=0; j<2; j++) 
      {
         glPushMatrix();
         glRotatef((j*2+1)*45.0, 1.0, 0.0, 0.0);
         energy(1.5, 1.75,  1.0,1.0,0.0); 
	      energy(1.75, 1.9,  1.0,0.0,0.0);
         glPopMatrix();
      }
   glPopMatrix();
   glutSwapBuffers();
}

void reshape(int w, int h) {
   glViewport(0, 0,(GLsizei) w,(GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(17.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(10.0,10.0,10.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
}


void keyboard(unsigned char key, int x, int y) {
   switch(key) {
	  case 27:  
         exit(0);
         break;
     // press s
     case 115:
         if(shademodel == 1)
         {
            glShadeModel(GL_FLAT);
            shademodel = -shademodel;
         }
         else
         {
            glShadeModel(GL_SMOOTH);
            shademodel = -shademodel;
         }
         // ask for redisplay
	      glutPostRedisplay();
         break;
   } 
}

void mouseMotion(int x, int y) {
// Called when mouse moves
	xRotation+=(y-mouseY);	mouseY= y;			
	yRotation+=(x-mouseX);	mouseX= x;			
	// keep all rotations between 0 and 360.
	if ( xRotation > 360.0) xRotation-= 360.0;	
	if ( xRotation < 0.0)   xRotation+= 360.0;	
	if ( yRotation > 360.0) yRotation-= 360.0;	
	if ( yRotation < 0.0)   yRotation+= 360.0;
	// ask for redisplay
	glutPostRedisplay();
}


void mousePress(int button, int state, int x, int y) {
// When left mouse button is pressed, save the mouse(x,y)
	if((button == GLUT_LEFT_BUTTON) &&(state == GLUT_DOWN)) {
		mouseX= x;	
		mouseY= y;
	}
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 800); 
   glutCreateWindow("COMP27112 Coursework Assignment 3: The Teapot");
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mousePress);		
   glutMotionFunc(mouseMotion);	
   initialise();
   glutMainLoop();
   return 0;
}
