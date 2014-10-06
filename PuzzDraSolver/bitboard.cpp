//#include <iostream>
//#include <vector>
//#include <string>
//#include <sstream>
//#include <unordered_set>
//#include <unordered_map>
//#include <array>
//#include <algorithm>
//
//using namespace std;
//
///*����
//(0, 0) (0, 1) (0, 2) (0, 3) (0, 4) (0, 5)
//(1, 0) (1, 1) (1, 2) (1, 3) (1, 4) (1, 5)
//(2, 0) (2, 1) (2, 2) (2, 3) (2, 4) (2, 5)
//(3, 0) (3, 1) (3, 2) (3, 3) (3, 4) (3, 5)
//(4, 0) (4, 1) (4, 2) (4, 3) (4, 4) (4, 5)
//
//���ꂼ�ꑮ�����Ƃ� 1, 2, 3, 4, 5, 6 �̒l�����
//*/
//
//const int H = 5;
//const int W = 6;
//const int SIZE = H * W;
//const int ATTRIBUTE_NUM = 6;
//const int TATE_LINE = 545392672;
//const int YOKO_LINE = (1 << W) - 1;
//const int FILLED_BORD = (1 << SIZE) - 1;
//const int COMBO_NUM_LEAST = 3;
//const pair<int, int> NO_COMBO = pair<int, int>();
//
////pushDown�������̂��߂Ɏg��
////find!=end�Ȃ珈������K�v���Ȃ�
////unordered_set<int> pushable;//����Ȃ���
//
//
////�c��bit���牡��bit�ɕϊ����邽�߂̃e�[�u��
////�����Ȃ邩����������]�T������Ύg���łƎg��Ȃ��łŔ�r�����ق�����������
////unordered_map<int, int> tateToYokoTable;
////void _initializeTateToYokoTable();
//
//
////combo����p�Ɏg��
////first����second�܂ŃR���{����
//array<pair<int, int>, 1 << W> comboPatterns;
//
//array<int, 1 << W> comboToDelete;
//
//array<int, 1 << W> yokoToTateTable;
//
//void _initializeYokoToTateTable()
//{
//	//�S�R���{�p�^�[����bit�S��
//	for (int i = 0; i <= YOKO_LINE; i++)
//	{
//		int tate = 0;
//		for (int k = 0; k < H; k++)
//		{
//			tate = (tate << W) | (((i >> (W - k - 1)) & 1) << W - 1);
//		}
//		yokoToTateTable[i] = tate;
//	}
//}
//
//void _initializeComboPattens()
//{
//	//�S�R���{�p�^�[����bit�S��
//	for (int i = 0; i <= YOKO_LINE; i++)
//	{
//		int renzoku = 0;
//		int comboCount = 0;
//		int from = 0;//���ƂŖ��O�ς���
//		int goal = 0;//���ƂŖ��O�ς���
//		for (int k = 0; k < W; k++)
//		{
//			if ((i >> (W - k - 1)) & 1)
//			{
//				renzoku++;
//				goal = k;
//			}
//			else
//			{
//				if (renzoku >= COMBO_NUM_LEAST)
//				{
//					break;
//				}
//				from = k + 1;
//				renzoku = 0;
//			}
//			if (renzoku >= COMBO_NUM_LEAST)
//			{
//				comboCount = renzoku;
//			}
//		}
//
//		if (comboCount)
//		{
//			auto p = pair<int, int>(from, goal);
//			comboPatterns[i] = p;
//
//			int bit = (1 << W) - 1;
//			for (int k = 0; k < W; k++)
//			{
//				if (p.first <= k && k <= p.second)
//				{
//					bit = bit & ~(1 << (W - k - 1));
//				}
//			}
//			comboToDelete[i] = bit;
//		}
//	}
//}
//
//
//void initializeData()
//{
//	_initializeComboPattens();
//	_initializeYokoToTateTable();
//}
//
////�c��bit������bit�ɕϊ�����
//inline int tateToYoko(int tate)
//{
//	int shifted = SIZE - 1;
//	int yoko = 0;
//	for (int i = 0; i < H; i++)
//	{
//		yoko = (yoko << 1) | ((tate >> shifted) & 1);
//		shifted -= W;
//	}
//	return yoko << 1;
//}
//
////���͂��r�b�g�{�[�h�ɕϊ�
////(0, 0)���ő�bit�ɂȂ�
//int toBitBoard(vector<vector<int> >& board, int attribute)
//{
//	int bits = 0;
//	for (auto line : board)
//	{
//		for (auto ball : line)
//		{
//			bits = (bits << 1) | (ball == attribute);
//		}
//	}
//	return bits;
//}
//
//
////�r�b�g�{�[�h��1bit���o��
//string toStringBitBoard(int bitBoard)
//{
//	int num = SIZE;
//	ostringstream ss;
//	for (int i = 0; i < H; i++)
//	{
//		for (int j = 0; j < W; j++)
//		{
//			ss << ((bitBoard >> (num - 1)) & 1) << ' ';
//			num--;
//		}
//		ss << '\n';
//	}
//	return ss.str();
//}
//
//
//string toStringBitBoards(vector<int>& bitBoards)
//{
//	ostringstream ss;
//	for (int i = 0; i < H; i++)
//	{
//		for (int j = 0; j < W; j++)
//		{
//			bool output = false;
//			for (int k = 0; k < ATTRIBUTE_NUM; k++)
//			{
//				if ((bitBoards[k] >> W * (H - i - 1) + (W - j - 1)) & 1)
//				{
//					ss << (k + 1) << ' ';
//					output = true;
//					break;
//				}
//			}
//			if (!output) ss << '0' << ' ';
//		}
//		ss << '\n';
//	}
//	return ss.str();
//}
//
//
//string debug(int bitBoard)
//{
//	return toStringBitBoard(bitBoard);
//}
//
//
////�S�Ẵr�b�g�{�[�h�̘_���a�����
//inline int toAllBoard(vector<int>& bitBoards)
//{
//	int allBoard = 0;
//	for (auto board : bitBoards)
//	{
//		allBoard |= board;
//	}
//
//	return allBoard & FILLED_BORD;//30,31bit�ڂ������Ă��邢��Ԃ�h��
//}
//
//
////����������
////bitboard : ������������O�̃{�[�h
////r : ������i�Ԗ�
////c : ������j�Ԗ�
//inline int pushDownBitBoard(int bitBoard, int r, int c)
//{
//	int deletedBoard = ~(TATE_LINE >> c) | (TATE_LINE >> c + W * (H - r));//�ҏW����Ƃ�����폜����bit
//	int nowTate = (bitBoard & (TATE_LINE >> c)) & ~deletedBoard;//�����Ă�����bit�B������������
//	int pushedTate = nowTate >> W;//��i����
//	return (bitBoard & deletedBoard) | pushedTate;
//}
//
//
////�R���{�ŋ󂢂����Ԃ𖄂߂� : �󂫂����������ǂ���
//bool pushDownBoards(vector<int>& bitBoards)
//{
//	int allBoard = toAllBoard(bitBoards);
//	if (allBoard == FILLED_BORD)
//	{
//		return false; //���Ԃ��󂢂Ă��Ȃ���Ζ߂�
//	}
//
//	int pushDownedBoard = allBoard;
//	//�l�߂�
//	for (int c = 0; c < W; c++)
//	{
//		int tateAll = (pushDownedBoard << c) & TATE_LINE;//��ԍ��̗�ɑ����Ă���ȊO������
//		if (tateAll == TATE_LINE || tateAll == 0) continue;//�󂫂��Ȃ���΋l�߂Ȃ�
//
//		int pushed = 0;
//		for (int r = 0; r < H - 1 - pushed;)//�����猩�Ă���
//		{
//			if (pushDownedBoard >> (r * W + (W - c - 1)) & 1)//�󂢂Ă��邩�ǂ���
//			{
//				r++;//���
//				continue;
//			}
//			pushDownedBoard = pushDownBitBoard(pushDownedBoard, r, c);//�S�̂�����������
//			for (auto& board : bitBoards)
//			{
//				board = pushDownBitBoard(board, r, c);//��̑����̃r�b�g�{�[�h������������
//			}
//			//�r��
//			pushed++;
//		}
//	}
//
//	return allBoard != (pushDownedBoard & FILLED_BORD);//�ύX���ꂽ���ǂ���
//}
//
//
////attribute1��(row1, column1)��attribute2��(row2, column2)�����ւ���
////�K�������Ƀ{�[�����Ȃ���΂Ȃ�Ȃ�
////row��column�͍��ォ�琔��������
////0 <= attribute < ATTRIBUTE_NUM
//void swapBall(vector<int>& bitBoards, int attribute1, int row1, int column1, int attribute2, int row2, int column2)
//{
//	if (attribute1 == attribute2)
//	{
//		return;//�����������Ȃ����ւ���K�v���Ȃ�
//	}
//
//	int board1 = bitBoards[attribute1];
//	int board2 = bitBoards[attribute2];
//
//	board1 = board1 & (FILLED_BORD & ~(1 << W * (H - row1 - 1) + (W - column1 - 1)));//����ւ��O��0�ɂ���
//	board1 = board1 | (1 << W * (H - row2 - 1) + (W - column2 - 1));//����ւ����1�ɂ���
//	board1 &= FILLED_BORD;
//	board2 = board2 & (FILLED_BORD & ~(1 << W * (H - row2 - 1) + (W - column2 - 1)));//����ւ��O��0�ɂ���
//	board2 = board2 | (1 << W * (H - row1 - 1) + (W - column1 - 1));//����ւ����1�ɂ���
//	board2 &= FILLED_BORD;
//	bitBoards[attribute1] = board1;
//	bitBoards[attribute2] = board2;
//}
//
//
////(row, column)�ɂ���{�[���̑�������肷��
//int getAttribute(vector<int>& bitBoards, int row, int column)
//{
//	for (int i = 0; i < ATTRIBUTE_NUM; i++)
//	{
//		if ((bitBoards[i] >> W * (H - row - 1) + (W - column - 1)) & 1)
//			return i;
//	}
//	return -1;
//}
//
//
////(row1, column1)��(row2, column2)�����ւ���
////�K�������Ƀ{�[�����Ȃ���΂Ȃ�Ȃ�
////row��column�͍��ォ�琔��������
////����ւ�����(����ւ���Ӗ�������)���ǂ�����Ԃ�
//inline bool swapBall(vector<int>& bitBoards, int row1, int column1, int row2, int column2)
//{
//	int attribute1 = getAttribute(bitBoards, row1, column1);
//	int attribute2 = getAttribute(bitBoards, row2, column2);
//	if (attribute1 == attribute2) return false;//�����������Ȃ����ւ���Ӗ�������
//
//	swapBall(bitBoards, attribute1, row1, column2, attribute2, row2, column2);
//	return true;
//}
//
//
////�������̃R���{������
////���R���{��������Ԃ�
//int yokoCombo(int bitBoard, int& deletedBoard)
//{
//	//1��Â�
//	deletedBoard = bitBoard;
//	int comboCount = 0;
//	for (int i = 0, shifted = 0; i < H; i++, shifted += W)
//	{
//		int yoko = (bitBoard >> shifted) & YOKO_LINE;
//
//		auto pattern = comboPatterns[yoko];
//		if (pattern == NO_COMBO) continue;
//
//		comboCount++;
//
//		yoko = yoko & comboToDelete[yoko];
//		deletedBoard = (~(YOKO_LINE << shifted) & deletedBoard) | (yoko << shifted);
//	}
//	return comboCount;
//}
//
//
////�c�����̃R���{
////�Ȃ�R���{��������Ԃ�
//int tateCombo(int bitBoard, int& deletedBoard)
//{
//	deletedBoard = bitBoard;
//	int comboCount = 0;
//	for (int i = 0, shifted = W - 1; i < W; i++, shifted--)
//	{
//		int tate = (bitBoard << i) & TATE_LINE;
//		int yoko = tateToYoko(tate);
//
//		auto pattern = comboPatterns[yoko];
//		if (pattern == NO_COMBO) continue;
//
//		comboCount++;
//
//		int yokoDeleted = yoko & comboToDelete[yoko];
//		tate = yokoToTateTable[yokoDeleted];
//		deletedBoard = (~(TATE_LINE >> i) & deletedBoard) | (tate >> i);
//	}
//	return comboCount;
//}
//
////�R���{�ŏ�������
//int comboDelete(vector<int>& bitBoards)
//{
//	int comboCount = 0;
//	for (auto& board : bitBoards)
//	{
//		int yokoDeleted, tateDeleted;
//
//		comboCount += yokoCombo(board, yokoDeleted);
//		comboCount += tateCombo(board, tateDeleted);
//
//		board = yokoDeleted & tateDeleted;
//		//TODO: ���Əc���Ȃ����Ă�p�^�[���ŃR���{�J�E���g�������̂��C��
//		//(yokoDeleted & tateDeleted) ^ board��bitCount��yoko��tate��bitCount�ɃY�����������ꍇBFS���邩
//		//�����ꂽ������vector�Ŏ󂯎���ē�d���[�v�Ŕ���//�������̂ق��������C������
//	}
//	return comboCount;
//}
//
////�R���{����
////pair<int, int> memo[1 << 6]�̔z��ɂ��̉��̔z�u�łǂ�����ǂ��܂ŘA������Ă��邩�����O�Ɍv�Z���Ă���
////�c�̃R���{������bit�ɕϊ����Ă��̔z��Ō���
//
//
////�����L������ �O�̈ړ��̎Q�Ƃ�������������ǂ�B
////�Q�ƃJ�E���^���g���Ď����I�ɍ폜�����悤��
////�O�̈ړ��̎Q�Ƃ̎�������bit�̏�Ԃ��L������ܑ͖̖̂����̂ňړ��������{�[���̈ʒu�݂̂ɂ���B�����͈ʒu���画�ʂł���B�����bit�ɓ����Έʒu��8bit�ōςށB
//
//
///*��@����
//��{�I�Ɏ��ԂŐ���
//�܂����ʂ�bfs
//�����t��������
//�r�[���T�[�`
//�r�[�����𒲐�
//*/
//
//#pragma region test
//void test_toALLBoard(vector<int>& bitBoards)
//{
//	int allBoard = toAllBoard(bitBoards);
//	cout << toStringBitBoard(allBoard) << endl;
//}
//
//void test_pushDownBitBoard(vector<int>& bitBoards)
//{
//	int allBoard = toAllBoard(bitBoards);
//	cout << toStringBitBoard(allBoard) << endl;
//	int pushedBoard = pushDownBitBoard(allBoard, 0, 0);
//	pushDownBoards(bitBoards);
//	int i = 0;
//	for (auto board : bitBoards)
//	{
//		cout << ++i << endl;
//		cout << toStringBitBoard(board) << endl;
//	}
//}
//
//void test_swapBall(vector<int>& bitBoards)
//{
//	swapBall(bitBoards, 0, 0, 0, 1, 0, 1);
//	cout << toStringBitBoard(bitBoards[0]) << endl;
//	cout << toStringBitBoard(bitBoards[1]) << endl;
//	cout << toStringBitBoards(bitBoards) << endl;
//}
//
//void test_yokoCombo(vector<int>& bitBoards)
//{
//	int deleted;
//	int ret = yokoCombo(bitBoards[0], deleted);
//	cout << ret << endl;
//	cout << toStringBitBoard(deleted) << endl;
//}
//
//void test_tateCombo(vector<int>& bitBoards)
//{
//	int deleted;
//	int ret = tateCombo(bitBoards[0], deleted);
//	cout << ret << endl;
//	cout << toStringBitBoard(deleted) << endl;
//}
//
//void test_comboDeleted(vector<int>& bitBoards)
//{
//	int count = comboDelete(bitBoards);
//	cout << count << endl;
//	cout << toStringBitBoards(bitBoards) << endl;
//}
//
//#pragma endregion
int hoge;