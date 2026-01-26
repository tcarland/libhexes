/**
  * @file HexStack.cpp
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
#define _HEXES_HEXSTACK_CPP_

#include "HexStack.h"

#include <sstream>

namespace hexes {


HexStack::HexStack ( const std::string & title )
  : HexPanel(title),
    _curPanel(NULL),
    _factory(new HexPanelFactory(this)),
    _curIndex(0),
    _nextPanelId(0),
    _winCmd(false)
{
    this->setDrawBorder(false);
    this->setDrawTitle(false);
    this->hide();

    // Always start with at least one panel.
    this->createPanel();
}

HexStack::HexStack ( const std::string & title,
                     int   height, int   width,
                     int   starty, int   startx )
  : HexPanel(title, height, width, starty, startx),
    _curPanel(NULL),
    _factory(new HexPanelFactory(this)),
    _curIndex(0),
    _nextPanelId(0),
    _winCmd(false)
{
    this->setDrawBorder(false);
    this->setDrawTitle(false);
    this->hide();

    // Always start with at least one panel.
    this->createPanel();
}


HexStack::~HexStack()
{
    for ( PanelStack::iterator it = _stack.begin(); it != _stack.end(); ++it )
    {
        if ( *it )
            delete *it;
    }
    _stack.clear();
    _curPanel = NULL;

    if ( _factory )
        delete _factory;
}


int
HexStack::redraw()
{
    if ( _curPanel == NULL )
        return 1;

    return _curPanel->redraw();
}

int
HexStack::poll()
{
    if ( _curPanel == NULL )
        return ERR;

    return _curPanel->poll();
}

void
HexStack:: resize ( int height, int width )
{
    HexPanel::resize(height, width);

    for ( PanelStack::iterator it = _stack.begin(); it != _stack.end(); ++it )
    {
        if ( *it )
            (*it)->resize(height, width);
    }

    return;
}


HexPanel*
HexStack::createPanel ( const std::string & title )
{
    std::string useTitle = title;
    if ( useTitle.empty() )
    {
        std::ostringstream ostr;
        ostr << this->getPanelName() << "-" << (_stack.size() + 1);
        useTitle = ostr.str();
    }

    HexPanel * panel = NULL;

    if ( _factory )
        panel = (*_factory)(_nextPanelId, useTitle);

    // Default factory creates a basic HexPanel sized to this stack.
    if ( panel == NULL )
        panel = new HexPanel(useTitle, this->height(), this->width(), this->startY(), this->startX());

    panel->setPanelId(_stack.size());
    panel->setTextColor(this->getTextColor());
    panel->setBorderColor(this->getBorderColor());
    panel->setBorderActiveColor(this->getBorderActiveColor());

    // Hide current before switching.
    if ( _curPanel )
        _curPanel->hide();

    _stack.push_back(panel);
    _curPanel  = panel;
    _curIndex  = _stack.size() - 1;
    _nextPanelId++;

    _curPanel->show();

    return _curPanel;
}


HexPanel*
HexStack::nextPanel()
{
    if ( _stack.empty() )
        return NULL;

    if ( _curPanel )
        _curPanel->hide();

    _curIndex++;
    if ( _curIndex >= _stack.size() )
        _curIndex = 0;

    _curPanel = _stack.at(_curIndex);
    if ( _curPanel )
        _curPanel->show();

    return _curPanel;
}


HexPanel*
HexStack::prevPanel()
{
    if ( _stack.empty() )
        return NULL;

    if ( _curPanel )
        _curPanel->hide();

    if ( _curIndex == 0 )
        _curIndex = _stack.size() - 1;
    else
        _curIndex--;

    _curPanel = _stack.at(_curIndex);
    if ( _curPanel )
        _curPanel->show();

    return _curPanel;
}


HexPanel*
HexStack::currentPanel() const
{
    return _curPanel;
}


size_t
HexStack::panelCount() const
{
    return _stack.size();
}


HexPanel*
HexStack::panelAt ( size_t index ) const
{
    if ( index >= _stack.size() )
        return NULL;
    return _stack.at(index);
}


bool
HexStack::setCurrentIndex ( size_t index )
{
    if ( index >= _stack.size() )
        return false;

    if ( _curPanel )
        _curPanel->hide();

    _curIndex = index;
    _curPanel = _stack.at(_curIndex);
    if ( _curPanel )
        _curPanel->show();

    return true;
}


size_t
HexStack::currentIndex() const
{
    return _curIndex;
}

void
HexStack::setPanelFactory ( HexPanelFactory * factory )
{
    if ( this->_factory )
        delete _factory;
    _factory = factory;
}

}  // namespace

// _HEXES_HEXSTACK_CPP_
