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

#include "SystemAreaPanel.hxx"
#include "DataGridOpsWidget.hxx"
#include "CpuWidget.hxx"
#include "RiotRamWidget.hxx"

SystemAreaPanel::SystemAreaPanel(GuiObject *boss, const GUI::Font& lfont, const GUI::Font& nfont,
                                 int x, int y, int w, int h)
  : Widget(boss, lfont, x, y, w, h),
    CommandSender(boss)
{
  int bwidth  = lfont.getStringWidth("Frame +1 ");
  int bwidth2 = lfont.getStringWidth("Options " + ELLIPSIS);
  int bheight = lfont.getLineHeight() + 2;

  int vBtnWidth = bheight;
  int vBtnHeight = bheight * 3 + 4 * 2;  // height of 3 buttons + close the gaps between them

  // CPU info (internal register display) -- upper left corner of System Area
  int cpuInfoX = 10;
  int cpuInfoY = 5;

  // buttonPanelWidth is the size of the button panel to the right of the CPU display
  int buttonPanelWidth = bwidth + 5 + vBtnWidth + 5 + bwidth2 + 10;
  int cpuInfoWidth = _w - buttonPanelWidth - 10;

  int buttonY = 5;
  int mainButtonX = _w - bwidth - 5;
  int rewindButtonX = mainButtonX - (bheight + 5);
  int optionButtonX = rewindButtonX - bwidth2 - 10;

  buildMainButtonList(mainButtonX, buttonY, bwidth, bheight);
  buildRewindButtons(rewindButtonX, buttonY, vBtnWidth, vBtnHeight);      //--- VERTICAL buttons


  optionButton = new ButtonWidget(this, lfont, optionButtonX, buttonY, bwidth2, bheight,
                                     "Options" + ELLIPSIS, kDDOptionsCmd);

  wid1.push_back(optionButton);
  wid1.push_back(myRewindButton);
  wid1.push_back(myUnwindButton);

  int xpos = rewindButtonX - bwidth2 - 10;
  int ypos = 30;

  opsWidget = new DataGridOpsWidget(this, lfont, xpos, ypos);

  myCpu = new CpuWidget(this, lfont, nfont, cpuInfoX, cpuInfoY, cpuInfoWidth);

  int ramWidgetX = 10;
  int ramWidgetY = myCpu->getHeight() + 15;

  myRam = new RiotRamWidget(this, lfont, nfont, ramWidgetX, ramWidgetY, _w - 10);
  //myRam->setHelpAnchor("M6532", true); // TODO: doesn't work

  addToFocusList(myCpu->getFocusList());
  addToFocusList(wid1);
  addToFocusList(wid2);
  addToFocusList(myRam->getFocusList());

  Vec::append(systemAreaWidgetList, myCpu->getFocusList());
  Vec::append(systemAreaWidgetList, wid1);
  Vec::append(systemAreaWidgetList, wid2);
  Vec::append(systemAreaWidgetList, myRam->getFocusList());

  // Add the DataGridOpsWidget to any widgets which contain a
  // DataGridWidget which we want controlled
  myCpu->setOpsWidget(opsWidget);
  myRam->setOpsWidget(opsWidget);

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int SystemAreaPanel::calcHeight(const GUI::Font& font) {
  int ramHeight = RamWidget::calcHeight(font, 8);
  int cpuHeight = CpuWidget::calcHeight(font);
  return ramHeight + cpuHeight + 15;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SystemAreaPanel::setSize(int w, int h) {
  Widget::setSize(w, h);

  // button sizes
  const int bwidth  = _font.getStringWidth("Frame +1 ");
  const int bwidth2 = _font.getStringWidth("Options " + ELLIPSIS);
  const int bheight = _font.getLineHeight() + 2;
  const int vBtnWidth = bheight;

  // buttonPanelWidth is the size of the button panel to the right of the CPU display
  int buttonPanelWidth = bwidth + 5 + vBtnWidth + 5 + bwidth2 + 10;


  // reposition buttons to stay against right side of panel area
  int mainButtonX = _w - bwidth - 5;
  int rewindButtonX = mainButtonX - (bheight + 5);
  int optionButtonX = rewindButtonX - bwidth2 - 10;
  int opsWidgetX = rewindButtonX - bwidth2 - 10;

  for (auto *widget : wid2) {
    widget->setPosX(mainButtonX);
  }
  myRewindButton->setPosX(rewindButtonX);
  myUnwindButton->setPosX(rewindButtonX);
  optionButton->setPosX(optionButtonX);
  opsWidget->setPosX(opsWidgetX);

  //-- CPU widget expands horizontally to fill the region
  int cpuInfoWidth = _w - buttonPanelWidth - 10;
  myCpu->setWidth(cpuInfoWidth);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SystemAreaPanel::~SystemAreaPanel() { // NOLINT (we need an empty d'tor)

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SystemAreaPanel::loadConfig() {
  myCpu->loadConfig();
  myRam->loadConfig();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int SystemAreaPanel::buildMainButtonList(int buttonX, int buttonY, int bwidth, int bheight) {
  ButtonWidget *b;
  b = new ButtonWidget(this, _font, buttonX, buttonY,
                       bwidth, bheight, "Step", kDDStepCmd, true);
  b->setToolTip("Ctrl+S");
  b->setHelpAnchor("GlobalButtons", true);
  wid2.push_back(b);
  buttonY += bheight + 4;

  b = new ButtonWidget(this, _font, buttonX, buttonY,
                       bwidth, bheight, "Trace", kDDTraceCmd, true);
  b->setToolTip("Ctrl+T");
  b->setHelpAnchor("GlobalButtons", true);
  wid2.push_back(b);
  buttonY += bheight + 4;

  b = new ButtonWidget(this, _font, buttonX, buttonY,
                       bwidth, bheight, "Scan +1", kDDSAdvCmd, true);
  b->setToolTip("Ctrl+L");
  b->setHelpAnchor("GlobalButtons", true);
  wid2.push_back(b);
  buttonY += bheight + 4;

  b = new ButtonWidget(this, _font, buttonX, buttonY,
                       bwidth, bheight, "Frame +1", kDDAdvCmd, true);
  b->setToolTip("Ctrl+F");
  b->setHelpAnchor("GlobalButtons", true);
  wid2.push_back(b);
  buttonY += bheight + 4;

  b = new ButtonWidget(this, _font, buttonX, buttonY,
                       bwidth, bheight, "Run", kDDRunCmd);
  b->setToolTip("Escape");
  b->setHelpAnchor("GlobalButtons", true);
  wid2.push_back(b);

  return buttonY;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SystemAreaPanel::buildRewindButtons(int buttonX, int buttonY,
                                         int bwidth, int bheight) {
  static constexpr std::array<uInt32, 11> LEFT_ARROW = {
          0b0000010,
          0b0000110,
          0b0001110,
          0b0011110,
          0b0111110,
          0b1111110,
          0b0111110,
          0b0011110,
          0b0001110,
          0b0000110,
          0b0000010
  };
  static constexpr std::array<uInt32, 11> RIGHT_ARROW = {
          0b0100000,
          0b0110000,
          0b0111000,
          0b0111100,
          0b0111110,
          0b0111111,
          0b0111110,
          0b0111100,
          0b0111000,
          0b0110000,
          0b0100000
  };

  myRewindButton = new ButtonWidget(this, _font, buttonX, buttonY,
                           bwidth, bheight, LEFT_ARROW.data(), 7, 11, kDDRewindCmd, true);
  myRewindButton->setToolTip("Alt[+Shift]+Left");
  myRewindButton->setHelpAnchor("GlobalButtons", true);
  myRewindButton->clearFlags(FLAG_ENABLED);

  buttonY += bheight + 4;
  bheight = (_font.getLineHeight() + 2) * 2 + 4 * 1;

  myUnwindButton = new ButtonWidget(this, _font, buttonX, buttonY,
                           bwidth, bheight, RIGHT_ARROW.data(), 7, 11, kDDUnwindCmd, true);
  myUnwindButton->setToolTip("Alt[+Shift]+Right");
  myUnwindButton->setHelpAnchor("GlobalButtons", true);
  myUnwindButton->clearFlags(FLAG_ENABLED);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Since this panel area as a container widget, the following functions
// are implemented to handle drawing and passing events to the children.
//
//   findWidget() - allow GUI system to find widgets within this panel
//   drawWidget() - pass 'dirty' to widgets contained within this panel
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Widget* SystemAreaPanel::findWidget(int x, int y)
{
  // Iterate over all child widgets and find the one which was clicked
  return Widget::findWidgetInChain(_firstWidget, x, y);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SystemAreaPanel::drawWidget(bool hilite) {
  // This panel area acts as both a widget and a dialog (it contains other widgets).
  // Because of the latter, it must assume responsibility for refreshing all its children.

  if (isDirty()) {
    clearDirty();
    // Mark all child widgets dirty, so that they also get redrawn
    Widget::setDirtyInChain(_firstWidget);
  }
}