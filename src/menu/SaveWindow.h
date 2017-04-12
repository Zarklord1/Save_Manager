/****************************************************************************
 * Copyright (C) 2015 Dimok
 * Modified by Maschell, 2016 for GX2_GUI_Template
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#pragma once

#include "gui/Gui.h"
#include "gui/GuiFrame.h"
#include "gui/Scrollbar.h"

class CVideo;

class SaveWindow : public GuiFrame, public sigslot::has_slots<>{
public:
    SaveWindow(int w, int h);
    virtual ~SaveWindow();

    void OnScrollbarListChange(f32 height, f32 position);
    void draw(CVideo *v);
private:
	void getSaves();
	int isOnNand(const char * titleid);
	std::string convertSize(u64 size);
    int width, height;

    GuiSound *buttonClickSound;

    GuiImageData * saveBoxData;
    GuiImageData * saveButtonBoxData;
    GuiImageData * selectedSaveButtonData;
    GuiImageData * unselectedSaveButtonData;
    GuiImageData * scrollbarLineData;
    GuiImageData * scrollbarBoxData;
    GuiImageData * arrowDownData;
    GuiImageData * arrowUpData;
    GuiImageData * bottomHiderData;
    GuiImageData * topHiderData;
    GuiImage saveBox;
    GuiImage saveButtonBox;
    GuiImage scrollbarLine;
    GuiImage scrollbarBox;
    GuiImage arrowDown;
    GuiImage arrowUp;
    GuiImage bottomHider;
    GuiImage topHider;
    GuiButton scrollbarBoxButton;
    GuiButton arrowDownButton;
    GuiButton arrowUpButton;
    GuiFrame saveBoxFrame;
    Scrollbar saveScrollbar;

    GuiText saveManagerVersionText;

    typedef struct {
        GuiImage *image;
        GuiImage *imageSelected;
    	GuiImage *iconTex;
        GuiImageData *iconTexData;
        GuiText *shortName;
        GuiText *titleID;
        GuiText *nandSizeText;
        GuiText *usbSizeText;
        GuiButton *button;
        std::string title_id;
        std::string nandPath;
        std::string usbPath;
        u64 nandSize;
        u64 usbSize;
    } saveButton;

    std::vector<saveButton> saveButtons;

    //buttonManagement
    GuiTrigger touchTrigger;
    GuiTrigger wpadTouchTrigger;
    //GuiTrigger scrollbarUpTrigger;
    //GuiTrigger scrollbarDownTrigger;
    //GuiTrigger selectBoxTrigger;
};