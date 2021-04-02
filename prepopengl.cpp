/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "prepopengl.h"

//premier constructeur, pour les segements, courbes et surfaces
PrepOpenGL::PrepOpenGL(Discretisation * d, GLfloat* color, QString type, bool isSegment) :
    d(d), m_color(color), type(type), isSegment(isSegment)
{
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    drawingType(); //détermine le type de dessin

    //en fonction du type d'objet, on appelle des fonctions de discrétisation différentes
    if (isSegment) vertData = tableToVBO(d->getP(), d->segmentToTable()); //segment
    else {
        if (d->isBezierSurface()) vertData = tableToVBO(d->getCount(),
                                                        d->altBezierToTable()); //surface de Bézier
        else vertData = tableToVBO(d->getCount(), d->bezierToTable()); //courbe de Bézier
    }

    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

//second constructeur, pour le point S(t,s)
PrepOpenGL::PrepOpenGL(Discretisation * d, GLfloat* color, Parametre t, Parametre s) :
    d(d), m_color(color), t(t), s(s)
{
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    isPoint = true;
    isSegment = false;

    vertData = pointToVBO(d->getPoint(t, s)); //récupération d'un point sur la surface
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

/**
 * @brief PrepOpenGL::drawingType : déterminer comment sera dessinée la surface à partir du type récupéré lors de l'instanciation
 */
void PrepOpenGL::drawingType(){
    if (type == "triangle"){
        isLine = false;
        isTriangle = true;
    }
    else if (type == "line"){
        isTriangle = false;
        isLine = true;
    }
    else if (type == "0"){
        isLine = false;
        isTriangle = false;
    }
}

//
/**
 * @brief PrepOpenGL::tableToVBO : récupérer le tableau de floats en sortie de discrétisation et l'intégrer dans un VBO
 * @param step : nombre d'étapes de discrétisation
 * @param tablePoint : le tableau de floats
 * @return le VBO correspondant
 */
QVector<GLfloat> PrepOpenGL::tableToVBO(int step, float * tablePoint){

    //2 Traduction en tableaux de floats
    GLfloat * vertices = new GLfloat[step*3]; //nb de sommets dépendant de la discrétisation
    GLfloat * colors = new GLfloat[step*3]; //1 couleur (RBG) par sommet

    //stockage des coordonnées des sommets ainsi que les couleurs
    for (int i=0; i < step*3; ++i){
        vertices[i] = tablePoint[i];
    }
    for (int i =0; i<step; i++){
        colors[i*3] = m_color[0];
        colors[i*3+1] = m_color[1];
        colors[i*3+2] = m_color[2];
    }

    //3 spécialisations OpenGL
    QVector<GLfloat> vertData;

    //si triangle, on réorganise le VBO pour que les points soient orientés de telle façon
    //à ce que l'on produise des triangles
    if (isTriangle){
        for (int i = 0; i < sqrt(step)-1; ++i) { //nb de sommets dépendant de la discrétisation
            for (int j = 0; j < sqrt(step)-1; ++j) {
                //on stocke chaque couple de sommets/couleurs pour ne pas modifier
                //l'organisation des buffers (3 sommets puis 3 couleurs sont attendus)
                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *i+3*j+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *i+3*j+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *i+3*(j+1)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *i+3*(j+1)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j+1)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j+1)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i)+3*(j+1)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i)+3*(j+1)+u]);
             }
        }
    //si filaire, on réorganise le VBO pour que les points soient orientés de telle façon
    //à ce que l'on produise des lignes (pas optimale, une ligne diagonale en trop à chaque étape)
    } else if (isLine){
        for (int i = 0; i < sqrt(step)-1; ++i) { //nb de sommets dépendant de la discrétisation
            for (int j = 0; j < sqrt(step)-1; ++j) {
                //on stocke chaque couple de sommets/couleurs pour ne pas modifier
                //l'organisation des buffers (3 sommets puis 3 couleurs sont attendus)
                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *i+3*j+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *i+3*j+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *i+3*(j+1)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *i+3*(j+1)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j+1)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j+1)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i+1)+3*(j)+u]);

                for (int u = 0; u < 3; u++) vertData.append(vertices[3*(int)(sqrt(step)-1)
                        *(i)+3*(j)+u]);
                for (int u = 0; u < 3; u++) vertData.append(colors[3*(int)(sqrt(step)-1)
                        *(i)+3*(j)+u]);
             }
        }
    //si affichage de points seulement, pas besoin de réorganiser le VBO
    } else{
        for (int i = 0; i < step; ++i) { //nb de sommets dépendant de la discrétisation
            // coordonnées sommets
        for (int j = 0; j < 3; j++) //3 coords par sommet
                vertData.append(vertices[i*3+j]);
            // couleurs sommets
        for (int j = 0; j < 3; j++) //1 RGB par sommet
                vertData.append(colors[i*3+j]);
        }
    }

    delete [] vertices;
    delete [] colors;

    return vertData;
}

/**
 * @brief PrepOpenGL::pointToVBO : récupérer le point S(t,s) et l'intégrer dans un VBO
 * @param point : le point S(t,s)
 * @return le VBO correspondant
 */
QVector<GLfloat> PrepOpenGL::pointToVBO(Point point){
    GLfloat * vertices = new GLfloat[3];
    GLfloat * colors = new GLfloat[3]; //1 couleur (RBG) par sommet

    //on récupère les coordonnées du point S(t,s)
    vertices[0] = point.getX();
    vertices[1] = point.getY();
    vertices[2] = point.getZ();

    colors[0] = m_color[0];
    colors[1] = m_color[1];
    colors[2] = m_color[2];

    QVector<GLfloat> vertData;

    for (int j = 0; j < 3; j++) //3 coords par sommet
            vertData.append(vertices[j]);
        // couleurs sommets
    for (int j = 0; j < 3; j++) //1 RGB par sommet
            vertData.append(colors[j]);

    delete [] vertices;
    delete [] colors;

    return vertData;
}

/**
 * @brief PrepOpenGL::draw : dessiner les objets voulus à partir d'un VBO
 */
void PrepOpenGL::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){

    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");

    if (isSegment){
        for(int i=0; i < (int)d->getP()-1; i++){

            // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
            // aux fonctions glBidule doivent être préfixés par glFuncs->.
            glPointSize(5.0f);
            glFuncs->glDrawArrays(GL_POINTS, i, 2);
            glFuncs->glDrawArrays(GL_LINES, i,2);

        }
    } else if (isTriangle){
        //pour un pas de 0.1, la discretisation crée 100 points (10*10), *6 car pour
        //créer deux triangles il nous faut 6 pts
        //on utilise l'expression du pas pour généraliser (utilise si changement de pas)
        for(int i=0; i < pow(sqrt(d->getCount())-1,2)*6-3; i++){

            glFuncs->glDrawArrays(GL_TRIANGLES, i, 3);
        }
    } else if (isLine){
            //pour un pas de 0.1, la discretisation crée 100 points (10*10),
            //*5 car on utilise 4 points pour créer un carré
            //puis on revient au premier point = 5 pts
            for(int i=0; i < pow(sqrt(d->getCount())-1,2)*5-5; i++){

            glPointSize(5.0f);
            glFuncs->glDrawArrays(GL_LINES, i,2);
            glFuncs->glDrawArrays(GL_POINTS, i,1);
        }
    } else if (isPoint){
            glPointSize(10.0f);
            glFuncs->glDrawArrays(GL_POINTS, 0,1);

    } else {
            for(int i=0; i < pow(sqrt(d->getCount()),2)-(sqrt(d->getCount())-1); i++){
                glPointSize(5.0f);
                glFuncs->glDrawArrays(GL_POINTS, i,1);
            }
        }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");

    m_vbo.release();
}
