/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef PREPOPENGL_H
#define PREPOPENGL_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <cmath>

#include "discretisation.h"
#include "segment.h"
#include "parametre.h"
#include "courbeparametrique.h"

class PrepOpenGL
{

private:
    QOpenGLBuffer m_vbo;
    Discretisation * d;
    bool isSegment;
    bool isTriangle;
    bool isLine;
    bool isPoint = false;
    GLfloat* m_color;
    QString type;

    Parametre t;
    Parametre s;

public:
    PrepOpenGL(Discretisation * d, GLfloat* color, QString type, bool isSegment = true);
    PrepOpenGL(Discretisation * d, GLfloat* color, Parametre t, Parametre s);

    void buildVertData(QVector<GLfloat> &data);
    QVector<GLfloat> tableToVBO(int step, float * tablePoint);
    QVector<GLfloat> pointToVBO(Point point);

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void drawingType();
};

#endif // PREPOPENGL_H
