// ViewInfo.h

#ifndef VIEWINFO_H

#define VIEWINFO_H

struct ViewInfo {
  static constexpr int TILESIZE = 512;
  int w, h; // widget width and height
  int xl, yt; // sbem coords of widget left, top
  int xr, yb; // sbem coords of widget right, bottom (exclusive)
  int z; // sbem z coord
  int a; // view scale
  int x0, y0; // scaled coords of left,top
  int c0, r0; // column, row at top left
  int dx, dy; // pixel offset of (xl,yt) in tile (c0,r0)
  int x1, y1; // scaled coords of right,bottom (incl.)
  int c1, r1; // column, row at bottom right
  ViewInfo(int w, int h, int x_, int y_, int z_, int a):
    w(w), h(h), xl(x_), yt(y_), z(z_), a(a) {
    xr = xl + (w<<a);
    yb = yt + (h<<a);
    x0 = xl>>a; // coordinates adjusted for zoom level
    y0 = yt>>a;
    c0 = x0 / TILESIZE; // tile number
    r0 = y0 / TILESIZE;
    dx = x0 - c0*TILESIZE; // offset of our top-left pixel in tile
    dy = y0 - r0*TILESIZE;
    x1 = (xl+(w<<a)-1)>>a; // coordinates of right most pixel (inclusive), adj.
    y1 = (yt+(h<<a)-1)>>a; // coordinates of right most pixel (inclusive), adj.
    c1 = x1 / TILESIZE;
    r1 = y1 / TILESIZE;
  }
};

#endif
