#pragma once

typedef struct _RecourceFile
{
	const char *filename;
	const u8   *DefaultFile;
	const u32  &DefaultFileSize;
	u8		   *CustomFile;
	u32		    CustomFileSize;
} RecourceFile;

extern const u8 player1_point_png[];
extern const u32 player1_point_png_size;

extern const u8 player2_point_png[];
extern const u32 player2_point_png_size;

extern const u8 player3_point_png[];
extern const u32 player3_point_png_size;

extern const u8 player4_point_png[];
extern const u32 player4_point_png_size;

extern const u8 font_ttf[];
extern const u32 font_ttf_size;

extern const u8 bottomButtonHider_png[];
extern const u32 bottomButtonHider_png_size;

extern const u8 saveBox_png[];
extern const u32 saveBox_png_size;

extern const u8 saveButtonBox_png[];
extern const u32 saveButtonBox_png_size;

extern const u8 selectedSaveButton_png[];
extern const u32 selectedSaveButton_png_size;

extern const u8 topButtonHider_png[];
extern const u32 topButtonHider_png_size;

extern const u8 unselectedSaveButton_png[];
extern const u32 unselectedSaveButton_png_size;

extern const u8 scrollBar_png[];
extern const u32 scrollBar_png_size;

extern const u8 scrollDown_png[];
extern const u32 scrollDown_png_size;

extern const u8 scrollLine_png[];
extern const u32 scrollLine_png_size;

extern const u8 scrollUp_png[];
extern const u32 scrollUp_png_size;

extern const u8 abcNoSort_png[];
extern const u32 abcNoSort_png_size;

extern const u8 abcSortDown_png[];
extern const u32 abcSortDown_png_size;

extern const u8 abcSortUp_png[];
extern const u32 abcSortUp_png_size;

extern const u8 nandSizeNoSort_png[];
extern const u32 nandSizeNoSort_png_size;

extern const u8 nandSizeSortDown_png[];
extern const u32 nandSizeSortDown_png_size;

extern const u8 nandSizeSortUp_png[];
extern const u32 nandSizeSortUp_png_size;

extern const u8 onNandFalse_png[];
extern const u32 onNandFalse_png_size;

extern const u8 onNandTrue_png[];
extern const u32 onNandTrue_png_size;

extern const u8 onUsbFalse_png[];
extern const u32 onUsbFalse_png_size;

extern const u8 onUsbTrue_png[];
extern const u32 onUsbTrue_png_size;

extern const u8 sortFilterButton_png[];
extern const u32 sortFilterButton_png_size;

extern const u8 titleIdNoSort_png[];
extern const u32 titleIdNoSort_png_size;

extern const u8 titleIdSortDown_png[];
extern const u32 titleIdSortDown_png_size;

extern const u8 titleIdSortUp_png[];
extern const u32 titleIdSortUp_png_size;

extern const u8 usbSizeNoSort_png[];
extern const u32 usbSizeNoSort_png_size;

extern const u8 usbSizeSortDown_png[];
extern const u32 usbSizeSortDown_png_size;

extern const u8 usbSizeSortUp_png[];
extern const u32 usbSizeSortUp_png_size;

extern const u8 bgMusic_ogg[];
extern const u32 bgMusic_ogg_size;

extern const u8 button_click_mp3[];
extern const u32 button_click_mp3_size;

static RecourceFile RecourceList[] =
{
	{"player1_point.png", player1_point_png, player1_point_png_size, NULL, 0},
	{"player2_point.png", player2_point_png, player2_point_png_size, NULL, 0},
	{"player3_point.png", player3_point_png, player3_point_png_size, NULL, 0},
	{"player4_point.png", player4_point_png, player4_point_png_size, NULL, 0},
	{"fonts/font.ttf", font_ttf, font_ttf_size, NULL, 0},
	{"save/bottomButtonHider.png", bottomButtonHider_png, bottomButtonHider_png_size, NULL, 0},
	{"save/saveBox.png", saveBox_png, saveBox_png_size, NULL, 0},
	{"save/saveButtonBox.png", saveButtonBox_png, saveButtonBox_png_size, NULL, 0},
	{"save/selectedSaveButton.png", selectedSaveButton_png, selectedSaveButton_png_size, NULL, 0},
	{"save/topButtonHider.png", topButtonHider_png, topButtonHider_png_size, NULL, 0},
	{"save/unselectedSaveButton.png", unselectedSaveButton_png, unselectedSaveButton_png_size, NULL, 0},
	{"save/scroller/scrollBar.png", scrollBar_png, scrollBar_png_size, NULL, 0},
	{"save/scroller/scrollDown.png", scrollDown_png, scrollDown_png_size, NULL, 0},
	{"save/scroller/scrollLine.png", scrollLine_png, scrollLine_png_size, NULL, 0},
	{"save/scroller/scrollUp.png", scrollUp_png, scrollUp_png_size, NULL, 0},
	{"save/sortfilter/abcNoSort.png", abcNoSort_png, abcNoSort_png_size, NULL, 0},
	{"save/sortfilter/abcSortDown.png", abcSortDown_png, abcSortDown_png_size, NULL, 0},
	{"save/sortfilter/abcSortUp.png", abcSortUp_png, abcSortUp_png_size, NULL, 0},
	{"save/sortfilter/nandSizeNoSort.png", nandSizeNoSort_png, nandSizeNoSort_png_size, NULL, 0},
	{"save/sortfilter/nandSizeSortDown.png", nandSizeSortDown_png, nandSizeSortDown_png_size, NULL, 0},
	{"save/sortfilter/nandSizeSortUp.png", nandSizeSortUp_png, nandSizeSortUp_png_size, NULL, 0},
	{"save/sortfilter/onNandFalse.png", onNandFalse_png, onNandFalse_png_size, NULL, 0},
	{"save/sortfilter/onNandTrue.png", onNandTrue_png, onNandTrue_png_size, NULL, 0},
	{"save/sortfilter/onUsbFalse.png", onUsbFalse_png, onUsbFalse_png_size, NULL, 0},
	{"save/sortfilter/onUsbTrue.png", onUsbTrue_png, onUsbTrue_png_size, NULL, 0},
	{"save/sortfilter/sortFilterButton.png", sortFilterButton_png, sortFilterButton_png_size, NULL, 0},
	{"save/sortfilter/titleIdNoSort.png", titleIdNoSort_png, titleIdNoSort_png_size, NULL, 0},
	{"save/sortfilter/titleIdSortDown.png", titleIdSortDown_png, titleIdSortDown_png_size, NULL, 0},
	{"save/sortfilter/titleIdSortUp.png", titleIdSortUp_png, titleIdSortUp_png_size, NULL, 0},
	{"save/sortfilter/usbSizeNoSort.png", usbSizeNoSort_png, usbSizeNoSort_png_size, NULL, 0},
	{"save/sortfilter/usbSizeSortDown.png", usbSizeSortDown_png, usbSizeSortDown_png_size, NULL, 0},
	{"save/sortfilter/usbSizeSortUp.png", usbSizeSortUp_png, usbSizeSortUp_png_size, NULL, 0},
	{"sounds/bgMusic.ogg", bgMusic_ogg, bgMusic_ogg_size, NULL, 0},
	{"sounds/button_click.mp3", button_click_mp3, button_click_mp3_size, NULL, 0},
	{NULL, NULL, 0, NULL, 0}
};