#pragma once

  // 構造体の宣言
struct OBJECT // プレイヤと敵用
{
	int x; // x座標
	int y; // y座標
	int vx; // x軸方向の速さ
	int vy; // y軸方向の速さ
	int image; // 画像
	int wid; // 画像の幅（ピクセル数）
	int hei; // 画像の高さ
};

// 関数プロトタイプ宣言
void InitGame(void);
void ScrollBG(int spd);
void MovePlayer(void);
void InitVariable(void);