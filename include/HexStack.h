/**
  * @file HexStack.h
  *
  * Copyright (c) 2011-2026 Timothy Charlton Arland <tcarland@gmail.com>
  *
  * @section LICENSE
  *
  * This file is part of libhexes.
  *
  * libhexes is free software: you can redistribute it and/or modify
  * it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * libhexes is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with libhexes.
  * If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef _HEXES_HEXSTACK_H_
#define _HEXES_HEXSTACK_H_

#include "HexPanel.h"
#include "HexApp.h"


namespace hexes {


/** A specialized widget that is itself a HexPanel, but also 
  * can contain a number of additional HexPanel instances in 
  * a stack under the current panel. A stack of panes.
 **/
class HexStack : public HexPanel {

  public:

    HexStack ( const std::string & title = "" );
    HexStack ( const std::string & title,
               int   height, int   width,
               int   starty, int   startx );

    virtual ~HexStack();

    
    virtual int  redraw();
    virtual int  poll();
    virtual void resize  ( int height, int width );


    /** Creates a new panel in the stack and makes it current.
      * If title is empty, an auto title is assigned.
      */
    HexPanel* createPanel ( const std::string & title = "" );

    /** Switches the current panel to the next/previous panel in the stack. */
    HexPanel* nextPanel();
    HexPanel* prevPanel();

    /** Returns the current active panel (may be NULL if none exist). */
    HexPanel* currentPanel() const;

    /** Returns the number of panels in the stack. */
    size_t panelCount() const;

    /** Returns the panel at the given index or NULL if out of range. */
    HexPanel* panelAt ( size_t index ) const;


    /** Sets the current panel by index; returns false if out of range. */
    bool setCurrentIndex ( size_t index );
    size_t currentIndex() const;

    /** Applies this stack's current text/border colors to all child panels. */
    void syncPanelColors();


    void setPanelFactory ( HexPanelFactory * factory );

  protected:

    PanelStack          _stack;
    HexPanel *          _curPanel;
    HexPanelFactory *   _factory;

    size_t              _curIndex;
    int                 _nextPanelId;

    bool                _winCmd;
};

}  // namespace

#endif  // _HEXES_HEXSTACK_H_
