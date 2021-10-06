/*
LT Timer
Copyright (c) 2021 Sora Arakawa
Licensed under the MIT License
*/

#include "DxLib.h"

typedef struct {
	int x1;
	int x2;
	int y1;
	int y2;
} Button;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	SetMainWindowText("LT Timer");
	SetWindowSize(320, 96);
	SetGraphMode(320, 96, 32);
	SetAlwaysRunFlag(TRUE);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowPos(GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	int Mode = 0;
	int StartButtonLength = 0;
	int Minute = 0;
	int Second = 0;
	int Frame = 0;
	int BGHandle, PartHandle, StartSoundHandle, PauseSoundHandle, MouseX, MouseY;
	int Count = 0;

	Button StartButton = {260, 293, 49, 81};

	BGHandle = LoadGraph("bg_ja.png");
	PartHandle = LoadGraph("part_ja.png");
	StartSoundHandle = LoadSoundMem("start.wav");
	PauseSoundHandle = LoadSoundMem("pause.wav");

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			GetMousePoint(&MouseX, &MouseY);
			StartButtonLength++;
			if (MouseX >= StartButton.x1 && MouseX <= StartButton.x2 && MouseY >= StartButton.y1 && MouseY <= StartButton.y2) {
				if (StartButtonLength == 1) {
					Mode++;
					Count = 15;
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

		Count++;
		if (Count >= 60) {
			Count = 0;
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawGraph(0, 0, BGHandle, FALSE);
		if (Mode == 0) {
			DrawRectGraph(18, 26, 0, 0, 64, 16, PartHandle, TRUE);
		} else if (Mode == 1 || Mode == 2) {
			DrawRectGraph(18, 50, 0, 16, 64, 16, PartHandle, TRUE);
		}
		DrawRectGraph(152, 53, 0, 32, 14, 14, PartHandle, TRUE);
		DrawRectGraph(233, 53, 14, 32, 14, 14, PartHandle, TRUE);

		if (Mode == 2) {
			int Opacity;
			if (Count == 0) {
				Opacity = 128;
			} else if (Count > 0 && Count < 30) {
				Opacity = 255;
			} else if (Count == 30) {
				Opacity = 128;
			} else if (Count > 30 && Count < 60) {
				Opacity = 0;
			}
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Opacity);
		} else {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		DrawRectGraph(96, 24, 64, 0, 26, 44, PartHandle, TRUE);
		DrawRectGraph(124, 24, 64, 0, 26, 44, PartHandle, TRUE);
		DrawRectGraph(177, 24, 64, 0, 26, 44, PartHandle, TRUE);
		DrawRectGraph(205, 24, 64, 0, 26, 44, PartHandle, TRUE);
		ScreenFlip();
	}
	DeleteGraph(BGHandle);
	DxLib_End();
	return 0;
}