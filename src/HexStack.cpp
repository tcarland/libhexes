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
    _curPanel(nullptr),
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
    _curPanel(nullptr),
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
    _curPanel = nullptr;

    if ( _factory )
        delete _factory;
}


/** Virtual method to ensure the current panel in the stack is valid, prior
  * to running redraw on the current panel.
 **/
int
HexStack::redraw()
{
    if ( _curPanel == nullptr )
        return 1;

    return _curPanel->redraw();
}

/** Runs Panel::poll() on the current panel in the stack. */
int
HexStack::poll()
{
    if ( _curPanel == nullptr )
        return ERR;

    return _curPanel->poll();
}

/** Runs resize for each panel in the stack */
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

/** Creates a new panel in the stack and makes it current.
  * If title is empty, an auto title is assigned.
 **/
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

    HexPanel * panel = nullptr;

    if ( _factory )
        panel = (*_factory)(_nextPanelId, useTitle);

    // Default factory creates a basic HexPanel sized to this stack.
    if ( panel == nullptr )
        panel = new HexPanel(useTitle, this->height(), this->width(), this->startY(), this->startX());

    panel->setPanelId(_stack.size());
    panel->setTextColor(this->getTextColor());
    panel->setBorderColor(this->getBorderColor());
    panel->setBorderActiveColor(this->getBorderActiveColor());

    bool wasFocused = false;
    if ( _curPanel ) {
        wasFocused = _curPanel->hasFocus();
        _curPanel->unsetFocus();
        _curPanel->hide();
    }

    _stack.push_back(panel);
    _curPanel  = panel;
    _curIndex  = _stack.size() - 1;
    _nextPanelId++;

    _curPanel->show();

    if ( wasFocused )
        _curPanel->setFocus();
    else
        _curPanel->unsetFocus();

    return _curPanel;
}


/** Destroys the current panel in the stack and switches to another.
  * If the stack has only one panel, it will be replaced with a fresh panel.
 **/
bool
HexStack::removeCurrentPanel()
{
    if ( _stack.empty() || _curPanel == nullptr )
        return false;

    bool wasFocused = _curPanel->hasFocus();
    size_t oldIndex = _curIndex;

    _curPanel->unsetFocus();
    _curPanel->hide();

    HexPanel * victim = _curPanel;
    _stack.erase(_stack.begin() + (long) _curIndex);
    delete victim;

    // Ensure at least one panel exists.
    if ( _stack.empty() ) 
    {
        this->createPanel();
        if ( _curPanel ) {
            if ( wasFocused )
                _curPanel->setFocus();
            else
                _curPanel->unsetFocus();
        }
        return true;
    }

    if ( oldIndex >= _stack.size() )
        _curIndex = _stack.size() - 1;
    else
        _curIndex = oldIndex;

    for ( size_t i = 0; i < _stack.size(); ++i ) {
        if ( _stack.at(i) )
            _stack.at(i)->setPanelId(i);
    }

    _curPanel = _stack.at(_curIndex);
    if ( _curPanel ) 
    {
        _curPanel->show();
        if ( wasFocused )
            _curPanel->setFocus();
        else
            _curPanel->unsetFocus();
    }

    return true;
}


/** Switches the current panel to the next/previous panel in the stack. */
HexPanel*
HexStack::nextPanel()
{
    if ( _stack.empty() )
        return nullptr;

    bool wasFocused = false;
    if ( _curPanel ) {
        wasFocused = _curPanel->hasFocus();
        _curPanel->unsetFocus();
        _curPanel->hide();
    }

    _curIndex++;
    if ( _curIndex >= _stack.size() )
        _curIndex = 0;

    _curPanel = _stack.at(_curIndex);
    if ( _curPanel )
        _curPanel->show();

    if ( _curPanel ) {
        if ( wasFocused )
            _curPanel->setFocus();
        else
            _curPanel->unsetFocus();
    }

    return _curPanel;
}


HexPanel*
HexStack::prevPanel()
{
    if ( _stack.empty() )
        return nullptr;

    bool wasFocused = false;
    if ( _curPanel ) {
        wasFocused = _curPanel->hasFocus();
        _curPanel->unsetFocus();
        _curPanel->hide();
    }

    if ( _curIndex == 0 )
        _curIndex = _stack.size() - 1;
    else
        _curIndex--;

    _curPanel = _stack.at(_curIndex);
    if ( _curPanel )
        _curPanel->show();

    if ( _curPanel ) {
        if ( wasFocused )
            _curPanel->setFocus();
        else
            _curPanel->unsetFocus();
    }

    return _curPanel;
}


/** Returns the current active panel (may be nullptr if none exist). */
HexPanel*
HexStack::currentPanel() const
{
    return _curPanel;
}


/** Returns the number of panels in the stack. */
size_t
HexStack::panelCount() const
{
    return _stack.size();
}


/** Returns the panel at the given index or nullptr if out of range. */
HexPanel*
HexStack::panelAt ( size_t index ) const
{
    if ( index >= _stack.size() )
        return nullptr;
    return _stack.at(index);
}


/** Sets the current panel by index; returns false if out of range. */
bool
HexStack::setCurrentIndex ( size_t index )
{
    if ( index >= _stack.size() )
        return false;

    bool wasFocused = false;
    if ( _curPanel ) {
        wasFocused = _curPanel->hasFocus();
        _curPanel->unsetFocus();
        _curPanel->hide();
    }

    _curIndex = index;
    _curPanel = _stack.at(_curIndex);
    if ( _curPanel )
        _curPanel->show();

    if ( _curPanel ) {
        if ( wasFocused )
            _curPanel->setFocus();
        else
            _curPanel->unsetFocus();
    }

    return true;
}


size_t
HexStack::currentIndex() const
{
    return _curIndex;
}


/** Applies this stack's current text/border colors to all child panels. */
void
HexStack::syncPanelColors()
{
    for ( PanelStack::iterator it = _stack.begin(); it != _stack.end(); ++it )
    {
        if ( *it == nullptr )
            continue;
        (*it)->setTextColor(this->getTextColor());
        (*it)->setBorderColor(this->getBorderColor());
        (*it)->setBorderActiveColor(this->getBorderActiveColor());
    }
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
