/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#include "prepopengl.h"

PrepOpenGL::PrepOpenGL(Discretisation * d, GLfloat* color, bool isSegment) : d(d), m_color(color), isSegment(isSegment)
{
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;

    if (isSegment) vertData = tableToVBO(d->getP(), d->segmentToTable());
    else vertData = tableToVBO(d->getCount(), d->bezierToTable());

    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}


QVector<GLfloat> PrepOpenGL::tableToVBO(int step, float * tablePoint)
{
    //Votre composant de traduction "sortie de discrétisation" ->
    //"structures OpenGL" doit pouvoir faire ce choix (GL_Lines, Points ou Triangles)


    //2 Traduction en tableaux de floats
    GLfloat * vertices = new GLfloat[step*3]; //nb de sommets dépendant de la discrétisation
    GLfloat * colors = new GLfloat[step*3]; //1 couleur (RBG) par sommet

    if (isSegment) tablePoint = d->segmentToTable();
    else tablePoint = d->bezierToTable();
    float * values = tablePoint;

    for (int i=0; i<step*3; ++i){
        vertices[i] = tablePoint[i];
        qDebug() << "tablePoint" << tablePoint[i];
    }

    delete[] values;

    for (int i =0; i<step; i++){
        colors[i*3] = m_color[0];
        colors[i*3+1] = m_color[1];
        colors[i*3+2] = m_color[2];
    }

    //3 spécialisation OpenGL
    QVector<GLfloat> vertData;
    for (int i = 0; i < step; ++i) { //nb de sommets dépendant de la discrétisation
        // coordonnées sommets
        for (int j = 0; j < 3; j++) //3 coords par sommet
            vertData.append(vertices[i*3+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++) //1 RGB par sommet
            vertData.append(colors[i*3+j]);
    }

    delete [] vertices;
    delete [] colors;

    return vertData;

}

void PrepOpenGL::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){

    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");

    for(int i=0; i < (int)d->getP()*2; i++){

        // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
        // aux fonctions glBidule doivent être préfixés par glFuncs->.
        glPointSize(5.0f);
        glFuncs->glDrawArrays(GL_LINE_STRIP, i, 2);
    }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");

    m_vbo.release();
}

void PrepOpenGL::drawPoints(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){

    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");

    for(int i=0; i < (int)d->getP(); i++){

        // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
        // aux fonctions glBidule doivent être préfixés par glFuncs->.
        glPointSize(5.0f);
        glFuncs->glDrawArrays(GL_POINTS, i, 1);
    }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");

    m_vbo.release();
}
