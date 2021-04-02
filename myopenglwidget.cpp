/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019


#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>

#include "segment.h"

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";


myOpenGLWidget::myOpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init myOpenGLWidget" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
                        // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    //MainWindow* main = new MainWindow();
    //connect(main->clearSceneButton, &QAction::triggered, this, onPushButtonClearScene)
}

myOpenGLWidget::~myOpenGLWidget()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void myOpenGLWidget::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    //shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);

    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }
}

void myOpenGLWidget::doProjection()
{
    //m_mod.setToIdentity();
    //modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}

PrepOpenGL * myOpenGLWidget::surfaceVBO(float step, Point start, Point end,
                                    Point * ctrlPointList, int order, QString type){

    GLfloat * colors = new GLfloat[3]; //1 couleur (RBG) par sommet

    courbe = new CourbeParametrique(start, end, ctrlPointList, order);
    d = new Discretisation(*courbe, step);
    colors[0] = 0.0f; colors[1] = 1.0f; colors[2] = 0.0f;
    return new PrepOpenGL(d, colors, type, false);
}

PrepOpenGL * myOpenGLWidget::ctrlPolyVBO(Point start, Point end){

    GLfloat * colors = new GLfloat[3]; //1 couleur (RBG) par sommet

    segment = new Segment(2, start, end);
    d = new Discretisation(*segment, 2);
    colors[0] = 1.0f; colors[1] = 0.0f; colors[2] = 0.0f;
    return new PrepOpenGL(d, colors, "0");
}

PrepOpenGL * myOpenGLWidget::pointVBO(Parametre t, Parametre s, float step, Point start, Point end,
                                      Point * ctrlPointList, int order){

    GLfloat * colors = new GLfloat[3]; //1 couleur (RBG) par sommet

    step = 10;
    courbe = new CourbeParametrique(start, end, ctrlPointList, order);
    d = new Discretisation(*courbe, step);
    colors[0] = 1.0f; colors[1] = 1.0f; colors[2] = 0.0f;
    if (t.getPValue()>1 || s.getPValue()>1 || t.getPValue()<0 || s.getPValue()<0){
        qDebug()<<"Les paramètres doivent être compris dans l'intervalle [0,1].";
        return new PrepOpenGL(d, colors, 0, 0);
    } else return new PrepOpenGL(d, colors, t, s);
}

void myOpenGLWidget::makeGLObjects()
{
    //objets géométriques
        //points de contrôle
        Point P00, P01, P02, P03, P04;
        Point P10, P11, P12, P13, P14;
        Point P20, P21, P22, P23, P24;
        Point P30, P31, P32, P33, P34;
        Point P40, P41, P42, P43, P44;
        float * coord = new float[3];

        //coordonnées des points de contrôle
        //p00-p03
        coord[0] = 0.0f; coord[1] = -0.8f; coord[2] = 0.0f;
        P00.set(coord);

        coord[0] = 0.7f; coord[1] = 0.2f; coord[2] = 0.0f;
        P01.set(coord);

        coord[0] = 1.8f; coord[1] = 0.4f; coord[2] = 0.0f;
        P02.set(coord);

        coord[0] = 2.3f; coord[1] = -0.05f; coord[2] = 0.0f;
        P03.set(coord);

        coord[0] = 2.8f; coord[1] = -0.2f; coord[2] = 0.0f;
        P04.set(coord);

        //p10-p14
        P10 = P00.translate(-0.1,0.2,0.15);
        P11 = P01.translate(-0.1,0.2,0.2);
        P12 = P02.translate(-0.1,0.15,0.2);
        P13 = P03.translate(-0.1,0.3,0.2);
        P14 = P04.translate(-0.1,0.3,0.2);

        //p20-p24
        P20 = P10.translate(-0.1,0.2,0.1);
        P21 = P11.translate(-0.1,0.2,0.1);
        P22 = P12.translate(-0.1,0.25,0.2);
        P23 = P13.translate(-0.1,0.3,0.15);
        P24 = P14.translate(-0.1,0.3,0.1);

        //p30-p34
        P30 = P20.translate(-0.1,0.2,0.2);
        P31 = P21.translate(-0.1,0.2,0.2);
        P32 = P22.translate(-0.1,0.2,0.25);
        P33 = P23.translate(-0.1,0.3,0.25);
        P34 = P24.translate(-0.1,0.3,0.2);

        //p40-p44
        P40 = P30.translate(-0.1,0.3,-0.4);
        P41 = P31.translate(-0.1,0.3,-0.4);
        P42 = P32.translate(-0.1,0.3,-0.4);
        P43 = P33.translate(-0.1,0.4,-0.45);
        P44 = P34.translate(-0.1,0.45,-0.4);


    ///////polyèdre de contrôle
    listVBO.push_back(ctrlPolyVBO(P00,P01));
    listVBO.push_back(ctrlPolyVBO(P01,P02));
    listVBO.push_back(ctrlPolyVBO(P02,P03));

    listVBO.push_back(ctrlPolyVBO(P10,P11));
    listVBO.push_back(ctrlPolyVBO(P11,P12));
    listVBO.push_back(ctrlPolyVBO(P12,P13));

    listVBO.push_back(ctrlPolyVBO(P20,P21));
    listVBO.push_back(ctrlPolyVBO(P21,P22));
    listVBO.push_back(ctrlPolyVBO(P22,P23));

    listVBO.push_back(ctrlPolyVBO(P30,P31));
    listVBO.push_back(ctrlPolyVBO(P31,P32));
    listVBO.push_back(ctrlPolyVBO(P32,P33));

    listVBO.push_back(ctrlPolyVBO(P00,P10));
    listVBO.push_back(ctrlPolyVBO(P10,P20));
    listVBO.push_back(ctrlPolyVBO(P20,P30));

    listVBO.push_back(ctrlPolyVBO(P01,P11));
    listVBO.push_back(ctrlPolyVBO(P11,P21));
    listVBO.push_back(ctrlPolyVBO(P21,P31));

    listVBO.push_back(ctrlPolyVBO(P02,P12));
    listVBO.push_back(ctrlPolyVBO(P12,P22));
    listVBO.push_back(ctrlPolyVBO(P22,P32));

    listVBO.push_back(ctrlPolyVBO(P03,P13));
    listVBO.push_back(ctrlPolyVBO(P13,P23));
    listVBO.push_back(ctrlPolyVBO(P23,P33));

    ///////courbes de Bézier
    Point *ctrlPointList = new Point[16];  //liste des points de contrôle de la courbe
    ctrlPointList[0] = P00;
    ctrlPointList[1] = P10;
    ctrlPointList[2] = P20;
    ctrlPointList[3] = P30;
    ctrlPointList[4] = P01;
    ctrlPointList[5] = P11;
    ctrlPointList[6] = P21;
    ctrlPointList[7] = P31;
    ctrlPointList[8] = P02;
    ctrlPointList[9] = P12;
    ctrlPointList[10] = P22;
    ctrlPointList[11] = P32;
    ctrlPointList[12] = P03;
    ctrlPointList[13] = P13;
    ctrlPointList[14] = P23;
    ctrlPointList[15] = P33;

    //autre exemple, avec polyèdre de contrôle tronqué (degré -1)
    Point *ctrlPointList2 = new Point[9];  //liste des points de contrôle de la courbe
    ctrlPointList2[0] = P00;
    ctrlPointList2[1] = P10;
    ctrlPointList2[2] = P20;
    ctrlPointList2[3] = P01;
    ctrlPointList2[4] = P11;
    ctrlPointList2[5] = P21;
    ctrlPointList2[6] = P02;
    ctrlPointList2[7] = P12;
    ctrlPointList2[8] = P22;

    //on instancie les VBO pour le dessins des courbes de Bézier
    vbo0 = surfaceVBO(0.2, P00, P33, ctrlPointList, 3, "line");
    //vbo1 = surfaceVBO(0.1, P00, P22, ctrlPointList2, 2, "line");

    t =0.8;
    s =0.5;
    ptVBO = pointVBO(t, s, 0.1, P00, P33, ctrlPointList, 3);

    delete [] coord;
    delete [] ctrlPointList;
    delete [] ctrlPointList2;

}


void myOpenGLWidget::tearGLObjects()
{
    m_vbo.destroy();
}

void myOpenGLWidget::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    //C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    //doProjection();
}

void myOpenGLWidget::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QOpenGLFunctions *glFuncs = context()->functions();

    m_program->bind(); // active le shader program


    /// Ajout RR pour gérer les 3 matrices utiles
    /// à mettre dans doProjection() pour clarifier
    /// -----------------------------------------
        m_modelView.setToIdentity();
        m_modelView.lookAt(QVector3D(0.0f, 0.0f, 3.0f),    // Camera Position
                         QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                         QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

        m_projection.setToIdentity ();
        m_projection.perspective(70.0, width() / height(), 0.1, 100.0); //ou m_ratio

        //centrer la vue sur les éléments géométriques dessinés
        m_model.translate(-0.35, -0.2, 0);
        m_model.translate(m_posX, m_posY, 0);

        // Rotation de la scène pour l'animation
        m_model.rotate(m_angle, 0, 1, 0);

        QMatrix4x4 m = m_projection * m_modelView * m_model;
    ///----------------------------

    m_program->setUniformValue("matrix", m);

    if (isDisplayed){
        //polyèdre de contrôle
        for(PrepOpenGL* vbo : listVBO){
            vbo->draw(m_program,glFuncs);
            //peu importe l'ordre dans lequel les opérations sont réalisées le polygone
            //de contrôle apparait toujours à l'arrière de la surface, bien que nous ne
            //sachions pas pourquoi (peut-être à cause de la partie plus "enfoncée" du polyèdre)
        }
    }

    //surface de Bézier
    vbo0->draw(m_program, glFuncs);
    //vbo1->draw(m_program, glFuncs);

    //point S(t,s)
    ptVBO->draw(m_program, glFuncs);

    m_program->release();
}

void myOpenGLWidget::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Z :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_A :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;

        //déplacer la caméra avec les flèches du clavier
        case Qt::Key_Left :
            m_posX -= 0.05f;
            update();
            break;
        case Qt::Key_Right :
            m_posX += 0.05f;
            update();
            break;
        case Qt::Key_Down :
            m_posY -= 0.05f;
            update();
            break;
        case Qt::Key_Up :
            m_posY += 0.05f;
            update();
            break;
        default: break;
    }
}


void myOpenGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void myOpenGLWidget::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void myOpenGLWidget::onTimeout()
{
    qDebug() << __FUNCTION__ ;

    update();
}

void myOpenGLWidget::displayPoly(bool b){
    isDisplayed=b;
    update();

}
