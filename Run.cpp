#include"DxLib.h"
#include"Run.h" // ヘッダーファイル

// 定数定義
const int WIDTH = 1280, HEIGHT = 768; // ウィンドウの幅と高さのピクセル数
const int FPS = 60; // フレームレート
const int chipSize = 64; // マップチップのサイズ
const int chipRow = 18; // マップチップ画像の一列に並んでる画像数

// ゲーム内で使用する変数、配列
int imgClo, imgTre, imgSol; // 背景画像
int imgPlayer[4]; // プレイヤ画像
int timer; // タイマー
int chipImage; // マップチップ画像
int pitch = 1; // マップチップ画像と画像の空白
int step = chipSize + pitch; // 空白の影響を考慮するための変数
int mapWIDTH = 20, mapHEIGHT = 12; // マップの横幅と縦幅（チップ数）
enum Chip{RI=314,JL=173,JM,JN}; // chipmapの横縦
int mapChipList[12][20] =
{
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,JL,JL,JL,RI,RI,RI},
	{RI,RI,RI,RI,RI,JL,JL,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{JM,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JN}
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
		DrawMapChip(); // マップチップ
		Gravity(); // 重力と仮当たり判定
		MovePlayer(); // プレイヤの操作
		//Collision(); // マップチップとの当たり判定

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
	// マップチップ画像
	chipImage = LoadGraph("Sprites/Tiles/mapChip.png");
}

// ゲーム開始時の初期値を代入する関数
void InitVariable(void)
{
	// プレイヤ構造体
	player.x = 576.0f; // プレイヤのXの位置
	player.y = 320.0f; // プレイヤのYの位置
	player.vx = 0.0f; // x方向の速さ
	player.vy = 0.0f; // y方向の速さ
	player.speed = 4.0f; // 移動速度（走る）
	player.jumpPower = 15.0f; // 初速度
	player.gravity = 0.6f; // 重力（常にかかる）
	player.size = 128; // プレイヤのサイズ
	player.jumpState = true; // 仮 ジャンプできる状態か
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
	// 背景中
		DrawGraph(treX, 256, imgTre, false);
		DrawGraph(treX + 256, 256, imgTre, false);
		DrawGraph(treX + 512, 256, imgTre, false); 
		DrawGraph(treX + 768, 256, imgTre, false);
		DrawGraph(treX + 1024, 256, imgTre, false);
		DrawGraph(treX + 1280, 256, imgTre, false);
	// 背景下
		DrawGraph(solX, 512, imgSol, false);
		DrawGraph(solX + 256, 512, imgSol, false);
		DrawGraph(solX + 512, 512, imgSol, false);
		DrawGraph(solX + 768, 512, imgSol, false);
		DrawGraph(solX + 1024, 512, imgSol, false);
		DrawGraph(solX + 1280, 512, imgSol, false);
}

// プレイヤの操作
void MovePlayer(void)
{
	if (CheckHitKey(KEY_INPUT_D)) { player.vx = -player.speed; } // Dキーで左方向の速さ
	else { player.vx = player.speed; }  // 何も入力がなかったら右方向の速さ
	if (CheckHitKey(KEY_INPUT_SPACE) && player.jumpState == false) // スペースキーが押されたらy方向に力を加える
	{
		player.vy = -player.jumpPower;
		player.jumpState = true;
	}
	if (player.jumpState == false) // 走ってる状態
	{
		if(player.vx > 0) { DrawGraph(player.x, player.y, imgPlayer[(timer / 8) % 2], true); }
		else { DrawTurnGraph(player.x, player.y, imgPlayer[(timer / 8) % 2], true); }
	}
	else // ジャンプ状態
	{
		if(player.vx > 0) { DrawGraph(player.x, player.y, imgPlayer[2], true); }
		else { DrawTurnGraph(player.x, player.y, imgPlayer[2], true); }
	}
	player.x += player.vx;
	CollisionX();
	if (player.x < -18) {player.x = -18;}
	if (player.x > 1170) {player.x = 1170;}
}

// 重力と仮当たり判定
void Gravity(void)
{
	player.vy += player.gravity; // 常に重力がかかる(速さに)
	player.y += player.vy;
	CollisionY();
}

// マップチップとプレイヤのX軸方向の当たり判定
void CollisionX(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			if (mapChipList[i][j] == RI) continue; // 空白の時当たり判定無し

			int playerCenterX = player.x + player.size / 2; // プレイヤの中心X座標
			int playerCenterY = player.y + player.size / 2; // プレイヤの中心Y座標
			int chipCenterX = j * chipSize + chipSize / 2; // チップの中心X座標
			int chipCenterY = i * chipSize + chipSize / 2; // チップの中心Y座標
			int dx = abs(playerCenterX - chipCenterX); // x座標の中心間距離
			int dy = abs(playerCenterY - chipCenterY); // y座標の中心間距離

			if (dx < (player.size + chipSize) / 2 && dy < (player.size + chipSize) / 2) // x,yの中心間距離がプレイヤとchipのx,yの長さを足した分より小さいとき
			{
				int x = (player.size + chipSize) / 2 - dx;
				if (player.vx > 0) { player.x -= x; }
				else if (player.vx < 0) { player.x += x; }
				player.vx = 0;
			}
		}
	}
}
// マップチップとプレイヤのY軸方向の当たり判定
void CollisionY(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			if (mapChipList[i][j] == RI) continue; // 空白の時当たり判定無し

			int playerCenterX = player.x + player.size / 2; // プレイヤの中心X座標
			int playerCenterY = player.y + player.size / 2; // プレイヤの中心Y座標
			int chipCenterX = j * chipSize + chipSize / 2; // チップの中心X座標
			int chipCenterY = i * chipSize + chipSize / 2; // チップの中心Y座標
			int dx = abs(playerCenterX - chipCenterX); // x座標の中心間距離
			int dy = abs(playerCenterY - chipCenterY); // y座標の中心間距離

			if (dx < (player.size + chipSize) / 2 && dy < (player.size + chipSize) / 2) // x,yの中心間距離がプレイヤとchipのx,yの長さを足した分より小さいとき
			{
				int y = (player.size + chipSize) / 2 - dy;
				if (player.vy > 0) 
				{
					player.y -= y;
					player.jumpState = false;
				}
				else if (player.vy < 0) { player.y += y; }
				player.vy = 0;
			}
		}
	}
}
				
// マップチップ
void DrawMapChip(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			int id = mapChipList[i][j];
			int sx = (id % chipRow) * step; // 縦列の指定
			int sy = (id / chipRow) * step; // 横列の指定
			// (描画するx座標,描画するy座標,切り取るx座標（左上）,切り取るy座標,描画する横サイズ,縦サイズ,描画するマップチップ画像,透過の有無)
			DrawRectGraph(j * chipSize, i * chipSize, sx, sy, chipSize, chipSize, chipImage, true);
		}
	}
}