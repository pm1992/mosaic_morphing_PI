// ======================================================================
//
// Tile.cpp - Tile class
//
// ======================================================================

#include "Tile.h"

Tile::Tile()
    : m_num(0)
{}



void Tile::setNum(int num)
{
    m_num = num;
}



int Tile::num()
{
    return m_num;
}



void Tile::addVertex(QVector2D &vtx)
{
    m_vertices.push_back(vtx);
}



QVector2D Tile::vertex(int k)
{
    return m_vertices[k];
}



void Tile::setRandColor()
{
    m_color.setRgb((rand() % 256), (rand() % 256), (rand() % 256));
}



QColor Tile::color()
{
    return m_color;
}

QVector2D Tile::centroid()
{
    float cx = 0.;
    float cy = 0.;

    for ( int i = 0; i < m_num; i++ )
    {
        cx = cx + m_vertices[i].x();
        cy = cy + m_vertices[i].y();
    }

    cx = cx / m_num;
    cy = cy / m_num;

    m_centroid.setX(cx);
    m_centroid.setY(cy);

    return m_centroid;
}
