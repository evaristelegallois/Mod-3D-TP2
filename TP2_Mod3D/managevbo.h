/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef MANAGEVBO_H
#define MANAGEVBO_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "discretisation.h"
#include "segment.h"
#include "parametre.h"
#include "courbeparametrique.h"

class ManageVBO
{

private:
    QOpenGLBuffer m_vbo;
    Discretisation * d;
    GLfloat* m_color;
    bool isSegment;

public:
    ManageVBO(Discretisation * d, GLfloat* color, bool isSegment = true);

    void buildVertData(QVector<GLfloat> &data);
    QVector<GLfloat> tableToVBO(int step, vector<float> tablePoint);

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void drawPoints(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
};

#endif // MANAGEVBO_H
