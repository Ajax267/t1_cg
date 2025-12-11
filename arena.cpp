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
        else if (fill == "black"){
            obstaculos.push_back(new Obstaculo(cx, cy, r));
        }
    }
}

void Arena::Desenha(){
    DesenhaCirc(gX, gY, gRadius, 0.0, 0.0, 1.0);

    for(auto o: obstaculos){
        o->Desenha();
    }

    if(j1) j1->Desenha();
    if(j2) j2->Desenha();
}

bool Arena::TestaObstaculo(float gx, float gy, float rj) const{
       for (auto o : obstaculos) {
        float ox = o->ObtemX();
        float oy = o->ObtemY();
        float ro = o->ObtemRaio();

        float dx = gx - ox;
        float dy = gy - oy;

        float dist2   = dx*dx + dy*dy;
        float minDist = rj + ro;
        float minDist2 = minDist * minDist;

        if (dist2 < minDist2) {
            return true;
        }
    }
    return false;
}

bool Arena::TestaColisaoJogador() const{

    float gx1 = j1->ObtemX();
    float gy1 = j1->ObtemY();
    float rj = j1->ObtemRaio();
    float gx2 = j2->ObtemX();
    float gy2 = j2->ObtemY();
    float dx = gx1 - gx2;
    float dy = gy1 - gy2;

    float dist2 = dx*dx + dy*dy;
    float minDist = 2* rj;
    float minDist2 = minDist * minDist;

    if (dist2 < minDist2){
        return true;
    }
    return false;
}