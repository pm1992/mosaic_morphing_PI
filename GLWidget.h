// ======================================================================
//
// GLWidget.h - GL widget class
//
// ======================================================================

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include "Tile.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
     GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void  setTiles(vector<Tile> &);
    void  drawTiles();
    void  setTimer();

signals:

public slots:
    void s_setRotate(int flag);
    void s_setCentroid(int flag);
    void s_setScale(int flag);
    void s_setSpeed(int flag);
    void s_setOffsetX(int flag);
    void s_setOffsetY(int flag);
    void s_play();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    vector<Tile>	m_tiles;
    float m_angle;
    float m_scale;
    float m_scaleStep;
    float m_angleStep;
    float m_xOffset;
    float m_yOffset;
    bool m_expand;
    bool m_play;
    QTimer *m_timer;
    int m_flagScale;
    int m_flagRotate;
    int m_flagCentroid;
};

#endif	// GLWIDGET_H
