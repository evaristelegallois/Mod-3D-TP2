/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019


#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>

#include "segment.h"
using namespace std;

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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat hr = m_radius, wr = hr * m_ratio;
    glFrustum(-wr, wr, -hr, hr, 1.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    //m_mod.setToIdentity();
    //modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}


ManageVBO* myOpenGLWidget::create_vbo_segment(Point P0, Point P1){
    GLfloat * colors = new GLfloat[3];
    int step = 2;
    Segment *segment = new Segment(P0, P1);
    ctrlPoly.push_back(segment);
    d = new Discretisation(*segment,step);
    colors[0] = 1.0f; colors[1] = 0.5f; colors[2] = 1.0f;
    return new ManageVBO(d, colors);
}


ManageVBO* myOpenGLWidget::create_vbo_courbe(Point P0, Point P1){
    GLfloat * colors = new GLfloat[3];
    int step = 2;
    curve = new CourbeParametrique(P0, P1, ctrlPoly, 3);
    d = new Discretisation(*curve,step);
    colors[0] = 0.5f; colors[1] = 1.0f; colors[2] = 0.0f;
    return new ManageVBO(d, colors, false);
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

    /////P00-P04
    //segment p00-p01
    ctrlPolyVBO.push_back(create_vbo_segment(P00,P01));
    //segment p01-p02
    ctrlPolyVBO.push_back(create_vbo_segment(P01,P02));
    //segment p02-p03
    ctrlPolyVBO.push_back(create_vbo_segment(P02,P03));
    //segment p03-p04
    ctrlPolyVBO.push_back(create_vbo_segment(P03,P04));
    //courbe de Bézier
    curveVBO.push_back(create_vbo_courbe(P00,P04));

    /////P10-P14
    ctrlPoly.clear();
    //segment p10-p11
    ctrlPolyVBO.push_back(create_vbo_segment(P10,P11));
    //segment p11-p12
    ctrlPolyVBO.push_back(create_vbo_segment(P11,P12));
    //segment p12-p13
    ctrlPolyVBO.push_back(create_vbo_segment(P12,P13));
    //segment p13-p14
    ctrlPolyVBO.push_back(create_vbo_segment(P13,P14));
    //courbe de Bézier
    curveVBO.push_back(create_vbo_courbe(P10,P14));

    /////P20-P24
    ctrlPoly.clear();
    //segment p20-p21
    ctrlPolyVBO.push_back(create_vbo_segment(P20,P21));
    //segment p21-p22
    ctrlPolyVBO.push_back(create_vbo_segment(P21,P22));
    //segment p22-p23
    ctrlPolyVBO.push_back(create_vbo_segment(P22,P23));
    //segment p23-p24
    ctrlPolyVBO.push_back(create_vbo_segment(P23,P24));
    //courbe de Bézier
    curveVBO.push_back(create_vbo_courbe(P20,P24));

    /////P30-P34
    ctrlPoly.clear();
    //segment p30-p31
    ctrlPolyVBO.push_back(create_vbo_segment(P30,P31));
    //segment p31-p32
    ctrlPolyVBO.push_back(create_vbo_segment(P31,P32));
    //segment p32-p33
    ctrlPolyVBO.push_back(create_vbo_segment(P32,P33));
    //segment p33-p34
    ctrlPolyVBO.push_back(create_vbo_segment(P33,P34));
    //courbe de Bézier
    curveVBO.push_back(create_vbo_courbe(P30,P34));

    /////P40-P44
    ctrlPoly.clear();
    //segment p40-p41
    ctrlPolyVBO.push_back(create_vbo_segment(P40,P41));
    //segment p41-p42
    ctrlPolyVBO.push_back(create_vbo_segment(P41,P42));
    //segment p42-p43
    ctrlPolyVBO.push_back(create_vbo_segment(P42,P43));
    //segment p43-p24
    ctrlPolyVBO.push_back(create_vbo_segment(P43,P44));
    //courbe de Bézier
    curveVBO.push_back(create_vbo_courbe(P40,P44));

    /////P00-P40
    ctrlPoly.clear();
    //segment p00-p10
    ctrlPolyVBO.push_back(create_vbo_segment(P00,P10));
    //segment p10-p20
    ctrlPolyVBO.push_back(create_vbo_segment(P10,P20));
    //segment p20-p30
    ctrlPolyVBO.push_back(create_vbo_segment(P20,P30));
    //segment p30-p40
    ctrlPolyVBO.push_back(create_vbo_segment(P30,P40));

    /////P01-P41
    ctrlPoly.clear();
    //segment p01-p11
    ctrlPolyVBO.push_back(create_vbo_segment(P01,P11));
    //segment p11-p21
    ctrlPolyVBO.push_back(create_vbo_segment(P11,P21));
    //segment p21-p31
    ctrlPolyVBO.push_back(create_vbo_segment(P21,P31));
    //segment p31-p41
    ctrlPolyVBO.push_back(create_vbo_segment(P31,P41));

    /////P02-P42
    ctrlPoly.clear();
    ctrlPolyVBO.push_back(create_vbo_segment(P02,P12));
    ctrlPolyVBO.push_back(create_vbo_segment(P12,P22));
    ctrlPolyVBO.push_back(create_vbo_segment(P22,P32));
    ctrlPolyVBO.push_back(create_vbo_segment(P32,P42));

    /////P03-P43
    ctrlPoly.clear();
    ctrlPolyVBO.push_back(create_vbo_segment(P03,P13));
    ctrlPolyVBO.push_back(create_vbo_segment(P13,P23));
    ctrlPolyVBO.push_back(create_vbo_segment(P23,P33));
    ctrlPolyVBO.push_back(create_vbo_segment(P33,P43));

    /////P04-P44
    ctrlPoly.clear();
    ctrlPolyVBO.push_back(create_vbo_segment(P04,P14));
    ctrlPolyVBO.push_back(create_vbo_segment(P14,P24));
    ctrlPolyVBO.push_back(create_vbo_segment(P24,P34));
    ctrlPolyVBO.push_back(create_vbo_segment(P34,P44));

    delete [] coord;

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

    // Rotation de la scène pour l'animation
    m_model.rotate(m_angle, 0, 1, 0);

    QMatrix4x4 m = m_projection * m_modelView * m_model;
    ///----------------------------

    m_program->setUniformValue("matrix", m);

    //polyèdre de contrôle
    for(ManageVBO* vbo : ctrlPolyVBO){
        vbo->draw(m_program,glFuncs);
    }

    //courbes/surface de Bézier
    for(ManageVBO* vbo : curveVBO){
        vbo->drawPoints(m_program,glFuncs);
    }

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
    case Qt::Key_R :
        if (ev->text() == "r")
             setRadius(m_radius-0.05);
        else setRadius(m_radius+0.05);
        break;
    }
}

void myOpenGLWidget::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        makeCurrent();
        doProjection();
        doneCurrent();
        update();
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
