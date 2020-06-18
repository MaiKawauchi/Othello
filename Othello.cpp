#include <iostream>
#include <iomanip>
#include <cstring>
#include <utility>

#include <GLFW/glfw3.h>

using namespace std;

#define BOARD 10                  /* 盤を表す配列 board のサイズ */
#define AITE(player) (3-(player)) /* playerの相手(敵) */


/*----------------------------------------------*/
// ・石が無い     0
// ・黒石がある   1
// ・白石がある   2
// ・盤外        -1
/*----------------------------------------------*/


void initBoard(int board[BOARD][BOARD])
{
	/*
	static int initData[BOARD][BOARD] =
	{	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
		{-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
		{-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
		{-1,  0,  0,  0,  2,  1,  0,  0,  0, -1},
		{-1,  0,  0,  0,  1,  2,  0,  0,  0, -1},
		{-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
		{-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
		{-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	};
	*/
	static int initData[BOARD][BOARD] =
	{   {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1,  0,  0,  2,  1,  1,  2,  1,  1, -1},
		{-1,  1,  2,  1,  1,  2,  1,  1,  2, -1},
		{-1,  1,  1,  2,  1,  1,  1,  2,  2, -1},
		{-1,  1,  1,  1,  1,  1,  2,  1,  1, -1},
		{-1,  1,  1,  1,  1,  2,  2,  2,  1, -1},
		{-1,  2,  1,  1,  1,  2,  2,  1,  1, -1},
		{-1,  2,  1,  1,  1,  2,  2,  0,  0, -1},
		{-1,  2,  2,  2,  2,  2,  2,  2,  0, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	};
	std::memcpy( board, initData, sizeof(initData) );
}

void printBoard(int board[BOARD][BOARD]) 
{
	cout << "    a b c d e f g h" << endl;
	cout << "   -----------------" << endl;
	for (int i = 1; i < BOARD - 1; ++i) {
		cout << " " << i << "|";
		for (int j = 1; j < BOARD - 1; ++j) {
			cout << std::right << std::setw(2) << board[i][j];
		}
		cout << " |" << endl;
	}
	cout << "   -----------------" << endl;
}

/*---------------------- 裏返すことができる石の数 ----------------------*/
int countTurnOber(int board[BOARD][BOARD], int player, int p, int q, int d, int e)
{
	int i;
	for (i = 1; board[p + i * d][q + i * e] == AITE(player); i++) {};

	if (board[p + i * d][q + i * e] == player) return i - 1;
	else return 0;
}

/*--------------------- 石を置いて挟んだ石を裏返す ---------------------*/
void setAndTurnOver(int board[BOARD][BOARD], int player, int p, int q)
{
	int count, d, e;
	for (d = -1; d <= 1; ++d) {      /* 上下方向 */
		for (e = -1; e <= 1; ++e) {  /* 左右方向 */
			if (d == 0 && e == 0) continue;
			/* ±d方向、±e方向において裏返すことができる石の数 */
			count = countTurnOber(board, player, p, q, d, e);
			
			/* countの数だけ(p, q)から(d, e)方向に裏返す */
			for (int i = 1; i <= count; ++i) {
				board[p + i * d][q + i * e] = player;
			}
		}
	}
	board[p][q] = player;
}


int isLegalMove(int board[BOARD][BOARD], int player, int* p, int* q) 
{
	if (*p < 1 || *p > 8 || *q < 1 || *q > 8) return 0;           /* 盤上でない 　*/
	if (board[*p][*q] != 0) return 0;                           /* 空いていない */
	/* 八方向のうちどれか一つでも裏返る石があれば合法的な手 */
	if (countTurnOber(board, player, *p, *q, -1,  0)) return 1; /* 上　 */
	if (countTurnOber(board, player, *p, *q,  1,  0)) return 1; /* 下　 */
	if (countTurnOber(board, player, *p, *q,  0, -1)) return 1; /* 左　 */
	if (countTurnOber(board, player, *p, *q,  0,  1)) return 1; /* 右 　*/
	if (countTurnOber(board, player, *p, *q, -1, -1)) return 1; /* 左上 */
	if (countTurnOber(board, player, *p, *q, -1,  1)) return 1; /* 右上 */
	if (countTurnOber(board, player, *p, *q,  1, -1)) return 1; /* 左下 */
	if (countTurnOber(board, player, *p, *q,  1,  1)) return 1; /* 右下 */
	return 0;
}


/*---------- player にとって合法的な手が一つでもあれば 1 を返す ----------*/
int existLegalMovePlayer(int board[BOARD][BOARD], int player)
{
	for (int i = 1; i < BOARD-1; ++i) {
		for (int j = 1; j < BOARD-1; ++j) {
			if (isLegalMove(board, player, &i, &j)) return 1;
		}
	}
	return 0;
}

/*------------------------ playerの指し手の入力 -------------------------*/
void getMove(int board[BOARD][BOARD], int player, int* p, int* q)
{
	char str[4];
	cout << "player number : " << player;

	while (1)
	{
		cout << " > ";
		cin >> str[0] >> str[1];

		/* 行、列 の順に入力された場合 */
		if ((str[0] >= 'a' && str[0] <= 'h') && (str[1] >= '1' && str[1] <= '8')) {
			*q = str[0] - 'a' + 1; // 行番号
			*p = str[1] - '1' + 1; // 列番号
			if (isLegalMove(board, player, p, q)) return;
			cout << "p = " << *p << " / q = " << *q << endl;
		}
		/* 列、行 の順に入力された場合 */
		else if ((str[1] > 'a' && str[1] < 'h') && (str[0] > '1' && str[0] < '9')) {
			*q = str[1] - 'a' + 1; // 行番号
			*p = str[0] - '1' + 1; // 列番号
			if (isLegalMove(board, player, p, q)) return;
			cout << "p = " << *p << " / q = " << *q << endl;
		}
		
		cout << "select a legal move : ";
		for (int i = 1; i < BOARD - 1; ++i) {
			for (int j = 1; j < BOARD - 1; ++j) {
				if (isLegalMove(board, player, &i, &j)) {
					str[2] = j + 'a' - 1;
					cout << str[2] << i << " ";
				}
			}
		}
		continue;
	}
}

void countStone(int board[BOARD][BOARD], int *c1, int *c2) 
{
	*c1 = *c2 = 0;
	for (int i = 1; i < BOARD-1; ++i) {
		for (int j = 1; j < BOARD-1; ++j) {
			switch (board[i][j]) {
			case 1 : (*c1)++;
				break;
			case 2 : (*c2)++;
				break;
			}
		}
	}
}

int game() 
{
	int board[BOARD][BOARD];
	int c1, c2;                  // 石の数
	int player;                  // 打ち手(1 or 2)
	int p, q;                    // p 行 q 列

	/*----- ゲーム盤の初期化 -----*/
	initBoard(board);
	player = 1;

	/*----- ゲームの進行 -----*/
	while (1)
	{
		/* ①ゲーム状況の表示 */
		printBoard(board);

		/* ②ゲームの終了判断 */
		// プレーヤーに合法的手がなければ，プレーヤー交代
		if (!existLegalMovePlayer(board, player)) {
			cout << "Player " << player << " has no legal moves > PASS" << endl;
			player = AITE(player);

			// 交代してもなお，合法的な手がなければ，ゲーム終了
			if (!existLegalMovePlayer(board, player)) {
				cout << "Player " << player << " has no legal moves > PASS" << endl;
				break;
			}
		}

		/* ③指し手入力 */
		getMove(board, player, &p, &q);

		/* ④ゲーム状況を変化させる */
		setAndTurnOver(board, player, p, q);

		/* ⑤プレイヤー交代 */
		player = AITE(player);
	}

	/*----- ゲームの最終局面の判断 -----*/
	/* プレーヤーの双方に合法的な手がなくなったら終了 */
	countStone(board, &c1, &c2);
	cout << "Game Over ! " << endl;
	cout << "Player 1 : " << c1 << " vs Player 2 : " << c2 << endl;

	return c1 - c2;
}

int main()
{
	/*---------------------------------------------------------------------*/
	if (glfwInit() == GL_FALSE) {
		std::cerr << "Can't initialize GLFW" << endl;
		return 1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* const window = glfwCreateWindow(640, 480, "Othello", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Can't create GLFW window." << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	/* ウィンドウを閉じるまで(ウィンドウが開いている間)繰り返したい処理 */
	while (glfwWindowShouldClose(window) == GL_FALSE) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwWaitEvents();
	}
	glfwTerminate();
	/*---------------------------------------------------------------------*/


	cout << "Let's play Othello" << endl;

	int result;
	result = game();
	if(result == 0){
		cout << "引き分け" << endl;
	}
	else if (result >= 0) {
		cout << "プレイヤー１の勝ち" << endl;
	}
	else {
		cout << "プレイヤー２の勝ち" << endl;
	}

	
	return 0;
}