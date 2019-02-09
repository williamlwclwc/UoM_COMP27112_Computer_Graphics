#include <GL/glut.h>

// call when opengl needs to update the display
void display ()
{
    glClearColor(0.9, 0.9, 0.9, 0.0); // set grey background
    glClear(GL_COLOR_BUFFER_BIT); // clear the window
    glFlush(); // force update of screen
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Initialise Opengl
    glutCreateWindow("ex1"); // create the window
    glutDisplayFunc(display); // register "display" function
    glutMainLoop();
    return 0;
}