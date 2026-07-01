#include"DxLib.h"
#include"Run.h" // ヘッダーファイル

// 定数定義
const int WIDTH = 1280, HEIGHT = 768; // ウィンドウの幅と高さのピクセル数
const int FPS = 60; // フレームレート

// ゲーム内で使用する変数、配列
int imgClo, imgTre, imgSol; // 背景画像
int imgPlayer[5]; // プレイヤ画像
bool runState = true; // プレイヤが左右どちらにうごいているか　true = 右、false =　左

// グローバル関数
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("Run Game"); // ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true); // ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1; // ライブラリ初期化、エラーが起きたら終了
	SetBackgroundColor(0, 0, 0); // 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏背景にする

	struct OBJECT player; // プレイヤの構造体変数

	InitGame(); // 初期化用関数
	InitVariable(); // ゲーム開始時の位置

	while (1) // メインループ
	{
		ClearDrawScreen(); // 画面をクリアする

		// ゲームの骨組みの処理
		ScrollBG(1); // 背景のスクロール
		MovePlayer(); // プレイヤの操作

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
	imgSol  = LoadGraph("Sprites/Backgrounds/solid.png"); // 背景下
	// プレイヤ画像  ここには初期化じゃなくて代入扱いになるから一個ずつするしかない
	imgPlayer[0] = LoadGraph("Sprites/Characters/flont.png");
	imgPlayer[1] = LoadGraph("Sprites/Characters/run1.png");
	imgPlayer[2] = LoadGraph("Sprites/Characters/run2.png");
	imgPlayer[3] = LoadGraph("Sprites/Characters/jump1.png");
	imgPlayer[4] = LoadGraph("Sprites/Characters/jump2.png");	
}

// 背景スクロール
void ScrollBG(int spd)
{
	static int cloX, treX, solX; // スクロール位置を管理する変数
	cloX = (cloX - spd) % 256; // 背景上
		DrawGraph(cloX, 0, imgClo, false);
		DrawGraph(cloX+ 256, 0, imgClo, false);
		DrawGraph(cloX+ 512, 0, imgClo, false);
		DrawGraph(cloX+ 768, 0, imgClo, false);
		DrawGraph(cloX+ 1024, 0, imgClo, false);
		DrawGraph(cloX+ 1280, 0, imgClo, false);
	treX = (treX - spd) % 256; // 背景中
		DrawGraph(treX, 256, imgTre, false);
		DrawGraph(treX+ 256, 256, imgTre, false);
		DrawGraph(treX+ 512, 256, imgTre, false);
		DrawGraph(treX+ 768, 256, imgTre, false);
		DrawGraph(treX+ 1024, 256, imgTre, false);
		DrawGraph(treX+ 1280, 256, imgTre, false);
	solX = (solX - spd) % 256; // 背景下
		DrawGraph(solX, 512, imgSol, false);
		DrawGraph(solX+ 256, 512, imgSol, false);
		DrawGraph(solX+ 512, 512, imgSol, false);
		DrawGraph(solX+ 768, 512, imgSol, false);
		DrawGraph(solX+ 1024, 512, imgSol, false);
		DrawGraph(solX + 1280, 512, imgSol, false);
}

// ゲーム開始時の初期値を代入する関数
void InitVariable(void)
{
	player.x = 256;
}

// プレイヤの操作
void MovePlayer(void)
{
	if (CheckHitKey(KEY_INPUT_D) && runState == true) // 右に走る
	{
		player.x 
	}
	if (CheckHitKey(KEY_INPUT_D) && runState == false) // 左に走る
	{

	}
}