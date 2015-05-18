// ======================================================================
//
// GLWidget.cpp - GL widget class
//
// ======================================================================

#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_tiles.clear();
    m_angle = 0.0f;
    m_scale = 1.0f;

    m_scaleStep = 0.995;
    m_angleStep = 1.0;
    m_flagCentroid = 0;
    m_expand = false;
    m_play = false;
}



GLWidget::~GLWidget()
{}

void GLWidget::setTimer()
{
    m_timer = new QTimer(this);
    m_timer->start(10);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // enable smooth points
    glEnable(GL_POINT_SMOOTH);

    // enable depth test
    glEnable(GL_DEPTH_TEST);
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawTiles();

    // Flag stating when to expand is set to true.
    if ( m_flagScale )
    {
        if ( m_scale < 0.5 )
            m_expand = true;
        else if ( m_scale > 1.0 )
            m_expand = false;

        if ( m_expand )
            m_scale /= m_scaleStep;
        else m_scale *= m_scaleStep;
    }


    if ( m_flagRotate )
    {
        m_angle += m_angleStep;
        if ( m_angle > 360 )
            m_angle -= 360;
    }
}



void GLWidget::resizeGL(int w, int h)
{
    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0, w/2.0, -h/2.0, h/2.0, -3, 3);
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

}


void GLWidget::setTiles(vector<Tile> &tiles)
{
    m_tiles = tiles;
    //updateGL();
    setTimer();
}



void GLWidget::s_setCentroid(int flag)
{
    m_flagCentroid = flag;
}

void GLWidget::s_setRotate(int flag)
{
    m_flagRotate = flag;
}

void GLWidget::s_setScale(int flag)
{
    m_flagScale = flag;

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::s_setSpeed:
//
//Setting Speed for the slider as it moves on the slider or by the spinBox

void GLWidget::s_setSpeed(int flag)
{
    if (flag == 1)
    {
        m_scaleStep = 0.99995;
        m_angleStep = 0.2;
    }
    if (flag == 2)
    {
        m_scaleStep = 0.999;
        m_angleStep = 0.6;
    }
    if (flag == 3)
    {
        m_scaleStep = 0.995;
        m_angleStep = 1.0;
    }
    if (flag == 4)
    {
        m_scaleStep = 0.990;
        m_angleStep = 1.4;
    }
    if (flag == 5)
    {
        m_scaleStep = 0.98;
        m_angleStep = 1.8;
    }
}


void GLWidget::s_setOffsetX(int flag)  //Sets the X Offset value, when Slider is in middle, no offset
{
    if (flag == 1)
    {
        m_xOffset = -0.015;

    }
    if (flag == 2)
    {
        m_xOffset = -0.01;

    }
    if (flag == 3)
    {
        m_xOffset = 0.0;
    }
    if (flag == 4)
    {
        m_xOffset = 0.01;

    }
    if (flag == 5)
    {
        m_xOffset = 0.015;

    }

}
void GLWidget::s_setOffsetY(int flag)  //Sets the Y Offset value, when Slider is in middle, no offset
{
    if (flag == 1)
    {
        m_yOffset = -0.015;

    }
    if (flag == 2)
    {
        m_yOffset = -0.01;

    }
    if (flag == 3)
    {
        m_yOffset = 0.0;
    }
    if (flag == 4)
    {
        m_yOffset = 0.01;

    }
    if (flag == 5)
    {
        m_yOffset = 0.015;

    }

}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::s_play:
//
// Slot function for playing/pausing the animation.
//
void GLWidget::s_play()
{
    m_play = !m_play;
    if(m_play) m_timer->start();
    else	   m_timer->stop();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::drawTiles:
//
void GLWidget::drawTiles()
{
    // error checking
    if (m_tiles.empty()) return;

    // for each tile, get color and draw polygon
    int n_tiles = m_tiles.size();
    for (int i = 0; i<n_tiles; ++i) {
        // get tile color and pass it to OpenGL
        QColor color = m_tiles[i].color();
        QVector2D cen = m_tiles[i].centroid();
        glColor3f(1.0,1.0,1.0); // (1.0,1.0,1.0)
        if(m_flagCentroid)
        {
              glEnable(GL_POINT_SMOOTH);
              glPointSize(7);
              glBegin(GL_POINTS);
              glVertex2f(cen.x() + m_xOffset, cen.y() + m_yOffset); //centroid  //Adds offset to centroid
              glEnd();
              glDisable(GL_POINT_SMOOTH);
        }

        // draw tile polygon
        glColor3f(color.redF(), color.greenF(), color.blueF());
        glPushMatrix(); // saves away the state of information at this point
        glTranslatef(cen.x() + m_xOffset, cen.y() + m_yOffset, 0);  //Adds offset to rotate
        //glTranslatef(cen.x(), cen.y(), 0);
        glScalef(m_scale, m_scale, m_scale);
        glRotatef(m_angle, 0,0,1);
        glTranslatef(-cen.x() + m_xOffset, -cen.y() + m_yOffset,0); //Adds offset to rotate
        //glTranslatef(-cen.x(), -cen.y(),0);

        //draw the polygon
        glBegin(GL_POLYGON);			// set polygon mode
        int n_vtx = m_tiles[i].num();		// get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {		// visit each tile vertex
            QVector2D vtx = m_tiles[i].vertex(j);	// assign (x,y) coords to vtx
            glVertex2f(vtx.x() + m_xOffset, vtx.y() + m_yOffset );	// assign vtx as next polygon vertex  //Adds offset to rotate
        }
        glEnd();				// end polygon mode
        glPopMatrix();
    }
}
