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

#include "Font.hxx"
#include "DataGridOpsWidget.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DataGridOpsWidget::DataGridOpsWidget(GuiObject* boss, const GUI::Font& font,
                                     int x, int y)
  : Widget(boss, font, x, y, 16, 16),
    CommandSender(boss)
{
  const int bwidth = _font.getMaxCharWidth() * 4 + 2,
            bheight = _font.getFontHeight() + 3;
  constexpr int space = 4;

  // Create operations buttons
  int xpos = x;  int ypos = y;
  _zeroButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                 "0", kDGZeroCmd);
  _zeroButton->setToolTip("Zero currently selected value (Z)");
  buttonList.push_back(_zeroButton);

  ypos += bheight + space;
  _invButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                "Inv", kDGInvertCmd);
  _invButton->setToolTip("Invert currently selected value (I)");
  buttonList.push_back(_invButton);

  ypos += bheight + space;
  _incButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                "++", kDGIncCmd);
  _incButton->setToolTip("Increase currently selected value. (=, Keypad +)");
  buttonList.push_back(_incButton);

  ypos += bheight + space;
  _shiftLeftButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                      "<<", kDGShiftLCmd);
  _shiftLeftButton->setToolTip("Shift currently selected value left (,)");
  buttonList.push_back(_shiftLeftButton);

  // Move to next column, skip a row
  xpos = x + bwidth + space;  ypos = y + bheight + space;
  _negButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                "Neg", kDGNegateCmd);
  _negButton->setToolTip("Negate currently selected value (N)");
  buttonList.push_back(_negButton);

  ypos += bheight + space;
  _decButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                "--", kDGDecCmd);
  _decButton->setToolTip("Decrease currently selected value (-, Keypad -)");
  buttonList.push_back(_decButton);

  ypos += bheight + space;
  _shiftRightButton = new ButtonWidget(boss, font, xpos, ypos, bwidth, bheight,
                                       ">>", kDGShiftRCmd);
  _shiftRightButton->setToolTip("Shift currently selected value right (.)");
  buttonList.push_back(_shiftRightButton);

  // Calculate real dimensions
  _w = 2 * (bwidth+space);
  _h = 4 * (bheight+space);

  // We don't enable the buttons until the DataGridWidget is attached
  // Don't call setEnabled(false), since that does an immediate redraw
  for (auto *widget : buttonList) {
    widget->clearFlags(Widget::FLAG_ENABLED);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DataGridOpsWidget::setPosX(int newX) {
  int oldX = _x;
  Widget::setPosX(newX);

  int delta = newX - oldX;
  for (auto *widget : buttonList) {
    widget->setPosX(widget->getLeft() + delta);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DataGridOpsWidget::setTarget(CommandReceiver* target)
{
  _zeroButton->setTarget(target);
  _invButton->setTarget(target);
  _negButton->setTarget(target);
  _incButton->setTarget(target);
  _decButton->setTarget(target);
  _shiftLeftButton->setTarget(target);
  _shiftRightButton->setTarget(target);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DataGridOpsWidget::setEnabled(bool e)
{
  for (auto *widget : buttonList) {
    widget->setEnabled(e);
  }
}
