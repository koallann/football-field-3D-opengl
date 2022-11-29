#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "constants/color.h"
#include "constants/dimen.h"
#include "constants/speed.h"
#include "constants/light.h"
#include "bresenham/bresenham.h"
#include "score/score.h"

#define SIMPLE_OBJ_IMP
#include "lib/obj_loader/obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/img_loader/stb_image.h"

GLfloat aspect = 1;

GLdouble cameraTranslateX = -25;
GLdouble cameraTranslateY = 25;
GLdouble cameraTranslateZ = 35;

GLfloat ballTranslateX = 0;
GLfloat ballTranslateZ = 0;

int isTranslatingX;
GLfloat ballRotateX;
GLfloat ballRotateZ;

GLuint groundTexId;

int ballWithinLeftNet = 0;
int goalsAtLeft = 0;
int ballWithinRightNet = 0;
int goalsAtRight = 0;

ScoreDigit* leftScoreDigit;
ScoreDigit* rightScoreDigit;

int isDay = 1;

void lighting();
void setLight();

void load2DTexture(GLuint id, char* filePath);

void drawField();
void drawFieldLines();
void drawScore();
void drawBall();
void drawCrossbars();
void drawGrandStand();
void drawSpotlights();

void checkGoal();

void init() {
    glClearColor(COLOR_SKY_R, COLOR_SKY_G, COLOR_SKY_B, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &groundTexId);
    load2DTexture(groundTexId, "assets/texture_field.jpg");

    lighting();

    leftScoreDigit = malloc(sizeof(ScoreDigit));
    rightScoreDigit = malloc(sizeof(ScoreDigit));
}

void lighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, INTENSITY_AMBIENT_LIGHT);
    setLight(isDay ? LIGHT_DAY : LIGHT_NIGHT);
}

void setLight(const GLfloat light[4][4]) {
    glLightfv(GL_LIGHT0, GL_POSITION, light[0]);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light[1]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light[2]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light[3]);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawField();
    drawFieldLines();
    drawFieldCircle(0, 0, 7.0);

    // drawScore();
    drawBall();
    drawCrossbars();
    drawGrandStand();
    drawSpotlights();

    glFlush();
}

void view() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspect, 0.5, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        cameraTranslateX, cameraTranslateY, cameraTranslateZ,
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

void load2DTexture(GLuint id, char* filePath) {
    int width, height, channels = 0;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);

    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void onKeyPress(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            isTranslatingX = 0;
            ballRotateZ -= 10;

            if (ballTranslateZ >= GROUND_WIDTH_START) {
                ballTranslateZ -= BALL_SPEED;
            }
            break;
        case 'a':
            isTranslatingX = 1;
            ballRotateX += 10;

            if (ballTranslateX >= GROUND_LENGTH_START) {
                ballTranslateX -= BALL_SPEED;
            }
            break;
        case 's':
            isTranslatingX = 0;
            ballRotateZ += 10;

            if (ballTranslateZ <= GROUND_WIDTH_END) {
                ballTranslateZ += BALL_SPEED;
            }
            break;
        case 'd':
            isTranslatingX = 1;
            ballRotateX -= 10;

            if (ballTranslateX <= GROUND_LENGTH_END) {
                ballTranslateX += BALL_SPEED;
            }
            break;
        case 'l':
            isDay = !isDay;
            setLight(isDay ? LIGHT_DAY : LIGHT_NIGHT);
            break;
        default:
            break;
    }
    checkGoal();
    glutPostRedisplay();
}

void onSpecialKeyPress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraTranslateX -= CAMERA_SPEED;
            break;
        case GLUT_KEY_RIGHT:
            cameraTranslateX += CAMERA_SPEED;
            break;
        case GLUT_KEY_UP:
            cameraTranslateY += CAMERA_SPEED;
            break;
        case GLUT_KEY_DOWN:
            cameraTranslateY -= CAMERA_SPEED;
            break;
        case GLUT_KEY_PAGE_UP:
            cameraTranslateZ -= CAMERA_SPEED;
            break;
        case GLUT_KEY_PAGE_DOWN:
            cameraTranslateZ += CAMERA_SPEED;
            break;
        default:
            break;
    }
    glLoadIdentity();
    gluLookAt(
        cameraTranslateX, cameraTranslateY, cameraTranslateZ,
        0, 0, 0,
        0, 1, 0
    );
    glutPostRedisplay();
}

void drawField() {
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, groundTexId);
        glBegin(GL_QUADS);
        glColor3f(COLOR_FIELD_R, COLOR_FIELD_G, COLOR_FIELD_B);
        glTexCoord2f(0.0, 0.0); glVertex3f(-(GROUND_LENGTH/2), 0, -(GROUND_WIDTH/2));
        glTexCoord2f(0.0, 1.0); glVertex3f((GROUND_LENGTH/2), 0, -(GROUND_WIDTH/2));
        glTexCoord2f(1.0, 1.0); glVertex3f((GROUND_LENGTH/2), 0, (GROUND_WIDTH/2));
        glTexCoord2f(1.0, 0.0); glVertex3f(-(GROUND_LENGTH/2), 0, (GROUND_WIDTH/2));
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void drawBall() {
    glPushMatrix();
        glColor3f(1, 0.55, 0);
        glTranslatef(ballTranslateX, 0.35f, ballTranslateZ);
        isTranslatingX ? glRotatef(ballRotateX, 0, 0, 1) : glRotatef(ballRotateZ, 1, 0, 0);
        glutSolidSphere(BALL_RADIUS, 20, 20);
    glPopMatrix();
}

void drawCrossbar(GLfloat translateX) {
    GLfloat translateY = CROSSBAR_HEIGHT / 2;
    GLfloat start = -(CROSSBAR_LENGTH / 2);
    GLfloat end = -start;

    glColor3f(1, 1, 1);

    // top bar
    glPushMatrix();
        glTranslatef(translateX, CROSSBAR_HEIGHT, 0);
        glScalef(0.1, 0.1, CROSSBAR_LENGTH);
        glutSolidCube(1);
    glPopMatrix();

    // left bar
    glPushMatrix();
        glTranslatef(translateX, translateY, start);
        glScalef(0.1, CROSSBAR_HEIGHT, 0.1);
        glutSolidCube(1);
    glPopMatrix();

    // right bar
    glPushMatrix();
        glTranslatef(translateX, translateY, end);
        glScalef(0.1, CROSSBAR_HEIGHT, 0.1);
        glutSolidCube(1);
    glPopMatrix();
}

void drawCrossbars() {
    GLfloat leftTranslateX = -(FIELD_LENGTH / 2) + 0.5;
    GLfloat rightTranslateX = -leftTranslateX;

    drawCrossbar(leftTranslateX);
    drawCrossbar(rightTranslateX);
}

void drawFieldLines() {
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

void drawScore() {
    GLfloat translateZ = -(FIELD_WIDTH / 2) - 2.5f;

    // pane
    glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(0, 4, translateZ);
        glScalef(15, 8, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    setScoreDigit(leftScoreDigit, goalsAtLeft);
    drawScoreDigit(leftScoreDigit, -4, translateZ);

    setScoreDigit(rightScoreDigit, goalsAtRight);
    drawScoreDigit(rightScoreDigit, 2, translateZ);
}

void drawGrandStand() {
    SimpleObj_t* topGrandStand = loadObj("assets/grandstand_top.obj");
    SimpleObj_t* bottomGrandStand = loadObj("assets/grandstand_bottom.obj");

    drawObj(topGrandStand);
    drawObj(bottomGrandStand);
}

void drawSpotlight(GLfloat x, GLfloat z, GLfloat yInclination) {
    glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(x, 3.5, z);
        glScalef(0.25, 7, 0.25);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(x, 7, z);
        glRotatef(yInclination, 0, 1, 0);
        glScalef(2, 2, 0.25);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(x, 7, z);
        glRotatef(yInclination, 0, 1, 0);
        glScalef(1.25, 1.25, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();
}

void drawSpotlights() {
    drawSpotlight(-(FIELD_LENGTH/2)-2, -(FIELD_WIDTH/2)-1, 45);
    drawSpotlight((FIELD_LENGTH/2)+2, -(FIELD_WIDTH/2)-1, -45);
}

void checkBallWithinNet(int isLeft) {
    GLfloat widthStart = isLeft ? CROSSBAR_LEFT_NET_WIDTH_START : CROSSBAR_RIGHT_NET_WIDTH_START;
    GLfloat widthEnd = isLeft ? CROSSBAR_LEFT_NET_WIDTH_END : CROSSBAR_RIGHT_NET_WIDTH_END;
    GLfloat lengthStart = isLeft ? CROSSBAR_LEFT_NET_LENGTH_START : CROSSBAR_RIGHT_NET_LENGTH_START;
    GLfloat lengthEnd = isLeft ? CROSSBAR_LEFT_NET_LENGTH_END : CROSSBAR_RIGHT_NET_LENGTH_END;

    int* ballWithinNet = isLeft ? &ballWithinLeftNet : &ballWithinRightNet;
    int* goals = isLeft ? &goalsAtLeft : &goalsAtRight;

    if (ballTranslateX >= widthStart && ballTranslateX <= widthEnd
        && ballTranslateZ >= lengthStart && ballTranslateZ <= lengthEnd) {

        if (!*ballWithinNet && *goals < 9) *goals += 1;
        *ballWithinNet = 1;
    } else {
        *ballWithinNet = 0;
    }
}

void checkGoal() {
    checkBallWithinNet(1);
    checkBallWithinNet(0);

    if (ballWithinLeftNet || ballWithinRightNet) {
        if (ballWithinLeftNet) setScoreDigit(leftScoreDigit, goalsAtLeft);
        if (ballWithinRightNet) setScoreDigit(rightScoreDigit, goalsAtRight);

        ballTranslateX = 0;
        ballTranslateZ = 0;
    }
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
