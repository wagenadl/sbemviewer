// ViewInfo.h

#ifndef VIEWINFO_H

#define VIEWINFO_H

struct ViewInfo {
  int w, h;
  int xl, yt;
  int z, a;
  int x0, y0;
  int c0, r0;
  int dx, dy;
  int x1, y1;
  int c1, r1;
  ViewInfo(int w, int h, int x_, int y_, int z_, int a):
    w(w), h(h), xl(x_), yt(y_), z(z_), a(a) {
    x0 = xl>>a; // coordinates adjusted for zoom level
    y0 = yt>>a;
    c0 = x0 / TileViewer::TILESIZE; // tile number
    r0 = y0 / TileViewer::TILESIZE;
    dx = x0 - c0*TileViewer::TILESIZE; // offset of our top-left pixel in tile
    dy = y0 - r0*TileViewer::TILESIZE;
    x1 = (xl+(w<<a)-1)>>a; // coordinates of right most pixel (inclusive), adj.
    y1 = (yt+(h<<a)-1)>>a; // coordinates of right most pixel (inclusive), adj.
    c1 = x1 / TileViewer::TILESIZE;
    r1 = y1 / TileViewer::TILESIZE;
  }
};

#endif
