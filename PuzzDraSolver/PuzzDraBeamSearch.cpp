#include "PuzzDraBeamSearch.h"
#include "PuzzDraTests.h"

#include <string>
#include <set>

namespace tnkt37
{
	namespace PuzzDraSolver
	{
		Route beamSearch(BOARD& board, int aheads, int length, int beamWeight, int startRow, int startColumn, int& outScore)
		{
			std::vector<Route> preRoutes(beamWeight);
			std::priority_queue<std::pair<int, Route>> ordered;
			for (const auto& route : _routes[1])
				ordered.emplace(0, route);

			Route ansRoute;
			int evalMax = -1;

			length = std::min(length, MAX_ROUTE_SIZE);//�傫���𐧌�

			auto at = _routes[aheads][0];
			int loopCount = (length - 1) / _routes[aheads][0].size() + !!((length - 1) % aheads);//length�܂ł͕K���T���ł���悤�ɂɃ��[�v�̉񐔂����߂�
			for (int k = 0; k < loopCount; k++)
			{
				preRoutes.clear();
				for (int i = 0; i < beamWeight && !ordered.empty(); i++)
				{
					preRoutes.push_back(ordered.top().second);
					ordered.pop();
				}
				//ordered = std::priority_queue<std::pair<int, Route>>();//������ς���ׂ����ۂ�

				for (int i = 1; i <= aheads; i++)
				{
					const int size = _routes[i].size();
					for (const auto& route : preRoutes)
					{
						for (int j = 0; j < size; j++)
						{
							const auto& added = _routes[i][j];

							vector<vector<int>> tmpBoard = board;
							Route next = route;
							Route dummy;

							const int rs = route.size();
							const int as = added.size();
							if (rs + as > length) continue;//�T�C�Y���ۑ��ł���ő�̑傫���𒴂���̂��֎~

							////�ǉ����郋�[�g�̈��ڂ̈ړ����Ӗ��̂Ȃ����̂ł���ꍇ�͂��̃��[�g����n�܂���̂��΂�
							////�Ȃ񂩐��x�������B�o�O���Ă�H
							//const int top = added.top();
							//if (top != -1 && (top + 2) % 4 == route.getBits(0) && size >= 4)
							//{
							//	j += size / 4 - 1;
							//	continue;
							//}

							next.push_back(added);
							if (simplifyRoute(next, startRow, startColumn, dummy) > 0)
								continue;

							int ms = next.size();
							moveByRoute(tmpBoard, next, startRow, startColumn);
							int evalNum = evalProcess(tmpBoard);

							if (evalNum > evalMax)
							{
								evalMax = evalNum;
								ansRoute = next;
							}
							ordered.emplace(evalNum, next);
						}
					}
				}
			}
			outScore = evalMax;
			return ansRoute;
		}

		bool outOfRangeMoved(int row, int column, int direction)
		{
			//�����x�^�ł���...
			return (row == 0 && direction == 0) || (row == HEIGHT - 1 && direction == 2) ||
				(column == 0 && direction == 3) || (column == WIDTH - 1 && direction == 1);
		}

		//��d�����T��
		//set���g�����Ƃō폜������Ⴂ������ł���悤��
		//�h���b�v�̈ړ�������������ƌ�����
		//beamWeight == limitWeight�Ńr�[���T�[�`
		//beamWeight == INF�ŕ��D��T��
		//beamWeight == 1���×~
		Route doubleLimitedSearch(BOARD& board, int beamWeight, int limitWeight, int startRow, int startColumn, int& outScore)
		{
			//���ۂɏ�������郋�[�g
			std::vector<Route> preRoutes(beamWeight);
			//���̃��[�g�̌��
			std::set<std::pair<int, Route>> ordered;

			const int DIR_NUM = 4;//�����̐� (���ƂŊO�Œ�`������������)
			for (int i = 0; i < DIR_NUM; i++)
			{
				//���ʃJ�b�g
				if (outOfRangeMoved(startRow, startColumn, i)) continue;

				ordered.emplace(0, Route(i));
			}
			//�ł��ǂ����[�g��ێ�
			Route ansRoute;
			//�ł��ǂ����[�g�̃X�R�A��ێ�
			int maxCombo = -1;
			//�ł��ǂ����[�g�̒�����ێ�
			int maxComboRouteLength = (1 << 30);


			//�ő啝����
			for (int k = 0; k < MAX_ROUTE_SIZE; k++)
			{
				preRoutes.clear();

				//���̏�Ԃ�
				for (int i = 0; i < beamWeight && !ordered.empty(); i++)
				{
					auto next = --ordered.end();
					preRoutes.push_back(next->second);
					ordered.erase(next);
				}
				//ordered.clear();//���������r�[�����𓯂��ɂ���Γ����̏󋵂ɂȂ�̂ł���Ȃ�

				for (const auto& route : preRoutes)
				{
					BOARD preBoard = board;//����moveByRoute�����񐔂����Ȃ�
					const int prePosition = moveByRoute(preBoard, route, startRow, startColumn);
					//�O��̈ړ��ł̍ŏI�I�ȏꏊ
					const int preRow = prePosition & ((1 << 3) - 1);
					const int preColumn = (prePosition >> 3) & ((1 << 3) - 1);

					//�O��̈ړ��̕���
					const int preMovement = route.top();

					//4�����S�T��
					for (int j = 0; j < DIR_NUM; j++)
					{
						//�s���ċA���Ă���p�^�[�� or �����Ȉړ��Ȃ珈�����Ȃ�
						if ((preMovement + 2) % 4 == j || outOfRangeMoved(preRow, preColumn, j)) continue;

						//���̏�Ԃ̍쐬
						BOARD tmpBoard = preBoard;
						Route next = route;
						next.push_back(j);

						//�h���b�v���ړ�
						slideDrop_4(tmpBoard, preRow, preColumn, j);

						//�R���{���̑傫�����]���l����Ȃ��Ȃ������΍�
						int nextCombo;
						int nextScore = evalProcess(tmpBoard, nextCombo);

						//�X�R�A���X�V�ł���Ƃ� or �X�R�A�������Ń��[�g�̒��������Z�����̓X�R�A�X�V
						const int routeSize = next.size();
						if (nextCombo > maxCombo || (nextCombo == maxCombo && routeSize < maxComboRouteLength))
						{
							maxCombo = nextCombo;
							ansRoute = next;
							maxComboRouteLength = routeSize;
						}

						//���[�g�ɂ����ǉ��ł��Ȃ��ꍇ�͎��̏�Ԃɒǉ����Ȃ�
						if (next.filled()) continue;

						//���̏�Ԃɒǉ�
						ordered.emplace(nextScore, next);

						//��������葽����Ώ��������̂���폜
						if (ordered.size() > limitWeight)
						{
							ordered.erase(ordered.begin());
						}

					}
				}
			}
			outScore = maxCombo;
			return ansRoute;
		}

		//�D�ǗD�搧���T���T��
		//�O��̒T���ƍ���̒T���������ď�������
		//loopLength�Ŏ��s���Ԃ𐧌�
		Route prioritySearch(BOARD& board, int limitWeight, int loopLength, int startRow, int startColumn, int& outScore)
		{
			//���[�g�̌��
			std::set<std::pair<int, Route>> ordered;

			const int DIR_NUM = 4;//�����̐� (���ƂŊO�Œ�`������������)
			for (int i = 0; i < DIR_NUM; i++)
			{
				//���ʃJ�b�g
				if (outOfRangeMoved(startRow, startColumn, i)) continue;

				ordered.emplace(0, Route(i));
			}

			//�ł��ǂ����[�g��ێ�
			Route ansRoute;
			//�ł��ǂ����[�g�̃X�R�A��ێ�
			int maxCombo = -1;
			//�ł��ǂ����[�g�̒�����ێ�
			int maxComboRouteLength = (1 << 30);


			//���̏�Ԃ�T��
			for (int k = 0; k < loopLength && !ordered.empty(); k++)
			{
				auto preState = --ordered.end();
				auto& route = preState->second;
				ordered.erase(preState);

				BOARD preBoard = board;//����moveByRoute�����񐔂����Ȃ�
				const int prePosition = moveByRoute(preBoard, route, startRow, startColumn);
				//�O��̈ړ��ł̍ŏI�I�ȏꏊ
				const int preRow = prePosition & ((1 << 3) - 1);
				const int preColumn = (prePosition >> 3) & ((1 << 3) - 1);

				//�O��̈ړ��̕���
				const int preMovement = route.top();

				//4�����S�T��
				for (int j = 0; j < DIR_NUM; j++)
				{
					//�s���ċA���Ă���p�^�[�� or �����Ȉړ��Ȃ珈�����Ȃ�
					if ((preMovement + 2) % 4 == j || outOfRangeMoved(preRow, preColumn, j)) continue;

					//���̏�Ԃ̍쐬
					BOARD tmpBoard = preBoard;
					Route next = route;
					next.push_back(j);

					//�h���b�v���ړ�
					slideDrop_4(tmpBoard, preRow, preColumn, j);

					//�R���{���̑傫�����]���l����Ȃ��Ȃ������΍�
					int nextCombo;
					int nextScore = evalProcess(tmpBoard, nextCombo);

					//�X�R�A���X�V�ł���Ƃ� or �X�R�A�������Ń��[�g�̒��������Z�����̓X�R�A�X�V
					const int routeSize = next.size();
					if (nextCombo > maxCombo || (nextCombo == maxCombo && routeSize < maxComboRouteLength))
					{
						maxCombo = nextCombo;
						ansRoute = next;
						maxComboRouteLength = routeSize;
					}

					//���[�g�ɂ����ǉ��ł��Ȃ��ꍇ�͎��̏�Ԃɒǉ����Ȃ�
					if (next.filled()) continue;

					//���̏�Ԃɒǉ�
					ordered.emplace(nextScore, next);

					//��������葽����Ώ��������̂���폜
					if (ordered.size() > limitWeight)
					{
						ordered.erase(ordered.begin());
					}

				}
			}

			outScore = maxCombo;
			return ansRoute;
		}

		void tnkt37::PuzzDraSolver::beamSearchTest()
		{
			int aheads, length, beamWeight;
			//cin >> aheads >> length >> beamWeight;
			aheads = 1;
			length = MAX_ROUTE_SIZE;
			cin >> beamWeight;

			while (true)
			{
				auto board = generateRandomBoard();

				int evalMax = -1;
				Route ansRoute;
				int row = 0, column = 0;
				for (int i = 0; i < HEIGHT; i++)
				{
					for (int j = 0; j < WIDTH; j++)
					{
						auto tmpBoard = board;
						int evalNum;
						Route route = beamSearch(tmpBoard, aheads, length, beamWeight, i, j, evalNum);

						if (evalNum > evalMax)
						{
							evalMax = evalNum;
							ansRoute = route;
							row = i;
							column = j;
						}
					}
				}

				debug(board);
				cout << row << ", " << column << "  size: " << ansRoute.size() << endl;
				debug(ansRoute);

				moveByRoute(board, ansRoute, row, column);
				debug(board);
				evalProcess(board);
				debug(board);
				cout << evalMax << endl;

				string t;
				std::getline(cin, t);
				if (t == "change")
				{
					cin >> aheads >> length >> beamWeight;
				}
			}

		}
	}

	void tnkt37::PuzzDraSolver::doubleLimitedSearchTest()
	{
		int aheads, length, beamWeight, limit;
		//cin >> aheads >> length >> beamWeight;
		aheads = 1;
		length = MAX_ROUTE_SIZE;
		limit = 100000;//�K���Ȑ��l
		cin >> beamWeight;

		while (true)
		{
			auto board = generateRandomBoard();

			//auto board = inputBoard();

			int evalMax = -1;
			int maxRouteLength = (1 << 30);
			Route ansRoute;
			int row = 0, column = 0;
			for (int i = 0; i < HEIGHT; i++)
			{
				for (int j = 0; j < WIDTH; j++)
				{
					auto tmpBoard = board;
					int evalNum;
					Route route = doubleLimitedSearch(tmpBoard, beamWeight, limit, i, j, evalNum);
					//Route route = beamSearch(tmpBoard, 1, MAX_ROUTE_SIZE, beamWeight, i, j, evalNum);


					if (evalNum > evalMax || (evalNum == evalMax && route.size() < maxRouteLength))
					{
						evalMax = evalNum;
						ansRoute = route;
						row = i;
						column = j;
						maxRouteLength = route.size();
						//cout << row << ", " << column << "  size: " << ansRoute.size() << "\r\n" << "evalMax: " << evalMax << endl;
						//debug(route);
						//Route tmp;
						//simplifyRoute(route, row, column, tmp);
						//debug(tmp);
						//cout << evalMax << endl;
					}
				}
			}

			debug(board);
			cout << row << ", " << column << "  size: " << ansRoute.size() << "\r\n" << "evalMax: " << evalMax << endl;
			debug(ansRoute);

			moveByRoute(board, ansRoute, row, column);
			debug(board);
			int out_combo;
			int score = evalProcess(board, out_combo);
			debug(board);
			cout << out_combo << endl;

			string t;
			std::getline(cin, t);
			if (t == "change")
			{
				cin >> beamWeight;
			}
		}

	}

}