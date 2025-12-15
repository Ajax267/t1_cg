#ifndef JOGADOR_H
#define JOGADOR_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "tinyxml2.h"
#include "tiro.h"
using namespace tinyxml2;

class Jogador
{
    GLfloat gX;
    GLfloat gY;
    GLfloat gRadius;
    GLfloat gThetaCorpo;
    GLfloat gThetaBraco;
    GLfloat gThetaPernas;
    GLfloat gVelocidade;

    GLfloat gR, gG, gB;

    int gP;
    int hp;

private:
    void DesenhaRect(GLfloat height, GLfloat width,
                     GLfloat R, GLfloat G, GLfloat B);
    void DesenhaCirc(GLint radius, GLfloat R,
                     GLfloat G, GLfloat B);
    void DesenhaJogador(GLfloat x, GLfloat y, GLfloat thetaCorpo);
    void DesenhaBraco(GLfloat R, GLfloat G, GLfloat B);
    void DesenhaPerna(GLfloat R, GLfloat G, GLfloat B);

public:
    Jogador(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b, int p)
    {
        gX = x;
        gY = y;
        gRadius = radius;
        gR = r;
        gG = g;
        gB = b;
        gP = p;

        gThetaCorpo = 0;
        gThetaBraco = 0;
        gThetaPernas = 0;
        gVelocidade = 120.0f;

        hp = 3;
    };

    void Desenha()
    {
        DesenhaJogador(gX, gY, gThetaCorpo);
    };

    void Move(GLfloat dist, GLdouble timeDiference);
    void Roda(GLfloat inc);
    void RodaBraco(GLfloat inc);

    GLfloat ObtemX()
    {
        return gX;
    };

    GLfloat ObtemY()
    {
        return gY;
    };

    GLfloat ObtemRaio()
    {
        return gRadius;
    };

    GLfloat ObtemAngulo()
    {
        return gThetaCorpo;
    };

    void SetaPosicao(float x, float y);

    GLfloat ObtemAnguloBraco()
    {
        return gThetaBraco;
    }

    Tiro *Atira(float velJogador);

    int ObtemHP() const
    {
        return hp;
    }

    void TomaDano()
    {
        if (hp > 0)
        {
            hp--;
        }
    }

    bool VerificaVivo() const{
        return hp > 0;
    }

    int ObtemJogador() const{
        return gP;
    }

    void SetaThetaCorpo(GLfloat ang){
        gThetaCorpo = ang;
    }
};

#endif