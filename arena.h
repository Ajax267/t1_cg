#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "tinyxml2.h"
#include "jogador.h"
#include "obstaculo.h"
using namespace tinyxml2;
using namespace std;

class Arena
{
    GLfloat gX, gY, gRadius;

    Jogador *j1;
    Jogador *j2;
    vector<Obstaculo *> obstaculos;

private:
    void DesenhaCirc(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B);

public:
    Arena()
    {
        gX = 0;
        gY = 0;
        gRadius = 0;
        j1 = nullptr;
        j2 = nullptr;
    };

    ~Arena()
    {
        if (j1)
            delete j1;
        if (j2)
            delete j2;
        for (auto o : obstaculos)
            delete o;
        obstaculos.clear();
    };

    void CarregarArena(const char *pathArquivo);
    void Desenha();

    Jogador *ObtemJ1()
    {
        return j1;
    };

    Jogador *ObtemJ2()
    {
        return j2;
    };

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

    bool TestaObstaculo(float gx, float gy, float rj) const;

    bool TestaColisaoJogador() const;
};
#endif