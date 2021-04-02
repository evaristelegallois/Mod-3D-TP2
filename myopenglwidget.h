/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QWidget>

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "segment.h"
#include "discretisation.h"
#include "prepopengl.h"
using namespace std;

class myOpenGLWidget : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit myOpenGLWidget(QWidget *parent = nullptr);
    ~myOpenGLWidget();
    void setIsDisplayed(bool b);
    bool getIsDisplayed();

public slots:

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.

protected slots:
    void onTimeout();
    void displayPoly(bool);

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

    PrepOpenGL * ctrlPolyVBO(Point start, Point end);
    PrepOpenGL * surfaceVBO(float step, Point start, Point end,
                          Point * ctrlPointList, int order, QString type);
    PrepOpenGL * pointVBO(Parametre t, Parametre s, float step, Point start, Point end,
                                          Point * ctrlPointList, int order);

private:
    double m_angleX = 0;
    QTimer *m_timer = nullptr;
    double m_ratio = 1;
    //coordonnées en x et en y de la caméra pour le déplacement dans la scène avec le clavier
    double m_posX = 0;
    double m_posY = 0;

    Segment * segment;
    CourbeParametrique * courbe;
    Discretisation * d;
    Parametre p;
    Parametre t;
    Parametre s;

    PrepOpenGL * vbo0;
    PrepOpenGL * vbo1;
    PrepOpenGL * ptVBO;

    vector<PrepOpenGL*> listVBO;

    bool isDisplayed = false;

    //RR matrices utiles
    QMatrix4x4 m_modelView;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_model;

    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;

    void makeGLObjects();
    void tearGLObjects();
};


#endif // MYOPENGLWIDGET_H
