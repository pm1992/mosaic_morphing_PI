// ======================================================================
//
// MainWindow.h - Main window widget class
//
// ======================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "Tile.h"
#include "GLWidget.h"

// forward declaration
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // constructor
    MainWindow(QWidget *parent = 0);
    void createActions();
    void createMenus();
    void createWidgets();

public slots:
    // slots
    void s_loadTiles();
    // read tile geometry from file
    //void s_play();
    //void s_setScale();
    //void s_setRotate();
    //void s_setCentroid();

private:
    vector<Tile>	 m_tiles;	// array of mosaic tiles
    GLWidget	*m_glwidget;	// widget in which to draw

    QPushButton *m_load;
    QPushButton *m_play;

    QCheckBox *m_rotate;
    QCheckBox *m_scale;
    QCheckBox *m_centroid;

    QAction *load;
    QAction *quit;
};

#endif	// MAINWINDOW_H
