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

			length = std::min(length, MAX_ROUTE_SIZE);//大きさを制限

			auto at = _routes[aheads][0];
			int loopCount = (length - 1) / _routes[aheads][0].size() + !!((length - 1) % aheads);//lengthまでは必ず探索できるようににループの回数を決める
			for (int k = 0; k < loopCount; k++)
			{
				preRoutes.clear();
				for (int i = 0; i < beamWeight && !ordered.empty(); i++)
				{
					preRoutes.push_back(ordered.top().second);
					ordered.pop();
				}
				//ordered = std::priority_queue<std::pair<int, Route>>();//ここを変えるべきか否か

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
							if (rs + as > length) continue;//サイズが保存できる最大の大きさを超えるのを禁止

							////追加するルートの一回目の移動が意味のないものである場合はそのルートから始まるものを飛ばす
							////なんか精度落ちた。バグってる？
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
			//ここベタ打ちぇ...
			return (row == 0 && direction == 0) || (row == HEIGHT - 1 && direction == 2) ||
				(column == 0 && direction == 3) || (column == WIDTH - 1 && direction == 1);
		}

		//二重制限探索
		//setを使うことで削除処理を低い順からできるように
		//ドロップの移動処理もちょっと効率化
		//beamWeight == limitWeightでビームサーチ
		//beamWeight == INFで幅優先探索
		//beamWeight == 1で貪欲
		Route doubleLimitedSearch(BOARD& board, int beamWeight, int limitWeight, int startRow, int startColumn, int& outScore)
		{
			//実際に処理されるルート
			std::vector<Route> preRoutes(beamWeight);
			//次のルートの候補
			std::set<std::pair<int, Route>> ordered;

			const int DIR_NUM = 4;//方向の数 (あとで外で定義した方がいい)
			for (int i = 0; i < DIR_NUM; i++)
			{
				//無駄カット
				if (outOfRangeMoved(startRow, startColumn, i)) continue;

				ordered.emplace(0, Route(i));
			}
			//最も良いルートを保持
			Route ansRoute;
			//最も良いルートのスコアを保持
			int maxCombo = -1;
			//最も良いルートの長さを保持
			int maxComboRouteLength = (1 << 30);


			//最大幅から
			for (int k = 0; k < MAX_ROUTE_SIZE; k++)
			{
				preRoutes.clear();

				//次の状態を
				for (int i = 0; i < beamWeight && !ordered.empty(); i++)
				{
					auto next = --ordered.end();
					preRoutes.push_back(next->second);
					ordered.erase(next);
				}
				//ordered.clear();//制限幅をビーム幅を同じにすれば同等の状況になるのでいらない

				for (const auto& route : preRoutes)
				{
					BOARD preBoard = board;//同じmoveByRouteをやる回数を少なく
					const int prePosition = moveByRoute(preBoard, route, startRow, startColumn);
					//前回の移動での最終的な場所
					const int preRow = prePosition & ((1 << 3) - 1);
					const int preColumn = (prePosition >> 3) & ((1 << 3) - 1);

					//前回の移動の方向
					const int preMovement = route.top();

					//4方向全探索
					for (int j = 0; j < DIR_NUM; j++)
					{
						//行って帰ってくるパターン or 無効な移動なら処理しない
						if ((preMovement + 2) % 4 == j || outOfRangeMoved(preRow, preColumn, j)) continue;

						//次の状態の作成
						BOARD tmpBoard = preBoard;
						Route next = route;
						next.push_back(j);

						//ドロップを移動
						slideDrop_4(tmpBoard, preRow, preColumn, j);

						//コンボ数の大きさ＝評価値じゃなくなった時対策
						int nextCombo;
						int nextScore = evalProcess(tmpBoard, nextCombo);

						//スコアを更新できるとき or スコアが同じでルートの長さがより短い時はスコア更新
						const int routeSize = next.size();
						if (nextCombo > maxCombo || (nextCombo == maxCombo && routeSize < maxComboRouteLength))
						{
							maxCombo = nextCombo;
							ansRoute = next;
							maxComboRouteLength = routeSize;
						}

						//ルートにもう追加できない場合は次の状態に追加しない
						if (next.filled()) continue;

						//次の状態に追加
						ordered.emplace(nextScore, next);

						//制限幅より多ければ小さいものから削除
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

		//優良優先制限探索探索
		//前回の探索と今回の探索を混ぜて処理する
		//loopLengthで実行時間を制御
		Route prioritySearch(BOARD& board, int limitWeight, int loopLength, int startRow, int startColumn, int& outScore)
		{
			//ルートの候補
			std::set<std::pair<int, Route>> ordered;

			const int DIR_NUM = 4;//方向の数 (あとで外で定義した方がいい)
			for (int i = 0; i < DIR_NUM; i++)
			{
				//無駄カット
				if (outOfRangeMoved(startRow, startColumn, i)) continue;

				ordered.emplace(0, Route(i));
			}

			//最も良いルートを保持
			Route ansRoute;
			//最も良いルートのスコアを保持
			int maxCombo = -1;
			//最も良いルートの長さを保持
			int maxComboRouteLength = (1 << 30);


			//次の状態を探索
			for (int k = 0; k < loopLength && !ordered.empty(); k++)
			{
				auto preState = --ordered.end();
				auto& route = preState->second;
				ordered.erase(preState);

				BOARD preBoard = board;//同じmoveByRouteをやる回数を少なく
				const int prePosition = moveByRoute(preBoard, route, startRow, startColumn);
				//前回の移動での最終的な場所
				const int preRow = prePosition & ((1 << 3) - 1);
				const int preColumn = (prePosition >> 3) & ((1 << 3) - 1);

				//前回の移動の方向
				const int preMovement = route.top();

				//4方向全探索
				for (int j = 0; j < DIR_NUM; j++)
				{
					//行って帰ってくるパターン or 無効な移動なら処理しない
					if ((preMovement + 2) % 4 == j || outOfRangeMoved(preRow, preColumn, j)) continue;

					//次の状態の作成
					BOARD tmpBoard = preBoard;
					Route next = route;
					next.push_back(j);

					//ドロップを移動
					slideDrop_4(tmpBoard, preRow, preColumn, j);

					//コンボ数の大きさ＝評価値じゃなくなった時対策
					int nextCombo;
					int nextScore = evalProcess(tmpBoard, nextCombo);

					//スコアを更新できるとき or スコアが同じでルートの長さがより短い時はスコア更新
					const int routeSize = next.size();
					if (nextCombo > maxCombo || (nextCombo == maxCombo && routeSize < maxComboRouteLength))
					{
						maxCombo = nextCombo;
						ansRoute = next;
						maxComboRouteLength = routeSize;
					}

					//ルートにもう追加できない場合は次の状態に追加しない
					if (next.filled()) continue;

					//次の状態に追加
					ordered.emplace(nextScore, next);

					//制限幅より多ければ小さいものから削除
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
		limit = 100000;//適当な数値
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