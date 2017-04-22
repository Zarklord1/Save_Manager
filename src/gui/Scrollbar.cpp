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
#include "Scrollbar.h"
#include "resources/Resources.h"
#include <math.h> //isnan 

Scrollbar::Scrollbar(GuiImage * scrollbarLineArg, GuiImage * scrollbarBoxArg, GuiButton * scrollbarBoxButtonArg, GuiButton * arrowDownArg, GuiButton * arrowUpArg, int PageSizeArg) {
	arrowUp = arrowUpArg;
	arrowDown = arrowDownArg;
	scrollbarLine = scrollbarLineArg;
	scrollbarBox = scrollbarBoxArg;
	scrollbarBoxButton = scrollbarBoxButtonArg;

	PageSize = PageSizeArg;
	EntryCount = 0;
	heightOfEntries = 0.0f;

	listChanged.connect(this, &Scrollbar::setScrollboxPosition);

	height = arrowUp->getHeight() + scrollbarLine->getHeight() + arrowDown->getHeight();
	width = scrollbarBox->getWidth();

	MaxHeight = height * 0.5f - (scrollbarBox ? ((scrollbarBox->getHeight() * scrollbarBox->getScaleY()) * 0.5f) : 0) - (arrowUp ? arrowUp->getHeight() : 0);
	MinHeight = -height * 0.5f + (scrollbarBox ? ((scrollbarBox->getHeight() * scrollbarBox->getScaleY()) * 0.5f) : 0) + (arrowDown ? arrowDown->getHeight() : 0);

	currentPosition = MaxHeight - MinHeight;

	scrollbarLine->setParent(this);
	scrollbarLine->setAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	scrollbarLine->setPosition(0, 0);

	arrowUp->setParent(this);
	arrowUp->setAlignment(ALIGN_CENTER | ALIGN_TOP);
	arrowUp->setPosition(0, 0);
	arrowUp->setHoldable(true);
	arrowUp->held.connect(this, &Scrollbar::OnUpButtonClick);

	arrowDown->setParent(this);
	arrowDown->setAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	arrowDown->setPosition(0, 0);
	arrowDown->setHoldable(true);
	arrowDown->held.connect(this, &Scrollbar::OnDownButtonClick);

	//set scrollbar hitbox to scrollbar lines height and scrollbar box's width
	scrollbarBoxButton->setSize(scrollbarBox->getWidth(), scrollbarLine->getHeight());
	scrollbarBoxButton->setParent(this);
	scrollbarBoxButton->setHoldable(true);
	scrollbarBoxButton->held.connect(this, &Scrollbar::OnBoxButtonHold);
	scrollbarBox->setPosition(0, currentPosition + MinHeight);
}

Scrollbar::~Scrollbar() {}

void Scrollbar::OnUpButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    if(EntryCount == 0)
        return;

    f32 defaultValue = 1.0f;
    currentPosition = LIMIT(currentPosition + ((defaultValue * SCROLLSPEED) / FPS), 0, MaxHeight - MinHeight);

	listChanged(MaxHeight - MinHeight, currentPosition);
}

void Scrollbar::OnDownButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    if(EntryCount == 0)
        return;

    f32 defaultValue = 1.0f;
    currentPosition = LIMIT(currentPosition - ((defaultValue * SCROLLSPEED) / FPS), 0, MaxHeight - MinHeight);

	listChanged(MaxHeight - MinHeight, currentPosition);
}

void Scrollbar::OnBoxButtonHold(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    if(EntryCount == 0)
        return;

	if(!controller->data.validPointer && !controller->data.validCursor)
		return;

	f32 y = controller->data.y - this->getCenterY();

	currentPosition = LIMIT(y - MinHeight, 0, MaxHeight - MinHeight);
	listChanged(MaxHeight - MinHeight, currentPosition);
}

void Scrollbar::Refresh() {
	listChanged(MaxHeight - MinHeight, currentPosition);
}

void Scrollbar::SetEntryCount(int cnt) {
	if(EntryCount == cnt)
		return;

	EntryCount = cnt;
	UpdateYScale();
	currentPosition = MaxHeight - MinHeight;
	listChanged(MaxHeight - MinHeight, currentPosition);
}

void Scrollbar::SetHeightOfEntries(f32 height) {
	if(heightOfEntries == height)
		return;

	heightOfEntries = height;
	UpdateYScale();
	currentPosition = MaxHeight - MinHeight;
	listChanged(MaxHeight - MinHeight, currentPosition);
}

void Scrollbar::setScrollboxPosition(f32 height, f32 position) {
    scrollbarBox->setPosition(0, position + MinHeight);
}

void Scrollbar::draw(CVideo * video) {
	scrollbarLine->draw(video);
	arrowUp->draw(video);
	arrowDown->draw(video);
	scrollbarBoxButton->draw(video);

	updateEffects();
}

void Scrollbar::update(GuiController * t) {
	if(this->isStateSet(STATE_DISABLED))
		return;

	arrowUp->update(t);
	arrowDown->update(t);
	scrollbarBoxButton->update(t);
}

void Scrollbar::UpdateYScale() {
	if(heightOfEntries == 0 || EntryCount == 0)
		return;
	f32 percentPerEntry = ((100.0f / (f32)EntryCount) * 0.01f);
	f32 scrollbarBoxYScale = LIMIT(1.0f - (percentPerEntry * (EntryCount - PageSize)), MINYSCALE, 1.0f);
	scrollbarBox->setScaleY(scrollbarBoxYScale);
	MaxHeight = height * 0.5f - (scrollbarBox ? ((scrollbarBox->getHeight() * scrollbarBox->getScaleY()) * 0.5f) : 0) - (arrowUp ? arrowUp->getHeight() : 0);
	MinHeight = -height * 0.5f + (scrollbarBox ? ((scrollbarBox->getHeight() * scrollbarBox->getScaleY()) * 0.5f) : 0) + (arrowDown ? arrowDown->getHeight() : 0);
}

f32 Scrollbar::getCurrentScrollYPixel(f32 scrollHeight) {
	f32 totalYPixelCount = ((f32)EntryCount * heightOfEntries);
	f32 returnVal = ((totalYPixelCount / scrollHeight) * currentPosition);
	return returnVal;
}

f32 Scrollbar::getNewPosition(f32 scrollHeight, f32 currentYPixel) {
	f32 totalYPixelCount = ((f32)EntryCount * heightOfEntries);
	f32 currentYToScrollNum = (totalYPixelCount / scrollHeight);
	f32 returnVal = (currentYPixel / currentYToScrollNum);
	return std::isnan(returnVal) ? scrollHeight : returnVal;
}

