#include"DxLib.h"
#include"Run.h" // ヘッダーファイル

// 定数定義
const int WIDTH = 1280, HEIGHT = 768; // ウィンドウの幅と高さのピクセル数
const int FPS = 60; // フレームレート
const int chipWIDTH = 64, chipHEIGHT = 64; // マップチップの横幅と縦幅


// ゲーム内で使用する変数、配列
int imgClo, imgTre, imgSol; // 背景画像
int imgPlayer[4]; // プレイヤ画像
// int imgGra1, imgGra2, imgGra3, imgSoi1, imgSoi2, imgSoi3; // 草と土ブロック画像
int mapChipHandle; // マップチップ画像
int timer;
int ground = 600; // 仮地面
int mapWIDTH = WIDTH / chipWIDTH, mapHEIGHT = HEIGHT / chipHEIGHT; // マップの横幅と縦幅（チップ数）
int mapChipList[20][12] // [20],[12]
{
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
	{298,298,298,298,298,165,165,165,298,298,298,298},
};

struct OBJECT player; // プレイヤの構造体変数

// グローバル関数
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("Run Game"); // ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true); // ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1; // ライブラリ初期化、エラーが起きたら終了
	SetBackgroundColor(0, 0, 0); // 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏背景にする

	InitGame(); // 初期化用関数
	InitVariable(); // ゲーム開始時の位置




	while (1) // メインループ
	{
		timer++;
		ClearDrawScreen(); // 画面をクリアする

		// ゲームの骨組みの処理
		ScrollBG(1); // 背景のスクロール
		MovePlayer(); // プレイヤの操作
		Gravity(); // 重力と仮当たり判定
		Jump(); // ジャンプ
		DrawMapChip(); // マップチップ

		ScreenFlip(); // 裏画面の内容を表画面に反映させる
		WaitTimer(1000 / FPS); // 一定時間待つ
		if (ProcessMessage() == -1) break; // windowsから情報を受け取りエラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
	}

	DxLib_End(); // DXライブラリの使用終了処理
	return 0; // ソフトの終了
}

// 自作関数記述
// 初期化用の変数（画像、サウンドの読み込み）
void InitGame(void)
{
	// 背景画像
	imgClo = LoadGraph("Sprites/Backgrounds/cloud.png"); // 背景上
	imgTre = LoadGraph("Sprites/Backgrounds/tree.png"); // 背景中
	imgSol = LoadGraph("Sprites/Backgrounds/solid.png"); // 背景下
	// プレイヤ画像  ここには初期化じゃなくて代入扱いになるから一個ずつするしかない
	imgPlayer[0] = LoadGraph("Sprites/Characters/run1.png");
	imgPlayer[1] = LoadGraph("Sprites/Characters/run2.png");
	imgPlayer[2] = LoadGraph("Sprites/Characters/jump1.png");
	imgPlayer[3] = LoadGraph("Sprites/Characters/flont.png");
	// マップチップ画像
	mapChipHandle = LoadGraph("Sprites/Tiles/mapChip.png");
	//// 地面画像
	//imgGra1 = LoadGraph("Sprites/Tils/grassLeft.png");
	//imgGra2 = LoadGraph("Sprites/Tils/grass.png");
	//imgGra3 = LoadGraph("Sprites/Tils/grassRight.png");
	//imgSoi1 = LoadGraph("Sprites/Tils/soilLeft.png");
	//imgSoi2 = LoadGraph("Sprites/Tils/soil.png");
	//imgSoi3 = LoadGraph("Sprites/Tils/soilRight.png");
}

// 背景スクロール
void ScrollBG(int spd)
{
	static int cloX, treX, solX; // スクロール位置を管理する変数
	cloX = (cloX - spd) % 256; // 背景上
		DrawGraph(cloX, 0, imgClo, false);
		DrawGraph(cloX + 256, 0, imgClo, false);
		DrawGraph(cloX + 512, 0, imgClo, false);
		DrawGraph(cloX + 768, 0, imgClo, false);
		DrawGraph(cloX + 1024, 0, imgClo, false);
		DrawGraph(cloX + 1280, 0, imgClo, false);
	//treX = (treX - spd) % 256; // 背景中
		DrawGraph(treX, 256, imgTre, false);
		DrawGraph(treX + 256, 256, imgTre, false);
		DrawGraph(treX + 512, 256, imgTre, false);
		DrawGraph(treX + 768, 256, imgTre, false);
		DrawGraph(treX + 1024, 256, imgTre, false);
		DrawGraph(treX + 1280, 256, imgTre, false);
	//solX = (solX - spd) % 256; // 背景下
		DrawGraph(solX, 512, imgSol, false);
		DrawGraph(solX + 256, 512, imgSol, false);
		DrawGraph(solX + 512, 512, imgSol, false);
		DrawGraph(solX + 768, 512, imgSol, false);
		DrawGraph(solX + 1024, 512, imgSol, false);
		DrawGraph(solX + 1280, 512, imgSol, false);
}

// ゲーム開始時の初期値を代入する関数
void InitVariable(void)
{
	player.x = 576.0f; // プレイヤのXの位置
	player.y = 320.0f; // プレイヤのYの位置
	player.vx = 4.0f; // 移動速度
	player.vy = 0.0f; // y方向の速さ
	player.jumpPower = 12.0f; // 初速度
	player.gravity = 0.6f; // 重力（常にかかる）
	player.jumpState = true;
}

// プレイヤの操作
void MovePlayer(void)
{
	if (CheckHitKey(KEY_INPUT_D)) // Dキー押すと左に走る、離すと右に走る
	{
		player.x -= player.vx;
		DrawTurnGraph(player.x, player.y, imgPlayer[(timer / 8) % 2], true);
		if (player.x < -18)
		{
			player.x = -18;
		}
	}
	else
	{
		player.x += player.vx;
		DrawGraph(player.x, player.y, imgPlayer[(timer / 8) % 2], true);
		if (player.x > 1170)
		{
			player.x = 1170;
		}
	}
}

// 重力と仮当たり判定
void Gravity(void)
{
	player.vy += player.gravity;
	player.y += player.vy;
	if (player.y >= ground)
	{
		player.y = ground;
		player.jumpState = false;
	}
}

// ジャンプ
void Jump(void)
{
	if (CheckHitKey(KEY_INPUT_SPACE) && player.jumpState == false)
	{
		player.vy = -player.jumpPower;
		player.jumpState = true;
	}
}

// マップチップ
void DrawMapChip(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			int num = mapChipList[i][j];
			int sx = num * 64;
			int dx = j * chipWIDTH;
			int dy = i * chipHEIGHT;
			DrawRectGraph(dx, dy, sx, 0, chipWIDTH, chipHEIGHT, mapChipHandle, true);
		}
	}
}
