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

#define MAX_FOLDERS_PER_PAGE 5

SaveWindow::SaveWindow(int w, int h)
    : GuiFrame(w, h)
    , width(w)
    , height(h)
    , buttonClickSound(Resources::GetSound("button_click.mp3"))
    , saveBoxData(Resources::GetImageData("save/saveBox.png"))
    , saveButtonBoxData(Resources::GetImageData("save/saveButtonBox.png"))
    , selectedSaveButtonData(Resources::GetImageData("save/selectedSaveButton.png"))
    , unselectedSaveButtonData(Resources::GetImageData("save/unselectedSaveButton.png"))
    , scrollbarLineData(Resources::GetImageData("save/scroller/scrollLine.png"))
    , scrollbarBoxData(Resources::GetImageData("save/scroller/scrollBar.png"))
    , arrowDownData(Resources::GetImageData("save/scroller/scrollDown.png"))
    , arrowUpData(Resources::GetImageData("save/scroller/scrollUp.png"))
    , bottomHiderData(Resources::GetImageData("save/bottomHider.png"))
    , topHiderData(Resources::GetImageData("save/topHider.png"))
    , saveBox(saveBoxData)
    , saveButtonBox(saveButtonBoxData)
    , scrollbarLine(scrollbarLineData)
    , scrollbarBox(scrollbarBoxData)
    , arrowDown(arrowDownData)
    , arrowUp(arrowUpData)
    , bottomHider(bottomHiderData)
    , topHider(topHiderData)
    , scrollbarBoxButton(scrollbarBoxData->getWidth(), scrollbarBoxData->getHeight())
    , arrowDownButton(arrowDownData->getWidth(), arrowDownData->getHeight())
    , arrowUpButton(arrowUpData->getWidth(), arrowUpData->getHeight())
    , saveScrollbar(&scrollbarLine, &scrollbarBox, &scrollbarBoxButton, &arrowDownButton, &arrowUpButton, MAX_FOLDERS_PER_PAGE)
    , saveBoxFrame(saveBox.getWidth(), saveBox.getHeight(), this)
    , saveManagerVersionText(SM_NAME " " SM_VERSION " By Zarklord", /*font size*/32, /*black*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
{
    saveBoxFrame.append(&saveBox);
    saveButtonBox.setPosition(-29, 0);
    saveBoxFrame.append(&saveButtonBox);

    getSaves();
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

    saveScrollbar.setPosition(169, 0);
    saveBoxFrame.append(&saveScrollbar);
    saveBoxFrame.setPosition(207,0);

    saveScrollbar.listChanged.connect(this, &SaveWindow::OnScrollbarListChange);
    saveScrollbar.SetEntryCount(saveButtons.size());
    saveScrollbar.SetHeightOfEntries(unselectedSaveButtonData->getHeight());

    topHider.setPosition(-29,305);
    bottomHider.setPosition(-29, -305);
    saveBoxFrame.append(&topHider);
    saveBoxFrame.append(&bottomHider);

    append(&saveBoxFrame);
}

SaveWindow::~SaveWindow() {
    for(u32 i = 0; i < saveButtons.size(); ++i)
    {
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
}

void SaveWindow::draw(CVideo *v) {
    GuiFrame::draw(v);
}

void SaveWindow::getSaves() {
    DirList nandDirList(NAND_SAVE_PATH, NULL, DirList::Dirs);
    DirList usbDirList(USB_SAVE_PATH, NULL, DirList::Dirs);
    for(int i = 0; i < nandDirList.GetFilecount(); i++) {

        if(nandDirList.GetFilename(i) == "." || nandDirList.GetFilename(i) == "..")
            continue;

        MetaXML * currentMeta = new MetaXML(((std::string)nandDirList.GetFilepath(i) + "/meta/meta.xml").c_str());

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
        saveButtons[current].nandPath = "";
        saveButtons[current].usbPath = "";
        saveButtons[current].nandSize = 0;
        saveButtons[current].usbSize = 0;

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
        saveButtons[current].button->setImageOver(saveButtons[current].imageSelected);
        saveButtons[current].button->setLabel(saveButtons[current].shortName, 0);
        saveButtons[current].button->setLabel(saveButtons[current].titleID, 1);
        saveButtons[current].button->setLabel(saveButtons[current].nandSizeText, 2);
        saveButtons[current].button->setLabel(saveButtons[current].usbSizeText, 3);
        saveButtons[current].button->setIcon(saveButtons[current].iconTex);
        saveButtons[current].button->setTrigger(&touchTrigger);
        saveButtons[current].button->setTrigger(&wpadTouchTrigger);
        saveButtons[current].button->setPosition(-29,0);
        saveButtons[current].button->setSoundClick(buttonClickSound);

        //nandPath
        saveButtons[current].nandPath = nandDirList.GetFilepath(i);

    }
    for(int i = 0; i < usbDirList.GetFilecount(); i++) {
        if(usbDirList.GetFilename(i) == "." || usbDirList.GetFilename(i) == "..")
            continue;

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
            saveButtons[current].nandPath = "";
            saveButtons[current].usbPath = "";
            saveButtons[current].nandSize = 0;
            saveButtons[current].usbSize = 0;

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
            saveButtons[current].button->setImageOver(saveButtons[current].imageSelected);
            saveButtons[current].button->setLabel(saveButtons[current].shortName, 0);
            saveButtons[current].button->setLabel(saveButtons[current].titleID, 1);
            saveButtons[current].button->setLabel(saveButtons[current].nandSizeText, 2);
            saveButtons[current].button->setLabel(saveButtons[current].usbSizeText, 3);
            saveButtons[current].button->setIcon(saveButtons[current].iconTex);
            saveButtons[current].button->setTrigger(&touchTrigger);
            saveButtons[current].button->setTrigger(&wpadTouchTrigger);
            saveButtons[current].button->setPosition(-29,0);
            saveButtons[current].button->setSoundClick(buttonClickSound);  

            //usbPath
            saveButtons[current].usbPath = usbDirList.GetFilepath(i);    
          }
    }
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

void SaveWindow::OnScrollbarListChange(f32 height, f32 position) {
    f32 reversedPos = (-position) + height;
    for(int i = 0; i < saveButtons.size(); i++) {
        f32 pixelAddY = (((saveButtons.size() - MAX_FOLDERS_PER_PAGE) * unselectedSaveButtonData->getHeight()) / height) * reversedPos;
        saveButtons[i].button->setPosition(saveButtons[i].button->getOffsetX(), (200 - (i*100)) + pixelAddY);
        if (saveButtons[i].button->getOffsetY() <= 299 && saveButtons[i].button->getOffsetY() >= -299) {
            if(!saveButtons[i].button->isVisible()) {
                saveButtons[i].button->setVisible(true);
                saveButtons[i].button->resetState();
            }
            //ensure the hitbox's are in the correct position
            if(saveButtons[i].button->getOffsetY() > 200) {
                saveButtons[i].button->setHitboxStart(saveButtons[i].button->getOffsetX(), 250 - ((-((saveButtons[i].button->getOffsetY() - 200) - 100)) / 2));
                saveButtons[i].button->setHitbox(saveButtons[i].image->getWidth(), -((saveButtons[i].button->getOffsetY() - 200) - 100));;
            } else if (saveButtons[i].button->getOffsetY() < -200) {
                saveButtons[i].button->setHitboxStart(saveButtons[i].button->getOffsetX(), -250 + (((saveButtons[i].button->getOffsetY() + 200) + 100) / 2));
                saveButtons[i].button->setHitbox(saveButtons[i].image->getWidth(), ((saveButtons[i].button->getOffsetY() + 200) + 100));
            } else {
                saveButtons[i].button->setHitbox(saveButtons[i].image->getWidth(), saveButtons[i].image->getHeight());
            }
        } else if((saveButtons[i].button->getOffsetY() >= 299 || saveButtons[i].button->getOffsetY() <= -299) && saveButtons[i].button->isVisible()){
            saveButtons[i].button->setVisible(false);
            saveButtons[i].button->setState(GuiElement::STATE_DISABLED);
        }
    }
}