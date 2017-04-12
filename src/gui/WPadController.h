/****************************************************************************
 * Copyright (C) 2015 Dimok
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

#include "GuiController.h"
#include "dynamic_libs/padscore_functions.h"
#include "utils/logger.h"

class WPadController : public GuiController
{
public:
    //!Constructor
    WPadController(int channel)
        : GuiController(channel)
    {
        memset(&kpadData, 0, sizeof(kpadData));
    }

    //!Destructor
    virtual ~WPadController()  {}

    u32 remapWiiMoteButtons(u32 buttons)
    {
        u32 conv_buttons = 0;

        if(buttons & WPAD_BUTTON_LEFT)
            conv_buttons |= GuiTrigger::BUTTON_LEFT;

        if(buttons & WPAD_BUTTON_RIGHT)
            conv_buttons |= GuiTrigger::BUTTON_RIGHT;

        if(buttons & WPAD_BUTTON_DOWN)
            conv_buttons |= GuiTrigger::BUTTON_DOWN;

        if(buttons & WPAD_BUTTON_UP)
            conv_buttons |= GuiTrigger::BUTTON_UP;

        if(buttons & WPAD_BUTTON_PLUS)
            conv_buttons |= GuiTrigger::BUTTON_PLUS;

        if(buttons & WPAD_BUTTON_2)
            conv_buttons |= GuiTrigger::BUTTON_2;

        if(buttons & WPAD_BUTTON_1)
            conv_buttons |= GuiTrigger::BUTTON_1;

        if(buttons & WPAD_BUTTON_B)
            conv_buttons |= GuiTrigger::BUTTON_B;

        if(buttons & WPAD_BUTTON_A)
            conv_buttons |= GuiTrigger::BUTTON_A;

        if(buttons & WPAD_BUTTON_MINUS)
            conv_buttons |= GuiTrigger::BUTTON_MINUS;

        if(buttons & WPAD_BUTTON_Z)
            conv_buttons |= GuiTrigger::BUTTON_Z;

        if(buttons & WPAD_BUTTON_C)
            conv_buttons |= GuiTrigger::BUTTON_C;

        if(buttons & WPAD_BUTTON_HOME)
            conv_buttons |= GuiTrigger::BUTTON_HOME;

        return conv_buttons;
    }
    u32 remapClassicButtons(u32 buttons)
    {
        u32 conv_buttons = 0;

        if(buttons & WPAD_CLASSIC_BUTTON_LEFT)
            conv_buttons |= GuiTrigger::BUTTON_LEFT;

        if(buttons & WPAD_CLASSIC_BUTTON_RIGHT)
            conv_buttons |= GuiTrigger::BUTTON_RIGHT;

        if(buttons & WPAD_CLASSIC_BUTTON_DOWN)
            conv_buttons |= GuiTrigger::BUTTON_DOWN;

        if(buttons & WPAD_CLASSIC_BUTTON_UP)
            conv_buttons |= GuiTrigger::BUTTON_UP;

        if(buttons & WPAD_CLASSIC_BUTTON_PLUS)
            conv_buttons |= GuiTrigger::BUTTON_PLUS;

        if(buttons & WPAD_CLASSIC_BUTTON_X)
            conv_buttons |= GuiTrigger::BUTTON_X;

        if(buttons & WPAD_CLASSIC_BUTTON_Y)
            conv_buttons |= GuiTrigger::BUTTON_Y;

        if(buttons & WPAD_CLASSIC_BUTTON_B)
            conv_buttons |= GuiTrigger::BUTTON_B;

        if(buttons & WPAD_CLASSIC_BUTTON_A)
            conv_buttons |= GuiTrigger::BUTTON_A;

        if(buttons & WPAD_CLASSIC_BUTTON_MINUS)
            conv_buttons |= GuiTrigger::BUTTON_MINUS;

        if(buttons & WPAD_CLASSIC_BUTTON_HOME)
            conv_buttons |= GuiTrigger::BUTTON_HOME;

        if(buttons & WPAD_CLASSIC_BUTTON_ZR)
            conv_buttons |= GuiTrigger::BUTTON_ZR;

        if(buttons & WPAD_CLASSIC_BUTTON_ZL)
            conv_buttons |= GuiTrigger::BUTTON_ZL;

        if(buttons & WPAD_CLASSIC_BUTTON_R)
            conv_buttons |= GuiTrigger::BUTTON_R;

        if(buttons & WPAD_CLASSIC_BUTTON_L)
            conv_buttons |= GuiTrigger::BUTTON_L;

        return conv_buttons;
    }
    bool update(int width, int height) {
        lastData = data;

        u32 controller_type;

        //! check if the controller is connected
        if(WPADProbe(chanIdx-1, &controller_type) != 0)
            return false;

        KPADRead(chanIdx-1, &kpadData, 1);

        if(kpadData.device_type == 0 || kpadData.device_type == 5) {//wiiremote
            data.validCursor = false;
            data.buttons_r = remapWiiMoteButtons(kpadData.btns_r);
            data.buttons_h = remapWiiMoteButtons(kpadData.btns_h);
            data.buttons_d = remapWiiMoteButtons(kpadData.btns_d);
            //wii remote so we use pointer data
            data.validPointer = (kpadData.pos_valid == 1 || kpadData.pos_valid == 2) && (kpadData.pos_x >= -1.0f && kpadData.pos_x <= 1.0f) && (kpadData.pos_y >= -1.0f && kpadData.pos_y <= 1.0f);
            //! calculate the screen offsets if pointer is valid else leave old value
            if(data.validPointer) {
                data.x = (width >> 1) * kpadData.pos_x;
                data.y = (height >> 1) * (-kpadData.pos_y);

                if(kpadData.angle_y > 0.0f)
                    data.pointerAngle = (-kpadData.angle_x + 1.0f) * 0.5f * 180.0f;
                else
                    data.pointerAngle = (kpadData.angle_x + 1.0f) * 0.5f * 180.0f - 180.0f;
            }
            data.lstick_x = 0.0f;
            data.lstick_y = 0.0f;
            data.rstick_x = 0.0f;
            data.rstick_y = 0.0f;
        } else if(kpadData.device_type == 1 || kpadData.device_type == 6){//nunchuck
            data.buttons_r = remapWiiMoteButtons(kpadData.btns_r);
            data.buttons_h = remapWiiMoteButtons(kpadData.btns_h);
            data.buttons_d = remapWiiMoteButtons(kpadData.btns_d);
            //nunchuck so we use joystick data
            //deadzone calculations
            //x axis
            bool moveXAxis = ((kpadData.nunchuck.stick_x >= -1.0f && kpadData.nunchuck.stick_x <= -0.2f) || (kpadData.nunchuck.stick_x <= 1.0f && kpadData.nunchuck.stick_x >= 0.2f));
            if(moveXAxis) {
                if(lastData.x) {
                    data.x = lastData.x + (kpadData.nunchuck.stick_x * POINTER_ACCELERATION);
                } else {
                    //pointer hasnt been activated yet
                    data.x = 0 + (kpadData.nunchuck.stick_x * POINTER_ACCELERATION);
                }
                data.validCursor = true;
                if(data.x > (width >> 1)) {
                    data.x = (width >> 1);
                } else if(data.x < -(width >> 1)) {
                    data.x = -(width >> 1);
                }
                data.pointerAngle = 0;
            }
            //y axis
            bool moveYAxis = ((kpadData.nunchuck.stick_y >= -1.0f && kpadData.nunchuck.stick_y <= -0.2f) || (kpadData.nunchuck.stick_y <= 1.0f && kpadData.nunchuck.stick_y >= 0.2f));
            if(moveYAxis) {
                if(lastData.y) {
                    data.y = lastData.y + (kpadData.nunchuck.stick_y * POINTER_ACCELERATION);
                } else {
                    //pointer hasnt been activated yet
                    data.y = 0 + (kpadData.nunchuck.stick_y * POINTER_ACCELERATION);
                }
                data.validCursor = true;
                if(data.y > (height >> 1)) {
                    data.y = (height >> 1);
                } else if(data.y < -(height >> 1)) {
                    data.y = -(height >> 1);
                }
                data.pointerAngle = 0;
            }
            data.lstick_x = kpadData.nunchuck.stick_x;
            data.lstick_y = kpadData.nunchuck.stick_y;
            data.rstick_x = 0.0f;
            data.rstick_y = 0.0f;
        } else if(kpadData.device_type == 2 || kpadData.device_type == 7 || kpadData.device_type == 31 || kpadData.device_type == 22){//classic and pro
            data.buttons_r = remapClassicButtons(kpadData.classic.btns_r);
            data.buttons_h = remapClassicButtons(kpadData.classic.btns_h);
            data.buttons_d = remapClassicButtons(kpadData.classic.btns_d);
            //classic/pro so we use left joystick data
            //deadzone calculations
            //x axis
            bool moveXAxis = ((kpadData.classic.lstick_x >= -1.0f && kpadData.classic.lstick_x <= -0.2f) || (kpadData.classic.lstick_x <= 1.0f && kpadData.classic.lstick_x >= 0.2f));
            if(moveXAxis) {
                if(lastData.x) {
                    data.x = lastData.x + (kpadData.classic.lstick_x * POINTER_ACCELERATION);
                } else {
                    //pointer hasnt been activated yet
                    data.x = 0 + (kpadData.classic.lstick_x * POINTER_ACCELERATION);
                }
                data.validCursor = true;
                if(data.x > (width >> 1)) {
                    data.x = (width >> 1);
                } else if(data.x < -(width >> 1)) {
                    data.x = -(width >> 1);
                }
                data.pointerAngle = 0;
            }
            //y axis
            bool moveYAxis = ((kpadData.classic.lstick_y >= -1.0f && kpadData.classic.lstick_y <= -0.2f) || (kpadData.classic.lstick_y <= 1.0f && kpadData.classic.lstick_y >= 0.2f));
            if(moveYAxis) {
                if(lastData.y) {
                    data.y = lastData.y + (kpadData.classic.lstick_y * POINTER_ACCELERATION);
                } else {
                    //pointer hasnt been activated yet
                    data.y = 0 + (kpadData.classic.lstick_y * POINTER_ACCELERATION);
                }
                data.validCursor = true;
                if(data.y > (height >> 1)) {
                    data.y = (height >> 1);
                } else if(data.y < -(height >> 1)) {
                    data.y = -(height >> 1);
                }
                data.pointerAngle = 0;
            }
            data.lstick_x = kpadData.classic.lstick_x;
            data.lstick_y = kpadData.classic.lstick_y;
            data.rstick_x = kpadData.classic.rstick_x;
            data.rstick_y = kpadData.classic.rstick_y;
        }
        return true;
    }

private:
    KPADData kpadData;
    u32 lastButtons;
};