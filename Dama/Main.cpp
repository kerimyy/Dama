#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <windows.h>
using namespace std;

const double IKI_PI = 6.2831853;

const int windowWidth = 800;
const int windowHeight = 800;

char stones[8][8] = {
	{'x','x','x','x','x','x','x','x'},
	{'b','b','b','b','b','b','b','b'},
	{'b','b','b','b','b','b','b','b'},
	{'x','x','x','x','x','x','x','x'},
	{'x','x','x','x','x','x','x','x'},
	{'s','s','s','s','s','s','s','s'},
	{'s','s','s','s','s','s','s','s'},
	{'x','x','x','x','x','x','x','x'}
};
int c = 0;

int selectedX = -1;
int selectedY = -1;
bool turn = 0;

void drawSquare(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4,GLboolean selected)
{
	// if color is 0 then draw white box and change value of color = 1
	if (c == 0)
	{
		 // white color value is 1 1 1
		glColor3f(0.7f, 0.7f, 0.5f);
		c = 1;
	}
	// if color is 1 then draw black box and change value of color = 0
	else
	{
		glColor3f(0.3f, 0.15f, 0.02f); // black color value is 0 0 0
		c = 0;
	}

	if (selected) {
		glColor3f(0.2f, 0.2f, 0.9f);
	}

	// Draw Square
	glBegin(GL_POLYGON);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glEnd();
}
void chessboard()
{
	//glClear(GL_COLOR_BUFFER_BIT); // Clear display window
	GLint x, y;

	for (x = 0; x <= 800; x += 100)
	{
		for (y = 0; y <= 800; y += 100)
		{
			if (selectedX == x / 100 && selectedY == y / 100) {
				drawSquare(x, y + 100, x + 100, y + 100, x + 100, y, x, y,true);
			}
			else {
				drawSquare(x, y + 100, x + 100, y + 100, x + 100, y, x, y,false);
			}
			
		}
	}
	// Process all OpenGL routine s as quickly as possible
	glFlush();
	c = 0;
}


void drawStone(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 30; 
	
	GLfloat twicePi = IKI_PI;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
}


void init(void)
{
	//glClearColor(1.0, 1.0, 1.0, 0.0);		//set display window color to white
	glMatrixMode(GL_PROJECTION);		//set projection parameters
	gluOrtho2D(0, windowWidth, windowHeight, 0);

}
void update(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	chessboard();
	GLint x, y;
	
	for (y = 0; y < 8; y++) {
		for (x = 0;x < 8; x++) {
			if (stones[y][x] == 'b') {
				glColor3f(1.0f, 1.0f, 1.0f);
				drawStone(x*100+50, y*100+50, 35);
			}
			else if (stones[y][x] == 's') {
				glColor3f(0.0f, 0.0f, 0.0f);
				drawStone(x * 100 + 50, y * 100 + 50, 35);
			}

			else if (stones[y][x] == 'o') {
				glColor3f(0.2f, 0.7f, 0.2f);
				drawStone(x * 100 + 50, y * 100 + 50, 20);
			}
		}
	}


	//glColor3f(0.0f, 1.0f, 0.5f);
	//glBegin(GL_LINES);
				//set line segment color to green
	//glVertex2i(180, 145);				//specify line segment geometry
	//glVertex2i(10, 15);
	//glEnd();

	glFlush();	//process all openGL routines as quickly as possible 
}

void stoneArray() {
	
}

void clearPath() {
	GLint x, y;
	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			if (stones[y][x] == 'o') {
				stones[y][x] = 'x';
			}
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	// Save the left button state
	if (button == GLUT_LEFT_BUTTON)
	{	
		
		if (state == GLUT_DOWN) {

			//glClear(GL_COLOR_BUFFER_BIT);
			//chessboard();
			//clearPath();
			x /= 100;
			y /= 100;
			cout << "x =" << x << endl;
			cout << "y = " << y << endl;
			cout << "þu var = " << stones[y][x] << endl;
			if (stones[y][x] != 'x' && stones[y][x] != 'o') {
				
				clearPath();
			}
			
			if (stones[y][x] == 's' && turn == 0) {
				selectedX = x;
				selectedY = y;
				if (stones[y - 1][x] == 'x') {
					stones[y - 1][x] = 'o';
				}
				if (x!= 7 && stones[y][x + 1] == 'x') {
					stones[y][x + 1] = 'o';
				}
				if (x != 0 && stones[y][x - 1] == 'x') {
					stones[y][x - 1] = 'o';
				}
			}

			else if (stones[y][x] == 'b' && turn == 1) {
				selectedX = x;
				selectedY = y;
				if (stones[y + 1][x] == 'x') {
					stones[y + 1][x] = 'o';
				}
				if (x != 7 && stones[y][x + 1] == 'x') {
					stones[y][x + 1] = 'o';
				}
				if (x != 0 && stones[y][x - 1] == 'x') {
					stones[y][x - 1] = 'o';
				}
			}

			else if (stones[y][x] == 'o') {
				stones[y][x] = stones[selectedY][selectedX];
				stones[selectedY][selectedX] = 'x';
				selectedX = -1;
				selectedY = -1;
				clearPath();
				if (turn == 0) turn = 1;
				else turn = 0;
			}
			
			
		}
		
		glutPostRedisplay(); 
	}

	
	
}

int main(int argc, _TCHAR* argv[])
{
	glutInit(&argc, (char**)argv);  // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //set display mode
	glutInitWindowPosition(50, 100); // Position the window's initial top-left corner
	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutCreateWindow("Dama");
	glutMouseFunc(mouse);	// Create a window with the given title
	init();	
	
	glutDisplayFunc(update);
	
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;




}