#include "tiro.h"
#include <math.h>
#define DISTANCIA_MAX 500

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
    for (int a = 0; a < 360; a += 20)
    {
        float rad = a * M_PI / 180.0f;
        float x = radius * cosf(rad);
        float y = radius * sinf(rad);
        glVertex2f(x, y);
    }
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(gDirectionAng, 0, 0, 1);
    DesenhaCirc(radiusTiro, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void Tiro::Move(GLdouble timeDiference)
{
  GLfloat rad = gDirectionAng * M_PI / 180.0f;
    gX += gVel * sin(rad)*timeDiference;
    gY += gVel * cos(rad)*timeDiference;
}

bool Tiro::Valido()
{
    GLfloat dx = gX - gXInit;
    GLfloat dy = gY - gYInit;
    GLfloat dist = sqrt(dx*dx + dy*dy);

    if (dist > DISTANCIA_MAX) {
        return false;
    }
    return true;
}
