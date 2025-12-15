#include "arena.h"
#include <iostream>
#include <math.h>

void Arena::DesenhaCirc(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i += 10)
    {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void Arena::CarregarArena(const char *caminhoArquivo)
{
    XMLDocument doc;
    if (doc.LoadFile(caminhoArquivo) != XML_SUCCESS)
    {
        std::cerr << "Erro ao abrir SVG" << std::endl;
        exit(1);
    }

    XMLElement *root = doc.FirstChildElement("svg");
    if (!root)
        return;

    for (XMLElement *elem = root->FirstChildElement("circle"); elem != NULL; elem = elem->NextSiblingElement("circle"))
    {
        float cx, cy, r;
        const char *fillRaw = elem->Attribute("fill");
        string fill = (fillRaw) ? fillRaw : "";

        elem->QueryFloatAttribute("cx", &cx);
        elem->QueryFloatAttribute("cy", &cy);
        elem->QueryFloatAttribute("r", &r);

        if (fill == "blue")
        {
            gX = cx;
            gY = cy;
            gRadius = r;
        }
        else if (fill == "green")
        {
            j1 = new Jogador(cx, cy, r, 0.0, 1.0, 0.0, 1);
        }
        else if (fill == "red")
        {
            j2 = new Jogador(cx, cy, r, 1.0, 0.0, 0.0, 2);
        }
        else if (fill == "black")
        {
            obstaculos.push_back(new Obstaculo(cx, cy, r));
        }
    }
    if (j1 && j2) {
        float x1 = j1->ObtemX();
        float y1 = j1->ObtemY();
        float x2 = j2->ObtemX();
        float y2 = j2->ObtemY();

        float dx = x2 - x1;
        float dy = y2 - y1;

        float phiDeg = atan2f(dy, dx) * 180.0f / M_PI;

        float theta1 = phiDeg - 90.0f;
        float theta2 = theta1 + 180.0f;

        j1->SetaThetaCorpo(theta1);
        j2->SetaThetaCorpo(theta2);
    }
}

void Arena::Desenha()
{
    DesenhaCirc(gX, gY, gRadius, 0.0, 0.0, 1.0);

    for (auto o : obstaculos)
    {
        o->Desenha();
    }

    if (j1 && j1->VerificaVivo())
        j1->Desenha();
    if (j2 && j2->VerificaVivo())
        j2->Desenha();

    for (auto &t : tiros)
    {
        t.Desenha();
    }
}

bool Arena::TestaObstaculo(float gx, float gy, float rj) const
{
    for (auto o : obstaculos)
    {
        float ox = o->ObtemX();
        float oy = o->ObtemY();
        float ro = o->ObtemRaio();

        float dx = gx - ox;
        float dy = gy - oy;

        float dist2 = dx * dx + dy * dy;
        float minDist = rj + ro;
        float minDist2 = minDist * minDist;

        if (dist2 < minDist2)
        {
            return true;
        }
    }
    return false;
}

bool Arena::TestaColisaoJogador() const
{

    float gx1 = j1->ObtemX();
    float gy1 = j1->ObtemY();
    float rj = j1->ObtemRaio();
    float gx2 = j2->ObtemX();
    float gy2 = j2->ObtemY();
    float dx = gx1 - gx2;
    float dy = gy1 - gy2;

    float dist2 = dx * dx + dy * dy;
    float minDist = 2 * rj;
    float minDist2 = minDist * minDist;

    if (dist2 < minDist2)
    {
        return true;
    }
    return false;
}

void Arena::DisparaJ1(float vel)
{
    if (!j1)
        return;

    Tiro *t = j1->Atira(vel);
    if (t)
    {
        tiros.emplace_back(*t);
        delete t;
    }
}

void Arena::DisparaJ2(float vel)
{
    if (!j2)
        return;

    Tiro *t = j2->Atira(vel);
    if (t)
    {
        tiros.emplace_back(*t);
        delete t;
    }
}

void Arena::AtualizaTiros(GLdouble dt)
{
    for (auto it = tiros.begin(); it != tiros.end();)
    {
        it->Move(dt);

        GLfloat tirox, tiroy;
        it->GetPos(tirox, tiroy);

        bool destruir = false;

        if (!it->Valido())
        {
            destruir = true;
        }

        if (!destruir)
        {
            GLfloat dx = tirox - gX;
            GLfloat dy = tiroy - gY;
            GLfloat dist = sqrt(dx * dx + dy * dy);

            if (dist >= gRadius)
            {
                destruir = true;
            }
        }

        if (!destruir && TestaObstaculo(tirox, tiroy, radiusTiro))
        {
            destruir = true;
        }

        if (!destruir)
        {
            int atirador = it->ObtemAtirador();

            if (j1 && atirador != 1 && j1->VerificaVivo())
            {
                GLfloat dxJ1 = tirox - j1->ObtemX();
                GLfloat dyJ1 = tiroy - j1->ObtemY();
                GLfloat dist = sqrt(dxJ1 * dxJ1 + dyJ1 * dyJ1);
                GLfloat raio = j1->ObtemRaio() + radiusTiro;

                if (dist <= raio)
                {
                    j1->TomaDano();
                    destruir = true;

                    if(!j1->VerificaVivo()){
                        gFim = true;
                        gVencedor = 2;
                    }
                }
            }

            if (!destruir && j2 && atirador != 2 && j2->VerificaVivo())
            {
                GLfloat dxJ2 = tirox - j2->ObtemX();
                GLfloat dyJ2 = tiroy - j2->ObtemY();
                GLfloat dist = sqrt(dxJ2 * dxJ2 + dyJ2 * dyJ2);
                GLfloat raio = j2->ObtemRaio() + radiusTiro;

                if (dist <= raio)
                {
                    j2->TomaDano();
                    destruir = true;

                    if(!j2->VerificaVivo()){
                        gFim = true;
                        gVencedor = 1;
                    }
                }
            }
        }
        if (destruir)
        {
            it = tiros.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Arena::Reinicia(){
    tiros.clear();

    for (auto o : obstaculos){
        delete o;
    }
    obstaculos.clear();

    if(j1){
        delete j1;
        j1 = nullptr;
    }

    if(j2){
        delete j2;
        j2 = nullptr;
    }

    gFim = false;
    gVencedor = 0;

    CarregarArena("arena.svg");
}
