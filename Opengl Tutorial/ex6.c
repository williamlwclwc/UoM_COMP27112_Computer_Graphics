#include <GL/glut.h>
#include <stdio.h>

GLfloat angle = 0.0;

void spin()
{
    angle+=1.0;
    glutPostRedisplay();
}

// call when opengl needs to update the display
void display()
{
    glClearColor(0.9, 0.9, 0.9, 0.0); // set grey background
    glClear(GL_COLOR_BUFFER_BIT); // clear the window
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angle, 1, 0, 0);
    glRotatef(angle, 0, 1, 0);
    glRotatef(angle, 0, 0, 1);
    glColor3f(1.0, 0.0, 0.0);
    glutWireCube(2.0);
    glFlush(); // force update screen
}

// call when a key is pressed
void keyboard(unsigned char key, int x, int y)
{
    if(key == 27) exit(0);
    else printf("You pressed %c\n", key);
}

// call when the window is created, moved, or resized
void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION); //Select the projection matrix
    glLoadIdentity (); //Initialise it
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Initialise Opengl
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ex6 A rotating cube"); // create the window
    glutDisplayFunc(display); // register "display" function
    glutKeyboardFunc(keyboard); // register "keyboard" function
    glutReshapeFunc(reshape);
    glutIdleFunc(spin);
    glutMainLoop();
    return 0;
}