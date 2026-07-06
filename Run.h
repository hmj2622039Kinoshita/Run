#pragma once

  // 構造体の宣言
struct OBJECT // プレイヤと敵用
{
	float x; // x座標
	float y; // y座標
	float vx; // x軸方向の速さ
	float vy; // y軸方向の速さ
	float jumpPower; // ジャンプ力
	float gravity; // 重力（落下速度）
	bool jumpState; // ジャンプ状態か否か
};
int chipX; // 画像に含まれるマップチップ数
int chipY;

// 関数プロトタイプ宣言
void InitGame(void);
void ScrollBG(int spd);
void MovePlayer(void);
void InitVariable(void);
void Gravity(void);
void Jump(void);
void DrawMapChip(void);
