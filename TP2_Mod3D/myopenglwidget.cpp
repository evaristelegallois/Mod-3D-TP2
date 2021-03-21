/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP1 Modélisation de surfaces 3D (05/03/2021)*/

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


void myOpenGLWidget::makeGLObjects()
{
    //1 Nos objets géométriques
    Point A, B;
    float * coord = new float[3];

    coord[0] = 0.0f;
    coord[1] = 0.0f;
    coord[2] = 0.0f;
    A.set(coord);

    coord[0] = 1.0f;
    coord[1] = 0.0f;
    coord[2] = 0.0f;
    B.set(coord);

    int step = 4;
    segment = new Segment(step, A, B);
    p = (float) step;
    d = new Discretisation(*segment, p);
    vbo0 = new PrepOpenGL(d);

    coord[0] = 0.5f;
    coord[1] = 2.0f;
    coord[2] = 0.0f;
    A.set(coord);

    coord[0] = 1.0f;
    coord[1] = 1.5f;
    coord[2] = 0.0f;
    B.set(coord);

    step = 8;
    segment = new Segment(step, A, B);
    p = (float) step;
    d = new Discretisation(*segment, p);
    vbo1 = new PrepOpenGL(d);

    coord[0] = 0.5f;
    coord[1] = 1.0f;
    coord[2] = 0.0f;
    A.set(coord);

    coord[0] = 2.0f;
    coord[1] = 0.5f;
    coord[2] = 0.0f;
    B.set(coord);

    step = 6;
    segment = new Segment(step, A, B);
    p = (float) step;
    d = new Discretisation(*segment, p);
    vbo2 = new PrepOpenGL(d);


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

        //m_model.translate(0, 0, -3.0);

        // Rotation de la scène pour l'animation
        m_model.rotate(m_angle, 0, 1, 0);

        QMatrix4x4 m = m_projection * m_modelView * m_model;
    ///----------------------------

    m_program->setUniformValue("matrix", m);

    vbo0->draw(m_program, glFuncs);
    vbo1->drawPoints(m_program, glFuncs);
    //segment1->draw(m_program, glFuncs, segment1->getStep());
    vbo2->drawPoints(m_program, glFuncs);

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
            break;
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





