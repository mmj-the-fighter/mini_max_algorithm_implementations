// This program demonstrates how minimax algorithm can be used for finding the best move in this game.
// Note that finding best move in this game is attainable with a better arithmetic method.
// So don't use minimax algorithm for this game in a production code.

//	About APPLE NIM
//	A small number of apples are placed on a basket.
//	Players can remove 1, 2, 3 or 4 apples in turns.
//	Whoever picks the last apple loses the game.
//

#include <iostream>
using namespace std;
//#define APPLE_NIM_DEBUG
#define APPLE_NIM_INFINITY 32000
#define BESTSCORE 1000
enum { NONE = -1, COMPUTER = 0, HUMAN = 1 };

class AppleNim
{
private:
	int m_numTotalApples;
	int m_turn;
	int m_useMiniMaxAI;
#ifdef APPLE_NIM_DEBUG
	int m_maxDepth;
	int m_nodesSearched;
#endif
public:
	AppleNim(void);
	void Init(int numApples, int turn, bool useMiniMaxAI = false);
	void GameLoop();
private:
	int FindBestMoveWithModuloOperator(int numTotalApples);//best method, use this function in production code
	int FindBestMoveWithSearchTree(int numTotalApples);//just for minimax algorithm demonstration, don't use this function in production code
	int MiniMax(int numTotalApples, bool maximizingPlayer, int depth);
};

AppleNim::AppleNim(void)
{
	m_numTotalApples = 21;
	m_turn = HUMAN;
	m_useMiniMaxAI = false;
}

void AppleNim::Init(int numApples, int turn, bool useMiniMaxAI)
{
	m_numTotalApples = numApples;
	m_turn = turn;
	m_useMiniMaxAI = useMiniMaxAI;
}

void AppleNim::GameLoop()
{
	int pick;
	bool invalidInput = false;
	cout << "***ABOUT APPLE NIM***" << endl;
	cout << "A small number of apples are placed on a basket." << endl;
	cout << "Players can remove 1, 2, 3 or 4 apples in turns." << endl;
	cout << "Whoever picks the last apple loses the game." << endl;
	cout << "***START***" << endl;
	while (m_numTotalApples > 0)
	{
		if (m_turn == HUMAN) {
			cout << "Number of apples left: [[ " << m_numTotalApples << " ]]" << endl;
			cout << "***YOUR TURN***" << endl;
			do {
				cout << "Enter your pick: ";
				cin >> pick;
				if (invalidInput = (pick < 0 || pick > 4 || (m_numTotalApples - pick) < 0)){
					cout << "Invalid input" << endl;
				}
			} while (invalidInput);
			m_numTotalApples -= pick;
			m_turn = (m_turn + 1) % 2;
		}
		else if (m_turn == COMPUTER) {
			cout << "Number of apples left: [ " << m_numTotalApples << " ]" << endl;
			cout << "***COMPUTER'S TURN***" << endl;
			if (!m_useMiniMaxAI)
				pick = FindBestMoveWithModuloOperator(m_numTotalApples);
			else
				pick = FindBestMoveWithSearchTree(m_numTotalApples);
			m_numTotalApples -= pick;
			m_turn = (m_turn + 1) % 2;
			cout << "Computer picks ( ";
			if (pick != 1)
				cout << pick << " ) apples." << endl;
			else
				cout << pick << " ) apple." << endl;
		}
		if (m_numTotalApples == 0) {
			cout << "Empty basket! " << endl;
			if (m_turn == COMPUTER)
				cout << "You lose." << endl;
			else
				cout << "You win." << endl;
		}
	}

	cout << "***Game Over***" << endl;
}

int AppleNim::FindBestMoveWithModuloOperator(int numTotalApples)
{
	for (int pick = 4; pick > 0; --pick)
		if ((numTotalApples - pick) % 5 == 1)
			return pick;
	return 1;
}


int AppleNim::FindBestMoveWithSearchTree(int numTotalApples)
{
#ifdef APPLE_NIM_DEBUG
	m_maxDepth = 0;
	m_nodesSearched = 0;
#endif
	int best = -APPLE_NIM_INFINITY;
	int bestPick = 0;
	for (int pick = 4; pick > 0; --pick) {
		numTotalApples = numTotalApples - pick;
		if (numTotalApples >= 0) {
#ifdef APPLE_NIM_DEBUG
			++m_nodesSearched;
#endif	
			int value = MiniMax(numTotalApples, false, 1);
			if (value > best) {
				best = value;
				bestPick = pick;
			}
		}
		numTotalApples = numTotalApples + pick;
	}
#ifdef APPLE_NIM_DEBUG
	cout << endl << "Searched " << m_nodesSearched << " nodes at a maximum depth of " << m_maxDepth << endl;
#endif	
	return bestPick;
}

int AppleNim::MiniMax(int totalApples, bool maximizingPlayer, int depth)
{
#ifdef APPLE_NIM_DEBUG
	if (depth > m_maxDepth)
		m_maxDepth = depth;
#endif
	if (totalApples == 0) {
		if (maximizingPlayer)
			return (BESTSCORE - depth); // computer is presented with empty basket
		else
			return -(BESTSCORE - depth);//human is presented with empty basket
	}
	else if (totalApples == 1) {
		if (maximizingPlayer)
			return -(BESTSCORE - depth);//computer is presented with one apple
		else
			return BESTSCORE - depth;//human is presented with one apple
	}
	else if (maximizingPlayer) {
		int best = -APPLE_NIM_INFINITY;
		for (int pick = 4; pick > 0; --pick) {
			totalApples = totalApples - pick;
			if (totalApples >= 0) {
#ifdef APPLE_NIM_DEBUG
				++m_nodesSearched;
#endif	
				int value = MiniMax(totalApples, false, depth + 1);
				if (value > best) {
					best = value;
				}
			}
			totalApples = totalApples + pick;
		}
		return best;
	}
	else
	{
		int best = APPLE_NIM_INFINITY;
		for (int pick = 4; pick > 0; --pick) {
			totalApples = totalApples - pick;
			if (totalApples >= 0) {
#ifdef APPLE_NIM_DEBUG
				++m_nodesSearched;
#endif	
				int value = MiniMax(totalApples, true, depth + 1);
				if (value < best) {
					best = value;
				}
			}
			totalApples = totalApples + pick;
		}
		return best;
	}
}

int main()
{
	AppleNim n;
	n.Init(26, HUMAN, true);
	//n.Init(26, HUMAN, false);
	n.GameLoop();
	return 0;
}

//Losing baskets 1, 6, 11, 16, 21, 26, 31
