#include "shape.h"
// here i draw the score graphics gaol, using the coordinates
int 
abRectCheckm(const Abmap *rect, const Vec2 *centerPos, const Vec2 *pixel)
{
  int row, col;
    row = pixel->axes[1]; col = pixel->axes[0];
  if((row>8 && row<20 && col>30 && col<98)||(row>74 && row<86 && col >30 && col<98) || (row>140 && row<152 && col>30 && col<98))
    return 1;
  else return 0;
}

// compute bounding box in screen coordinates for rect at centerPos
void abRectGetBoundsm(const Abmap *rect, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rect->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rect->halfSize);
}



// true if pixel is in rect centerPosed at rectPos
int 
abRectOutlineCheckm(const AbRectOutlinem *rect, const Vec2 *centerPos, const Vec2 *pixel)
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
void abRectOutlineGetBoundsm(const AbRectOutlinem *rect, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rect->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rect->halfSize);
}



