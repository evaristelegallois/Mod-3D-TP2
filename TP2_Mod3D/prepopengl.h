#ifndef PREPOPENGL_H
#define PREPOPENGL_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "discretisation.h"
#include "segment.h"
#include "parametre.h"

class PrepOpenGL
{

private:
    QOpenGLBuffer m_vbo;
    Discretisation * d;

public:
    PrepOpenGL(Discretisation * d);

    void buildVertData(QVector<GLfloat> &data);
    QVector<GLfloat> tableToVBO(int step, float * tablePoint);

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void drawPoints(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
};

#endif // PREPOPENGL_H
