/*
LT Timer
Copyright (c) 2021 Sora Arakawa
Licensed under the MIT License
*/

#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetMainWindowText("LT Timer");
	SetWindowSize(320, 96);
	SetGraphMode(320, 96, 32);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowPos(GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	int BGHandle;
	BGHandle = LoadGraph("bg.png");
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		DrawGraph(0, 0, BGHandle, TRUE);
		ScreenFlip();
	}
	DeleteGraph(BGHandle);
	DxLib_End();
	return 0;
}