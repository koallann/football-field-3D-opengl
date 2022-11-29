#include <GL/glut.h>

const GLfloat INTENSITY_AMBIENT_LIGHT[4] = {0.1, 0.1, 0.1, 1.0};

const GLfloat LIGHT_DAY[4][4] = {
        {1.0, 1.0, 0.0, 0.0}, // position
        {0.9, 0.9, 0.9, 1.0}, // ambient
        {0.8, 0.8, 0.8, 1.0}, // diffuse
        {1.0, 1.0, 1.0, 1.0}  // specular
    };

const GLfloat LIGHT_NIGHT[4][4] = {
    {1.0, 1.0, 0.0, 0.0}, // position
    {0.1, 0.1, 0.1, 1.0}, // ambient
    {0.3, 0.3, 0.3, 1.0}, // diffuse
    {1.0, 1.0, 1.0, 1.0}  // specular
};
