/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "managevbo.h"

ManageVBO::ManageVBO(Discretisation * d, GLfloat* color, bool isSegment) : d(d), m_color(color), isSegment(isSegment)
{
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;

    if (isSegment) vertData = tableToVBO((int)d->getP(), d->segmentToTable());
    else vertData = tableToVBO((int)d->getCount(), d->bezierToTable());

    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}


QVector<GLfloat> ManageVBO::tableToVBO(int step, vector<float> tablePoint)
{
    //2 Traduction en tableaux de floats
    GLfloat * vertices = new GLfloat[step*3];
    GLfloat * colors = new GLfloat[step*3];


    for (int i =0; i<step; i++){
        colors[i*3] = m_color[0];
        colors[i*3+1] = m_color[1];
        colors[i*3+2] = m_color[2];
    }

    QVector<GLfloat> vertData;
    for (int i = 0; i < step; ++i) { //nb de sommets dépendant de la discrétisation
        // coordonnées sommets
        for (int j = 0; j < 3; j++) //3 coords par sommet
            vertData.append(tablePoint[i*3+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++) //1 RGB par sommet
            vertData.append(colors[i*3+j]);
    }

    delete [] vertices;
    delete [] colors;

    return vertData;

}

void ManageVBO::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){

    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
                                GL_FLOAT, 0 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
                                GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");


    for(int i=0; i < (int)d->getP()-1; i++){
        glPointSize(7.0);
        glFuncs->glDrawArrays(GL_LINES, i,2);
        glFuncs->glDrawArrays(GL_POINTS, i,2);
    }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");

    m_vbo.release();
}

void ManageVBO::drawPoints(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){

    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
                                GL_FLOAT, 0 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
                                GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");

    for(int i=0; i < d->getCount()-1; i++){
        glPointSize(7.0);
        glFuncs->glDrawArrays(GL_LINES, i,2);
        //glFuncs->glDrawArrays(GL_POINTS, i,2);
    }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");

    m_vbo.release();
}
