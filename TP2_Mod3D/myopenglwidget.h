/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

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

class myOpenGLWidget : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit myOpenGLWidget(QWidget *parent = nullptr);
    ~myOpenGLWidget();

public slots:

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.

protected slots:
    void onTimeout();

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

private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_ratio = 1;

    Segment * segment;
    Discretisation * d;
    Parametre p;

    PrepOpenGL * vbo0;
    PrepOpenGL * vbo1;
    PrepOpenGL * vbo2;

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
