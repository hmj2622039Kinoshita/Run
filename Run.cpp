#include"DxLib.h"
#include"Run.h" // ヘッダーファイル

// 定数定義
const int WIDTH = 1280, HEIGHT = 768; // ウィンドウの幅と高さのピクセル数
const int FPS = 60; // フレームレート
const int chipSize = 64; // マップチップのサイズ
const int chipRow = 18; // マップチップ画像の一列に並んでる画像数

// ゲーム内で使用する変数、配列
int timer = 0; // タイマー
int scene = TITLE;
int imgClo, imgTre, imgSol; // 背景画像
int imgPlayer[4]; // プレイヤ画像
int chipImage; // マップチップ画像
int pitch = 1; // マップチップ画像と画像の空白
int step = chipSize + pitch; // 空白の影響を考慮するための変数
int mapWIDTH = 20, mapHEIGHT = 12; // マップの横幅と縦幅（チップ数）

enum{TITLE,PLAY1,PLAY2,PLAY3,CLEAR,OVER}; // シーン
enum Chip { BL = 29, DH = 61, GG = 114, GH, GQ = 124, JF = 167, JL, JM, JN, RI = 314 }; // mapchipの横縦

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
	{RI,RI,RI,RI,RI,JL,JL,RI,RI,RI,JL,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,RI,RI,RI,RI,RI,RI,JL,JL,RI,RI,RI,RI,RI,RI,RI,RI,RI},
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

		switch (scene) // タイトル画面
		{
		case TITLE:
			ScrollBG(0);
			Title();
			break;

		case PLAY1:
			ScrollBG(1); // 背景のスクロール
			DrawMapChip(); // マップチップ
			MovePlayer(); // プレイヤの操作
			Gravity(); // 重力
			break;

		case CLEAR:
			Result();
			break;

		case OVER:
			Result();
			break;

		}

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
	player.sizeX = 64; // プレイヤのXサイズ
	player.sizeY = 128; // プレイヤのYサイズ
	player.jumpState = true; // ジャンプできる状態か
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
		player.jumpState = true; // ジャンプできない状態
	}
	if (player.jumpState == false) // 走ってる状態
	{
		if(player.vx > 0) { DrawGraph(player.x, player.y, imgPlayer[(timer / 8) % 2], true); } // 右向きの画像
		else { DrawTurnGraph(player.x, player.y, imgPlayer[(timer / 8) % 2], true); } // 左向き画像
	}
	else // ジャンプ状態
	{
		if(player.vx > 0) { DrawGraph(player.x, player.y, imgPlayer[2], true); } // 右向き
		else { DrawTurnGraph(player.x, player.y, imgPlayer[2], true); } // 左向き
	}
	player.x += player.vx;
	CollisionX(); // 左右方向の当たり判定
	if (player.x < -18) {player.x = -18;}
	if (player.x > 1170) {player.x = 1170;}
}

// 重力
void Gravity(void)
{
	player.vy += player.gravity; // 常に重力がかかる(速さに)
	player.y += player.vy;
	CollisionY(); // 上下方向の当たり判定
}

// マップチップとプレイヤのX軸方向の当たり判定
void CollisionX(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			if (mapChipList[i][j] == RI) continue; // 空白の時当たり判定無し

			int playerCenterX = player.x + player.sizeX / 2; // プレイヤの中心X座標
			int playerCenterY = player.y + player.sizeY / 2; // プレイヤの中心Y座標
			int chipCenterX = j * chipSize + chipSize / 2; // チップの中心X座標
			int chipCenterY = i * chipSize + chipSize / 2; // チップの中心Y座標
			int dx = abs(playerCenterX - chipCenterX); // x座標の中心間距離
			int dy = abs(playerCenterY - chipCenterY); // y座標の中心間距離

			if (dx < (player.sizeX + chipSize) / 2 && dy < (player.sizeY + chipSize) / 2) // x,yの中心間距離がプレイヤとchipのx,yの長さの半分より小さいとき
			{
				int x = (player.sizeX + chipSize) / 2 - dx; // x方向に重なっている長さ
				if (player.vx > 0) { player.x -= x; } // プレイヤが左側
				else if (player.vx < 0) { player.x += x; } // プレイヤが右側
				player.vx = 0; // 衝突したら進む量０にする
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
			if (mapChipList[i][j] == RI) continue;

			int playerCenterX = player.x + player.sizeX / 2;
			int playerCenterY = player.y + player.sizeY / 2; 
			int chipCenterX = j * chipSize + chipSize / 2; 
			int chipCenterY = i * chipSize + chipSize / 2;
			int dx = abs(playerCenterX - chipCenterX);
			int dy = abs(playerCenterY - chipCenterY);

			if (dx < (player.sizeX + chipSize) / 2 && dy < (player.sizeY + chipSize) / 2)
			{
				int y = (player.sizeY + chipSize) / 2 - dy; // y方向に重なっている長さ
				if (player.vy > 0) // 落下しているとき
				{
					player.y -= y;
					player.jumpState = false; // ジャンプできない状態
				}
				else if (player.vy < 0) { player.y += y; } // 下から上に移動してるとき
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

// タイトル
void Title(void)
{
	if (CheckHitKey(KEY_INPUT_1))
	{
		scene = PLAY1;
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		scene = PLAY2;
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		scene = PLAY3;
	}
}

// PLAY画面
void Play(void)
{

}

// RESULT画面
void Result(void)
{

}