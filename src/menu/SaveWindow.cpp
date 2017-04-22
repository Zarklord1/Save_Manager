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
#include "SaveWindow.h"
#include "utils/logger.h"
#include "common/common.h"
#include "utils/MetaXML.h"
#include "fs/DirList.h"
#include "fs/fs_utils.h"
#include "utils/logger.h"
#include <string.h>
#include <string>
#include <algorithm>

#define MAX_FOLDERS_PER_PAGE 5

//#define HIDEREXTRABUTTONS

SaveWindow::SaveWindow(int w, int h)
    : GuiFrame(w, h)
    , width(w)
    , height(h)
    , buttonClickSound(Resources::GetSound("sounds/button_click.mp3"))
    //containers
    , saveBoxData(Resources::GetImageData("save/saveBox.png"))
    , saveButtonBoxData(Resources::GetImageData("save/saveButtonBox.png"))
    , saveBox(saveBoxData)
    , saveButtonBox(saveButtonBoxData)
    , saveBoxFrame(saveBox.getWidth(), saveBox.getHeight(), this)
    //button textures
    , selectedSaveButtonData(Resources::GetImageData("save/selectedSaveButton.png"))
    , unselectedSaveButtonData(Resources::GetImageData("save/unselectedSaveButton.png"))
    //button hider
    , bottomButtonHiderData(Resources::GetImageData("save/bottomButtonHider.png"))
    , topButtonHiderData(Resources::GetImageData("save/topButtonHider.png"))
    , bottomButtonHider(bottomButtonHiderData)
    , topButtonHider(topButtonHiderData)
    //scrollbar
    , scrollbarLineData(Resources::GetImageData("save/scroller/scrollLine.png"))
    , scrollbarBoxData(Resources::GetImageData("save/scroller/scrollBar.png"))
    , arrowDownData(Resources::GetImageData("save/scroller/scrollDown.png"))
    , arrowUpData(Resources::GetImageData("save/scroller/scrollUp.png"))
    , scrollbarLine(scrollbarLineData)
    , scrollbarBox(scrollbarBoxData)
    , arrowDown(arrowDownData)
    , arrowUp(arrowUpData)
    , scrollbarBoxButton(scrollbarBoxData->getWidth(), scrollbarBoxData->getHeight())
    , arrowDownButton(arrowDownData->getWidth(), arrowDownData->getHeight())
    , arrowUpButton(arrowUpData->getWidth(), arrowUpData->getHeight())
    , saveScrollbar(&scrollbarLine, &scrollbarBox, &scrollbarBoxButton, &arrowDownButton, &arrowUpButton, MAX_FOLDERS_PER_PAGE)
    // version text TODO!!!
    , saveManagerVersionText(SM_NAME " " SM_VERSION " By Zarklord", /*font size*/32, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
    //button triggers
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
    //sorting/filtering
    , sortFilterButtonData(Resources::GetImageData("save/sortfilter/sortFilterButton.png"))
    , abcNoSortData(Resources::GetImageData("save/sortfilter/abcNoSort.png"))
    , abcSortDownData(Resources::GetImageData("save/sortfilter/abcSortDown.png"))
    , abcSortUpData(Resources::GetImageData("save/sortfilter/abcSortUp.png"))
    , titleIdNoSortData(Resources::GetImageData("save/sortfilter/titleIdNoSort.png"))
    , titleIdSortDownData(Resources::GetImageData("save/sortfilter/titleIdSortDown.png"))
    , titleIdSortUpData(Resources::GetImageData("save/sortfilter/titleIdSortUp.png"))
    , nandSizeNoSortData(Resources::GetImageData("save/sortfilter/nandSizeNoSort.png"))
    , nandSizeSortDownData(Resources::GetImageData("save/sortfilter/nandSizeSortDown.png"))
    , nandSizeSortUpData(Resources::GetImageData("save/sortfilter/nandSizeSortUp.png"))
    , onNandTrueData(Resources::GetImageData("save/sortfilter/onNandTrue.png"))
    , onNandFalseData(Resources::GetImageData("save/sortfilter/onNandFalse.png"))
    , onUsbTrueData(Resources::GetImageData("save/sortfilter/onUsbTrue.png"))
    , onUsbFalseData(Resources::GetImageData("save/sortfilter/onUsbFalse.png"))
    , usbSizeNoSortData(Resources::GetImageData("save/sortfilter/usbSizeNoSort.png"))
    , usbSizeSortDownData(Resources::GetImageData("save/sortfilter/usbSizeSortDown.png"))
    , usbSizeSortUpData(Resources::GetImageData("save/sortfilter/usbSizeSortUp.png"))
    , abcSortImage(sortFilterButtonData)
    , titleIdSortImage(sortFilterButtonData)
    , nandSizeSortImage(sortFilterButtonData)
    , usbSizeSortImage(sortFilterButtonData)
    , nandFilterImage(sortFilterButtonData)
    , usbFilterImage(sortFilterButtonData)
    , abcNoSort(abcNoSortData)
    , abcSortDown(abcSortDownData)
    , abcSortUp(abcSortUpData)
    , titleIdNoSort(titleIdNoSortData)
    , titleIdSortDown(titleIdSortDownData)
    , titleIdSortUp(titleIdSortUpData)
    , nandSizeNoSort(nandSizeNoSortData)
    , nandSizeSortDown(nandSizeSortDownData)
    , nandSizeSortUp(nandSizeSortUpData)
    , onNandTrue(onNandTrueData)
    , onNandFalse(onNandFalseData)
    , onUsbTrue(onUsbTrueData)
    , onUsbFalse(onUsbFalseData)
    , usbSizeNoSort(usbSizeNoSortData)
    , usbSizeSortDown(usbSizeSortDownData)
    , usbSizeSortUp(usbSizeSortUpData)
    , abcSortButton(sortFilterButtonData->getWidth(), sortFilterButtonData->getHeight())
    , titleIdSortButton(sortFilterButtonData->getWidth(), sortFilterButtonData->getHeight())
    , nandSizeSortButton(sortFilterButtonData->getWidth(), sortFilterButtonData->getHeight())
    , usbSizeSortButton(sortFilterButtonData->getWidth(), sortFilterButtonData->getHeight())
    , nandFilterButton(sortFilterButtonData->getWidth(), sortFilterButtonData->getHeight())
    , usbFilterButton(sortFilterButtonData->getWidth(), sortFilterButtonData->getHeight())
    , sortState(0)
    , onNand(true)
    , onUsb(true)
    , totalNandSaves(0)
    , totalUsbSaves(0)
{
    saveBoxFrame.append(&saveBox);
    saveButtonBox.setPosition(10, -26.5f);
    saveBoxFrame.append(&saveButtonBox);

    getSaveButtons();
    for (int i = 0; i < saveButtons.size(); i++) {
        saveBoxFrame.append(saveButtons[i].button);
    }

    //scrollbar
    arrowUpButton.setImage(&arrowUp);
    arrowUpButton.setTrigger(&touchTrigger, 0);
    arrowUpButton.setTrigger(&wpadTouchTrigger, 1);
    arrowUpButton.setSoundClick(buttonClickSound);
    arrowUpButton.setEffectGrow();

    arrowDownButton.setImage(&arrowDown);
    arrowDownButton.setTrigger(&touchTrigger, 0);
    arrowDownButton.setTrigger(&wpadTouchTrigger, 1);
    arrowDownButton.setSoundClick(buttonClickSound);
    arrowDownButton.setEffectGrow();

    scrollbarBoxButton.setImage(&scrollbarBox);
    scrollbarBoxButton.setTrigger(&touchTrigger, 0);
    scrollbarBoxButton.setTrigger(&wpadTouchTrigger, 1);

    saveScrollbar.setPosition(208, -27.5f);
    saveBoxFrame.append(&saveScrollbar);

    saveScrollbar.listChanged.connect(this, &SaveWindow::OnScrollbarListChange);
    saveScrollbar.SetEntryCount(saveButtons.size());
    saveScrollbar.SetHeightOfEntries(unselectedSaveButtonData->getHeight());

    //buttonHiders
#ifndef HIDEREXTRABUTTONS
    topButtonHider.setPosition(10,360 - (topButtonHider.getHeight() / 2));
    bottomButtonHider.setPosition(10, -360 + (bottomButtonHider.getHeight() / 2));
    saveBoxFrame.append(&topButtonHider);
    saveBoxFrame.append(&bottomButtonHider);
#endif
    //filter/sorting buttons
    //bg image
    abcSortButton.setImage(&abcSortImage);
    titleIdSortButton.setImage(&titleIdSortImage);
    nandSizeSortButton.setImage(&nandSizeSortImage);
    usbSizeSortButton.setImage(&usbSizeSortImage);
    nandFilterButton.setImage(&nandFilterImage);
    usbFilterButton.setImage(&usbFilterImage);

    //default icon
    abcSortButton.setIcon(&abcNoSort);
    titleIdSortButton.setIcon(&titleIdNoSort);
    nandSizeSortButton.setIcon(&nandSizeNoSort);
    usbSizeSortButton.setIcon(&usbSizeNoSort);
    nandFilterButton.setIcon(&onNandTrue);
    usbFilterButton.setIcon(&onUsbTrue);

    //positioning
    abcSortButton.setPosition(-198, 198 - (/* current button number*/ 0 * (abcSortImage.getHeight() + 20)));
    titleIdSortButton.setPosition(-198, 198 - (/* current button number*/ 1 * (abcSortImage.getHeight() + 20)));
    nandSizeSortButton.setPosition(-198, 198 - (/* current button number*/ 2 * (nandSizeSortImage.getHeight() + 20)));
    usbSizeSortButton.setPosition(-198, 198 - (/* current button number*/ 3 * (usbSizeSortImage.getHeight() + 20)));
    nandFilterButton.setPosition(-198, 198 - (/* current button number*/ 4 * (nandFilterImage.getHeight() + 20)));
    usbFilterButton.setPosition(-198, 198 - (/* current button number*/ 5 * (usbFilterImage.getHeight() + 20)));

    //adding button triggers and sound click
    abcSortButton.setTrigger(&touchTrigger, 0);
    abcSortButton.setTrigger(&wpadTouchTrigger, 1);
    abcSortButton.setSoundClick(buttonClickSound);

    titleIdSortButton.setTrigger(&touchTrigger, 0);
    titleIdSortButton.setTrigger(&wpadTouchTrigger, 1);
    titleIdSortButton.setSoundClick(buttonClickSound);
    
    nandSizeSortButton.setTrigger(&touchTrigger, 0);
    nandSizeSortButton.setTrigger(&wpadTouchTrigger, 1);
    nandSizeSortButton.setSoundClick(buttonClickSound);
    
    usbSizeSortButton.setTrigger(&touchTrigger, 0);
    usbSizeSortButton.setTrigger(&wpadTouchTrigger, 1);
    usbSizeSortButton.setSoundClick(buttonClickSound);
    
    nandFilterButton.setTrigger(&touchTrigger, 0);
    nandFilterButton.setTrigger(&wpadTouchTrigger, 1);
    nandFilterButton.setSoundClick(buttonClickSound);
    
    usbFilterButton.setTrigger(&touchTrigger, 0);
    usbFilterButton.setTrigger(&wpadTouchTrigger, 1);
    usbFilterButton.setSoundClick(buttonClickSound);

    //connecting clicked to methods
    abcSortButton.clicked.connect(this, &SaveWindow::abcSortButtonClicked);
    titleIdSortButton.clicked.connect(this, &SaveWindow::titleIdSortButtonClicked);
    nandSizeSortButton.clicked.connect(this, &SaveWindow::nandSizeSortButtonClicked);
    usbSizeSortButton.clicked.connect(this, &SaveWindow::usbSizeSortButtonClicked);
    nandFilterButton.clicked.connect(this, &SaveWindow::nandFilterButtonClicked);
    usbFilterButton.clicked.connect(this, &SaveWindow::usbFilterButtonClicked);

    //appending
    saveBoxFrame.append(&abcSortButton);
    saveBoxFrame.append(&titleIdSortButton);
    saveBoxFrame.append(&nandSizeSortButton);
    saveBoxFrame.append(&usbSizeSortButton);
    saveBoxFrame.append(&nandFilterButton);
    saveBoxFrame.append(&usbFilterButton);

    saveManagerVersionText.setAlignment(ALIGN_BOTTOM | ALIGN_RIGHT);
    saveManagerVersionText.setPosition(-30, 30);
    append(&saveManagerVersionText);
    
    saveBoxFrame.setPosition(0 + (saveBoxFrame.getWidth() / 2),0);
    append(&saveBoxFrame);
}

SaveWindow::~SaveWindow() {
    for(u32 i = 0; i < saveButtons.size(); ++i) {
        delete saveButtons[i].image;
        delete saveButtons[i].imageSelected;
        delete saveButtons[i].iconTex;
        delete saveButtons[i].iconTexData;
        delete saveButtons[i].shortName;
        delete saveButtons[i].titleID;
        delete saveButtons[i].nandSizeText;
        delete saveButtons[i].usbSizeText;
        delete saveButtons[i].button;
    }
    Resources::RemoveSound(buttonClickSound);
    Resources::RemoveImageData(saveBoxData);
    Resources::RemoveImageData(saveButtonBoxData);
    Resources::RemoveImageData(selectedSaveButtonData);
    Resources::RemoveImageData(unselectedSaveButtonData);
    Resources::RemoveImageData(scrollbarLineData);
    Resources::RemoveImageData(scrollbarBoxData);
    Resources::RemoveImageData(arrowDownData);
    Resources::RemoveImageData(arrowUpData);
    Resources::RemoveImageData(bottomButtonHiderData);
    Resources::RemoveImageData(topButtonHiderData);
    Resources::RemoveImageData(sortFilterButtonData);
    Resources::RemoveImageData(abcNoSortData);
    Resources::RemoveImageData(abcSortDownData);
    Resources::RemoveImageData(abcSortUpData);
    Resources::RemoveImageData(titleIdNoSortData);
    Resources::RemoveImageData(titleIdSortDownData);
    Resources::RemoveImageData(titleIdSortUpData);
    Resources::RemoveImageData(nandSizeNoSortData);
    Resources::RemoveImageData(nandSizeSortDownData);
    Resources::RemoveImageData(nandSizeSortUpData);
    Resources::RemoveImageData(onNandTrueData);
    Resources::RemoveImageData(onNandFalseData);
    Resources::RemoveImageData(onUsbTrueData);
    Resources::RemoveImageData(onUsbFalseData);
    Resources::RemoveImageData(usbSizeNoSortData);
    Resources::RemoveImageData(usbSizeSortDownData);
    Resources::RemoveImageData(usbSizeSortUpData);
}

void SaveWindow::draw(CVideo *v) {
    GuiFrame::draw(v);
}

void SaveWindow::getSaveButtons() {
    DirList nandDirList(NAND_SAVE_PATH, NULL, DirList::Dirs);
    DirList usbDirList(USB_SAVE_PATH, NULL, DirList::Dirs);
    for(int i = 0; i < nandDirList.GetFilecount(); i++) {
        

        if(nandDirList.GetFilename(i) == "." || nandDirList.GetFilename(i) == "..")
            continue;
        totalNandSaves++;
        MetaXML * currentMeta = new MetaXML((((std::string)nandDirList.GetFilepath(i)) + "/meta/meta.xml").c_str());
        
        int current = saveButtons.size();
        saveButtons.resize(saveButtons.size() + 1);
        
        //init all data
        saveButtons[current].image = new GuiImage(unselectedSaveButtonData);
        saveButtons[current].imageSelected = new GuiImage(selectedSaveButtonData);
        saveButtons[current].iconTex = NULL;
        saveButtons[current].iconTexData = NULL;
        saveButtons[current].shortName = new GuiText("", /*font size*/20, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        saveButtons[current].titleID = new GuiText("", /*font size*/18, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        saveButtons[current].nandSizeText = new GuiText("Nand: N/A", /*font size*/15, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        saveButtons[current].usbSizeText = new GuiText("Usb: N/A", /*font size*/15, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        saveButtons[current].button = new GuiButton(saveButtons[current].image->getWidth(), saveButtons[current].image->getHeight());
        saveButtons[current].title_id = "";
        saveButtons[current].long_name = "";
        saveButtons[current].nandPath = "";
        saveButtons[current].usbPath = "";
        saveButtons[current].nandSize = 0;
        saveButtons[current].usbSize = -1;
        saveButtons[current].saveButtonSelected = false;

        
        //iconTex.tga
        u8 * iconData = NULL;
        u32 iconDataSize = 0;

        LoadFileToMem(((std::string)nandDirList.GetFilepath(i) + "/meta/iconTex.tga").c_str(), &iconData, &iconDataSize);
        if(iconData != NULL) {
            saveButtons[current].iconTexData = new GuiImageData(iconData, iconDataSize);
            free(iconData);
            iconData = NULL;
        }
        
        saveButtons[current].iconTex = new GuiImage(saveButtons[current].iconTexData);
        saveButtons[current].iconTex->setScale(0.625f);
        saveButtons[current].iconTex->setPosition(-92,0);
        
        //name
        saveButtons[current].shortName->setText(currentMeta->GetShortName());
        saveButtons[current].shortName->setPosition(45, 27);
        saveButtons[current].shortName->setMaxWidth(160,GuiText::SCROLL_HORIZONTAL);
        saveButtons[current].long_name = currentMeta->GetLongName();
        
        //titleid
        saveButtons[current].titleID->setText(currentMeta->GetTitleID());
        saveButtons[current].titleID->setPosition(45, 0);
        saveButtons[current].title_id = currentMeta->GetTitleID();
        
        //nandsizeString
        saveButtons[current].nandSize = nandDirList.GetFoldersize(((std::string)nandDirList.GetFilepath(i) + "/user"));
        saveButtons[current].nandSizeText->setText(((std::string)"Nand: " + convertSize(saveButtons[current].nandSize)).c_str());
        saveButtons[current].nandSizeText->setPosition(-3, -25);
        
        //usbsizeString
        saveButtons[current].usbSizeText->setPosition(96, -25);
        
        //button
        saveButtons[current].button->setImage(saveButtons[current].image);
        saveButtons[current].button->setLabel(saveButtons[current].shortName, 0);
        saveButtons[current].button->setLabel(saveButtons[current].titleID, 1);
        saveButtons[current].button->setLabel(saveButtons[current].nandSizeText, 2);
        saveButtons[current].button->setLabel(saveButtons[current].usbSizeText, 3);
        saveButtons[current].button->setIcon(saveButtons[current].iconTex);
        saveButtons[current].button->setTrigger(&touchTrigger);
        saveButtons[current].button->setTrigger(&wpadTouchTrigger);
        saveButtons[current].button->setHoldable(false);
        saveButtons[current].button->setPosition(10,0);
        saveButtons[current].button->setSoundClick(buttonClickSound);

        saveButtons[current].button->clicked.connect(this, &SaveWindow::saveButtonClicked);
        
        //nandPath
        saveButtons[current].nandPath = nandDirList.GetFilepath(i);
        
    }
    for(int i = 0; i < usbDirList.GetFilecount(); i++) {
        if(usbDirList.GetFilename(i) == "." || usbDirList.GetFilename(i) == "..")
            continue;
        
        totalUsbSaves++;
        MetaXML * currentMeta = new MetaXML(((std::string)usbDirList.GetFilepath(i) + "/meta/meta.xml").c_str());
        int current = isOnNand(currentMeta->GetTitleID());
        if (current != -1) {
            //title is on nand aswell
            saveButtons[current].usbPath = usbDirList.GetFilepath(i);
            saveButtons[current].usbSize = usbDirList.GetFoldersize((saveButtons[current].usbPath + "/user"));
            saveButtons[current].usbSizeText->setText(((std::string)"Usb: " + convertSize(saveButtons[current].usbSize)).c_str());
            
        } else {

            current = saveButtons.size();
            saveButtons.resize(saveButtons.size() + 1);
            
            //init all data
            saveButtons[current].image = new GuiImage(unselectedSaveButtonData);
            saveButtons[current].imageSelected = new GuiImage(selectedSaveButtonData);
            saveButtons[current].iconTex = NULL;
            saveButtons[current].iconTexData = NULL;
            saveButtons[current].shortName = new GuiText("", /*font size*/20, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            saveButtons[current].titleID = new GuiText("", /*font size*/18, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            saveButtons[current].nandSizeText = new GuiText("Nand: N/A", /*font size*/15, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            saveButtons[current].usbSizeText = new GuiText("Usb: N/A", /*font size*/15, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            saveButtons[current].button = new GuiButton(saveButtons[current].image->getWidth(), saveButtons[current].image->getHeight());
            saveButtons[current].title_id = "";
            saveButtons[current].long_name = "";
            saveButtons[current].nandPath = "";
            saveButtons[current].usbPath = "";
            saveButtons[current].nandSize = -1;
            saveButtons[current].usbSize = 0;
            saveButtons[current].saveButtonSelected = false;
            
            //iconTex.tga
            u8 * iconData = NULL;
            u32 iconDataSize = 0;

            LoadFileToMem(((std::string)usbDirList.GetFilepath(i) + "/meta/iconTex.tga").c_str(), &iconData, &iconDataSize);
            if(iconData != NULL) {
                saveButtons[current].iconTexData = new GuiImageData(iconData, iconDataSize);
                free(iconData);
                iconData = NULL;
            }
            
            saveButtons[current].iconTex = new GuiImage(saveButtons[current].iconTexData);
            saveButtons[current].iconTex->setScale(0.625f);
            saveButtons[current].iconTex->setPosition(-92,0);
            
            //name
            saveButtons[current].shortName->setText(currentMeta->GetShortName());
            saveButtons[current].shortName->setPosition(45, 27);
            saveButtons[current].shortName->setMaxWidth(160,GuiText::SCROLL_HORIZONTAL);
            saveButtons[current].long_name = currentMeta->GetLongName();
            
            //titleid
            saveButtons[current].titleID->setText(currentMeta->GetTitleID());
            saveButtons[current].titleID->setPosition(45, 0);
            saveButtons[current].title_id = currentMeta->GetTitleID();
            
            //nandsizeString
            saveButtons[current].nandSizeText->setPosition(-3, -25);
            
            //usbsizeString
            saveButtons[current].usbSize = usbDirList.GetFoldersize(((std::string)usbDirList.GetFilepath(i) + "/user"));
            saveButtons[current].usbSizeText->setText(((std::string)"Usb: " + convertSize(saveButtons[current].usbSize)).c_str());
            saveButtons[current].usbSizeText->setPosition(96, -25);
        
            //button
            saveButtons[current].button->setImage(saveButtons[current].image);
            saveButtons[current].button->setLabel(saveButtons[current].shortName, 0);
            saveButtons[current].button->setLabel(saveButtons[current].titleID, 1);
            saveButtons[current].button->setLabel(saveButtons[current].nandSizeText, 2);
            saveButtons[current].button->setLabel(saveButtons[current].usbSizeText, 3);
            saveButtons[current].button->setIcon(saveButtons[current].iconTex);
            saveButtons[current].button->setTrigger(&touchTrigger);
            saveButtons[current].button->setTrigger(&wpadTouchTrigger);
            saveButtons[current].button->setHoldable(false);
            saveButtons[current].button->setPosition(10,0);
            saveButtons[current].button->setSoundClick(buttonClickSound);

            saveButtons[current].button->clicked.connect(this, &SaveWindow::saveButtonClicked);

            //usbPath
            saveButtons[current].usbPath = usbDirList.GetFilepath(i);  
        }
    }
    //created sorted versions of this
    saveButtonsNoSort.resize(saveButtons.size());
    for(int i = 0; i < saveButtons.size(); i++) {
        saveButtonsNoSort[i] = &(saveButtons[i]);
    }

    saveButtonAbcSortUp = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonAbcSortUp.begin(),saveButtonAbcSortUp.end(), AbcSortUpFuntion);

    saveButtonAbcSortDown = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonAbcSortDown.begin(),saveButtonAbcSortDown.end(), AbcSortDownFuntion);

    saveButtonTitleIdSortUp = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonTitleIdSortUp.begin(),saveButtonTitleIdSortUp.end(), TitleIdSortUpFuntion);

    saveButtonTitleIdSortDown = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonTitleIdSortDown.begin(),saveButtonTitleIdSortDown.end(), TitleIdSortDownFuntion);

    saveButtonNandSizeSortUp = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonNandSizeSortUp.begin(),saveButtonNandSizeSortUp.end(), NandSizeSortUpFuntion);

    saveButtonNandSizeSortDown = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonNandSizeSortDown.begin(),saveButtonNandSizeSortDown.end(), NandSizeSortDownFuntion);

    saveButtonUsbSizeSortUp = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonUsbSizeSortUp.begin(),saveButtonUsbSizeSortUp.end(), UsbSizeSortUpFuntion);

    saveButtonUsbSizeSortDown = std::vector<saveButton*>(saveButtonsNoSort.begin(),saveButtonsNoSort.end());
    std::sort(saveButtonUsbSizeSortDown.begin(),saveButtonUsbSizeSortDown.end(), UsbSizeSortDownFuntion);
}

std::string SaveWindow::convertSize(u64 size) {               
    static const char *SIZES[] = {"b", "kb", "mb", "gb"};
    int div = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        div++;
        size /= 1024;
    }

    char buffer[20];
    sprintf(buffer, "%llu%s", size, SIZES[div]);
    std::string result = buffer;
    return result;
}

int SaveWindow::isOnNand(const char * titleid) {
    for(int i = 0; i < saveButtons.size(); i++) {
        if(strcmp(saveButtons[i].title_id.c_str(),titleid) == 0) {
            return i;
        }
    }
    return -1;
}


#define BUTTONYSTART       173
#define BUTTONYEND         -227
#define BUTTONMAXHITBOXY   223
#define BUTTONMINHITBOXY   -277
#define MAXYOFFSET         273
#define MINYOFFSET         -327

void SaveWindow::OnScrollbarListChange(f32 height, f32 position) {
    std::vector<saveButton*> currentSaveButtonList;
    switch(sortState) {
        case STATE_DEFAULT:
            currentSaveButtonList = saveButtonsNoSort;
            break;
        case STATE_ABCSORTUP:
            currentSaveButtonList = saveButtonAbcSortUp;
            break;
        case STATE_ABCSORTDOWN:
            currentSaveButtonList = saveButtonAbcSortDown;
            break;
        case STATE_TITLEIDSORTUP:
            currentSaveButtonList = saveButtonTitleIdSortUp;
            break;
        case STATE_TITLEIDSORTDOWN:
            currentSaveButtonList = saveButtonTitleIdSortDown;
            break;
        case STATE_NANDSIZESORTUP:
            currentSaveButtonList = saveButtonNandSizeSortUp;
            break;
        case STATE_NANDSIZESORTDOWN:
            currentSaveButtonList = saveButtonNandSizeSortDown;
            break;
        case STATE_USBSIZESORTUP:
            currentSaveButtonList = saveButtonUsbSizeSortUp;
            break;
        case STATE_USBSIZESORTDOWN:
            currentSaveButtonList = saveButtonUsbSizeSortDown;
            break;
        default:
            currentSaveButtonList = saveButtonsNoSort;
            break;
    }
    f32 reversedPos = (-position) + height;
    f32 pixelAddY = 0;
    if (onNand && onUsb) {
        if (height == 0) {
            pixelAddY = 0;
        } else {
            pixelAddY = (((currentSaveButtonList.size() - MAX_FOLDERS_PER_PAGE) * unselectedSaveButtonData->getHeight()) / height) * reversedPos;
        }
    } else if (onNand && !onUsb) {
        if (height == 0) {
            pixelAddY = 0;
        } else {
            pixelAddY = (((totalNandSaves - MAX_FOLDERS_PER_PAGE) * unselectedSaveButtonData->getHeight()) / height) * reversedPos;
        }
    } else if (!onNand && onUsb) {
        if (height == 0) {
            pixelAddY = 0;
        } else {
            pixelAddY = (((totalUsbSaves - MAX_FOLDERS_PER_PAGE) * unselectedSaveButtonData->getHeight()) / height) * reversedPos;
        }
    }
    int current = 0;
    for(int i = 0; i < currentSaveButtonList.size(); i++) {
        if ((!onNand && currentSaveButtonList[i]->nandPath.length() != 0 && currentSaveButtonList[i]->usbPath.length() == 0) || 
            (!onUsb && currentSaveButtonList[i]->usbPath.length() != 0 && currentSaveButtonList[i]->nandPath.length() == 0)) {
            if (currentSaveButtonList[i]->button->isVisible()) {
                currentSaveButtonList[i]->button->setVisible(false);
                currentSaveButtonList[i]->button->setState(GuiElement::STATE_DISABLED);
            }
            continue;
        }
        currentSaveButtonList[i]->button->setPosition(currentSaveButtonList[i]->button->getOffsetX(), (BUTTONYSTART - (current*100)) + pixelAddY);
        if (currentSaveButtonList[i]->button->getOffsetY() < MAXYOFFSET && currentSaveButtonList[i]->button->getOffsetY() > MINYOFFSET) {
            if(!currentSaveButtonList[i]->button->isVisible()) {
                currentSaveButtonList[i]->button->setVisible(true);
                currentSaveButtonList[i]->button->resetState();
            }
            //ensure the hitbox's are in the correct position
            if(currentSaveButtonList[i]->button->getOffsetY() > BUTTONYSTART) {
                currentSaveButtonList[i]->button->setHitboxStart(currentSaveButtonList[i]->button->getOffsetX(), BUTTONMAXHITBOXY - ((-((currentSaveButtonList[i]->button->getOffsetY() - BUTTONYSTART) - 100)) / 2));
                currentSaveButtonList[i]->button->setHitbox(currentSaveButtonList[i]->image->getWidth(), -((currentSaveButtonList[i]->button->getOffsetY() - BUTTONYSTART) - 100));;
            } else if (currentSaveButtonList[i]->button->getOffsetY() < BUTTONYEND) {
                currentSaveButtonList[i]->button->setHitboxStart(currentSaveButtonList[i]->button->getOffsetX(), BUTTONMINHITBOXY + (((currentSaveButtonList[i]->button->getOffsetY() - BUTTONYEND) + 100) / 2));
                currentSaveButtonList[i]->button->setHitbox(currentSaveButtonList[i]->image->getWidth(), ((currentSaveButtonList[i]->button->getOffsetY() - BUTTONYEND) + 100));
            } else {
                currentSaveButtonList[i]->button->setHitbox(currentSaveButtonList[i]->image->getWidth(), currentSaveButtonList[i]->image->getHeight());
            }
        } else if((currentSaveButtonList[i]->button->getOffsetY() >= MAXYOFFSET || currentSaveButtonList[i]->button->getOffsetY() <= MINYOFFSET) && currentSaveButtonList[i]->button->isVisible()){
            currentSaveButtonList[i]->button->setVisible(false);
            currentSaveButtonList[i]->button->setState(GuiElement::STATE_DISABLED);
        }
        current++;
    }
}

void SaveWindow::saveButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    for(int i = 0; i < saveButtons.size(); i++) {
        if (saveButtons[i].button == clickedButton) {
            if (saveButtons[i].saveButtonSelected) {
                //load the next menu WIP!
                log_printf("selected button was reselected!\n");
            } else {
                //select this button
                saveButtons[i].button->setImage(saveButtons[i].imageSelected);
                saveButtons[i].saveButtonSelected = true;
            }
        } else {
            if (saveButtons[i].saveButtonSelected) {
                //deselect the old button
                saveButtons[i].button->setImage(saveButtons[i].image);
                saveButtons[i].saveButtonSelected = false;
            }
        }
    }
}

void SaveWindow::abcSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    if (sortState == STATE_ABCSORTUP) {
        sortState = STATE_ABCSORTDOWN;
        abcSortButton.setIcon(&abcSortDown);
    } else if (sortState == STATE_ABCSORTDOWN) {
        sortState = STATE_DEFAULT;
        abcSortButton.setIcon(&abcNoSort);
    } else {
        sortState = STATE_ABCSORTUP;
        abcSortButton.setIcon(&abcSortUp);
    }
    titleIdSortButton.setIcon(&titleIdNoSort);
    nandSizeSortButton.setIcon(&nandSizeNoSort);
    usbSizeSortButton.setIcon(&usbSizeNoSort);
    saveScrollbar.Refresh();
}

void SaveWindow::titleIdSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    if (sortState == STATE_TITLEIDSORTUP) {
        sortState = STATE_TITLEIDSORTDOWN;
        titleIdSortButton.setIcon(&titleIdSortDown);
    } else if (sortState == STATE_TITLEIDSORTDOWN) {
        sortState = STATE_DEFAULT;
        titleIdSortButton.setIcon(&titleIdNoSort);
    } else {
        sortState = STATE_TITLEIDSORTUP;
        titleIdSortButton.setIcon(&titleIdSortUp);
    }
    abcSortButton.setIcon(&abcNoSort);
    nandSizeSortButton.setIcon(&nandSizeNoSort);
    usbSizeSortButton.setIcon(&usbSizeNoSort);
    saveScrollbar.Refresh();
}

void SaveWindow::nandSizeSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    if (sortState == STATE_NANDSIZESORTUP) {
        sortState = STATE_NANDSIZESORTDOWN;
        nandSizeSortButton.setIcon(&nandSizeSortDown);
    } else if (sortState == STATE_NANDSIZESORTDOWN) {
        sortState = STATE_DEFAULT;
        nandSizeSortButton.setIcon(&nandSizeNoSort);
    } else {
        sortState = STATE_NANDSIZESORTUP;
        nandSizeSortButton.setIcon(&nandSizeSortUp);
    }
    abcSortButton.setIcon(&abcNoSort);
    titleIdSortButton.setIcon(&titleIdNoSort);
    usbSizeSortButton.setIcon(&usbSizeNoSort);
    saveScrollbar.Refresh();
}

void SaveWindow::usbSizeSortButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    if (sortState == STATE_USBSIZESORTUP) {
        sortState = STATE_USBSIZESORTDOWN;
        usbSizeSortButton.setIcon(&usbSizeSortDown);
    } else if (sortState == STATE_USBSIZESORTDOWN) {
        sortState = STATE_DEFAULT;
        usbSizeSortButton.setIcon(&usbSizeNoSort);
    } else {
        sortState = STATE_USBSIZESORTUP;
        usbSizeSortButton.setIcon(&usbSizeSortUp);
    }
    abcSortButton.setIcon(&abcNoSort);
    titleIdSortButton.setIcon(&titleIdNoSort);
    nandSizeSortButton.setIcon(&nandSizeNoSort);
    saveScrollbar.Refresh();
}

void SaveWindow::nandFilterButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    if (onNand && onUsb && totalUsbSaves > 0) {
        //hide nand only saves if totalUsbSaves is greater than 0
        onNand = false;
        clickedButton->setIcon(&onNandFalse);
        saveScrollbar.SetEntryCount(totalUsbSaves);
    } else if (onNand && !onUsb && totalUsbSaves > 0) {
        //we cant have both disabled so we disable this one and enable the other one...
        onNand = false;
        clickedButton->setIcon(&onNandFalse);
        onUsb = true;
        usbFilterButton.setIcon(&onUsbTrue);
        saveScrollbar.SetEntryCount(totalUsbSaves);
    } else if (!onNand && onUsb) {
        //unhide nand only saves
        onNand = true;
        clickedButton->setIcon(&onNandTrue);
        saveScrollbar.SetEntryCount(saveButtons.size());
    }
}

void SaveWindow::usbFilterButtonClicked(GuiButton * clickedButton, const GuiController * controller, GuiTrigger * trigger) {
    if (onNand && onUsb && totalNandSaves > 0) {
        //hide usb only saves if totalNandSaves is greater than 0
        onUsb = false;
        clickedButton->setIcon(&onUsbFalse);
        saveScrollbar.SetEntryCount(totalNandSaves);
    } else if (onUsb && !onNand && totalNandSaves > 0) {
        //we cant have both disabled so we disable this one and enable the other one...
        onUsb = false;
        clickedButton->setIcon(&onUsbFalse);
        onNand = true;
        nandFilterButton.setIcon(&onNandTrue);
        saveScrollbar.SetEntryCount(totalNandSaves);
    } else if (!onUsb && onNand) {
        //unhide usb only saves
        onUsb = true;
        clickedButton->setIcon(&onUsbTrue);
        saveScrollbar.SetEntryCount(saveButtons.size());
    }
}

bool AbcSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    return first->long_name < second->long_name;
}
bool AbcSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    return first->long_name > second->long_name;
}
bool TitleIdSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    std::string firstTiD = first->title_id;
    std::string secondTiD = second->title_id;
    firstTiD.replace(8, 1, "");
    secondTiD.replace(8, 1, "");
    return strtoll(firstTiD.c_str(), NULL, 16) < strtoll(secondTiD.c_str(), NULL, 16);
}
bool TitleIdSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    std::string firstTiD = first->title_id;
    std::string secondTiD = second->title_id;
    firstTiD.replace(8, 1, "");
    secondTiD.replace(8, 1, "");
    return strtoll(firstTiD.c_str(), NULL, 16) > strtoll(secondTiD.c_str(), NULL, 16);
}
bool NandSizeSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    return first->nandSize > second->nandSize;
}
bool NandSizeSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    if (first->nandSize == -1) return false;
    if (second->nandSize == -1) return true;
    return first->nandSize < second->nandSize;
}
bool UsbSizeSortUpFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    return first->usbSize > second->usbSize;
}
bool UsbSizeSortDownFuntion(SaveWindow::saveButton * first, SaveWindow::saveButton * second) {
    if (first->usbSize == -1) return false;
    if (second->usbSize == -1) return true;
    return first->usbSize < second->usbSize;
}