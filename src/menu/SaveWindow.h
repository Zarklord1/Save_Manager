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
    void saveButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);
    void abcSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);
    void titleIdSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);
    void nandSizeSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);
    void usbSizeSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);
    void nandFilterButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);
    void usbFilterButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger);

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
        std::string long_name;
        std::string nandPath;
        std::string usbPath;
        s64 nandSize;
        s64 usbSize;
        bool saveButtonSelected;
    } saveButton;

    void draw(CVideo *v);
private:
	void getSaveButtons();
	int isOnNand(const char * titleid);
	std::string convertSize(u64 size);
    int width, height;

    GuiSound *buttonClickSound;

    //container textures
    GuiImageData * saveBoxData;
    GuiImageData * saveButtonBoxData;
    GuiImage saveBox;
    GuiImage saveButtonBox;
    GuiFrame saveBoxFrame;
    //button textures
    GuiImageData * selectedSaveButtonData;
    GuiImageData * unselectedSaveButtonData;
    //scrollbar
    GuiImageData * scrollbarLineData;
    GuiImageData * scrollbarBoxData;
    GuiImageData * arrowDownData;
    GuiImageData * arrowUpData;
    GuiImage scrollbarLine;
    GuiImage scrollbarBox;
    GuiImage arrowDown;
    GuiImage arrowUp;
    GuiButton scrollbarBoxButton;
    GuiButton arrowDownButton;
    GuiButton arrowUpButton;
    Scrollbar saveScrollbar;
    //button hiders
    GuiImageData * bottomButtonHiderData;
    GuiImageData * topButtonHiderData;
    GuiImage bottomButtonHider;
    GuiImage topButtonHider;
    //sorting/filtering
    GuiImageData * sortFilterButtonData;
    GuiImageData * abcNoSortData;
    GuiImageData * abcSortDownData;
    GuiImageData * abcSortUpData;
    GuiImageData * titleIdNoSortData;
    GuiImageData * titleIdSortDownData;
    GuiImageData * titleIdSortUpData;
    GuiImageData * nandSizeNoSortData;
    GuiImageData * nandSizeSortDownData;
    GuiImageData * nandSizeSortUpData;
    GuiImageData * onNandTrueData;
    GuiImageData * onNandFalseData;
    GuiImageData * onUsbTrueData;
    GuiImageData * onUsbFalseData;
    GuiImageData * usbSizeNoSortData;
    GuiImageData * usbSizeSortDownData;
    GuiImageData * usbSizeSortUpData;

    GuiImage abcSortImage;
    GuiImage titleIdSortImage;
    GuiImage nandSizeSortImage;
    GuiImage usbSizeSortImage;
    GuiImage nandFilterImage;
    GuiImage usbFilterImage;

    GuiImage abcNoSort;
    GuiImage abcSortDown;
    GuiImage abcSortUp;

    GuiImage titleIdNoSort;
    GuiImage titleIdSortDown;
    GuiImage titleIdSortUp;

    GuiImage nandSizeNoSort;
    GuiImage nandSizeSortDown;
    GuiImage nandSizeSortUp;

    GuiImage usbSizeNoSort;
    GuiImage usbSizeSortDown;
    GuiImage usbSizeSortUp;

    GuiImage onNandTrue;
    GuiImage onNandFalse;

    GuiImage onUsbTrue;
    GuiImage onUsbFalse;

    GuiButton abcSortButton;
    GuiButton titleIdSortButton;
    GuiButton nandSizeSortButton;
    GuiButton usbSizeSortButton;
    GuiButton nandFilterButton;
    GuiButton usbFilterButton;

    GuiText saveManagerVersionText;

    std::vector<saveButton> saveButtons;
    std::vector<saveButton*> saveButtonsNoSort;
    std::vector<saveButton*> saveButtonAbcSortUp;
    std::vector<saveButton*> saveButtonAbcSortDown;
    std::vector<saveButton*> saveButtonTitleIdSortUp;
    std::vector<saveButton*> saveButtonTitleIdSortDown;
    std::vector<saveButton*> saveButtonNandSizeSortUp;
    std::vector<saveButton*> saveButtonNandSizeSortDown;
    std::vector<saveButton*> saveButtonUsbSizeSortUp;
    std::vector<saveButton*> saveButtonUsbSizeSortDown;

    //buttonManagement
    GuiTrigger touchTrigger;
    GuiTrigger wpadTouchTrigger;
    //GuiTrigger scrollbarUpTrigger;
    //GuiTrigger scrollbarDownTrigger;
    //GuiTrigger selectBoxTrigger;

    u8 sortState;
    bool onNand;
    bool onUsb;
    u16 totalNandSaves;
    u16 totalUsbSaves;
    enum {
    	STATE_DEFAULT 			= 0x0,
    	STATE_ABCSORTUP 		= 0x1,
    	STATE_ABCSORTDOWN 		= 0x2,
    	STATE_TITLEIDSORTUP		= 0x4,
    	STATE_TITLEIDSORTDOWN	= 0x8,
    	STATE_NANDSIZESORTUP 	= 0x10,
    	STATE_NANDSIZESORTDOWN 	= 0x20,
    	STATE_USBSIZESORTUP 	= 0x40,
    	STATE_USBSIZESORTDOWN 	= 0x80
    };
};

bool AbcSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool AbcSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool TitleIdSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool TitleIdSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool NandSizeSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool NandSizeSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool UsbSizeSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);
bool UsbSizeSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second);