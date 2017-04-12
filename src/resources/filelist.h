#pragma once

typedef struct _RecourceFile
{
	const char *filename;
	const u8   *DefaultFile;
	const u32  &DefaultFileSize;
	u8		   *CustomFile;
	u32		    CustomFileSize;
} RecourceFile;

extern const u8 bgMusic_ogg[];
extern const u32 bgMusic_ogg_size;

extern const u8 button_click_mp3[];
extern const u32 button_click_mp3_size;

extern const u8 font_ttf[];
extern const u32 font_ttf_size;

extern const u8 player1_point_png[];
extern const u32 player1_point_png_size;

extern const u8 player2_point_png[];
extern const u32 player2_point_png_size;

extern const u8 player3_point_png[];
extern const u32 player3_point_png_size;

extern const u8 player4_point_png[];
extern const u32 player4_point_png_size;

extern const u8 saveBox_png[];
extern const u32 saveBox_png_size;

extern const u8 saveButtonBox_png[];
extern const u32 saveButtonBox_png_size;

extern const u8 selectedSaveButton_png[];
extern const u32 selectedSaveButton_png_size;

extern const u8 unselectedSaveButton_png[];
extern const u32 unselectedSaveButton_png_size;

extern const u8 bottomHider_png[];
extern const u32 bottomHider_png_size;

extern const u8 topHider_png[];
extern const u32 topHider_png_size;

extern const u8 scrollUp_png[];
extern const u32 scrollUp_png_size;

extern const u8 scrollDown_png[];
extern const u32 scrollDown_png_size;

extern const u8 scrollLine_png[];
extern const u32 scrollLine_png_size;

extern const u8 scrollBar_png[];
extern const u32 scrollBar_png_size;

static RecourceFile RecourceList[] =
{
	{"bgMusic.ogg", bgMusic_ogg, bgMusic_ogg_size, NULL, 0},
	{"button_click.mp3", button_click_mp3, button_click_mp3_size, NULL, 0},
	{"font.ttf", font_ttf, font_ttf_size, NULL, 0},
	{"player1_point.png", player1_point_png, player1_point_png_size, NULL, 0},
	{"player2_point.png", player2_point_png, player2_point_png_size, NULL, 0},
	{"player3_point.png", player3_point_png, player3_point_png_size, NULL, 0},
	{"player4_point.png", player4_point_png, player4_point_png_size, NULL, 0},
	{"save/saveBox.png", saveBox_png, saveBox_png_size, NULL, 0},
	{"save/saveButtonBox.png", saveButtonBox_png, saveButtonBox_png_size, NULL, 0},
	{"save/selectedSaveButton.png", selectedSaveButton_png, selectedSaveButton_png_size, NULL, 0},
	{"save/unselectedSaveButton.png", unselectedSaveButton_png, unselectedSaveButton_png_size, NULL, 0},
	{"save/bottomHider.png", bottomHider_png, bottomHider_png_size, NULL, 0},
	{"save/topHider.png", topHider_png, topHider_png_size, NULL, 0},
	{"save/scroller/scrollUp.png", scrollUp_png, scrollUp_png_size, NULL, 0},
	{"save/scroller/scrollDown.png", scrollDown_png, scrollDown_png_size, NULL, 0},
	{"save/scroller/scrollLine.png", scrollLine_png, scrollLine_png_size, NULL, 0},
	{"save/scroller/scrollBar.png", scrollBar_png, scrollBar_png_size, NULL, 0},
	{NULL, NULL, 0, NULL, 0}
};