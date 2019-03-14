/*
==========================================================================
File:        ex2.c 
Author:     Wenchang Liu
==========================================================================
*/

/* The following ratios are not to scale: */
/* Moon orbit : planet orbit */
/* Orbit radius : body radius */
/* Sun radius : planet radius */

#ifdef MACOS
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BODIES 25
#define TOP_VIEW 1
#define ECLIPTIC_VIEW 2
#define SHIP_VIEW 3
#define EARTH_VIEW 4
#define PI 3.141593
#define DEG_TO_RAD 0.01745329
#define ORBIT_POLY_SIDES 50
#define TIME_STEP 0.5   /* days per frame */

#define MILLION 1000000
#define RUN_SPEED  1.6*MILLION
#define TURN_ANGLE 4.0

typedef struct { 
  char    name[25];       /* name */
  GLfloat r,g,b;          /* colour */
  GLfloat orbital_radius; /* distance to parent body (km) */
  GLfloat orbital_tilt;   /* angle of orbit wrt ecliptic (deg) */
  GLfloat orbital_period; /* time taken to orbit (days) */
  GLfloat radius;         /* radius of body (km) */
  GLfloat axis_tilt;      /* tilt of axis wrt body's orbital plane (deg) */
  GLfloat rot_period;     /* body's period of rotation (days) */
  GLint   orbits_body;    /* identifier of parent body */
  GLfloat spin;           /* current spin value (deg) */
  GLfloat orbit;          /* current orbit value (deg) */
} body;

body  bodies[MAX_BODIES];
int   numBodies, current_view, draw_orbits, draw_labels, draw_starfield;
float  date;
int draw_axs;
float stars[3000];
GLdouble lat = 0, lon = 0;
GLfloat dir_x, dir_y, dir_z;
GLfloat eyex = 0, eyey = 0, eyez = -300*MILLION;


/*****************************/

float myRand (void)
{
  /* return a random float in the range [0,1] */

  return (float) rand() / RAND_MAX;
}

/*****************************/

void starfieldGenerator(float *stars)
{
  int i;
  for(i = 0; i < 3000; i++)
  {
    stars[i] = (myRand()-myRand())*300*MILLION;
  }
}

void drawStarfield (float *stars)
{
  /* This is for you to complete. */
  glBegin(GL_POINTS);
  glColor3f(1, 1, 1);
  int i;
  for(i = 0; i < 3000; i++)
  {
    glVertex3f(stars[i], stars[i+1], stars[i+3]);
  }
  glEnd();
}

/*****************************/

void readSystem(void)
{
  /* reads in the description of the solar system */

  FILE *f;
  int i;

  f= fopen("sys", "r");
  if (f == NULL) {
      printf("ex2.c: Couldn't open 'sys'\n");
      printf("To get this file, use the following command:\n");
      printf("  cp /opt/info/courses/COMP27112/ex2/sys .\n");
      exit(0);
  }
  fscanf(f, "%d", &numBodies);
  for (i= 0; i < numBodies; i++)
  {
    fscanf(f, "%s %f %f %f %f %f %f %f %f %f %d", 
      bodies[i].name,
      &bodies[i].r, &bodies[i].g, &bodies[i].b,
      &bodies[i].orbital_radius,
      &bodies[i].orbital_tilt,
      &bodies[i].orbital_period,
      &bodies[i].radius,
      &bodies[i].axis_tilt,
      &bodies[i].rot_period,
      &bodies[i].orbits_body);

    /* Initialise the body's state */
    bodies[i].spin= 0.0;
    bodies[i].orbit= myRand() * 360.0; /* Start each body's orbit at a
                                          random angle */
    bodies[i].radius*= 1000.0; /* Magnify the radii to make them visible */
  }
  fclose(f);
}

/*****************************/

void drawString (void *font, float x, float y, char *str)
{ /* Displays the string "str" at (x,y,0), using font "font" */

  /* This is for you to complete. */

}

/*****************************/

void setView (void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (current_view) {
  case TOP_VIEW:
    /* This is for you to complete. */
    gluLookAt(0.1, 550.0*MILLION, 0.1,  // camera position 
              0.0, 0.0, 0.0,    // point to look at
              0.0, 1.0, 0.0 );  // "upright" vector
    break;  
  case ECLIPTIC_VIEW:
    /* This is for you to complete. */
    // from far out z axis
    gluLookAt(0.1, 0.1, 400.0*MILLION,  // camera position 
              0.0, 0.0, 0.0,    // point to look at
              0.0, 1.0, 0.0 );  // "upright" vector
    break;  
  case SHIP_VIEW:
    /* This is for you to complete. */  
    // from somewhere in space
    dir_x = 300*MILLION* cos(DEG_TO_RAD * lat) * sin(DEG_TO_RAD * lon);
    dir_y = 300*MILLION* sin(DEG_TO_RAD * lat);
    dir_z = 300*MILLION* cos(DEG_TO_RAD * lat) * cos(DEG_TO_RAD * lon);
    gluLookAt(eyex, eyey, eyez,
              dir_x + eyex, dir_y + eyey, dir_z + eyez,
              0.0, 1.0, 0.0);
    // gluLookAt(400*MILLION, 400*MILLION, 50*MILLION,  
    //           0.0, 0.0, 0.0,    
    //           0.0, 1.0, 0.0 );
    break;  
  case EARTH_VIEW: 
    /* This is for you to complete. */
    // from earth
    gluLookAt(bodies[3].orbital_radius * cos(bodies[3].orbit * DEG_TO_RAD),
              bodies[3].radius * 2.0,
              bodies[3].orbital_radius * sin(bodies[3].orbit * DEG_TO_RAD),
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    break;  
  }
}

/*****************************/

void menu (int menuentry) {
  switch (menuentry) {
  case 1: current_view= TOP_VIEW;
          break;
  case 2: current_view= ECLIPTIC_VIEW;
          break;
  case 3: current_view= SHIP_VIEW;
          break;
  case 4: current_view= EARTH_VIEW;
          break;
  case 5: draw_labels= !draw_labels;
          break;
  case 6: draw_orbits= !draw_orbits;
          break;
  case 7: draw_starfield= !draw_starfield;
          break;
  case 8: exit(0);
  }
}

/*****************************/

void init(void)
{
  /* Define background colour */
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glutCreateMenu (menu);
  glutAddMenuEntry ("Top view", 1);
  glutAddMenuEntry ("Ecliptic view", 2);
  glutAddMenuEntry ("Spaceship view", 3);
  glutAddMenuEntry ("Earth view", 4);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Toggle labels", 5);
  glutAddMenuEntry ("Toggle orbits", 6);
  glutAddMenuEntry ("Toggle starfield", 7);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Quit", 8);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

  current_view= TOP_VIEW;
  // current_view = SHIP_VIEW;
  draw_labels= 1;
  draw_orbits= 1;
  draw_starfield= 1;
  draw_axs = 1;
}

/*****************************/

void animate(void)
{
  int i;

    date+= TIME_STEP;

    for (i= 0; i < numBodies; i++)  {
      bodies[i].spin += 360.0 * TIME_STEP / bodies[i].rot_period;
      bodies[i].orbit += 360.0 * TIME_STEP / bodies[i].orbital_period;
      glutPostRedisplay();
    }
}

/*****************************/

void drawOrbit (int n)
{ /* Draws a polygon to approximate the circular
     orbit of body "n" */

    /* This is for you to complete. */
    int init_angle = 0;
    double step = 360.0 / ORBIT_POLY_SIDES;
    double orb_len = bodies[n].orbital_radius;

    float pre_x = orb_len * cos(init_angle * DEG_TO_RAD);
    float pre_z = orb_len * sin(init_angle * DEG_TO_RAD);

    glColor3f(bodies[n].r, bodies[n].g, bodies[n].b);
    float x, z;
    int i;
    for(i = 0; i <= 400; i = i+step)
    {
      x = orb_len * cos(i * DEG_TO_RAD);
      z = orb_len * sin(i * DEG_TO_RAD);

      glBegin(GL_LINES);
      glVertex3f(pre_x, 0.0, pre_z);
      glVertex3f(x, 0.0, z);
      glEnd();

      pre_x = x;
      pre_z = z;
    }
}

/*****************************/

void drawLabel(int n)
{ /* Draws the name of body "n" */

    /* This is for you to complete. */

}

/*****************************/

void drawBody(int n)
{
 /* Draws body "n" */

    /* This is for you to complete. */
    float r = bodies[n].r;
    float g = bodies[n].g;
    float b = bodies[n].b;

    // check if body is moon: parent is not sun
    int parent = bodies[n].orbits_body;
    if(parent != 0)
    {
      // Totilt: tilted orbit
      glRotatef(bodies[parent].orbital_tilt, 0.0, 0.0, 1.0);

      // Torb:
      glTranslatef(bodies[parent].orbital_radius * cos(bodies[parent].orbit * DEG_TO_RAD),
                    0.0,
                   bodies[parent].orbital_radius * sin(bodies[parent].orbit * DEG_TO_RAD));
    }

    // Totilt: tilted orbit
    glRotatef(bodies[n].orbital_tilt, 0.0, 0.0, 1.0);

    if(draw_orbits) drawOrbit(n);

    // Torb:
    glTranslatef(bodies[n].orbital_radius * cos(bodies[n].orbit * DEG_TO_RAD),
                  0.0,
                 bodies[n].orbital_radius * sin(bodies[n].orbit * DEG_TO_RAD));

    // Tatilt: tilted axis
    glRotatef(bodies[n].axis_tilt, 0.0, 0.0, 1.0);

    // draw axis
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 1.5 * bodies[n].radius, 0.0);
    glVertex3f(0.0, -1.5 * bodies[n].radius, 0.0);
    glEnd();

    // Tspin: spin
    glRotatef(bodies[n].spin , 0.0, 1.0, 0.0);

    // Tvert:
    glRotatef(90.0, 1.0, 0.0, 0.0);

    // draw planet
    glColor3f(r, g, b);
    glutWireSphere(bodies[n].radius, 20, 30);
}

/*****************************/

void drawAxes (void) {

// Draws X Y and Z axis lines, of length LEN

  float LEN= 1000.0*MILLION*MILLION;

  glLineWidth(5.0);

  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0); // red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(LEN, 0.0, 0.0);

  glColor3f(0.0,1.0,0.0); // green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, LEN, 0.0);

  glColor3f(0.0,0.0,1.0); // blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, LEN);
  glEnd();

  glLineWidth(1.0);
}

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT);

  /* set the camera */
  setView();

  if (draw_starfield) drawStarfield(stars);

  if (draw_axs) drawAxes();

  for (i= 0; i < numBodies; i++)
  {
    glPushMatrix();
      drawBody (i);
    glPopMatrix();
  }

  glutSwapBuffers();
}

/*****************************/

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (48.0, (GLfloat)w/(GLfloat)h,  10000.0, 800000000.0);
}
  
/*****************************/

void keyboard(unsigned char key, int x, int y)
{
  GLdouble heading = 0;
  switch(key)
  {
    case 97:
      draw_axs = !draw_axs;
      break;

    case 87: // W
      // move to left
      heading = 90;
      eyey += RUN_SPEED * sin(DEG_TO_RAD*heading);
      break;
    case 83: // S
      // move to right
      heading = -90;
      eyey += RUN_SPEED * sin(DEG_TO_RAD*heading);
      break;
    case 65: // A
      // move to left
      heading = lon + 90;
      eyex += RUN_SPEED * sin(DEG_TO_RAD*heading);
      eyez += RUN_SPEED * cos(DEG_TO_RAD*heading);
      break;
    case 68: // D
      // move to right
      heading = lon - 90;
      eyex += RUN_SPEED * sin(DEG_TO_RAD*heading);
      eyez += RUN_SPEED * cos(DEG_TO_RAD*heading);
      break;
    case 27:  /* Escape key */
      exit(0);
  }
}

void cursor_keys(int key, int x, int y) 
{
  GLdouble temp = 0;
  GLdouble heading = 0;
  switch (key) 
  {
    /* To be completed */
    case GLUT_KEY_LEFT:
      // rotate to the left
      temp = lon + TURN_ANGLE;
      lon = temp;
      break;
    case GLUT_KEY_RIGHT:
      // rotate to the right
      temp = lon - TURN_ANGLE;
      lon = temp;
      break;
    case GLUT_KEY_PAGE_UP:
      // tilt up
      temp = lat + TURN_ANGLE;
      if(temp < 90 && temp > -90)
      {
        lat = temp;
      }
      break;
    case GLUT_KEY_PAGE_DOWN:
      // tilt down
      temp = lat - TURN_ANGLE;
      if(temp < 90 && temp > -90)
      {
        lat = temp;
      }
      break;
    case GLUT_KEY_HOME:
      // re-center lat
      lat = 0;
      break;
    case GLUT_KEY_UP:
      // step forwards 
      heading = lon;
      eyex += RUN_SPEED * sin(DEG_TO_RAD*heading);
      eyez += RUN_SPEED * cos(DEG_TO_RAD*heading);
      break;
    case GLUT_KEY_DOWN:
      // step backwards
      heading = lon;
      eyex -= RUN_SPEED * sin(DEG_TO_RAD*heading);
      eyez -= RUN_SPEED * cos(DEG_TO_RAD*heading);
      break;
  }
} 

/*****************************/

int main(int argc, char** argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow ("COMP27112 Exercise 2");
  glutFullScreen();
  init();
  starfieldGenerator(stars);
  glutDisplayFunc (display); 
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard);
  glutSpecialFunc (cursor_keys); // add cursor keys
  glutIdleFunc (animate);
  readSystem();
  glutMainLoop();
  return 0;
}
/* end of ex2.c */
