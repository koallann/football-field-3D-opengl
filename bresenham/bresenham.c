#include <GL/glut.h>

#define DRAW_SMOOTH_FACTOR 0.01f

void drawLinePoint(GLdouble x, GLdouble y);
void drawCirclePoints(GLdouble xc, GLdouble yc, GLdouble x, GLdouble y);

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
            y += DRAW_SMOOTH_FACTOR;
        } else if (d <= 0) {
            d += incE;
            x += DRAW_SMOOTH_FACTOR;
        } else {
            d += incNE;
            x += DRAW_SMOOTH_FACTOR;
            y += DRAW_SMOOTH_FACTOR;
        }
        drawLinePoint(x, y);
    }
}

void drawFieldCircle(GLdouble xc, GLdouble yc, GLdouble r) {
    GLdouble x = 0;
    GLdouble y = r;
    GLdouble d = 5/4 - r;

    glColor3f(1, 1, 1);
    glPointSize(4);

    drawCirclePoints(xc, yc, x, y);

    while (y > x) {
        if (d < 0) {
            d += 2 * x;
            x += DRAW_SMOOTH_FACTOR;
        } else {
            d += 2 * (x-y) + 1;
            x += DRAW_SMOOTH_FACTOR;
            y -= DRAW_SMOOTH_FACTOR;
        }
        drawCirclePoints(xc, yc, x, y);
    }
}

void drawLinePoint(GLdouble x, GLdouble y) {
    glBegin(GL_POINTS);
    glVertex3d(y, 0, x);
    glEnd();
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
