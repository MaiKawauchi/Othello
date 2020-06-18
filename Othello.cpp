#include <iostream>
#include <iomanip>
#include <cstring>
#include <utility>

#include <GLFW/glfw3.h>

using namespace std;

#define BOARD 10                  /* �Ղ�\���z�� board �̃T�C�Y */
#define AITE(player) (3-(player)) /* player�̑���(�G) */


/*----------------------------------------------*/
// �E�΂�����     0
// �E���΂�����   1
// �E���΂�����   2
// �E�ՊO        -1
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

/*---------------------- ���Ԃ����Ƃ��ł���΂̐� ----------------------*/
int countTurnOber(int board[BOARD][BOARD], int player, int p, int q, int d, int e)
{
	int i;
	for (i = 1; board[p + i * d][q + i * e] == AITE(player); i++) {};

	if (board[p + i * d][q + i * e] == player) return i - 1;
	else return 0;
}

/*--------------------- �΂�u���ċ��񂾐΂𗠕Ԃ� ---------------------*/
void setAndTurnOver(int board[BOARD][BOARD], int player, int p, int q)
{
	int count, d, e;
	for (d = -1; d <= 1; ++d) {      /* �㉺���� */
		for (e = -1; e <= 1; ++e) {  /* ���E���� */
			if (d == 0 && e == 0) continue;
			/* �}d�����A�}e�����ɂ����ė��Ԃ����Ƃ��ł���΂̐� */
			count = countTurnOber(board, player, p, q, d, e);
			
			/* count�̐�����(p, q)����(d, e)�����ɗ��Ԃ� */
			for (int i = 1; i <= count; ++i) {
				board[p + i * d][q + i * e] = player;
			}
		}
	}
	board[p][q] = player;
}


int isLegalMove(int board[BOARD][BOARD], int player, int* p, int* q) 
{
	if (*p < 1 || *p > 8 || *q < 1 || *q > 8) return 0;           /* �Տ�łȂ� �@*/
	if (board[*p][*q] != 0) return 0;                           /* �󂢂Ă��Ȃ� */
	/* �������̂����ǂꂩ��ł����Ԃ�΂�����΍��@�I�Ȏ� */
	if (countTurnOber(board, player, *p, *q, -1,  0)) return 1; /* ��@ */
	if (countTurnOber(board, player, *p, *q,  1,  0)) return 1; /* ���@ */
	if (countTurnOber(board, player, *p, *q,  0, -1)) return 1; /* ���@ */
	if (countTurnOber(board, player, *p, *q,  0,  1)) return 1; /* �E �@*/
	if (countTurnOber(board, player, *p, *q, -1, -1)) return 1; /* ���� */
	if (countTurnOber(board, player, *p, *q, -1,  1)) return 1; /* �E�� */
	if (countTurnOber(board, player, *p, *q,  1, -1)) return 1; /* ���� */
	if (countTurnOber(board, player, *p, *q,  1,  1)) return 1; /* �E�� */
	return 0;
}


/*---------- player �ɂƂ��č��@�I�Ȏ肪��ł������ 1 ��Ԃ� ----------*/
int existLegalMovePlayer(int board[BOARD][BOARD], int player)
{
	for (int i = 1; i < BOARD-1; ++i) {
		for (int j = 1; j < BOARD-1; ++j) {
			if (isLegalMove(board, player, &i, &j)) return 1;
		}
	}
	return 0;
}

/*------------------------ player�̎w����̓��� -------------------------*/
void getMove(int board[BOARD][BOARD], int player, int* p, int* q)
{
	char str[4];
	cout << "player number : " << player;

	while (1)
	{
		cout << " > ";
		cin >> str[0] >> str[1];

		/* �s�A�� �̏��ɓ��͂��ꂽ�ꍇ */
		if ((str[0] >= 'a' && str[0] <= 'h') && (str[1] >= '1' && str[1] <= '8')) {
			*q = str[0] - 'a' + 1; // �s�ԍ�
			*p = str[1] - '1' + 1; // ��ԍ�
			if (isLegalMove(board, player, p, q)) return;
			cout << "p = " << *p << " / q = " << *q << endl;
		}
		/* ��A�s �̏��ɓ��͂��ꂽ�ꍇ */
		else if ((str[1] > 'a' && str[1] < 'h') && (str[0] > '1' && str[0] < '9')) {
			*q = str[1] - 'a' + 1; // �s�ԍ�
			*p = str[0] - '1' + 1; // ��ԍ�
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
	int c1, c2;                  // �΂̐�
	int player;                  // �ł���(1 or 2)
	int p, q;                    // p �s q ��

	/*----- �Q�[���Ղ̏����� -----*/
	initBoard(board);
	player = 1;

	/*----- �Q�[���̐i�s -----*/
	while (1)
	{
		/* �@�Q�[���󋵂̕\�� */
		printBoard(board);

		/* �A�Q�[���̏I�����f */
		// �v���[���[�ɍ��@�I�肪�Ȃ���΁C�v���[���[���
		if (!existLegalMovePlayer(board, player)) {
			cout << "Player " << player << " has no legal moves > PASS" << endl;
			player = AITE(player);

			// ��サ�Ă��Ȃ��C���@�I�Ȏ肪�Ȃ���΁C�Q�[���I��
			if (!existLegalMovePlayer(board, player)) {
				cout << "Player " << player << " has no legal moves > PASS" << endl;
				break;
			}
		}

		/* �B�w������� */
		getMove(board, player, &p, &q);

		/* �C�Q�[���󋵂�ω������� */
		setAndTurnOver(board, player, p, q);

		/* �D�v���C���[��� */
		player = AITE(player);
	}

	/*----- �Q�[���̍ŏI�ǖʂ̔��f -----*/
	/* �v���[���[�̑o���ɍ��@�I�Ȏ肪�Ȃ��Ȃ�����I�� */
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

	/* �E�B���h�E�����܂�(�E�B���h�E���J���Ă����)�J��Ԃ��������� */
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
		cout << "��������" << endl;
	}
	else if (result >= 0) {
		cout << "�v���C���[�P�̏���" << endl;
	}
	else {
		cout << "�v���C���[�Q�̏���" << endl;
	}

	
	return 0;
}