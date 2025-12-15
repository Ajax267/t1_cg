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
static char str[2000];
void *font = GLUT_BITMAP_9_BY_15;

int pMouseX = 0;
int uX = 0;
bool firstMouse = true;
bool atiraJ1 = false;
bool atiraJ2 = false;
const GLfloat cooldownTiro = 0.3;
GLdouble ultimoTiroJ1 = 0.0;
GLdouble ultimoTiroJ2 = 0.0;


void PrintTexto(GLfloat x, GLfloat y, const char *texto)
{
    // Define the position to start printing
    glRasterPos2f(x, y);
    // Print each of the other Char at time
    while (*texto)
    {
        glutBitmapCharacter(font, *texto);
        texto++;
    }
}

void display(void)
{

    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);
    jogo.Desenha();
    Jogador *p1 = jogo.ObtemJ1();
    Jogador *p2 = jogo.ObtemJ2();

    float cx = jogo.ObtemX();
    float cy = jogo.ObtemY();
    float r = jogo.ObtemRaio();

    glColor3f(255, 0, 0); 

    int vidas1 = p1 ? p1->ObtemHP() : 0;
    int vidas2 = p2 ? p2->ObtemHP() : 0;

    sprintf(str, "J1: %d    J2: %d", vidas1, vidas2);

    PrintTexto(cx - r + 20.0f, cy + r - 30.0f, str);

    if (jogo.EncerraJogo())
    {
        if (jogo.ObtemVencedor() == 1)
            sprintf(str, "JOGADOR 1 GANHOU!");
        else if (jogo.ObtemVencedor() == 2)
            sprintf(str, "JOGADOR 2 GANHOU!");
        else
            sprintf(str, "FIM DE JOGO");

        PrintTexto(cx - 80.0f, cy, str);
        PrintTexto(cx - 140.0f, cy - 30.0f, "APERTE R PARA REINICIAR A PARTIDA");
    }

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); // Funcao apropriada para janela double buffer
}


void keyboard(unsigned char key, int x, int y)
{
    keyState[key] = true;

    if (key == '5')
    {
        atiraJ2 = true;
    }

    if (key == 'r' || key == 'R')
    {
        atiraJ1 = false;
        atiraJ2 = false;
        ultimoTiroJ1 = 0.0;
        ultimoTiroJ2 = 0.0;

        jogo.Reinicia();
    }
}

void keyboardUp(unsigned char key, int x, int y)
{
    keyState[key] = false;

    if (key == '5')
    {
        atiraJ2 = false;
    }
}

void mouse(int button, int state, int x, int y)
{
    GLfloat fX = (GLfloat)x / TAMANHO_JANELA;
    GLfloat fY = (GLfloat)(TAMANHO_JANELA - y) / TAMANHO_JANELA;

    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN)
        {
            atiraJ1 = true;
        }
        else if (state == GLUT_UP)
        {
            atiraJ1 = false;
        }
}

void motion(int x, int y)
{
    pMouseX = x;
    if (firstMouse)
    {
        uX = x;
        firstMouse = false;
    }

    glutPostRedisplay();
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
    if (timeDiference > 0.1)
        timeDiference = 0.1;

    if (jogo.EncerraJogo())
    {
        glutPostRedisplay();
        return;
    }
    Jogador *p1 = jogo.ObtemJ1();
    Jogador *p2 = jogo.ObtemJ2();

    ultimoTiroJ1 += timeDiference;
    ultimoTiroJ2 += timeDiference;
    const float velLinear = 150.0f;
    const float velAngular = 90.0f;

    if (atiraJ1 && ultimoTiroJ1 >= cooldownTiro)
    {
        jogo.DisparaJ1(velLinear);
        ultimoTiroJ1 = 0.0;
    }

    if (atiraJ2 && ultimoTiroJ2 >= cooldownTiro)
    {
        jogo.DisparaJ2(velLinear);
        ultimoTiroJ2 = 0.0;
    }

    float velAndar = velLinear * float(timeDiference);
    float velGirar = velAngular * float(timeDiference);
    float cx = jogo.ObtemX();
    float cy = jogo.ObtemY();
    float r = jogo.ObtemRaio();

    bool praFrente = (keyState['w'] || keyState['W']);
    bool praTras = (keyState['s'] || keyState['S']);

    if (p1)
    {
        float oldX = p1->ObtemX();
        float oldY = p1->ObtemY();
        float rj = p1->ObtemRaio();
        if (praFrente)
            p1->Move(velAndar, timeDiference);
        if (praTras)
            p1->Move(-velAndar, timeDiference);
        if (keyState['a'] || keyState['A'])
        {
            float direcao = velGirar;
            if (praTras && !praFrente)
                direcao = -direcao;
            p1->Roda(direcao);
        }
        if (keyState['d'] || keyState['D'])
        {
            float direcao = -velGirar;
            if (praTras && !praFrente)
                direcao = -direcao;
            p1->Roda(direcao);
        }

        float jx = p1->ObtemX();
        float jy = p1->ObtemY();

        float dx = jx - cx;
        float dy = jy - cy;

        float dist2 = dx * dx + dy * dy;
        float maxDist = r - rj;
        float maxDist2 = maxDist * maxDist;

        bool foraDaArena = (dist2 > maxDist2);
        bool bateu = jogo.TestaObstaculo(jx, jy, rj);
        bool colisao = jogo.TestaColisaoJogador();

        if (foraDaArena || bateu || colisao)
        {
            p1->SetaPosicao(oldX, oldY);
        }

        int dxMouse = pMouseX - uX;
        uX = pMouseX;

        float sensibilidade = 0.4f;
        float inc = dxMouse * sensibilidade;

        p1->RodaBraco(inc);
    }

    if (p2)
    {
        bool praFrente2 = keyState['o'] || keyState['O'];
        bool praTras2 = keyState['l'] || keyState['L'];
        float oldX2 = p2->ObtemX();
        float oldY2 = p2->ObtemY();
        float rj2 = p2->ObtemRaio();
        if (keyState['o'] || keyState['O'])
            p2->Move(velAndar, timeDiference);
        if (keyState['l'] || keyState['L'])
            p2->Move(-velAndar, timeDiference);
        if (keyState['k'] || keyState['K'])
        {
            float direcao2 = velGirar;
            if (praTras2 && !praFrente2)
                direcao2 = -direcao2;
            p2->Roda(direcao2);
        }

        if (keyState[';'])
        {
            float direcao2 = -velGirar;
            if (praTras2 && !praFrente2)
                direcao2 = -direcao2;
            p2->Roda(direcao2);
        }
        float jx2 = p2->ObtemX();
        float jy2 = p2->ObtemY();

        float dx2 = jx2 - cx;
        float dy2 = jy2 - cy;

        float dist2 = dx2 * dx2 + dy2 * dy2;
        float maxDist = r - rj2;
        float maxDist2 = maxDist * maxDist;

        bool foraDaArena = (dist2 > maxDist2);
        bool bateu = jogo.TestaObstaculo(jx2, jy2, rj2);

        bool colisao = jogo.TestaColisaoJogador();

        if (foraDaArena || bateu || colisao)
        {
            p2->SetaPosicao(oldX2, oldY2);
        }

        const float velBraco = 120.0f;
        float inc2 = velBraco * timeDiference;

        if (keyState['4'])
            p2->RodaBraco(-inc2);

        if (keyState['6'])
            p2->RodaBraco(+inc2);
    }

    jogo.AtualizaTiros(timeDiference);

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
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
