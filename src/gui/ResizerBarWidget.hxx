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

#ifndef RESIZER_BAR_WIDGET_HXX
#define RESIZER_BAR_WIDGET_HXX

class GuiObject;

#include "Widget.hxx"
#include "Command.hxx"

// Direction Resizer Bar will move
enum class ResizeDir: uInt8 {
  Horizontal,
  Vertical
};

class ResizerBarWidget : public Widget, public CommandSender {
  public:
    ResizerBarWidget(GuiObject *boss, const GUI::Font& font, int x, int y, int w, int h, ResizeDir dir);
    ~ResizerBarWidget() override = default;

    void setLimits(int minPoint, int maxPoint);

    void handleMouseDown(int x, int y, MouseButton b, int clickCount) override;
    void handleMouseUp(int x, int y, MouseButton b, int clickCount) override;
    void handleMouseMoved(int x, int y) override;

  private:
    void drawWidget(bool hilite) override;

    bool _dragging{};

    ResizeDir _resizeDir;
    int _minPoint;
    int _maxPoint;

  private:
    // Following constructors and assignment operators not supported
    ResizerBarWidget() = delete;
    ResizerBarWidget(const ResizerBarWidget&) = delete;
    ResizerBarWidget(ResizerBarWidget&&) = delete;
    ResizerBarWidget& operator=(const ResizerBarWidget&) = delete;
    ResizerBarWidget& operator=(ResizerBarWidget&&) = delete;
};

#endif //RESIZER_BAR_WIDGET_HXX
