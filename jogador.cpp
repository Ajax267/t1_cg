#include "jogador.h"
#include <math.h>

void Jogador::DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
    glVertex2f(-width / 2.0, 0.0);   
    glVertex2f(width / 2.0, 0.0);      
    glVertex2f(width / 2.0, -height);  
    glVertex2f(-width / 2.0, -height);
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
    glTranslatef(gRadius * 1.75f,0, 0);

    glRotatef(-gThetaBraco + 180, 0, 0, 1);

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
    float largTronco = gRadius * 4.0f;

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

void Jogador::Move(GLfloat dist, GLdouble timeDiference)
{

    float rad = gThetaCorpo * M_PI / 180.0;

    gX -= sin(rad) * dist;
    gY += cos(rad) * dist;

    if (gThetaPernas > 45 || gThetaPernas < -45)
        gVelocidade *= -1;
    gThetaPernas += gVelocidade * timeDiference;
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

void RotatePoint(GLfloat x, GLfloat y, GLfloat angleDeg, GLfloat &xOut, GLfloat &yOut)
{
    GLfloat rad = angleDeg * M_PI / 180.0f;
    xOut = x * cosf(rad) - y * sinf(rad);
    yOut = x * sinf(rad) + y * cosf(rad);
}

Tiro* Jogador::Atira(float velJogador)
{
    GLfloat altBraco = gRadius * 1.5f;
    GLfloat baseX = 0.0f, baseY = 0.0f;
    GLfloat tipX  = 0.0f, tipY  = -altBraco;   

    GLfloat baseX_arm, baseY_arm;
    GLfloat tipX_arm,  tipY_arm;

    RotatePoint(baseX, baseY, -gThetaBraco + 180.0f, baseX_arm, baseY_arm);
    RotatePoint(tipX,  tipY,  -gThetaBraco + 180.0f, tipX_arm,  tipY_arm);

    GLfloat ombroX = gRadius * 1.75f;
    GLfloat ombroY = 0.0f;

    baseX_arm += ombroX;
    baseY_arm += ombroY;
    tipX_arm  += ombroX;
    tipY_arm  += ombroY;

    GLfloat baseX_body, baseY_body;
    GLfloat tipX_body,  tipY_body;

    RotatePoint(baseX_arm, baseY_arm, gThetaCorpo, baseX_body, baseY_body);
    RotatePoint(tipX_arm,  tipY_arm,  gThetaCorpo, tipX_body,  tipY_body);

    GLfloat baseX_world = baseX_body + gX;
    GLfloat baseY_world = baseY_body + gY;
    GLfloat tipX_world  = tipX_body  + gX;
    GLfloat tipY_world  = tipY_body  + gY;

    GLfloat dirX = tipX_world - baseX_world;
    GLfloat dirY = tipY_world - baseY_world;

    GLfloat angRadX = atan2f(dirY, dirX);
    GLfloat angDegX = angRadX * 180.0f / M_PI;

    GLfloat directionAng = 90.0f - angDegX; 

    GLfloat velTiro = 2.0f * velJogador;

    return new Tiro(tipX_world, tipY_world, directionAng, velTiro, gP);
}



