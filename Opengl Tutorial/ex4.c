#include <GL/glut.h>
#include <stdio.h>

// call when opengl needs to update the display
void display()
{
    glClearColor(0.9, 0.9, 0.9, 0.0); // set grey background
    glClear(GL_COLOR_BUFFER_BIT); // clear the window

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.3, -0.3, 0.0);
        glVertex3f(0.0, 0.3, 0.0);
        glVertex3f(0.3, -0.3, 0.0);
    glEnd();

    glFlush(); // force update of screen
}

// call when a key is pressed
void keyboard(unsigned char key, int x, int y)
{
    if(key == 27) exit(0);
    else printf("You pressed %c\n", key);
}

// call when the window is created, moved, or resized
void reshape(int width, int height)
{
    glViewport (0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode (GL_PROJECTION); //Select the projection matrix
    glLoadIdentity (); //Initialise it
    glOrtho(-1.0,1.0, -1.0,1.0, -1.0,1.0);/*The unit cube*/
    glMatrixMode (GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Initialise Opengl
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ex4"); // create the window
    glutDisplayFunc(display); // register "display" function
    glutKeyboardFunc(keyboard); // register "keyboard" function
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}