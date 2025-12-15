#ifndef OBSTACULO_H
#define OBSTACULO_H

#include<GL/gl.h>
#include<GL/glu.h>

class Obstaculo{
    GLfloat gX;
    GLfloat gY;
    GLfloat gRadius;

private:
    void DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B);

public:
    Obstaculo(GLfloat x, GLfloat y, GLfloat radius){
        gX = x;
        gY = y;
        gRadius = radius;
    };

    void Desenha(){
        DesenhaCirc(gRadius, 0.0, 0.0, 0.0);
    };

    GLfloat ObtemX(){
        return gX;
    };
    GLfloat ObtemY(){
        return gY;
    };
    GLfloat ObtemRaio(){
        return gRadius;
    };

};

#endif