#include "obstaculo.h"
#include <math.h>

void Obstaculo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R,G,B);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(gX, gY);
    for(int i =0; i<=360; i+=10){
        float angle = i * M_PI / 180.0f;
        glVertex2f(gX + cos(angle)*radius, gY + sin(angle)*radius);
    }
    glEnd();
}