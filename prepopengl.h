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

/**
 * @brief The PrepOpenGL class : préparation OpenGL, faite après la discrétisation
 */
class PrepOpenGL
{

private:
    QOpenGLBuffer m_vbo;
    Discretisation * d;

    //permet de déterminer le type
    //de dessin utilisé
    QString type;
    bool isSegment;
    bool isTriangle;
    bool isLine;
    bool isPoint = false;

    GLfloat* m_color;

    //pour récupérer le point S(t,s)
    Parametre t;
    Parametre s;

public:
    //constructeurs (surface/courbes/segments & point)
    PrepOpenGL(Discretisation * d, GLfloat* color, QString type, bool isSegment = true);
    PrepOpenGL(Discretisation * d, GLfloat* color, Parametre t, Parametre s);

    void buildVertData(QVector<GLfloat> &data);
    //stockage des coordonnées dans des VBO
    QVector<GLfloat> tableToVBO(int step, float * tablePoint);
    QVector<GLfloat> pointToVBO(Point point);

    //dessin des objets voulus
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void drawingType();
};

#endif // PREPOPENGL_H
