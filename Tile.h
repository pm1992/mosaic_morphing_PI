// ======================================================================
//
// Tile.h - Tile class
//
// ======================================================================

#ifndef TILE_H
#define TILE_H

#include <vector>
#include <QVector2D>
#include <QColor>

using namespace std;

class Tile {
public:
    Tile();
    void		setNum(int num);
    int		num();
    void		addVertex(QVector2D &);
    QVector2D centroid();
    void		setRandColor();
    QColor		color();
    QVector2D	vertex(int);

private:
    int			m_num;		// number of vertices
    vector<QVector2D>	m_vertices;	// coordinates
    QVector2D		m_centroid;	// tile centroid
    QColor			m_color;	// tile color
};

#endif	// TILE_H
