#pragma once

  // 構造体の宣言
struct OBJECT // プレイヤと敵用
{
	int x; // x座標
	int y; // y座標
	int vx; // x軸方向の速さ
	int vy; // y軸方向の速さ
	int image; // 画像
	int jumpStatus; // ジャンプ状態
};

// 関数プロトタイプ宣言
void InitGame(void);
void ScrollBG(int spd);
void MovePlayer(void);
void InitVariable(void);
