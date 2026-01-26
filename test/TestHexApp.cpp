#define _HEXES_TESTHEXAPP_CPP_

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "TestHexApp.h"

#include "HexPanel.h"
#include "HexStack.h"
#include "HexDialog.h"
#include "LineInputHandler.h"
#include "LineOutputHandler.h"

namespace hexes {

TestHexApp::TestHexApp()
  : mainStack(NULL),
    mainPanel(NULL),
    statPanel(NULL),
    conPanel(NULL),
    _conheight(3),
    _titleheight(1),
    _statheight(0),
    _mainheight(0)
{}

void
TestHexApp::resize()
{
    int  statstarty, constarty;
    int  ht, wd;

    ht = this->height();
    wd = this->width();

    _statheight = (ht * .33) - _conheight;
    _mainheight = ht - _statheight - _conheight - _titleheight;
    statstarty  = ht - _statheight - _conheight;
    constarty   = ht - _conheight;

    if ( mainStack )
        mainStack->resize(_mainheight, wd);
    statPanel->resize(_statheight, wd);
    conPanel->resize(_conheight, wd);
    statPanel->erase();
    statPanel->moveWindow(statstarty, 0);
    conPanel->erase();
    conPanel->moveWindow(constarty, 0);

    std::ostringstream  res;

    res << "ht = " << ht << " wd = " << wd;
    statPanel->addText(res.str());

    res.seekp(std::ios::beg);
    res << "mainPanel: " << _mainheight << ", " << wd << ", 0, 0";
    statPanel->addText(res.str());

    res.seekp(std::ios::beg);
    res << "statPanel: " << _statheight << ", " << wd << ", " 
        << statstarty << ", 0";
    statPanel->addText(res.str());

    res.seekp(std::ios::beg);
    res << "conPanel: " << _conheight << ", " << wd << ", " 
        << constarty << ", 0";
    statPanel->addText(res.str());
}

void
TestHexApp::help()
{
    mainPanel->addText(" ");
    mainPanel->addText("This app tests a few basic features of libhexes.");
    mainPanel->addText("  /help  - displays this help");
    mainPanel->addText("  /bomb  - blow stuff up");
    mainPanel->addText("  /clear - clears the main window");
    mainPanel->addText("  /new   - create a new main panel (HexStack)");
    mainPanel->addText("  /kill  - remove the current main panel (HexStack)");
    mainPanel->addText("  /next  - switch to next main panel");
    mainPanel->addText("  /prev  - switch to previous main panel");
    mainPanel->addText("  /intro - show intro dialog window");
    mainPanel->addText("  /echo  - toggle the display of character input");
    mainPanel->addText("  /quit  - exits the app.");
    mainPanel->addText(" ");
    mainPanel->addText(" <CTRL>-w : performs a window command");
    mainPanel->addText("     + UP   = switch focus to the previous window");
    mainPanel->addText("     + DOWN = switch focus to the next window");
    mainPanel->addText(" ");
    mainPanel->addText(" When focus is on the main stack:");
    mainPanel->addText("     + 'n'      = create a new panel");
    mainPanel->addText("     + 'k'      = kill the current panel");
    mainPanel->addText("     + LEFT/RIGHT (or TAB) = switch panels");
    mainPanel->addText(" ");
    mainPanel->addText(" UP/DOWN Arrows = cycle command history or scrolls the window depending on the window focus");
    mainPanel->addText(" ");
}

void
TestHexApp::bomb()
{
    HexString hexstr;

    int clr = mainPanel->getTextColor();
    hexstr  = HexString("mainPanel text color set to ", HEX_RED, 0);
    hexstr.append(HexString::ToString(clr));
    hexstr.append(" attributes for HEX_NORMAL = ");
    hexstr.append(HexString::ToString(HEX_NORMAL));
    mainPanel->addText(hexstr);

    //mainPanel->setTextColor(HEX_BLUE_BLACK);
    mainPanel->addText(" ");
    mainPanel->addText(" ");
    mainPanel->addText("                     _-^--^=-_");
    mainPanel->addText("                _.-^^          -~_");
    mainPanel->addText("             _--                  --_");
    mainPanel->addText("            <                        >)");
    mainPanel->addText("            |                         |");
    mainPanel->addText("             \\._                   _./");
    mainPanel->addText("                ```--. . , ; .--'''");
    mainPanel->addText("                      | |   |");
    mainPanel->addText("                   .-=||  | |=-.");
    mainPanel->addText("                   `-=#$%&%$#=-'");
    mainPanel->addText("                      | ;  :|");
    mainPanel->addText("             _____.,-#%&$@%#&#~,._____");
    mainPanel->addText(" ");

    clr     = mainPanel->getTextColor();
    hexstr  = HexString("mainPanel text color set to ", HEX_RED, 0);
    hexstr.append(HexString::ToString(clr));

    mainPanel->addText(hexstr);
    //mainPanel->setTextColor(0);

    return;
}

void
TestHexApp::showIntro()
{
    std::string intro = "     Welcome to libhexes";

    HexDialog d("intro", HexString(intro, HEX_CYAN, HEX_BOLD));
    d.setDrawTitle(false);
    d.setTextColor(HEX_CYAN);
    d.setBorderColor(HEX_GREEN);
    d.echoResults(true, '*');  // example if we wanted to request info
    d.setMaxInput(12);
    d.addText("use /help (/?) for assistance\n \n", HEX_WHITE, HEX_NORMAL);
    d.addText("            <OK>", 0, HEX_BOLD);
    d.showDialog();
    if ( ! d.getResult().empty() )
        statPanel->addText(d.getResult());
}

void
TestHexApp::run()
{
    bool alarm      = false;
    int  conheight  = 3;
    int  statheight = (LINES * .33) - conheight;
    bool echo       = false;
    int  ch;

    LineInputHandler * cinput;

    this->setCursor(0);
    this->setBorderColor(HEX_MAGENTA);
    this->setBorderActiveColor(HEX_GREEN);

    mainStack = new HexStack("main-stack", LINES-statheight-1-conheight, COLS, 1, 0);
    this->addPanel(mainStack);
    mainPanel = mainStack->currentPanel();
    statPanel = this->createPanel("status", statheight, COLS, LINES-statheight-conheight, 0);
    conPanel  = this->createPanel("console", conheight, COLS, LINES-conheight, 0);

    if ( mainPanel ) {
        mainPanel->setWindowTitle(" Main [1] ", HEX_GREEN);
        mainPanel->enableScroll(true);
        mainPanel->setMaxLines(500);
    }

    statPanel->setWindowTitle(" Status ", HEX_GREEN);
    statPanel->enableScroll(true);
    statPanel->setTextColor(HEX_GREEN);

    conPanel->setDrawTitle(false);
    conPanel->setDrawBorder(false);
    conPanel->setInputHandler(new LineInputHandler());

    std::string top = "  TestHexApp  -  libhexes ";
    top.append(LIBHEXES_VERSION);
    this->print(0, 1, top, HEX_RED, HEX_BOLD);
    this->setFocus(conPanel);
    
    cinput = (LineInputHandler*) conPanel->getInputHandler();

    std::string  cmd, prompt;
    prompt = " > ";

    conPanel->addText(prompt);
    cinput->setPrefix(prompt);

    this->draw();
    this->showIntro();

    auto refreshMainTitles = [&]() {
        if ( ! mainStack )
            return;
        size_t total = mainStack->panelCount();
        for ( size_t i = 0; i < total; ++i )
        {
            HexPanel * p = mainStack->panelAt(i);
            if ( ! p )
                continue;
            std::ostringstream t;
            t << " Main [" << (i + 1) << "/" << total << "] ";
            p->setWindowTitle(t.str(), HEX_GREEN);
        }
    };

    refreshMainTitles();

    HexPanel * cur = this->getPanel();

    while ( ! alarm ) 
    {
        bool wcmd = false;
        this->draw();

        cmd = "";
        ch  = this->poll();

        while ( ! cinput->isReady() ) 
        {
            std::ostringstream  ostr;
            ostr << "ch = " << ch << "   '" << (char) ch << "'";

            bool consumed = false;

            if ( wcmd ) {              // window command reset
                wcmd = false;
                cinput->setParse(true);
                ostr << " <window command>";
                consumed = true;

                if ( ch == KEY_DOWN )
                    cur = this->setFocusNext();
                else if ( ch == KEY_UP )
                    cur = this->setFocusPrev();
            }

            if ( ch == HEX_KEY_WINDOW ) { // window command detect
                wcmd = true;
                cinput->setParse(false);
            }

            if ( consumed ) {
                this->draw();
                ch = this->poll();
                continue;
            }

            if ( cur != conPanel ) {  // alt input?
                if ( ch == KEY_UP ) {
                    if ( mainStack && cur == mainStack && mainPanel )
                        mainPanel->scrollUp();
                    else
                        cur->scrollUp();
                } else if ( ch == KEY_DOWN ) {
                    if ( mainStack && cur == mainStack && mainPanel )
                        mainPanel->scrollDown();
                    else
                        cur->scrollDown();
                } else if ( mainStack && cur == mainStack ) {
                    if ( ch == 'n' ) {
                        mainPanel = mainStack->createPanel();
                        if ( mainPanel ) {
                            mainPanel->enableScroll(true);
                            mainPanel->setMaxLines(500);
                            mainPanel->addText("-- created new panel --");
                            refreshMainTitles();
                        }
                    } else if ( ch == 'k' ) {
                        if ( mainStack->killCurrentPanel() ) {
                            mainPanel = mainStack->currentPanel();
                            refreshMainTitles();
                        }
                    } else if ( ch == KEY_RIGHT || ch == '\t' ) {
                        mainPanel = mainStack->nextPanel();
                        refreshMainTitles();
                    } else if ( ch == KEY_LEFT ) {
                        mainPanel = mainStack->prevPanel();
                        refreshMainTitles();
                    }
                }
            } else if ( echo ) {
                // switch for asciionly? - if ( ch >= 32 && ch < 128 )
                mainPanel->addText(ostr.str());
            }

            this->draw();

            ch = this->poll();
        }

        cmd = cinput->getLine();
        conPanel->setText(prompt);

        if ( ! cmd.empty() )
            statPanel->addText(cmd);

        if ( cmd.compare("/quit") == 0 ) {
            alarm = true;
        } else if ( cmd.compare("/clear") == 0 ) {
            mainPanel->clear();
        } else if ( cmd.compare("/new") == 0 || cmd.compare("/newpanel") == 0 ) {
            if ( mainStack ) {
                mainPanel = mainStack->createPanel();
                if ( mainPanel ) {
                    mainPanel->enableScroll(true);
                    mainPanel->setMaxLines(500);
                    mainPanel->addText("-- created new panel --");
                    refreshMainTitles();
                }
            }
        } else if ( cmd.compare("/next") == 0 ) {
            if ( mainStack ) {
                mainPanel = mainStack->nextPanel();
                refreshMainTitles();
            }
        } else if ( cmd.compare("/prev") == 0 ) {
            if ( mainStack ) {
                mainPanel = mainStack->prevPanel();
                refreshMainTitles();
            }
        } else if ( cmd.compare("/kill") == 0 ) {
            if ( mainStack ) {
                if ( mainStack->killCurrentPanel() ) {
                    mainPanel = mainStack->currentPanel();
                    refreshMainTitles();
                }
            }
        } else if ( cmd.compare("/bomb") == 0 ) {
            this->bomb();
        } else if ( cmd.compare("/echo") == 0 ) {
            if ( echo ) {
                echo = false;
                mainPanel->addText(" -- character echo disabled --");
            } else {
                echo = true;
                mainPanel->addText(" -- character echo enabled --");
            }
        } else if ( cmd.compare("/intro") == 0 ) {
            this->showIntro();
        } else if ( cmd.compare("/help") == 0 || cmd.compare("/?") == 0 ) {
            this->help();
        }

    }

    return;
}

} // namespace
