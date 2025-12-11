#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "tinyxml2.h"
#include "arena.h"
#include <math.h>
#define TAMANHO_JANELA 500

Arena jogo;
bool keyState[256] = {false};

void display(void)
{

    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);
    jogo.Desenha();
    /* Desenhar no frame buffer! */
    glutSwapBuffers(); // Funcao apropriada para janela double buffer
}

void keyboard(unsigned char key, int x, int y)
{
    keyState[key] = true;
}

void keyboardUp(unsigned char key, int x, int y)
{
    keyState[key] = false;
}

void idle()
{

    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    GLdouble currentTime, timeDiference;
    // Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    // Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;
    Jogador *p1 = jogo.ObtemJ1();
    Jogador *p2 = jogo.ObtemJ2();

    if (timeDiference > 0.1)
        timeDiference = 0.1;

    const float velLinear = 150.0f;
    const float velAngular = 90.0f;

    float velAndar = velLinear * float(timeDiference);
    float velGirar = velAngular * float(timeDiference);
    float cx = jogo.ObtemX();
    float cy = jogo.ObtemY();
    float r = jogo.ObtemRaio();


    if (p1)
    {
        float oldX = p1->ObtemX();
        float oldY = p1->ObtemY();
        float rj = p1->ObtemRaio();
        if (keyState['w'] || keyState['W'])
            p1->Move(velAndar);
        if (keyState['s'] || keyState['S'])
            p1->Move(-velAndar);
        if (keyState['a'] || keyState['A'])
            p1->Roda(velGirar);
        if (keyState['d'] || keyState['D'])
            p1->Roda(-velGirar);

        float jx = p1->ObtemX();
        float jy = p1->ObtemY();

        float dx = jx - cx;
        float dy = jy - cy;

        float dist2 = dx * dx + dy * dy;
        float maxDist = r - rj;
        float maxDist2 = maxDist * maxDist;

        bool foraDaArena = (dist2> maxDist2);
        bool bateu = jogo.TestaObstaculo(jx, jy,rj);
        bool colisao = jogo.TestaColisaoJogador();
        


        if (foraDaArena || bateu || colisao)
        {
            p1->SetaPosicao(oldX, oldY);
        }
    }

    if (p2)
    {
        float oldX2 = p2->ObtemX();
        float oldY2 = p2->ObtemY();
        float rj2 = p2->ObtemRaio();
        if (keyState['o'] || keyState['O'])
            p2->Move(velAndar);
        if (keyState['l'] || keyState['L'])
            p2->Move(-velAndar);
        if (keyState['k'] || keyState['K'])
            p2->Roda(velGirar);
        if (keyState[';'] || keyState[';'])
            p2->Roda(-velGirar);

        float jx2 = p2->ObtemX();
        float jy2 = p2->ObtemY();

        float dx2 = jx2 - cx;
        float dy2 = jy2 - cy;

        float dist2 = dx2 * dx2 + dy2 * dy2;
        float maxDist = r - rj2;
        float maxDist2 = maxDist * maxDist;

        bool foraDaArena = (dist2> maxDist2);
        bool bateu = jogo.TestaObstaculo(jx2, jy2,rj2);

        bool colisao = jogo.TestaColisaoJogador();
        

        if (foraDaArena || bateu || colisao)
        {
            p2->SetaPosicao(oldX2, oldY2);
        }

    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(TAMANHO_JANELA, TAMANHO_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("T1");

    if (argc > 1)
        jogo.CarregarArena(argv[1]);
    else
        jogo.CarregarArena("arena.svg");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float cx = jogo.ObtemX();
    float cy = jogo.ObtemY();
    float r = jogo.ObtemRaio();

    glOrtho(cx - r, cx + r, cy - r, cy + r, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
