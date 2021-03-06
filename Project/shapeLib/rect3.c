#include "shape.h"
// this set up the score goal graphics
int 
abRectCheckb(const Abbak *rect, const Vec2 *centerPos, const Vec2 *pixel)
{
  int row, col;
    row = pixel->axes[1]; col = pixel->axes[0];
    if((row>47 && row<55 && col>30 && col<98)||(row>113 && row<121 && col >30 && col<98))
    return 1;
  else return 0;
}

// compute bounding box in screen coordinates for rect at centerPos
void abRectGetBoundsb(const Abbak *rect, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rect->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rect->halfSize);
}



// true if pixel is in rect centerPosed at rectPos
int 
abRectOutlineCheckb(const AbRectOutlineb *rect, const Vec2 *centerPos, const Vec2 *pixel)
{
  Region bounds;
  abRectOutlineGetBounds(rect, centerPos, &bounds);
  int col = pixel->axes[0], row = pixel->axes[1];
  return (
	  ((col == bounds.topLeft.axes[0] || col == bounds.botRight.axes[0])
	   && 
	   (row >= bounds.topLeft.axes[1] && row <= bounds.botRight.axes[1]))
	  ||
	  ((row == bounds.topLeft.axes[1] || row == bounds.botRight.axes[1])
	   &&
	   (col >= bounds.topLeft.axes[0] && col <= bounds.botRight.axes[0]))
	  );
}
 
// compute bounding box in screen coordinates for rect at centerPos
void abRectOutlineGetBoundsb(const AbRectOutlineb *rect, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rect->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rect->halfSize);
}



