#include <stdio.h>
#include <GL/glut.h>

// constants

GLclampf COLOR_SKY_R = 0.53f,
        COLOR_SKY_G = 0.8f,
        COLOR_SKY_B = 0.92f;

GLclampf COLOR_FIELD_R = 0.09f,
        COLOR_FIELD_G = 0.52f,
        COLOR_FIELD_B = 0.07f;

GLfloat GROUND_WIDTH = 30.0f,
        GROUND_LENGTH = 50.0f,
        GROUND_HEIGHT = 0.25f;

GLfloat FIELD_WIDTH = 25.0f,
        FIELD_LENGTH = 45.0f;

GLfloat CROSSBAR_LENGTH = 5.0f,
        CROSSBAR_HEIGHT = 2.2f;

GLdouble RADIUS_BALL = 0.25;

// control variables

GLfloat aspect = 1;

GLdouble obsX = 0,
        obsY = 30,
        obsZ = 30;

GLfloat ballTranslateX = 0,
        ballTranslateZ = 0;

GLfloat ballRotateX = 0,
        ballRotateZ = 0;

// functions

void init() {
    glClearColor(COLOR_SKY_R, COLOR_SKY_G, COLOR_SKY_B, 1);
}

void drawField() {
    glPushMatrix();
        glColor3f(COLOR_FIELD_R, COLOR_FIELD_G, COLOR_FIELD_B);
        glScalef(GROUND_LENGTH, GROUND_HEIGHT, GROUND_WIDTH);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void drawBall() {
    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(ballTranslateX, 0.35f, ballTranslateZ);
        glutWireSphere(RADIUS_BALL, 20, 20);
    glPopMatrix();
}

void drawCrossbar(GLfloat crossbarTopX) {
    GLfloat crossbarSideY = CROSSBAR_HEIGHT / 2;
    GLfloat crossbarRightZ = CROSSBAR_LENGTH / 2;
    GLfloat crossbarLeftZ = -1 * crossbarRightZ;

    glColor3f(1, 1, 1);

    glPushMatrix();
        glTranslatef(crossbarTopX, CROSSBAR_HEIGHT, 0);
        glScalef(0.1, 0.1, 5);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(crossbarTopX, crossbarSideY, crossbarLeftZ);
        glScalef(0.1, CROSSBAR_HEIGHT, 0.1);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(crossbarTopX, crossbarSideY, crossbarRightZ);
        glScalef(0.1, CROSSBAR_HEIGHT, 0.1);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void drawCrossbars() {
    GLfloat rightCrossbarTopX = (GROUND_LENGTH / 2) - (GROUND_LENGTH - FIELD_LENGTH) / 2;
    GLfloat leftCrossbarTopX = -(GROUND_LENGTH / 2) + (GROUND_LENGTH - FIELD_LENGTH) / 2;

    drawCrossbar(rightCrossbarTopX);
    drawCrossbar(leftCrossbarTopX);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawField();
    drawBall();
    drawCrossbars();
    drawLines();

    glFlush();
}

void view() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspect, 0.5, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        obsX, obsY, obsZ,
        0, 0, 0,
        0, 1, 0
    );
}

void reshape(int width, int height) {
    if (height == 0) return;

    glViewport(0, 0, width, height);
    aspect = (GLfloat) width / (GLfloat) height;
    view();
}

void onKeyPress(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
            ballTranslateZ -= 0.25f;
            break;
		case 'a':
            ballTranslateX -= 0.25f;
			break;
		case 's':
            ballTranslateZ += 0.25f;
			break;
		case 'd':
            ballTranslateX += 0.25f;
			break;
		default:
			break;
	}
    glutPostRedisplay();
}

void onSpecialKeyPress(int key, int x, int y) {
    switch (key) {
		case GLUT_KEY_LEFT:
            obsX -= 1;
			break;
        case GLUT_KEY_RIGHT:
			obsX += 1;
            break;
		case GLUT_KEY_UP:
			obsY += 1;
            break;
		case GLUT_KEY_DOWN:
            obsY -= 1;
			break;
        case GLUT_KEY_PAGE_UP:
            obsZ -= 1;
            break;
        case GLUT_KEY_PAGE_DOWN:
            obsZ += 1;
            break;
		default:
			break;
	}
    glLoadIdentity();
    gluLookAt(
        obsX, obsY, obsZ,
        0, 0, 0,
        0, 1, 0
    );
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Football Field 3D");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(onKeyPress);
    glutSpecialFunc(onSpecialKeyPress);
    init();
    glutMainLoop();

    return 0;
}
