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

#ifndef DEBUGGER_DIALOG_HXX
#define DEBUGGER_DIALOG_HXX

class Debugger;
class OSystem;
class DialogContainer;
class FSNode;
class ButtonWidget;
class CpuWidget;
class PromptWidget;
class RamWidget;
class RomWidget;
class TabWidget;
class EditTextWidget;
class TiaInfoWidget;
class TiaOutputWidget;
class TiaZoomWidget;
class CartDebugWidget;
class CartRamWidget;
class OptionsDialog;
class DataGridOpsWidget;
class SystemAreaPanel;

namespace GUI {
  class MessageBox;
}  // namespace GUI
namespace Common {
  struct Rect;
}  // namespace Common

#include "Dialog.hxx"

class DebuggerDialog : public Dialog
{
  public:
    // Note: these sizes make sure that all major tabs are fully visible
    //       cart dependend information (e.g. DPC+) may require more space
    enum: uInt16 {
      kSmallFontMinW  = 1090, kSmallFontMinH  = 720,
      kMediumFontMinW = 1160, kMediumFontMinH = 770,
      kLargeFontMinW  = 1160, kLargeFontMinH  = 870
    };

    DebuggerDialog(OSystem& osystem, DialogContainer& parent,
                   int x, int y, int w, int h);
    ~DebuggerDialog() override;

    const GUI::Font& lfont() const     { return *myLFont;        }
    const GUI::Font& nfont() const     { return *myNFont;        }
    PromptWidget& prompt() const       { return *myPrompt;       }
    TiaInfoWidget& a() const           { return *myTiaInfo;      }
    TiaOutputWidget& tiaOutput() const { return *myTiaOutput;    }
    TiaZoomWidget& tiaZoom() const     { return *myTiaZoom;      }
    RomWidget& rom() const             { return *myRom;          }
    CartDebugWidget* cartDebug() const { return myCartDebug;    }
    CartRamWidget& cartRam() const     { return *myCartRam;      }
    EditTextWidget& message() const    { return *myMessageBox;   }
    ButtonWidget& rewindButton() const { return *myRewindButton; }
    ButtonWidget& unwindButton() const { return *myUnwindButton; }

    void showFatalMessage(string_view msg);
    void saveConfig() override;
    void refreshLayout() override;

  private:
    void setPosition() override { positionAt(0); }
    void loadConfig() override;
    void handleKeyDown(StellaKey key, StellaMod mod, bool repeated) override;
    void handleKeyUp(StellaKey key, StellaMod mod) override;
    void handleCommand(CommandSender* sender, int cmd, int data, int id) override;

    void doStep();
    void doTrace();
    void doScanlineAdvance();
    void doAdvance();
    void doRewind();
    void doUnwind();
    void doRewind10();
    void doUnwind10();
    void doRewindAll();
    void doUnwindAll();
    void doExitDebugger();
    void doExitRom();

    void createFont();
    void addTiaArea();
    void addSystemTabArea();
    void addStatusArea();
    void addRomTabArea();
    void addSystemArea();

    void resizeStatusArea();
    void resizeSystemTabArea();

    void calculateLayout();

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

    TabWidget *myTab{nullptr}, *myRomTab{nullptr};

    PromptWidget*    myPrompt{nullptr};
    TiaInfoWidget*   myTiaInfo{nullptr};
    TiaOutputWidget* myTiaOutput{nullptr};
    TiaZoomWidget*   myTiaZoom{nullptr};
    CpuWidget*       myCpu{nullptr};
    RamWidget*       myRam{nullptr};
    RomWidget*       myRom{nullptr};
    CartDebugWidget* myCartInfo{nullptr};
    CartDebugWidget* myCartDebug{nullptr};
    CartRamWidget*   myCartRam{nullptr};
    EditTextWidget*  myMessageBox{nullptr};
    ButtonWidget*    myRewindButton{nullptr};
    ButtonWidget*    myUnwindButton{nullptr};

    DataGridOpsWidget* opsWidget{nullptr};

    SystemAreaPanel* sysAreaPanel{nullptr};

    Common::Rect tiaRect;
    Common::Rect tiaStatusRect;
    Common::Rect systemTabAreaRect;
    Common::Rect cpuRiotRect;
    Common::Rect romTabAreaRect;


    unique_ptr<GUI::MessageBox> myFatalError;
    unique_ptr<OptionsDialog>   myOptions;

    unique_ptr<GUI::Font> myLFont;  // used for labels
    unique_ptr<GUI::Font> myNFont;  // used for normal text
    Widget* myFocusedWidget{nullptr};
    bool myExitPressed{false};

  private:
    // Following constructors and assignment operators not supported
    DebuggerDialog() = delete;
    DebuggerDialog(const DebuggerDialog&) = delete;
    DebuggerDialog(DebuggerDialog&&) = delete;
    DebuggerDialog& operator=(const DebuggerDialog&) = delete;
    DebuggerDialog& operator=(DebuggerDialog&&) = delete;
};

#endif
