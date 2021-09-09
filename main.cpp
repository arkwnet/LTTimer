/*
LT Timer
Copyright (c) 2021 Sora Arakawa
Licensed under the MIT License
*/

#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	SetMainWindowText("LT Timer");
	SetWindowSize(320, 96);
	SetGraphMode(320, 96, 32);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowPos(GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	int Mode = 0;
	int StartButtonLength = 0;
	int BGHandle, PartHandle, StartSoundHandle, PauseSoundHandle, MouseX, MouseY;

	BGHandle = LoadGraph("bg_ja.png");
	PartHandle = LoadGraph("part_ja.png");
	StartSoundHandle = LoadSoundMem("start.wav");
	PauseSoundHandle = LoadSoundMem("pause.wav");

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			GetMousePoint(&MouseX, &MouseY);
			StartButtonLength++;
			if (MouseX >= 260 && MouseX <= 293 && MouseY >= 49 && MouseY <= 81) {
				if (StartButtonLength == 1) {
					Mode++;
					if (Mode == 1) {
						PlaySoundMem(StartSoundHandle, DX_PLAYTYPE_BACK);
					}
					if (Mode == 2) {
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
					}
					if (Mode == 3) {
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
						Mode = 1;
					}
				}
				if (StartButtonLength >= 30) {
					if (Mode == 1 || Mode == 2) {
						PlaySoundMem(StartSoundHandle, DX_PLAYTYPE_BACK);
						Mode = 0;
					}
				}
			}
		} else {
			StartButtonLength = 0;
		}

		DrawGraph(0, 0, BGHandle, FALSE);
		if (Mode == 0) {
			DrawRectGraph(18, 26, 0, 0, 64, 16, PartHandle, TRUE);
		} else if (Mode == 1 || Mode == 2) {
			DrawRectGraph(18, 50, 0, 16, 64, 16, PartHandle, TRUE);
		}
		ScreenFlip();
	}
	DeleteGraph(BGHandle);
	DxLib_End();
	return 0;
}