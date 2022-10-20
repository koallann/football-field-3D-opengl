#include <GL/glut.h>

#ifndef SCORE_H
#define SCORE_H

typedef struct {
    int v1, v2, v3, v4;
    int h1, h2, h3;
} ScoreDigit;

void drawScoreDigit(ScoreDigit* digit, GLfloat translateX);

void setScoreDigit(ScoreDigit* digit, int value);

#endif
