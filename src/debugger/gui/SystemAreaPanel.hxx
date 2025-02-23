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

#ifndef SYSTEM_AREA_PANEL_HXX
#define SYSTEM_AREA_PANEL_HXX

#include "Widget.hxx"

class CpuWidget;
class RamWidget;
class ButtonWidget;
class DataGridOpsWidget;

class SystemAreaPanel : public Widget, public CommandSender
{
  public:
    SystemAreaPanel(GuiObject* boss, const GUI::Font& lfont, const GUI::Font& nfont, int x, int y, int w, int h);
    ~SystemAreaPanel() override;

    ButtonWidget *getRewindButton() { return myRewindButton; }
    ButtonWidget *getUnwindButton() { return myUnwindButton; }

    void loadConfig();
    Widget* findWidget(int x, int y);
    void drawWidget(bool hilite);

    // Get total height of this widget based on font provided
    static int calcHeight(const GUI::Font& font);
protected:

  private:
    enum {
        kDDStepCmd      = 'DDst',
        kDDTraceCmd     = 'DDtr',
        kDDAdvCmd       = 'DDav',
        kDDSAdvCmd      = 'DDsv',
        kDDRewindCmd    = 'DDrw',
        kDDUnwindCmd    = 'DDuw',
        kDDRunCmd       = 'DDex',
        kDDExitFatalCmd = 'DDer',
        kDDOptionsCmd   = 'DDop'
    };

    CpuWidget*       myCpu{nullptr};
    RamWidget*       myRam{nullptr};
    ButtonWidget*    myRewindButton{nullptr};
    ButtonWidget*    myUnwindButton{nullptr};
    ButtonWidget*    optionButton{nullptr};

    DataGridOpsWidget* opsWidget{nullptr};

    WidgetArray systemAreaWidgetList{};
    WidgetArray wid1, wid2;

    int buildMainButtonList(int buttonX, int buttonY, int bwidth, int bheight);
    void buildRewindButtons(int buttonX, int buttonY, int bwidth, int bheight);

    // Following constructors and assignment operators not supported
    SystemAreaPanel() = delete;
    SystemAreaPanel(const SystemAreaPanel&) = delete;
    SystemAreaPanel(SystemAreaPanel&&) = delete;
    SystemAreaPanel& operator=(const SystemAreaPanel&) = delete;
    SystemAreaPanel& operator=(SystemAreaPanel&&) = delete;

};

#endif //SYSTEM_AREA_PANEL_HXX
