#include"DxLib.h"
#include"Run.h" // ヘッダーファイル

// 定数定義
const int WIDTH = 1280, HEIGHT = 768; // ウィンドウの幅と高さのピクセル数
const int FPS = 60; // フレームレート
const int chipSize = 64; // マップチップのサイズ
const int chipRow = 18; // マップチップ画像の一列に並んでる画像数

// 構造体
struct OBJECT player; // プレイヤの構造体変数
struct ITEM spiky1; // とげのギミック
struct ITEM spiky2;
struct ITEM spiky3;
struct ITEM key; // 鍵 PLAY1
struct ITEM keyDoor; // 鍵付きドア
struct ITEM fakeDoor; // 偽物のドア PLAY1
struct ITEM torch1; // 松明
struct ITEM torch2;
struct ITEM trueDoor; // 本物ドア PLAY2
struct ITEM falseDoor1; // 偽物ドア PLAY2
struct ITEM falseDoor2;
struct ITEM falseDoor3;
struct ITEM saw1; // ぐるとげ
struct ITEM saw2;
struct ITEM saw3;
struct ITEM saw4;
struct ITEM saw5;
struct ITEM saw6;
struct ITEM saw7;
struct ITEM saw8;
struct ITEM saw9;
struct ITEM blueButton; // 青ボタン
struct ITEM redButton; // 赤ボタン
struct ITEM flag; // セーブポイント

enum{TITLE,PLAY1,PLAY2,CLEAR,OVER}; // シーン
enum Chip { AQ = 16, BJ = 27, BK, BF = 41, BG,BH,BI, DH = 61, GG = 114, GH,GL = 119, GQ = 124, HA = 126, HN = 139, HO, HP, HQ, HR, IA, IB, IC, ID,IJ=153,IK,In = 157,JB = 163,JC,JD, JF = 167, JG, JH, JI, JJ, JK, JL, JM, JN, KB = 181, KC, KF = 185, KL = 191, KM, KN, RI = 314 }; // mapchipの横縦

// ゲーム内で使用する変数、配列
float pai = 3.1415926535f; // 円周率
int timer = 0; // タイマー
int scene = TITLE; // TITLEシーン
int imgClo, imgTre, imgSol; // 背景画像
int imgPlayer[3]; // プレイヤ画像
int imgTorch[2]; // 松明画像
int imgGra, imgTer; // タイトル用地面画像
int imgDie; // ゲームオーバー画面用プレイヤ画像
int imgKey; // 鍵の画像
int imgBlock; // 持ち物背景のブロック
int imgDoor1; // ドア画像
int imgDoor2; // ドア画像
int imgFakeDoor1; // 偽ドア画像
int imgFakeDoor2; // 偽ドア画像
int imgBlueButton1; // 青ボタンfalse
int imgBlueButton2; // true
int imgRedButton1; // 赤ボタンfalse
int imgRedButton2; // true
int imgSaw; // ぐるとげ
int imgFlagOff; // セーブポイントの画像
int imgFlag[2]; // セーブポイント　動く旗の画像
int imgChip; // マップチップ画像
int px = -2; // Clearシーンのプレイヤのx座標
int pitch = 1; // マップチップ画像と画像の空白
int step = chipSize + pitch; // 空白の影響を考慮するための変数
int mapWIDTH = 20, mapHEIGHT = 12; // マップの横幅と縦幅（チップ数）
int mapChipList1[12][20] =
{
	{RI,RI,KN,RI,RI,RI,RI,RI,RI,RI,RI,RI,KM,BG,RI,RI,KM,RI,BG,RI},
	{BJ,RI,KL,RI,AQ,RI,BJ,BJ,RI,RI,RI,RI,KM,BF,RI,RI,KM,RI,BF,RI},
	{RI,RI,RI,KN,RI,RI,RI,RI,RI,RI,RI,HA,KM,KF,RI,RI,KM,RI,BJ,BJ},
	{RI,KB,KC,KM,RI,RI,GG,RI,AQ,RI,AQ,AQ,KL,RI,RI,RI,KM,RI,RI,RI},
	{RI,RI,RI,KM,RI,RI,KN,RI,RI,RI,RI,RI,RI,RI,RI,AQ,KL,BJ,BJ,RI},
	{JN,RI,RI,KL,HA,AQ,KL,RI,RI,RI,RI,RI,RI,BJ,RI,RI,RI,RI,RI,RI},
	{JH,BJ,RI,RI,BJ,RI,RI,RI,BJ,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,BJ},
	{RI,RI,RI,RI,BJ,RI,RI,RI,GG,RI,RI,RI,RI,BJ,BJ,RI,RI,RI,RI,RI},
	{RI,RI,RI,HA,BJ,RI,RI,KB,KF,RI,RI,RI,RI,RI,RI,RI,KB,KF,RI,RI},
	{RI,KB,KC,KF,BJ,RI,RI,RI,RI,RI,RI,KB,KF,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,DH,RI,RI,GQ,RI,RI,RI,RI,RI,GH,RI,RI,RI,DH,RI,RI,RI,RI},
	{JM,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JN}
};
int mapChipList2[12][20] =
{
	{BI,RI,RI,RI,BK,RI,RI,BI,BK,RI,RI,RI,RI,BI,JC,RI,RI,RI,RI,BI},
	{BH,BK,RI,RI,BK,RI,RI,BH,BK,RI,RI,RI,RI,BH,JB,RI,RI,RI,RI,BH},
	{AQ,RI,RI,RI,RI,RI,RI,IJ,BK,RI,AQ,RI,RI,AQ,RI,RI,RI,RI,RI,BK},
	{RI,RI,IC,ID,RI,RI,RI,RI,RI,RI,GG,RI,RI,RI,RI,RI,RI,BK,RI,RI},
	{RI,RI,HR,IA,RI,AQ,RI,RI,RI,IJ,In,RI,RI,RI,RI,AQ,RI,RI,RI,RI},
	{BK,RI,HO,IA,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI},
	{RI,RI,RI,JB,AQ,AQ,RI,RI,RI,RI,RI,RI,BK,RI,RI,RI,RI,RI,RI,RI},
	{GG,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,BK,RI,RI,RI,RI},
	{IJ,IK,In,RI,RI,RI,RI,AQ,RI,RI,RI,IJ,In,RI,RI,BK,RI,RI,GG,RI},
	{RI,BI,RI,RI,RI,RI,AQ,RI,RI,RI,RI,RI,RI,RI,RI,BK,RI,RI,IJ,In},
	{RI,BH,GL,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,RI,GL,RI,RI,RI,RI,RI},
	{JM,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JL,JN}
};

bool keyState; // 鍵の有無
bool blueState; // 青ボタン
bool redState; // 赤ボタン
bool flagState; // セーブポイント

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
			ScrollBG(1, 1, 1);
			Title();
			break;

		case PLAY1:
			ScrollBG(1, 0, 0); // 背景のスクロール
			DrawMapChip1(); // マップチップ
			Gimmick1(); // ギミック 
			Gravity(); // 重力
			MovePlayer(); // プレイヤの操作
			Play();
			break;

		case PLAY2:
			ScrollBG(1, 0, 0); // 背景のスクロール
			DrawMapChip2(); // マップチップ
			Gimmick2(); // ギミック
			Gravity(); // 重力
			MovePlayer(); // プレイヤの操作
			Play();
			break;

		case CLEAR:
			ScrollBG(0, 0, 0);
			Clear();
			break;

		case OVER:
			Over();
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
	imgClo = LoadGraph("Sprites/Backgrounds/cloud.png"); // 上
	imgTre = LoadGraph("Sprites/Backgrounds/tree.png"); // 中
	imgSol = LoadGraph("Sprites/Backgrounds/solid.png"); // 下
	// プレイヤ画像  ここには初期化じゃなくて代入扱いになるから一個ずつするしかない
	imgPlayer[0] = LoadGraph("Sprites/Characters/run1.png");
	imgPlayer[1] = LoadGraph("Sprites/Characters/run2.png");
	imgPlayer[2] = LoadGraph("Sprites/Characters/jump1.png");
	imgDie = LoadGraph("Sprites/Characters/die.png");
	// マップチップ画像
	imgChip = LoadGraph("Sprites/Tiles/mapChip.png");
	// タイトル用地面画像
	imgGra = LoadGraph("Sprites/Tiles/grass.png");
	imgTer = LoadGraph("Sprites/Tiles/terrain.png");
	// ギミック画像
	imgKey = LoadGraph("Sprites/TIles/key.png"); // 鍵
	imgBlock = LoadGraph("Sprites/Tiles/block.png"); // 持ち物ブロック
	imgDoor1 = LoadGraph("Sprites/Tiles/openDoor1.png"); // ドア上
	imgDoor2 = LoadGraph("Sprites/Tiles/openDoor2.png"); // ドア下
	imgFakeDoor1 = LoadGraph("Sprites/Tiles/fakeDoor1.png"); // 偽ドア上
	imgFakeDoor2 = LoadGraph("Sprites/Tiles/fakeDoor2.png"); // 偽ドア下
	imgTorch[0] = LoadGraph("Sprites/Tiles/torch1.png"); // 松明１
	imgTorch[1] = LoadGraph("Sprites/Tiles/torch2.png"); // 松明２
	imgSaw = LoadGraph("Sprites/Tiles/saw.png"); // ぐるとげ
	imgBlueButton1 = LoadGraph("Sprites/Tiles/blueButton1.png"); // 青ボタン false
	imgBlueButton2 = LoadGraph("Sprites/Tiles/blueButton2.png"); // true
	imgRedButton1 = LoadGraph("Sprites/Tiles/redButton1.png"); // 赤ボタン false
	imgRedButton2 = LoadGraph("Sprites/Tiles/redButton2.png"); // true
	imgFlagOff = LoadGraph("Sprites/Tiles/flagOff.png"); // セーブポイント 棒
	imgFlag[0] = LoadGraph("Sprites/Tiles/flag1.png"); // セーブポイント1
	imgFlag[1] = LoadGraph("Sprites/Tiles/flag2.png"); // セーブポイント2
}

// ゲーム開始時の初期値を代入する関数
void InitVariable(void)
{
	// プレイヤ構造体
	player.x = 0.0f; // プレイヤのXの中心座標
	player.y = 0.0f; // プレイヤのYの中心座標
	player.vx = 0.0f; // x方向の速さ
	player.vy = 0.0f; // y方向の速さ
	player.speed = 4.0f; // 移動速度（走る）
	player.jumpPower = 12.8f; // 初速度
	player.gravity = 0.6f; // 重力（常にかかる）
	player.sizeX = 128; // プレイヤのXサイズ
	player.sizeY = 128; // プレイヤのYサイズ
	player.mag = 0.5f; // プレイヤの倍率
	player.jumpState = true; // ジャンプできる状態か
	// とげギミック
	spiky1.x = 224; // 中心のx座標
	spiky1.y = 560; // 中心のy座標
	spiky1.wid = 64; // 幅
	spiky1.hei = 32; // 高さ
	spiky2.x = 288;
	spiky2.y = 368;
	spiky3.x = 736;
	spiky3.y = 176;
	// 鍵とドア PLAY1
	key.x = 224;
	key.y = 96;
	key.wid = 64;
	key.hei = 64;
	keyDoor.x = 1184;
	keyDoor.y = 64;
	keyDoor.wid = 64;
	keyDoor.hei = 128;
	fakeDoor.x = 864;
	fakeDoor.y = 64;
	fakeDoor.wid = 64;
	fakeDoor.hei = 128;
	// 松明
	torch1.x = 160;
	torch1.y = 608;
	torch2.x = 928;
	torch2.y = 608;
	// ドア PLAY2
	trueDoor.x = 864;
	trueDoor.y = 64;
	trueDoor.wid = 64;
	trueDoor.hei = 128;
	falseDoor1.x = 32;
	falseDoor1.y = 64;
	falseDoor1.wid = 64;
	falseDoor1.hei = 128;
	falseDoor2.x = 480;
	falseDoor3.x = 1248;
	// ぐるとげ
	saw1.x = 224;
	saw1.y = 160;
	saw1.wid = 64;
	saw1.hei = 64;
	saw2.x = 352;
	saw2.y = 32;
	saw3.x = 416;
	saw3.y = 32;
	saw4.x = 416;
	saw4.y = 288;
	saw5.x = 544;
	saw5.y = 224;
	saw6.x = 1184;
	saw6.y = 160;
	saw7.x = 1184;
	saw7.y = 352;
	saw8.x = 480;
	saw8.y = 608;
	saw9.x = 736;
	saw9.y = 160;
	// 青、赤ボタン
	blueButton.x = 544;
	blueButton.y = 640;
	blueButton.wid = 64;
	blueButton.hei = 32;
	redButton.x = 736;
	redButton.y = 640;
	redButton.wid = 64;
	redButton.hei = 32;
	// セーブポイントの旗
	flag.x = 288;
	flag.y = 672;
	flag.wid = 64;
	flag.hei = 64;
	// bool値
	keyState = false; // 鍵の有無　持ってない状態
	blueState = false; // 青ボタン　押されてない
	redState = false; // 赤ボタン　押されてない
	flagState = false; // セーブポイント
}

// 背景スクロール
void ScrollBG(int spd1,int spd2,int spd3)
{
	static int cloX, treX, solX; // スクロール位置を管理する変数
	cloX = (cloX - spd1) % 256; // 背景上
		DrawGraph(cloX, 0, imgClo, false);
		DrawGraph(cloX + 256, 0, imgClo, false);
		DrawGraph(cloX + 512, 0, imgClo, false);
		DrawGraph(cloX + 768, 0, imgClo, false);
		DrawGraph(cloX + 1024, 0, imgClo, false);
		DrawGraph(cloX + 1280, 0, imgClo, false);
	treX = (treX - spd2) % 256;// 背景中
		DrawGraph(treX, 256, imgTre, false);
		DrawGraph(treX + 256, 256, imgTre, false);
		DrawGraph(treX + 512, 256, imgTre, false); 
		DrawGraph(treX + 768, 256, imgTre, false);
		DrawGraph(treX + 1024, 256, imgTre, false);
		DrawGraph(treX + 1280, 256, imgTre, false);
	solX = (solX - spd3) % 256;// 背景下
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
	else { player.vx = player.speed; }  // 右方向の速さ
	if (CheckHitKey(KEY_INPUT_SPACE) && player.jumpState == false) // スペースキーが押されたらy方向に力を加える
	{
		player.vy = -player.jumpPower;
		player.jumpState = true; // ジャンプできない状態
	}
	if (player.jumpState == false) // 走ってる状態
	{ // DrawRotaGraph→最後のtrue = 左右反転
		if (player.vx > 0) { DrawRotaGraph(player.x, player.y, player.mag, 0 , imgPlayer[(timer / 8) % 2], true,false); } // 右向きの画像
		else { DrawRotaGraph(player.x, player.y, player.mag, 0, imgPlayer[(timer / 8) % 2], true,true); } // 左向き画像
	}
	else // ジャンプ状態
	{
		if (player.vx > 0) { DrawRotaGraph(player.x, player.y, player.mag, 0, imgPlayer[2], true , false); } // 右向き
		else { DrawRotaGraph(player.x, player.y, player.mag, 0, imgPlayer[2], true, true); } // 左向き
	}
	player.x += player.vx;
	if (scene == PLAY1) { CollisionX1(); }  // シーンごとのプレイヤの当たり判定
	else if (scene == PLAY2) { CollisionX2(); }
	if (player.x < 20) {player.x = 20;}  // 画面外に出ないようにする
	if (player.x > 1260) {player.x = 1260;}
}

// 重力
void Gravity(void)
{
	player.vy += player.gravity; // 常に重力がかかる(速さに)
	player.y += player.vy;
	if (scene == PLAY1) { CollisionY1(); }
	else if (scene == PLAY2) { CollisionY2(); }
	if (player.y < 24) { player.y = 24; }
}

// マップチップ
void DrawMapChip1(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			int id = mapChipList1[i][j];
			int sx = (id % chipRow) * step; // 縦列の指定
			int sy = (id / chipRow) * step; // 横列の指定
			// (描画するx座標,描画するy座標,切り取るx座標（左上）,切り取るy座標,描画する横サイズ,縦サイズ,描画するマップチップ画像,透過の有無)
			DrawRectGraph(j * chipSize, i * chipSize, sx, sy, chipSize, chipSize, imgChip, true);
		}
	}
}

void DrawMapChip2(void)
{
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			int id = mapChipList2[i][j];
			int sx = (id % chipRow) * step; // 縦列の指定
			int sy = (id / chipRow) * step; // 横列の指定
			// (描画するx座標,描画するy座標,切り取るx座標（左上）,切り取るy座標,描画する横サイズ,縦サイズ,描画するマップチップ画像,透過の有無)
			DrawRectGraph(j * chipSize, i * chipSize, sx, sy, chipSize, chipSize, imgChip, true);
		}
	}
}

// マップチップとプレイヤのX軸方向の当たり判定
void CollisionX1(void)
{
	int playerCenterX = player.x; // プレイヤの中心X座標
	int playerCenterY = player.y + 6;// プレイヤの中心Y座標
	int hitSizeX = player.sizeX * 3 / 10 + 1; // 当たり判定の幅
	int hitSizeY = player.sizeY * 3 / 8; // 当たり判定の高さ
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			int chipCenterX = j * chipSize + chipSize / 2; // チップの中心X座標
			int chipCenterY = i * chipSize + chipSize / 2; // チップの中心Y座標
			int dx = abs(playerCenterX - chipCenterX); // x座標の中心間距離
			int dy = abs(playerCenterY - chipCenterY); // y座標の中心間距離

			if (mapChipList1[i][j] == RI || mapChipList1[i][j] == HA || mapChipList1[i][j] == BF || mapChipList1[i][j] == BG || mapChipList1[i][j] == DH || mapChipList1[i][j] == GQ || mapChipList1[i][j] == GG || mapChipList1[i][j] == GH) continue; // 当たり判定無し

			if (dx < (hitSizeX + chipSize) / 2 && dy < (hitSizeY + chipSize) / 2) // x,yの中心間距離がプレイヤとchipのx,yの長さの半分より小さいとき
			{
				int x = (hitSizeX + chipSize) / 2 - dx; // x方向に重なっている長さ
				if (player.vx > 0) { player.x -= x; } // プレイヤが左側
				else if (player.vx < 0) { player.x += x; } // プレイヤが右側
				player.vx = 0; // 衝突したら進む量０にする
			}
		}
	}
}

void CollisionX2(void)
{
	int playerCenterX = player.x; // プレイヤの中心X座標
	int playerCenterY = player.y + 6;// プレイヤの中心Y座標
	int hitSizeX = player.sizeX * 3 / 10 + 1; // 当たり判定の幅
	int hitSizeY = player.sizeY * 3 / 8; // 当たり判定の高さ
	for (int i = 0; i < mapHEIGHT; i++)
	{
		for (int j = 0; j < mapWIDTH; j++)
		{
			int chipCenterX = j * chipSize + chipSize / 2; // チップの中心X座標
			int chipCenterY = i * chipSize + chipSize / 2; // チップの中心Y座標
			int dx = abs(playerCenterX - chipCenterX); // x座標の中心間距離
			int dy = abs(playerCenterY - chipCenterY); // y座標の中心間距離

			if (mapChipList2[i][j] == RI || mapChipList2[i][j] == HA || mapChipList2[i][j] == BF || mapChipList2[i][j] == BG || mapChipList2[i][j] == DH || mapChipList2[i][j] == GQ || mapChipList2[i][j] == GG || mapChipList2[i][j] == GH || mapChipList2[i][j] == BI || mapChipList2[i][j] == BH || mapChipList2[i][j] == GL) continue; // 当たり判定無し

			if (dx < (hitSizeX + chipSize) / 2 && dy < (hitSizeY + chipSize) / 2) // x,yの中心間距離がプレイヤとchipのx,yの長さの半分より小さいとき
			{
				int x = (hitSizeX + chipSize) / 2 - dx; // x方向に重なっている長さ
				if (player.vx > 0) { player.x -= x; } // プレイヤが左側
				else if (player.vx < 0) { player.x += x; } // プレイヤが右側
				player.vx = 0; // 衝突したら進む量０にする
			}
		}
	}
}

// マップチップとプレイヤのY軸方向の当たり判定
void CollisionY1(void)
{
	int playerCenterX = player.x; // プレイヤの中心X座標
	int playerCenterY = player.y + 6;// プレイヤの中心Y座標
	int hitSizeX = player.sizeX * 3 / 10 + 1;// 当たり判定の幅
	int hitSizeY = player.sizeY * 3 / 8; // 当たり判定の高さ
	for (int i = 0; i < mapHEIGHT; i++)
	{   
		for (int j = 0; j < mapWIDTH; j++)
		{
			if (mapChipList1[i][j] == RI || mapChipList1[i][j] == HA || mapChipList1[i][j] == BF || mapChipList1[i][j] == BG || mapChipList1[i][j] == DH || mapChipList1[i][j] == GQ || mapChipList1[i][j] == GG || mapChipList1[i][j] == GH) continue; // 当たり判定無し

			int chipCenterX = j * chipSize + chipSize / 2; 
			int chipCenterY = i * chipSize + chipSize / 2;
			int dx = abs(playerCenterX - chipCenterX);
			int dy = abs(playerCenterY - chipCenterY);

			if (dx < (hitSizeX + chipSize) / 2 && dy < (hitSizeY + chipSize) / 2)
			{
				int y = (hitSizeY  + chipSize) / 2 - dy; // y方向に重なっている長さ
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

void CollisionY2(void)
{
	int playerCenterX = player.x; // プレイヤの中心X座標
	int playerCenterY = player.y + 6;// プレイヤの中心Y座標
	int hitSizeX = player.sizeX * 3 / 10 + 1;// 当たり判定の幅
	int hitSizeY = player.sizeY * 3 / 8; // 当たり判定の高さ
	for (int i = 0; i < mapHEIGHT; i++)
	{   
		for (int j = 0; j < mapWIDTH; j++)
		{
			if (mapChipList2[i][j] == RI || mapChipList2[i][j] == HA || mapChipList2[i][j] == BF || mapChipList2[i][j] == BG || mapChipList2[i][j] == DH || mapChipList2[i][j] == GQ || mapChipList2[i][j] == GG || mapChipList2[i][j] == GH || mapChipList2[i][j] == BI || mapChipList2[i][j] == BH || mapChipList2[i][j] == GL) continue; // 当たり判定無し

			int chipCenterX = j * chipSize + chipSize / 2; 
			int chipCenterY = i * chipSize + chipSize / 2;
			int dx = abs(playerCenterX - chipCenterX);
			int dy = abs(playerCenterY - chipCenterY);

			if (dx < (hitSizeX + chipSize) / 2 && dy < (hitSizeY + chipSize) / 2)
			{
				int y = (hitSizeY  + chipSize) / 2 - dy; // y方向に重なっている長さ
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

// ギミック
void Gimmick1(void)
{
	int playerCenterX = player.x; // プレイヤの中心X座標
	int playerCenterY = player.y + 6;// プレイヤの中心Y座標
	int playerW = 32;
	int playerH = 48;
	// とげとげ
	if (abs(playerCenterX - (int)spiky1.x) < (playerW + spiky1.wid) / 2 && abs(playerCenterY - (int)spiky1.y) < (playerH + spiky1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)spiky2.x) < (playerW + spiky1.wid) / 2 && abs(playerCenterY - (int)spiky2.y) < (playerH + spiky1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)spiky3.x) < (playerW + spiky1.wid) / 2 && abs(playerCenterY - (int)spiky3.y) < (playerH + spiky1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	// 鍵
	if (keyState == false)
	{
		DrawRotaGraph(key.x, key.y, 0.9, pai / 180 * 120, imgKey, true);
		if(abs(playerCenterX - (int)key.x) < (playerW + key.wid) / 2 && abs(playerCenterY - (int)key.y) < (playerH + key.hei) / 2)
		{
			keyState = true;
		}
	}
	else 
	{
		DrawGraph(1157,704,imgBlock,true);
		DrawRotaGraph(1191, 738, 0.8, pai / 180 * 120, imgKey, true);
	}
	// 鍵ドア
	if (abs(playerCenterX - (int)keyDoor.x) < (playerW + keyDoor.wid) / 2 && abs(playerCenterY - (int)keyDoor.y) < (playerH + keyDoor.hei) / 2 && keyState == true)
	{
		timer = 0;
		DrawGraph(keyDoor.x - 32, keyDoor.y - 64, imgDoor1, true);
		DrawGraph(keyDoor.x - 32, keyDoor.y, imgDoor2, true);
		scene = PLAY2;
		player.x = 96;
		player.y = 672;
	}
	// 偽ドア
	if (abs(playerCenterX - (int)fakeDoor.x) < (playerW + fakeDoor.wid) / 2 && abs(playerCenterY - (int)fakeDoor.y) < (playerH + fakeDoor.hei) / 2 && keyState == true)
	{
		DrawGraph(fakeDoor.x - 32, fakeDoor.y - 64, imgFakeDoor1, true);
		DrawGraph(fakeDoor.x - 32, fakeDoor.y, imgFakeDoor2, true);
	}
}

void Gimmick2(void)
{
	int playerCenterX = player.x; // プレイヤの中心X座標
	int playerCenterY = player.y + 6;// プレイヤの中心Y座標
	int playerW = 32;
	int playerH = 48;
	//ぐるとげ
	DrawRotaGraph(saw1.x, saw1.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw2.x, saw2.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw3.x, saw3.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw4.x, saw4.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw5.x, saw5.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw6.x, saw6.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw7.x, saw7.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw8.x, saw8.y, 1, pai / 180 * timer * 2, imgSaw, true);
	DrawRotaGraph(saw9.x, saw9.y, 1, pai / 180 * timer * 2, imgSaw, true);
	if (abs(playerCenterX - (int)saw1.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw1.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw2.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw2.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw3.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw3.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw4.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw4.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw5.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw5.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw6.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw6.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw7.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw7.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw8.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw8.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	if (abs(playerCenterX - (int)saw9.x) < (playerW + saw1.wid) / 2 && abs(playerCenterY - (int)saw9.y) < (playerH + saw1.hei) / 2)
	{
		timer = 0;
		scene = OVER;
	}
	// 松明
	DrawGraph(torch1.x - 32, torch1.y - 32, imgTorch[(timer / 12) % 2], true);
	DrawGraph(torch2.x - 32, torch2.y - 32, imgTorch[(timer / 12) % 2], true);
	// 本物ドア
	if (abs(playerCenterX - (int)trueDoor.x) < (playerW + trueDoor.wid) / 2 && abs(playerCenterY - (int)trueDoor.y) < (playerH + trueDoor.hei) / 2)
	{
		player.vx = 0;
		player.vy = 0;
		scene = CLEAR;
	}
	// 偽ドア
	if (abs(playerCenterX - (int)falseDoor1.x) < (playerW + falseDoor1.wid) / 2 && abs(playerCenterY - (int)falseDoor1.y) < (playerH + falseDoor1.hei) / 2 )
	{
		DrawGraph(falseDoor1.x - 32, falseDoor1.y - 64, imgFakeDoor1, true);
		DrawGraph(falseDoor1.x - 32, falseDoor1.y, imgFakeDoor2, true);
	}
	if (abs(playerCenterX - (int)falseDoor2.x) < (playerW + falseDoor1.wid) / 2 && abs(playerCenterY - (int)falseDoor1.y) < (playerH + falseDoor1.hei) / 2 )
	{
		DrawGraph(falseDoor2.x - 32, falseDoor1.y - 64, imgFakeDoor1, true);
		DrawGraph(falseDoor2.x - 32, falseDoor1.y, imgFakeDoor2, true);
	}
	if (abs(playerCenterX - (int)falseDoor3.x) < (playerW + falseDoor1.wid) / 2 && abs(playerCenterY - (int)falseDoor1.y) < (playerH + falseDoor1.hei) / 2 )
	{
		DrawGraph(falseDoor3.x - 32, falseDoor1.y - 64, imgFakeDoor1, true);
		DrawGraph(falseDoor3.x - 32, falseDoor1.y, imgFakeDoor2, true);
	}
	// 青ボタン
	if (blueState == false) // 押されていない状態
	{
		DrawGraph(blueButton.x - 32, blueButton.y, imgBlueButton1, true);
		if (abs(playerCenterX - (int)blueButton.x) < (playerW + blueButton.wid) / 2 && abs(playerCenterY - (int)blueButton.y) < (playerH + blueButton.hei) / 2)
		{
			blueState = true;
			redState = false;
			player.jumpPower = 15.8f; // プレイヤのジャンプ量を大きくする
		}
	}
	else
	{
		DrawGraph(blueButton.x - 32, blueButton.y, imgBlueButton2, true);
	}
	// 赤ボタン
	if (redState == false) // 押されていない状態
	{
		DrawGraph(redButton.x - 32, redButton.y, imgRedButton1, true);
		if (abs(playerCenterX - (int)redButton.x) < (playerW + redButton.wid) / 2 && abs(playerCenterY - (int)redButton.y) < (playerH + redButton.hei) / 2)
		{
			redState = true;
			blueState = false;
			player.jumpPower = 12.8f; // プレイヤのジャンプ量を元に戻す
		}
	}
	else {DrawGraph(redButton.x - 32, redButton.y, imgRedButton2, true);}
	// セーブポイント
	if (flagState == false)
	{
		DrawGraph(flag.x - 32, flag.y - 32, imgFlagOff, true);
		if (abs(playerCenterX - (int)flag.x) < (playerW + flag.wid) / 2 && abs(playerCenterY - (int)flag.y) < (playerH + flag.hei) / 2)
		{
			flagState = true;
		}
	}
	else {DrawGraph(flag.x - 32, flag.y - 32, imgFlag[(timer / 12) % 2], true);}
}
				
// タイトル
void Title(void)
{
	SetFontSize(130);
	DrawString(WIDTH / 4 - 16, HEIGHT / 3 - 47, "Run to Goal", 0x1E90FF); // タイトル表示
	if (timer % 60 < 30)
	{
		SetFontSize(40);
		DrawString(WIDTH / 5 * 2 - 95, HEIGHT / 5 * 3 + 10, "-Press SPACE To PLAY-", 0xffffff); // スペースキー指示表示 点滅する
	}
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		scene = PLAY1;
		player.x = 32.0f;
		player.y = 32.0f;
	}
	DrawGraph(WIDTH / 2 - 64, HEIGHT / 5 * 3 + 56, imgPlayer[(timer / 8) % 2], true); // プレイヤの表示
	int timerGR = timer % chipSize;
	for (int x = 0; x < 1344; x += chipSize) // 地面のスクロール
	{
		DrawGraph(x - timerGR, HEIGHT - chipSize * 2, imgGra, true); // 草ブロック
		DrawGraph(x - timerGR, HEIGHT - chipSize, imgTer, true); // 土ブロック
	}
}

// PLAY画面
void Play(void)
{
	SetFontSize(30);
	DrawString(96, 726, "方向転換：D　　ジャンプ：SPACE", 0xffffff);
}

// CLEAR画面
void Clear(void)
{
	SetFontSize(200);
	DrawString(194, 118, "C L A E R", 0xfeff0f);
	SetFontSize(200);
	DrawString(192, 113, "C L A E R", 0xfeff0f);
	SetFontSize(200);
	DrawString(185, 110, "C L A E R", 0xffffff);
	for (int x = 0; x < 1280; x += chipSize) 
	{
		DrawGraph(x, HEIGHT - chipSize * 2, imgGra, true); // 草ブロック
		DrawGraph(x, HEIGHT - chipSize, imgTer, true); // 土ブロック
	}
	px += player.speed + 3 ;
	if ( px < WIDTH + 10)
	{
		DrawGraph(px, 515, imgPlayer[(timer / 8) % 2], true);
	}
	else 
	{ 
		InitVariable();
		scene = TITLE;
		px = -2;
	}
}

// OVER画面
void Over(void)
{
	SetFontSize(200);
	DrawString(182, 180, "GAME OVER", 0xff0000);
	DrawRotaGraph(WIDTH / 2, HEIGHT / 3 * 2, 1.3 , 0, imgDie, true);
	if (timer == 60)
	{
		if (flagState == false)
		{
			InitVariable();
			scene = TITLE; // タイトルへシーン遷移
		}
		else 
		{
			InitVariable();
			scene = PLAY2;
			player.x = flag.x;
			player.y = flag.y;
		}
	}
}