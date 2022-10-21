#include <GL/glut.h>

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
            y += 0.1;
        } else if (d <= 0) {
            d += incE;
            x += 0.1;
        } else {
            d += incNE;
            x += 0.1;
            y += 0.1;
        }
        drawLinePoint(x, y);
    }
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
