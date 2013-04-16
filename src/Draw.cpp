/*****************************************************************************

    (c) 2013 Hobu, Inc. hobu.inc@gmail.com

    Author: Andrew Bell andrew.bell.ia at gmail.com

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/


#include <iostream>

#include <hexer/Draw.hpp>

#include <hexer/Hexagon.hpp>
#include <hexer/HexGrid.hpp>
#include <hexer/Mathpair.hpp>
#include <hexer/Segment.hpp>
#include <hexer/hexer_defines.h>

using namespace std;

double red[] = { 1, 0, 0 };
double yellow[] = { 1, 1, 0 };
double blue[] = { 0, 0, 1 };

typedef double Color[];

namespace Pshape
{


Draw::Draw(HexGrid *grid_p) : m_grid_p(grid_p)
{
    // m_dpy_p = XOpenDisplay(NULL);
    // if (!m_dpy_p)
    // {
    //     cerr << "Can't open display!\n";
    // }
    // m_window = XCreateSimpleWindow(m_dpy_p, RootWindow(m_dpy_p, 0), 0, 0,
    //         750, 1000, 0, 0, BlackPixel(m_dpy_p, 0));
            
            
    // XSelectInput(m_dpy_p, m_window, StructureNotifyMask | ExposureMask);
    // XMapWindow(m_dpy_p, m_window);
    // m_surface_p = cairo_xlib_surface_create(m_dpy_p, m_window,
    //         DefaultVisual(m_dpy_p, 0), 750, 1000);

#ifdef HEXER_HAVE_CAIRO    
    m_surface_p = cairo_svg_surface_create("svgfile.svg", 750, 1000);
    m_cairo_p = cairo_create(m_surface_p);

    // Move things to the center.
    cairo_translate(m_cairo_p, 400, 50 );
#endif
    
}

Draw::~Draw()
{
#ifdef HEXER_HAVE_CAIRO    
    cairo_surface_destroy(m_surface_p);
    cairo_destroy(m_cairo_p);
    // XCloseDisplay(m_dpy_p);
#endif

}

void Draw::drawHexagon(Hexagon *hex_p, bool fill)
{
#ifdef HEXER_HAVE_CAIRO    
    Point pos;
    
    cairo_set_line_width(m_cairo_p, 1);
    cairo_set_source_rgb(m_cairo_p, .5, .5, 0);
    pos.m_x = hex_p->x() * m_grid_p->width();
    pos.m_y = hex_p->y() * m_grid_p->height();

    if (hex_p->xodd())
    {
        pos.m_y += (m_grid_p->height() / 2);
    }
    cairo_move_to(m_cairo_p, pos.m_x, pos.m_y);
    for (int i = 1; i <= 5; ++i)
    {
        Point p = pos + m_grid_p->offset(i);
        cairo_line_to(m_cairo_p, p.m_x, p.m_y);
    }
    cairo_close_path(m_cairo_p);
    if ( hex_p->x() == 0 && hex_p->y() == 0 )
    {
        cairo_fill(m_cairo_p);
    }
    if ( hex_p->dense() )
    {
        cairo_set_source_rgb(m_cairo_p, .3, .3, 0);
        cairo_fill(m_cairo_p);
    }
    if ( hex_p->possibleRoot())
    {
        Segment s(hex_p, 0);
        drawSegment(s, Color(1, 0, 0));
    }
    
    cairo_stroke(m_cairo_p);
    cairo_surface_flush(m_surface_p);
#endif    
    // XFlush(m_dpy_p);
}

void Draw::drawSegment(Segment s, Color c)
{
#ifdef HEXER_HAVE_CAIRO    
    // cairo_set_line_width(m_cairo_p, 2);
    // cairo_set_source_rgb(m_cairo_p, c.m_red, c.m_blue, c.m_green);
    // 
    // Pshape::Point startpoint = s.startPos() - m_grid_p->origin();
    // Pshape::Point endpoint = s.endPos() - m_grid_p->origin();
    // cairo_move_to(m_cairo_p, startpoint.m_x, startpoint.m_y);
    // cairo_line_to(m_cairo_p, endpoint.m_x, endpoint.m_y);
    // cairo_stroke(m_cairo_p);
    // cairo_surface_flush(m_surface_p);
    // XFlush(m_dpy_p);
#endif    
}

void Draw::drawPoint(Pshape::Point p)
{
#ifdef HEXER_HAVE_CAIRO    
    p -= m_grid_p->origin();
    cairo_arc(m_cairo_p, p.m_x, p.m_y, 1.0, 0.0, 2 * M_PI);
    cairo_stroke(m_cairo_p);
#endif
}


} //namespace
