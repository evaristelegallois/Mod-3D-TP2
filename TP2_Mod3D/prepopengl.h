/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#ifndef PREPOPENGL_H
#define PREPOPENGL_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

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
    GLfloat* m_color;

public:
    PrepOpenGL(Discretisation * d, GLfloat* color, bool isSegment = true);

    void buildVertData(QVector<GLfloat> &data);
    QVector<GLfloat> tableToVBO(int step, float * tablePoint);

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void drawPoints(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
};

#endif // PREPOPENGL_H
