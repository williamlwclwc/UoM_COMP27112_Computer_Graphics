#include <GL/glut.h>
#include <stdio.h>

// call when opengl needs to update the display
void display()
{
    glClearColor(0.9, 0.9, 0.9, 0.0); // set grey background
    glClear(GL_COLOR_BUFFER_BIT); // clear the window
    glFlush(); // force update of screen
}

// call when a key is pressed
void keyboard(unsigned char key, int x, int y)
{
    if(key == 27) exit(0);
    else printf("You pressed %c\n", key);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Initialise Opengl
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ex3"); // create the window
    glutDisplayFunc(display); // register "display" function
    glutKeyboardFunc(keyboard); // register "keyboard" function
    glutMainLoop();
    return 0;
}