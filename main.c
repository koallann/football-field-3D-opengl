#include <stdio.h>
#include <math.h>
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

GLdouble SPEED_BALL = 0.25;
GLdouble SPEED_CAMERA = 0.5;

// control variables

GLfloat aspect = 1;

GLdouble obsX = 0,
        obsY = 30,
        obsZ = 30;

GLfloat ballTranslateX = 0,
        ballTranslateZ = 0;

int isRotateX;

GLfloat rotateAngleX, rotateAngleZ;

GLdouble LIMIT_FIELD_TOP_X;
GLdouble LIMIT_FIELD_BOTTOM_X;
GLdouble LIMIT_FIELD_TOP_Z;
GLdouble LIMIT_FIELD_BOTTOM_Z;

GLdouble LIMIT_GROUND_BOTTOM_X;
GLdouble LIMIT_GROUND_TOP_X;

int ballWithinLeftNet = 0;
int leftGoals = 0;
int ballWithinRightNet = 0;
int rightGoals = 0;

// functions

void init() {
    glClearColor(COLOR_SKY_R, COLOR_SKY_G, COLOR_SKY_B, 1);

    LIMIT_FIELD_TOP_X = (GROUND_LENGTH / 2);
    LIMIT_FIELD_BOTTOM_X = -1 * (GROUND_LENGTH / 2);
    LIMIT_FIELD_TOP_Z = -1 * (GROUND_WIDTH / 2);
    LIMIT_FIELD_BOTTOM_Z = (GROUND_WIDTH / 2);
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
        glColor3f(1, 0.55, 0);
        glTranslatef(ballTranslateX, 0.35f, ballTranslateZ);
        isRotateX ? glRotatef(rotateAngleX, 0, 0, 1) : glRotatef(rotateAngleZ, 1, 0, 0);
        glutWireSphere(RADIUS_BALL, 20, 20);
        // glutSolidCube(0.5);
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

void drawLinePixel(GLdouble x, GLdouble y) {
	glBegin(GL_POINTS);
	glVertex3d(y, 0, x);
	glEnd();
}

void drawLine(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {
	GLdouble dx = x2 - x1;
	GLdouble dy = y2 - y1;

	GLdouble d  = 2 * dy - dx;
	GLdouble incE  = 2 * dy;
	GLdouble incNE = 2 * (dy - dx);

	GLdouble x = x1;
	GLdouble y = y1;

    glColor3f(1, 1, 1);
    glPointSize(4);

	while (x < x2 || y < y2) {
        if (x == x2) {
            y += 0.1;
        } else if (d <= 0) {
			d += incE;
			x += 0.1;
		} else {
			d += incNE;
			x += 0.1;
			y += 0.1;
		}
        drawLinePixel(x, y);
	}
}

void drawLines() {
    drawLine(-13.5, -23.5, -13.5, 23.5);
    drawLine(13.5, -23.5, 13.5, 23.5);
    drawLine(-13.5, -23.5, 13.4, -23.5);
    drawLine(-13.5, 23.5, 13.4, 23.5);

    drawLine(-13.5, 0, 13.4, 0);

    drawLine(-7.5, -23.5, -7.5, -14.5);
    drawLine(7.5, -23.5, 7.5, -14.5);
    drawLine(-7.5, -14.5, 7.5, -14.5);

    drawLine(-4.5, -23.5, -4.5, -18.5);
    drawLine(4.5, -23.5, 4.5, -18.5);
    drawLine(-4.5, -18.5, 4.5, -18.5);

    drawLine(-7.5, 14.5, -7.5, 23.5);
    drawLine(7.5, 14.5, 7.5, 23.5);
    drawLine(-7.5, 14.5, 7.5, 14.5);

    drawLine(-4.5, 18.5, -4.5, 23.5);
    drawLine(4.5, 18.5, 4.5, 23.5);
    drawLine(-4.5, 18.5, 4.5, 18.5);
}

void drawCirclePoints(GLdouble xc, GLdouble yc, GLdouble x, GLdouble y) {
	glBegin(GL_POINTS);
    glVertex3d(yc+y, 0, xc+x);
    glVertex3d(yc+y, 0, xc-x);
    glVertex3d(yc-y, 0, xc+x);
    glVertex3d(yc-y, 0, xc-x);
    glVertex3d(yc+x, 0, xc+y);
    glVertex3d(yc+x, 0, xc-y);
    glVertex3d(yc-x, 0, xc+y);
    glVertex3d(yc-x, 0, xc-y);
	glEnd();
}

void drawCircle(GLdouble xc, GLdouble yc, GLdouble r) {
	GLdouble x = 0;
	GLdouble y = r;
	GLdouble d = 5/4 - r;

    glColor3f(1, 1, 1);
    glPointSize(4);

    drawCirclePoints(xc, yc, x, y);

	while (y > x) {
		if (d < 0) {
			d += 2 * x;
			x += 0.1;
		} else {
			d += 2 * (x-y) + 1;
			x += 0.1;
			y -= 0.1;
		}
		drawCirclePoints(xc, yc, x, y);
	}
}

// score

int lScoreH1, lScoreH2, lScoreH3, lScoreH4;
int lScoreV1, lScoreV2, lScoreV3;

void lTurnOff() {
    lScoreH1 = lScoreH2 = lScoreH3 = lScoreH4 = 0;
    lScoreV1 = lScoreV2 = lScoreV3 = 0;
}
void lTurnOnZero() {
    lScoreH1 = lScoreH2 = lScoreH3 = lScoreH4 = 1;
    lScoreV1 = lScoreV3 = 1;
}
void lTurnOnOne() {
    lScoreH1 = lScoreH2 = 1;
}
void lTurnOnTwo() {
    lScoreH1 = lScoreH3 = 1;
    lScoreV1 = lScoreV2 = lScoreV3 = 1;
}
void lTurnOnThree() {
    lScoreH1 = lScoreH2 = 1;
    lScoreV1 = lScoreV2 = lScoreV3 = 1;
}
void lTurnOnFour() {
    lScoreH1 = lScoreH2 = lScoreH4 = 1;
    lScoreV2 = 1;
}
void lTurnOnFive() {
    lScoreH2 = lScoreH4 = 1;
    lScoreV1 = lScoreV2 = lScoreV3 = 1;
}
void lTurnOnSix() {
    lScoreH2 = lScoreH3 = lScoreH4 = 1;
    lScoreV1 = lScoreV2 = lScoreV3 = 1;
}
void lTurnOnSeven() {
    lScoreH1 = lScoreH2 = 1;
    lScoreV1 = 1;
}
void lTurnOnEight() {
    lScoreH1 = lScoreH2 = lScoreH3 = lScoreH4 = 1;
    lScoreV1 = lScoreV2 = lScoreV3 = 1;
}
void lTurnOnNine() {
    lScoreH1 = lScoreH2 = lScoreH4 = 1;
    lScoreV1 = lScoreV2 = 1;
}
void lTurnOn(int value) {
    lTurnOff();
    switch (value) {
        case 0:
            lTurnOnZero();
            break;
        case 1:
            lTurnOnOne();
            break;
        case 2:
            lTurnOnTwo();
            break;
        case 3:
            lTurnOnThree();
            break;
        case 4:
            lTurnOnFour();
            break;
        case 5:
            lTurnOnFive();
            break;
        case 6:
            lTurnOnSix();
            break;
        case 7:
            lTurnOnSeven();
            break;
        case 8:
            lTurnOnEight();
            break;
        case 9:
            lTurnOnNine();
            break;
        default:
            break;
    }
}

void drawLeftScore() {
    glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(0, 4, -15);
        glScalef(15, 8, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    // vertical lines
    
    glPushMatrix();
        lScoreH1 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-2, 5, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        lScoreH2 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-2, 3, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        lScoreH3 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-4, 3, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        lScoreH4 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-4, 5, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    // horizontal lines

    glPushMatrix();
        lScoreV1 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-3, 6, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        lScoreV2 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-3, 4, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        lScoreV3 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(-3, 2, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();
}

int rScoreH1, rScoreH2, rScoreH3, rScoreH4;
int rScoreV1, rScoreV2, rScoreV3;

void rTurnOff() {
    rScoreH1 = rScoreH2 = rScoreH3 = rScoreH4 = 0;
    rScoreV1 = rScoreV2 = rScoreV3 = 0;
}
void rTurnOnZero() {
    rScoreH1 = rScoreH2 = rScoreH3 = rScoreH4 = 1;
    rScoreV1 = rScoreV3 = 1;
}
void rTurnOnOne() {
    rScoreH1 = rScoreH2 = 1;
}
void rTurnOnTwo() {
    rScoreH1 = rScoreH3 = 1;
    rScoreV1 = rScoreV2 = rScoreV3 = 1;
}
void rTurnOnThree() {
    rScoreH1 = rScoreH2 = 1;
    rScoreV1 = rScoreV2 = rScoreV3 = 1;
}
void rTurnOnFour() {
    rScoreH1 = rScoreH2 = rScoreH4 = 1;
    rScoreV2 = 1;
}
void rTurnOnFive() {
    rScoreH2 = rScoreH4 = 1;
    rScoreV1 = rScoreV2 = rScoreV3 = 1;
}
void rTurnOnSix() {
    rScoreH2 = rScoreH3 = rScoreH4 = 1;
    rScoreV1 = rScoreV2 = rScoreV3 = 1;
}
void rTurnOnSeven() {
    rScoreH1 = rScoreH2 = 1;
    rScoreV1 = 1;
}
void rTurnOnEight() {
    rScoreH1 = rScoreH2 = rScoreH3 = rScoreH4 = 1;
    rScoreV1 = rScoreV2 = rScoreV3 = 1;
}
void rTurnOnNine() {
    rScoreH1 = rScoreH2 = rScoreH4 = 1;
    rScoreV1 = rScoreV2 = 1;
}
void rTurnOn(int value) {
    rTurnOff();
    switch (value) {
        case 0:
            rTurnOnZero();
            break;
        case 1:
            rTurnOnOne();
            break;
        case 2:
            rTurnOnTwo();
            break;
        case 3:
            rTurnOnThree();
            break;
        case 4:
            rTurnOnFour();
            break;
        case 5:
            rTurnOnFive();
            break;
        case 6:
            rTurnOnSix();
            break;
        case 7:
            rTurnOnSeven();
            break;
        case 8:
            rTurnOnEight();
            break;
        case 9:
            rTurnOnNine();
            break;
        default:
            break;
    }
}

void drawRightScore() {
    int offset = 6;

    // vertical lines
    
    glPushMatrix();
        rScoreH1 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -2, 5, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        rScoreH2 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -2, 3, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        rScoreH3 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -4, 3, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        rScoreH4 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -4, 5, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    // horizontal lines

    glPushMatrix();
        rScoreV1 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -3, 6, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        rScoreV2 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -3, 4, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        rScoreV3 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(offset + -3, 2, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawField();
    drawLines();
    drawCircle(0, 0, 7.0);
    drawLeftScore();
    drawRightScore();
    drawBall();
    drawCrossbars();

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

void checkBallWithinLeftNet() {
    GLdouble netStartLimitX = -1 * (GROUND_LENGTH / 2);
    GLdouble netEndLimitX = netStartLimitX + (GROUND_LENGTH - FIELD_LENGTH) / 2 - 0.5;
    GLdouble netStartLimitZ = -1 * (CROSSBAR_LENGTH / 2);
    GLdouble netEndLimitZ = CROSSBAR_LENGTH / 2;
    
    if (ballTranslateX >= netStartLimitX && ballTranslateX <= netEndLimitX
        && ballTranslateZ >= netStartLimitZ && ballTranslateZ <= netEndLimitZ) {
        if (!ballWithinLeftNet && leftGoals < 9) {
            leftGoals++;
            lTurnOn(leftGoals);
        }
        ballWithinLeftNet = 1;
    } else {
        ballWithinLeftNet = 0;
    }
}

void checkBallWithinRightNet() {
    GLdouble netStartLimitX = (GROUND_LENGTH / 2) - (GROUND_LENGTH - FIELD_LENGTH) / 2 + 0.5;
    GLdouble netEndLimitX = GROUND_LENGTH / 2;
    GLdouble netStartLimitZ = -1 * (CROSSBAR_LENGTH / 2);
    GLdouble netEndLimitZ = CROSSBAR_LENGTH / 2;
    
    if (ballTranslateX >= netStartLimitX && ballTranslateX <= netEndLimitX
        && ballTranslateZ >= netStartLimitZ && ballTranslateZ <= netEndLimitZ) {
        if (!ballWithinRightNet && rightGoals < 9) {
            rightGoals++;
            rTurnOn(rightGoals);
        }
        ballWithinRightNet = 1;
    } else {
        ballWithinRightNet = 0;
    }
}

void onKeyPress(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
            isRotateX = 0;
            rotateAngleZ -= 10;
            if (ballTranslateZ >= LIMIT_FIELD_TOP_Z) ballTranslateZ -= SPEED_BALL;
            break;
		case 'a':
            isRotateX = 1;
            rotateAngleX += 10;
            if (ballTranslateX >= LIMIT_FIELD_BOTTOM_X) ballTranslateX -= SPEED_BALL;
			break;
		case 's':
            isRotateX = 0;
            rotateAngleZ += 10;
            if (ballTranslateZ <= LIMIT_FIELD_BOTTOM_Z) ballTranslateZ += SPEED_BALL;
			break;
		case 'd':
            isRotateX = 1;
            rotateAngleX -= 10;
            if (ballTranslateX <= LIMIT_FIELD_TOP_X) ballTranslateX += SPEED_BALL;
			break;
		default:
			break;
	}
    checkBallWithinLeftNet();
    checkBallWithinRightNet();

    if (ballWithinLeftNet || ballWithinRightNet) {
        ballTranslateX = 0;
        ballTranslateZ = 0;
    }

    glutPostRedisplay();
}

void onSpecialKeyPress(int key, int x, int y) {
    switch (key) {
		case GLUT_KEY_LEFT:
            obsX -= SPEED_CAMERA;
			break;
        case GLUT_KEY_RIGHT:
			obsX += SPEED_CAMERA;
            break;
		case GLUT_KEY_UP:
			obsY += SPEED_CAMERA;
            break;
		case GLUT_KEY_DOWN:
            obsY -= SPEED_CAMERA;
			break;
        case GLUT_KEY_PAGE_UP:
            obsZ -= SPEED_CAMERA;
            break;
        case GLUT_KEY_PAGE_DOWN:
            obsZ += SPEED_CAMERA;
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
