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


    HexPanel*   createPanel ( const std::string & title = "" );
    HexPanel*   currentPanel() const;
    HexPanel*   nextPanel();
    HexPanel*   prevPanel();
    bool        removeCurrentPanel();

    HexPanel*   panelAt ( size_t index ) const;
    size_t      panelCount() const;

    bool        setCurrentIndex ( size_t index );
    size_t      currentIndex() const;

    void        syncPanelColors();

    void        setPanelFactory ( HexPanelFactory * factory );


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
