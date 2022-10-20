#include <GL/glut.h>

#include "score.h"

void turnOff(ScoreDigit* digit);
void turnZero(ScoreDigit* digit);
void turnOne(ScoreDigit* digit);
void turnTwo(ScoreDigit* digit);
void turnThree(ScoreDigit* digit);
void turnFour(ScoreDigit* digit);
void turnFive(ScoreDigit* digit);
void turnSix(ScoreDigit* digit);
void turnSeven(ScoreDigit* digit);
void turnEight(ScoreDigit* digit);
void turnNine(ScoreDigit* digit);

void drawScoreDigit(ScoreDigit* digit, GLfloat translateX) {
    // vertical dashes

    glPushMatrix();
        digit->v1 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX + 2, 5, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        digit->v2 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX + 2, 3, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        digit->v3 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX, 3, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        digit->v4 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX, 5, -15);
        glScalef(0.2, 1.5, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    // horizontal dashes

    glPushMatrix();
        digit->h1 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX + 1, 6, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        digit->h2 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX + 1, 4, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        digit->h3 ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
        glTranslatef(translateX + 1, 2, -15);
        glScalef(1, 0.2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();
}

void setScoreDigit(ScoreDigit* digit, int value) {
    turnOff(digit);
    switch (value) {
        case 0:
            turnZero(digit);
            break;
        case 1:
            turnOne(digit);
            break;
        case 2:
            turnTwo(digit);
            break;
        case 3:
            turnThree(digit);
            break;
        case 4:
            turnFour(digit);
            break;
        case 5:
            turnFive(digit);
            break;
        case 6:
            turnSix(digit);
            break;
        case 7:
            turnSeven(digit);
            break;
        case 8:
            turnEight(digit);
            break;
        case 9:
            turnNine(digit);
            break;
        default:
            break;
    }
}

void turnOff(ScoreDigit* digit) {
    digit->v1 = digit->v2 = digit->v3 = digit->v4 = 0;
    digit->h1 = digit->h2 = digit->h3 = 0;
}

void turnZero(ScoreDigit* digit) {
    digit->v1 = digit->v2 = digit->v3 = digit->v4 = 1;
    digit->h1 = digit->h3 = 1;
}

void turnOne(ScoreDigit* digit) {
    digit->v1 = digit->v2 = 1;
}
void turnTwo(ScoreDigit* digit) {

    digit->v1 = digit->v3 = 1;
    digit->h1 = digit->h2 = digit->h3 = 1;
}

void turnThree(ScoreDigit* digit) {
    digit->v1 = digit->v2 = 1;
    digit->h1 = digit->h2 = digit->h3 = 1;
}

void turnFour(ScoreDigit* digit) {
    digit->v1 = digit->v2 = digit->v4 = 1;
    digit->h2 = 1;
}

void turnFive(ScoreDigit* digit) {
    digit->v2 = digit->v4 = 1;
    digit->h1 = digit->h2 = digit->h3 = 1;
}

void turnSix(ScoreDigit* digit) {
    digit->v2 = digit->v3 = digit->v4 = 1;
    digit->h1 = digit->h2 = digit->h3 = 1;
}

void turnSeven(ScoreDigit* digit) {
    digit->v1 = digit->v2 = 1;
    digit->h1 = 1;
}

void turnEight(ScoreDigit* digit) {
    digit->v1 = digit->v2 = digit->v3 = digit->v4 = 1;
    digit->h1 = digit->h2 = digit->h3 = 1;
}

void turnNine(ScoreDigit* digit) {
    digit->v1 = digit->v2 = digit->v4 = 1;
    digit->h1 = digit->h2 = 1;
}
