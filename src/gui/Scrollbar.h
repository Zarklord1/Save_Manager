/***************************************************************************
 * Copyright (C) 2011
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#pragma once

#include "gui/GuiElement.h"
#include "gui/GuiButton.h"

#define SCROLLSPEED 200
#define MINYSCALE 0.l
#define FPS 60

class Scrollbar : public GuiElement, public sigslot::has_slots<>
{
	public:
		Scrollbar(GuiImage * scrollbarLineArg,GuiImage * scrollbarBoxArg, GuiButton * scrollbarBoxButtonArg, GuiButton * arrowDownArg, GuiButton * arrowUpArg, int PageSizeArg);
		virtual ~Scrollbar();

		void draw(CVideo * video);
		void update(GuiController * t);

		//! Signals
		sigslot::signal2<f32, f32> listChanged;
		//! Slots
		void SetPageSize(int size);
		void SetEntryCount(int cnt);
		void SetHeightOfEntries(f32 height);
	protected:
		void UpdateYScale();
		f32 getCurrentScrollYPixel(f32 scrollHeight);
		f32 getNewPosition(f32 scrollHeight, f32 currentYPixel);
		void setScrollboxPosition(f32 height, f32 position);
		void OnUpButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
		void OnDownButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
		void OnBoxButtonHold(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);

		f32 MinHeight;
		f32 MaxHeight;
		int EntryCount;
		int PageSize;
		f32 heightOfEntries;
		f32 currentPosition;

		GuiButton * arrowUp;
		GuiButton * arrowDown;
		GuiButton * scrollbarBoxButton;
		GuiImage * scrollbarLine;
		GuiImage * scrollbarBox;
};
