#pragma once

  // 構造体の宣言
struct OBJECT // プレイヤと敵用
{
	float x; // x座標
	float y; // y座標
	float vx; // x軸方向の速さ
	float vy; // y軸方向の速さ
	float speed; // 走るスピード
	float jumpPower; // ジャンプ力
	float gravity; // 重力（落下速度）
	int sizeX; // プレイヤのXサイズ
	int sizeY; // プレイヤのYサイズ
	bool jumpState; // ジャンプ状態か否か
};


// 関数プロトタイプ宣言
void InitGame(void);
void ScrollBG(int spd);
void MovePlayer(void);
void InitVariable(void);
void Gravity(void);
void DrawMapChip(void);
void CollisionX(void);
void CollisionY(void);
void Title(void);
void Play(void);
void Result(void);