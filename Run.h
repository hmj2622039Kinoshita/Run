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
	int sizeX; // Xサイズ
	int sizeY; // Yサイズ
	float mag; // 倍率
	bool jumpState; // ジャンプ状態か否か
};

struct ITEM // ギミック、アイテム用
{
	float x; // 中心のx座標
	float y; // 中心のy座標
	float wid; // 幅
	float hei; // 高さ
};


// 関数プロトタイプ宣言
void InitGame(void);
void ScrollBG(int spd1,int spd2,int spd3);
void MovePlayer(void);
void InitVariable(void);
void Gravity(void);
void DrawMapChip1(void);
void DrawMapChip2(void);
void CollisionX1(void);
void CollisionX2(void);
void CollisionY1(void);
void CollisionY2(void);
void Gimmick1(void);
void Gimmick2(void);
void Title(void);
void Play(void);
void Clear(void);
void Over(void);