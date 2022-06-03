#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <windows.h>
using namespace std;


const int windowWidth = 800;
const int windowHeight = 800;

char board[8][8] = {
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
int deadStoneX = -1;
int deadStoneY = -1;
bool turn = 0;
bool forcedMove = 0;

void drawSquare(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4,GLboolean selected)
{
	
	if (c == 0)
	{
		 
		glColor3f(0.7f, 0.7f, 0.5f);
		c = 1;
	}
	
	else
	{
		glColor3f(0.3f, 0.15f, 0.02f); 
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
	int triangleAmount = 40; 
	
	GLfloat twicePi = 6.2831853;
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
	glClearColor(1.0, 1.0, 1.0, 0.0);		
	glMatrixMode(GL_PROJECTION);		
	gluOrtho2D(0, windowWidth, windowHeight, 0);

}
void update(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);
	chessboard();
	GLint x, y;
	
	for (y = 0; y < 8; y++) {
		for (x = 0;x < 8; x++) {
			if (board[y][x] == 'b') {
				glColor3f(1.0f, 1.0f, 1.0f);
				drawStone(x*100+50, y*100+50, 35);
			}
			else if (board[y][x] == 's') {
				glColor3f(0.0f, 0.0f, 0.0f);
				drawStone(x * 100 + 50, y * 100 + 50, 35);
			}

			else if (board[y][x] == 'o') {
				glColor3f(0.2f, 0.7f, 0.2f);
				drawStone(x * 100 + 50, y * 100 + 50, 20);
			}

			else if (board[y][x] == 'y') {
				glColor3f(0.8f, 0.2f, 0.2f);
				drawStone(x * 100 + 50, y * 100 + 50, 20);
			}
		}
	}




	glFlush();	
}



void clearBoard() {
	GLint x, y;
	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			if (board[y][x] == 'o' || board[y][x] == 'y') {

				board[y][x] = 'x';
			}
		}
	}
}
void forceControl(int lastY, int lastX) {
	forcedMove = 0;
	if (turn)
	{
		if (board[lastY - 1][lastX] == 'x' && board[lastY + 1][lastX] == 's' ||
			board[lastY][lastX - 1] == 'x' && board[lastY][lastX + 1] == 's' ||
			board[lastY][lastX + 1] == 'x' && board[lastY, lastX - 1] == "s") {
			forcedMove = 1;
		}

	}
	else
	{
		if (board[lastY + 1][lastX] == 'x' && board[lastY - 1][lastX] == 'b' ||
			board[lastY][lastX - 1] == 'x' && board[lastY][lastX + 1] == 'b' ||
			board[lastY][lastX + 1] == 'x' && board[lastY, lastX - 1] == "b") {
			forcedMove = 1;
		}
	}
}
void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
	{	
		
		if (state == GLUT_DOWN) {

			x /= 100;
			y /= 100;
			cout << "x =" << x << endl;
			cout << "y = " << y << endl;
			cout << " = " << board[y][x] << endl;
			bool jump = false;
			if (board[y][x] != 'x' && board[y][x] != 'o' && board[y][x] != 'y') {
				
				clearBoard();
			}
			
			if (board[y][x] == 's' && turn == 0) {
				selectedX = x;
				selectedY = y;
				jump = false;

				if (board[y - 1][x] == 'b' && board[y - 2][x] == 'x') {
					board[y - 2][x] = 'y';
					deadStoneX = x;
					deadStoneY = y - 1;
					jump = true;
				}
				if (x < 6 && board[y][x + 1] == 'b' && board[y][x + 2] == 'x') {
					board[y][x + 2] = 'y';
					deadStoneX = x + 1;
					deadStoneY = y;
					jump = true;
				}
				if (x > 1 && board[y][x - 1] == 'b' && board[y][x - 2] == 'x') {
					board[y][x - 2] = 'y';
					deadStoneX = x - 1;
					deadStoneY = y;
					jump = true;
				}

				if (!jump && !forcedMove) {
					if (board[y - 1][x] == 'x') {
						board[y - 1][x] = 'o';
					}
					if (x != 7 && board[y][x + 1] == 'x') {
						board[y][x + 1] = 'o';
					}
					if (x != 0 && board[y][x - 1] == 'x') {
						board[y][x - 1] = 'o';
					}
				}

				
			}

			else if (board[y][x] == 'b' && turn == 1) {
				selectedX = x;
				selectedY = y;
				jump = false;

				if (board[y + 1][x] == 's' && board[y + 2][x] == 'x') {
					board[y + 2][x] = 'y';
					deadStoneX = x;
					deadStoneY = y + 1;
					jump = true;					
				}
				if (x < 6 && board[y][x + 1] == 's' && board[y][x + 2] == 'x') {
					board[y][x + 2] = 'y';
					deadStoneX = x + 1;
					deadStoneY = y;
					jump = true;					
				}
				if (x > 1 && board[y][x - 1] == 's' && board[y][x - 2] == 'x') {
					board[y][x - 2] = 'y';
					deadStoneX = x - 1;
					deadStoneY = y;
					jump = true;
				}

				if (!jump && !forcedMove) {
					if (board[y + 1][x] == 'x') {
						board[y + 1][x] = 'o';
					}
					if (x != 7 && board[y][x + 1] == 'x') {
						board[y][x + 1] = 'o';
					}
					if (x != 0 && board[y][x - 1] == 'x') {
						board[y][x - 1] = 'o';
					}
				}
			}

			else if (board[y][x] == 'o') {
				board[y][x] = board[selectedY][selectedX];
				board[selectedY][selectedX] = 'x';
				selectedX = -1;
				selectedY = -1;
				clearBoard();
				forceControl(y, x);
				if (turn == 0) turn = 1;
				else turn = 0;
			}

			else if (board[y][x] == 'y') {
				board[y][x] = board[selectedY][selectedX];
				board[selectedY][selectedX] = 'x';
				board[deadStoneY][deadStoneX] = 'x';
				selectedX = -1;
				selectedY = -1;
				deadStoneY = -1;
				deadStoneX = -1;
				clearBoard();
				
				
				if (turn)
				{
					if (board[y + 1][x] == 's' && board[y + 2][x] == 'x') {
						board[y + 2][x] = 'y';
						deadStoneX = x;
						deadStoneY = y + 1;
						selectedX = x;
						selectedY = y;
						jump = true;
					}
					if (x < 6 && board[y][x + 1] == 's' && board[y][x + 2] == 'x') {
						board[y][x + 2] = 'y';
						deadStoneX = x + 1;
						deadStoneY = y;
						selectedX = x;
						selectedY = y;
						jump = true;
					}
					if (x > 1 && board[y][x - 1] == 's' && board[y][x - 2] == 'x') {
						board[y][x - 2] = 'y';
						deadStoneX = x - 1;
						deadStoneY = y;
						selectedX = x;
						selectedY = y;
						jump = true;
					}
				}
				else
				{
					if (board[y - 1][x] == 'b' && board[y - 2][x] == 'x') {
						board[y - 2][x] = 'y';
						deadStoneX = x;
						deadStoneY = y - 1;
						selectedX = x;
						selectedY = y;
						jump = true;
					}
					if (x < 6 && board[y][x + 1] == 'b' && board[y][x + 2] == 'x') {
						board[y][x + 2] = 'y';
						deadStoneX = x + 1;
						deadStoneY = y;
						selectedX = x;
						selectedY = y;
						jump = true;
					}
					if (x > 1 && board[y][x - 1] == 'b' && board[y][x - 2] == 'x') {
						board[y][x - 2] = 'y';
						deadStoneX = x - 1;
						deadStoneY = y;
						selectedX = x;
						selectedY = y;
						jump = true;
					}
				}

				
				if (!jump) {
					forceControl(y, x);
					if (turn == 0) turn = 1;
					else turn = 0;
				}
				

			}
			
			
		}
		
		glutPostRedisplay(); 
	}

	
	
}



int main(int argc, _TCHAR* argv[])
{
	glutInit(&argc, (char**)argv);  // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //set display mode
	glutInitWindowPosition(300, 100); // Position the window's initial top-left corner
	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutCreateWindow("Dama");
	glutMouseFunc(mouse);	// Create a window with the given title
	init();	
	
	glutDisplayFunc(update);
	
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;




}