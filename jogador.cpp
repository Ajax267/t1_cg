#include "jogador.h"
#include <math.h>

void Jogador::DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
    glVertex2f(-width / 2.0, 0.0);     // Base Esquerda
    glVertex2f(width / 2.0, 0.0);      // Base Direita
    glVertex2f(width / 2.0, -height);  // Topo Direito
    glVertex2f(-width / 2.0, -height); // Topo Esquerdo
    glEnd();
}

void Jogador::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 360; i += 10)
    {
        float angle = i * M_PI / 180.0f;
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    };
    glEnd();
}

void Jogador::DesenhaBraco(GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(gRadius * 1.5f, gRadius * 1.5f, 0);

    glRotatef(gThetaBraco, 0, 0, 1);

    DesenhaRect(gRadius * 1.5, gRadius * 0.3, R, G, B);
    glPopMatrix();
}

void Jogador::DesenhaPerna(GLfloat R, GLfloat G, GLfloat B)
{
    float larguraPerna = gRadius * 0.3;
    float alturaPerna = gRadius * 1.2;

    float passo = (gThetaPernas / 45.0f) * (gRadius * 0.8f);

    glPushMatrix();
    glTranslatef(-gRadius * 0.5, 0, 0);
    glTranslatef(0, passo, 0);
    DesenhaRect(alturaPerna, larguraPerna, R, G, B);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(gRadius * 0.5, 0, 0);
    glTranslatef(0, -passo, 0);
    DesenhaRect(alturaPerna, larguraPerna, R, G, B);
    glPopMatrix();
}

void Jogador::DesenhaJogador(GLfloat x, GLfloat y, GLfloat thetaCorpo)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(thetaCorpo, 0, 0, 1);

    DesenhaCirc(gRadius, gR, gG, gB);

    float altTronco = gRadius * 0.7f;
    float largTronco = gRadius * 3.5f;

    glPushMatrix();
    glTranslatef(0, altTronco / 2.0f, 0);
    DesenhaRect(altTronco, largTronco, gR, gG, gB);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, altTronco, 0);
    DesenhaPerna(gR, gG, gB);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 0);
    DesenhaBraco(gR, gG, gB);
    glPopMatrix();

    glPopMatrix();
}

void Jogador::Move(GLfloat dist)
{

    float rad = gThetaCorpo * M_PI / 180.0;

    gX -= sin(rad) * dist;
    gY += cos(rad) * dist;

    if (gThetaPernas > 45 || gThetaPernas < -45)
        gVelocidade *= -1;
    gThetaPernas += gVelocidade * 0.5;
}

void Jogador::Roda(GLfloat inc)
{
    gThetaCorpo += inc;
}

void Jogador::RodaBraco(GLfloat inc)
{
    gThetaBraco += inc;
    if (gThetaBraco > 45)
        gThetaBraco = 45;
    if (gThetaBraco < -45)
        gThetaBraco = -45;
}

void Jogador::SetaPosicao(float x, float y)
{
    gX = x;
    gY = y;
}