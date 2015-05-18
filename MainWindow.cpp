// ======================================================================
//
// MainWindow.h - Main window widget class
//
// ======================================================================

#include "MainWindow.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// Constructor. Initialize user-interface elements.
//
MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
{
    createActions();
    createMenus();
    createWidgets();
}
//START USING QT ASSISTANT

void MainWindow::createActions()
{
    load = new QAction("&Load Tiles", this);
    load->setShortcut(tr("Ctrl+L"));
    connect(load, SIGNAL(triggered()), this, SLOT(s_loadTiles()));

    // create quit action and set signal-slot connection
    quit = new QAction("&Quit", this);
    quit->setShortcut(tr("Ctrl+Q"));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createMenus()
{
    QMenu *file;
    file = menuBar()->addMenu("&File");
    file->addAction(load);
    file->addAction(quit);
}


void MainWindow::createWidgets()
{
    m_glwidget = new GLWidget;

    m_load = new QPushButton("&Load");
    m_play = new QPushButton("&Play/Pause");

    m_rotate = new QCheckBox("&Rotate");
    m_scale = new QCheckBox("&Scale");
    m_centroid = new QCheckBox("&Centroid");

    // checkboxes statechanged
    connect(m_load, SIGNAL(clicked()), this, SLOT(s_loadTiles()));
    connect(m_scale, SIGNAL(stateChanged(int)), m_glwidget, SLOT(s_setScale(int)));
    connect(m_rotate, SIGNAL(stateChanged(int)), m_glwidget, SLOT(s_setRotate(int)));
    connect(m_centroid, SIGNAL(stateChanged(int)), m_glwidget, SLOT(s_setCentroid(int)));
    connect(m_play, SIGNAL(clicked()), m_glwidget, SLOT(s_play()));

    //turn on all checked settings
    m_scale->setChecked(1);
    m_rotate->setChecked(1);
    m_centroid->setChecked(1);

    //Create speed slider
    QLabel *label1 = new QLabel("Speed");
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(5);
    //QSpinBox spinbox
    QSpinBox *spinBox = new QSpinBox();
    spinBox->setMinimum(1);
    spinBox->setMaximum(5);

    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)) );
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)) );
    connect(slider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(s_setSpeed(int)) );
    connect(slider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(s_setSpeed(int)) );


    slider->setValue(3);

    //Create speed slider
    QLabel *xoffset1 = new QLabel("X - Offset");
    QSlider *xoffset = new QSlider(Qt::Horizontal);
    xoffset->setMinimum(1);
    xoffset->setMaximum(5);
    xoffset->setValue(3);


    QLabel *yoffset1 = new QLabel("Y - Offset");
    QSlider *yoffset = new QSlider(Qt::Horizontal);
    yoffset->setMinimum(1);
    yoffset->setMaximum(5);
    yoffset->setValue(3);

    connect(xoffset, SIGNAL(valueChanged(int)), m_glwidget, SLOT(s_setOffsetX(int)) );
    connect(yoffset, SIGNAL(valueChanged(int)), m_glwidget, SLOT(s_setOffsetY(int)) );


    QGridLayout *grid2 = new QGridLayout;
    grid2->addWidget(label1,  0, 0);
    grid2->addWidget(slider,  0, 1);
    grid2->addWidget(spinBox, 0, 2);
    grid2->addWidget(xoffset1,0, 3);
    grid2->addWidget(xoffset, 0, 4);
    grid2->addWidget(yoffset1,0, 5);
    grid2->addWidget(yoffset, 0, 6);

    // add pushbuttons into horizontal alignment
    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(m_load);
    buttons->addWidget(m_play);

    // add widgets for settings into vertical alignment
    QVBoxLayout *settingLayout = new QVBoxLayout();
    settingLayout->addLayout(buttons );
    settingLayout->addWidget(m_scale );
    settingLayout->addWidget(m_rotate);
    settingLayout->addWidget(m_centroid);
    settingLayout->addLayout(grid2);
    //settingLayout->addStretch();

    // add settings and OpenGL widget into horizontal alignment
    // Added Settings at the bottom to have a full GLWidget
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addLayout(settingLayout, 1, 1);
    mainLayout->addWidget(m_glwidget,    0, 1);
    mainLayout->setColumnStretch(1, 2);

    // make the OpenGL widget be the central widget
    QWidget *central = new QWidget();
    central->setLayout(mainLayout);
    setCentralWidget(central);

}
//
// Slot function for File|Load.
// Loads tile geometry.
//
void MainWindow::s_loadTiles()
{
    // launch file dialog and get file containing tile geometry
    QString fileName = QFileDialog::getOpenFileName(this, "Open Tiles", "", "Tiles (*.txt)");

    // error checking
    if (fileName == NULL) return;

    // get file path, name, and basename
    // QFileInfo *info = new QFileInfo(fileName);
    // QString tiles_path = info->path();
    // QString tiles_name = info->fileName();
    // QString tiles_base = info->baseName();

    // open file for reading a stream of text
    QFile	file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    // read first three lines to get mosaic width, height, and number of tiles;
    // start with first line: mosaic width
    QString line = in.readLine();		// read mosaic width (inches)
    int   w  = line.toInt();		// convert string to int
    float w2 = w >> 1;			// half-width for computing center

    // read second line: mosaic height
    line = in.readLine();			// read mosaic height (inches)
    int h  = line.toInt();			// convert string to int
    int h2 = h >> 1;			// half-height for computing center

    // read third line: number of tiles
    line = in.readLine();			// read number of tiles
    int n_tiles = line.toInt();		// convert string to int
    for (int i = 0; i<n_tiles; ++i) {
        Tile tile;
        line = in.readLine();		// read number of vertices
        int n_vertices = line.toInt();	// convert string to int
        tile.setNum(n_vertices);	// set it in tile class

        // visit all vertices
        for (int j = 0; j<n_vertices; ++j) {
            line = in.readLine();	// read coordinates
            double x = line.section(',', 0, 0).toDouble();
            double y = line.section(',', 1, 1).toDouble();

            // constructor function assigns normalized coordinate [-1,1] to vtx;
            // note: w2 and h2 are floats so that result is float too
            QVector2D vtx((x-w2)/w2, (y-h2)/h2);

            // add vertex to list of vertices in tile object
            tile.addVertex(vtx);
        }

        // set a random color for tile
        tile.setRandColor();

        // add tile to the m_tiles array
        m_tiles.push_back(tile);
    }

    // close file
    file.close();

    // assign m_tiles to the OpenGL widget
    m_glwidget->setTiles(m_tiles);
}


