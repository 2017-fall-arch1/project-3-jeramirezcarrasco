#include "shape.h"
// this also set up the score goal (tripple score)
int 
abRectCheckC(const Abcat *rect, const Vec2 *centerPos, const Vec2 *pixel)
{
  int row, col;
    row = pixel->axes[1]; col = pixel->axes[0];
    if((row>33 && row<38 && col>37 && col<99)||(row>99 && row<104 && col >37 && col<99))
    return 1;
  else return 0;
}

// compute bounding box in screen coordinates for rect at centerPos
void abRectGetBoundsC(const Abcat *rect, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rect->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rect->halfSize);
}



// true if pixel is in rect centerPosed at rectPos
int 
abRectOutlineCheckC(const AbRectOutlineC *rect, const Vec2 *centerPos, const Vec2 *pixel)
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
void abRectOutlineGetBoundsC(const AbRectOutlineC *rect, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rect->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rect->halfSize);
}



