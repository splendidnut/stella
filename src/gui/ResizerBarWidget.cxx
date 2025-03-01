//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2025 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "Dialog.hxx"
#include "FBSurface.hxx"
#include "ResizerBarWidget.hxx"
#include "bspf.hxx"

ResizerBarWidget::ResizerBarWidget(GuiObject *boss, const GUI::Font& font, int x, int y, int w, int h, ResizeDir dir)
  : Widget(boss, font, x, y, w, h),
  _resizeDir{dir}
{
  _flags = Widget::FLAG_ENABLED | Widget::FLAG_TRACK_MOUSE | Widget::FLAG_CLEARBG;
  _bgcolor = kWidColor;
  _bgcolorhi = kWidColor;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ResizerBarWidget::setLimits(int minPoint, int maxPoint) {
  _minPoint = minPoint;
  _maxPoint = maxPoint;

  // Re-check bounds:
  if (_x < _minPoint) _x = _minPoint;
  if (_x > _maxPoint) _x = _maxPoint;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ResizerBarWidget::drawWidget(bool hilite)
{
  FBSurface& s = _boss->dialog().surface();

  // draw border / fill
  s.frameRect(_x, _y, _w, _h, hilite ? kWidColorHi : kColor);
  s.fillRect(_x + 1, _y + 1, _w - 2, _h - 2, hilite ? kScrollColorHi : kScrollColor);

  if (_resizeDir == ResizeDir::Horizontal) {
    // vertical bar
    s.vLine(_x + (_w / 2), _y + _h / 3, _y + (_h / 3 * 2) - 1, kSliderBGColorHi);

  } else {
    // horizontal bar
    s.hLine(_x + (_w / 3), _y + (_h / 2), _x + (_w / 3 * 2) - 1, kSliderBGColorHi);
  }

  clearDirty();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ResizerBarWidget::handleMouseDown(int x, int y, MouseButton b, int clickCount)
{
  _dragging = true;
  handleMouseMoved(x,y);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ResizerBarWidget::handleMouseUp(int x, int y, MouseButton b, int clickCount)
{
  _dragging = false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ResizerBarWidget::handleMouseMoved(int x, int y) {
  if (!_dragging) return;

  cerr << "ResizerBarWidget::handleMouseMoved() = " << x << ',' << y << '\n';

  int newPos = (_resizeDir == ResizeDir::Horizontal)
      ? _x + x
      : _y + y;

  // check bounds:
  if (newPos < _minPoint) newPos = _minPoint;
  if (newPos > _maxPoint) newPos = _maxPoint;

  if (_resizeDir == ResizeDir::Horizontal) {

    // Handle resizing in the horizontal direction
    if (_x != newPos) {
      _x += x;
      setDirty();
      sendCommand(kResizeCmd, newPos, _id);
    }
  } else {

    // Handle resizing in the vertical direction
    if (_y != newPos) {
      _y += y;
      setDirty();
      sendCommand(kResizeCmd, newPos, _id);
    }
  }

}
