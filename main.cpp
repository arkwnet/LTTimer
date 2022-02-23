/*
Lightning Talk Timer
Copyright (c) 2021,2022 Sora Arakawa
Licensed under the MIT License
*/

#include "DxLib.h"
#include "Windows.h"

class Fps {
	int mStartTime;
	int mCount;
	float mFps;
	static const int N = 30;
	static const int FPS = 30;

	public:
		Fps() {
			mStartTime = 0;
			mCount = 0;
			mFps = 0;
		}

		bool Update() {
			if (mCount == 0) {
				mStartTime = GetNowCount();
			}
			if (mCount == N) {
				int t = GetNowCount();
				mFps = 1000.f / ((t - mStartTime) / (float)N);
				mCount = 0;
				mStartTime = t;
			}
			mCount++;
			return true;
		}

		void Draw() {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawFormatString(10, 10, GetColor(255, 255, 255), L"%.1f fps", mFps);
		}

		void Wait() {
			int tookTime = GetNowCount() - mStartTime;
			int waitTime = mCount * 1000 / FPS - tookTime;
			if (waitTime > 0) {
				Sleep(waitTime);
			}
		}

		int Get() {
			return FPS;
		}
};

typedef struct {
	int x1;
	int x2;
	int y1;
	int y2;
} Button;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8) ;
	ChangeWindowMode(TRUE);
	SetMainWindowText(L"LTタイマー");
	SetWindowSize(320, 96);
	SetGraphMode(320, 96, 32);
	SetAlwaysRunFlag(TRUE);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowPos(GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	bool ShowFPS = false;
	bool ShowCurrentDirectory = false;

	int Mode = 0;
	int StartButtonLength = 0;
	int Minute = 0;
	int Second = 0;
	int SetMinute;
	int TimerCount = 0;
	int Frame = 0;
	int BGHandle, PartHandle, StartSoundHandle, PauseSoundHandle, EndSoundHandle, MouseX, MouseY;
	int Count = 0;
	bool IsCountDown = true;

	TCHAR CurrentDirectory[255];
	GetCurrentDirectory(255, CurrentDirectory);

	Button StartButton = {260, 293, 49, 81};
	Button UpButton = {286, 313, 5, 27};
	Button DownButton = {286, 313, 30, 52};
	Fps fps;

	BGHandle = LoadGraph(L"bg_ja.png");
	PartHandle = LoadGraph(L"part_ja.png");
	StartSoundHandle = LoadSoundMem(L"start.wav");
	PauseSoundHandle = LoadSoundMem(L"pause.wav");
	EndSoundHandle = LoadSoundMem(L"end.wav");

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		fps.Update();
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			GetMousePoint(&MouseX, &MouseY);
			StartButtonLength++;
			if (MouseX >= StartButton.x1 && MouseX <= StartButton.x2 && MouseY >= StartButton.y1 && MouseY <= StartButton.y2) {
				if (StartButtonLength == (fps.Get() / 2)) {
					if (Mode == 0 || Mode == 2) {
						PlaySoundMem(StartSoundHandle, DX_PLAYTYPE_BACK);
						Minute = 0;
						Second = 0;
						Mode = 0;
					}
				}
			}
			if (Mode >= 0 && Mode <= 3) {
				if (MouseX >= UpButton.x1 && MouseX <= UpButton.x2 && MouseY >= UpButton.y1 && MouseY <= UpButton.y2) {
					if (StartButtonLength == 1) {
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
						if (Minute < 99) {
							Minute++;
						}
					}
				}
				if (MouseX >= DownButton.x1 && MouseX <= DownButton.x2 && MouseY >= DownButton.y1 && MouseY <= DownButton.y2) {
					if (StartButtonLength == 1) {
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
						if (Minute > 0) {
							Minute--;
						}
					}
				}
			}
		} else {
			GetMousePoint(&MouseX, &MouseY);
			if (MouseX >= StartButton.x1 && MouseX <= StartButton.x2 && MouseY >= StartButton.y1 && MouseY <= StartButton.y2) {
				if (StartButtonLength >= 1 && StartButtonLength < fps.Get() / 2) {
					Mode++;
					Count = 10;
					if (Mode == 1) {
						PlaySoundMem(StartSoundHandle, DX_PLAYTYPE_BACK);
						SetMinute = Minute;
						if (Minute == 0) {
							IsCountDown = false;
						} else {
							IsCountDown = true;
						}
					}
					if (Mode == 2) {
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
					}
					if (Mode == 3) {
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
						Mode = 1;
					}
					if (Mode == 5) {
						StopSoundMem(EndSoundHandle);
						PlaySoundMem(PauseSoundHandle, DX_PLAYTYPE_BACK);
						Minute = SetMinute;
						Second = 0;
						Mode = 0;
					}
				}
			}
			StartButtonLength = 0;
		}

		Count++;
		if (Count >= fps.Get()) {
			Count = 0;
		}

		if (Mode == 1) {
			TimerCount++;
			if (TimerCount >= fps.Get()) {
				TimerCount = 0;
				if (IsCountDown == true) {
					Second--;
					if (Second < 0) {
						Second = 59;
						Minute--;
					}
					if (Minute < 0) {
						Minute = 0;
						Second = 0;
						Count = fps.Get() / 4;
						Mode = 4;
					}
				} else {
					Second++;
					if (Second >= 60) {
						Second = 0;
						Minute++;
					}
					if (Minute > 99) {
						Minute = 0;
					}
				}
			}
		}

		if (Mode == 4) {
			if (Count == fps.Get() / 4) {
				PlaySoundMem(EndSoundHandle, DX_PLAYTYPE_BACK);
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawGraph(0, 0, BGHandle, FALSE);
		if (Mode == 0) {
			DrawRectGraph(18, 26, 0, 0, 64, 16, PartHandle, TRUE);
		} else if (Mode == 1 || Mode == 2 || Mode == 4) {
			DrawRectGraph(18, 50, 0, 16, 64, 16, PartHandle, TRUE);
		}
		DrawRectGraph(152, 53, 0, 32, 14, 14, PartHandle, TRUE);
		DrawRectGraph(233, 53, 14, 32, 14, 14, PartHandle, TRUE);

		if (Mode == 2 || Mode == 4) {
			int Opacity = 0;
			if (Count == 0) {
				Opacity = 128;
			} else if (Count > 0 && Count < fps.Get() / 2) {
				Opacity = 255;
			} else if (Count == fps.Get() / 2) {
				Opacity = 128;
			}
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Opacity);
		} else {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		DrawRectGraph(96, 24, 64 + (((Minute - (Minute % 10)) / 10) * 32), 0, 26, 44, PartHandle, TRUE);
		DrawRectGraph(124, 24, 64 + ((Minute % 10) * 32), 0, 26, 44, PartHandle, TRUE);
		DrawRectGraph(177, 24, 64 + (((Second - (Second % 10)) / 10) * 32), 0, 26, 44, PartHandle, TRUE);
		DrawRectGraph(205, 24, 64 + ((Second % 10) * 32), 0, 26, 44, PartHandle, TRUE);
		if (ShowFPS == true) {
			fps.Draw();
		}
		if (ShowCurrentDirectory == true) {
			DrawString(10, 10, CurrentDirectory, GetColor(255, 255, 255));
		}
		ScreenFlip();
		fps.Wait();
	}

	DeleteGraph(BGHandle);
	DxLib_End();
	return 0;
}