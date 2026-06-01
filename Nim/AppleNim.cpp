// This program demonstrates how minimax algorithm can be used for finding the best move in this game.
// Note that finding best move in this game is attainable with a better arithmetic method.
// So don't use minimax algorithm for this game in a production code.

//	About APPLE NIM
//	A small number of apples are placed on a basket.
//	Players can remove 1, 2, 3 or 4 apples in turns.
//	Whoever picks the last apple loses the game.
//

#include <iostream>
#define APPLE_NIM_DEBUG
#define APPLE_NIM_INFINITY 32000
#define BESTSCORE 1000
enum { NONE = -1, COMPUTER = 0, HUMAN = 1 };
enum { MODULO, NAIVE_MINIMAX, ALPHA_BETA_MINIMAX };

class AppleNim
{
private:
	int m_numTotalApples;
	int m_turn;
	int m_algorithmIndex;
#ifdef APPLE_NIM_DEBUG
	int m_maxDepth;
	int m_nodesSearched;
#endif
public:
	AppleNim(void);
	void Init(int numApples, int turn, int algorithm);
	void GameLoop();
private:
	int FindBestMoveWithModuloOperator(int numTotalApples);//best method, use this function in production code
	int FindBestMoveWithMiniMaxAlphaBeta(int numTotalApples);//just for minimax algorithm demonstration, don't use this function in production code
	int FindBestMoveWithMiniMax(int numTotalApples);//just for minimax algorithm demonstration, don't use this function in production code
	int MiniMaxAlphaBeta(int numTotalApples, bool maximizingPlayer, int depth, int alpha, int beta);
	int MiniMax(int numTotalApples, bool maximizingPlayer, int depth);
};

AppleNim::AppleNim()
{
	m_numTotalApples = 21;
	m_turn = HUMAN;
	m_algorithmIndex = MODULO;
}

void AppleNim::Init(int numApples, int turn, int algorithm)
{
	m_numTotalApples = numApples;
	m_turn = turn;
	m_algorithmIndex = algorithm;
	switch (algorithm) {
	case MODULO:
		std::cout << "Modulo selected\n";
		break;
	case NAIVE_MINIMAX:
		std::cout << "MiniMax selected\n";
		break;
	case ALPHA_BETA_MINIMAX:
		std::cout << "MiniMax+alpha beta pruning selected\n";
		break;
	}
}

void AppleNim::GameLoop()
{
	int pick=1;
	bool invalidInput = false;
	std::cout << "***ABOUT APPLE NIM***\n";
	std::cout << "A small number of apples are placed on a basket.\n";
	std::cout << "Players can remove 1, 2, 3 or 4 apples in turns.\n";
	std::cout << "Whoever picks the last apple loses the game.\n";
	std::cout << "***START***\n";
	while (m_numTotalApples > 0)
	{
		if (m_turn == HUMAN) {
			std::cout << "Number of apples left: [[ " << m_numTotalApples << " ]]\n";
			std::cout << "***YOUR TURN***\n";
			do {
				std::cout << "Enter your pick: ";
				std::cin >> pick;
				if (invalidInput = (pick < 0 || pick > 4 || (m_numTotalApples - pick) < 0)){
					std::cout << "Invalid input\n";
				}
			} while (invalidInput);
			m_numTotalApples -= pick;
			m_turn = (m_turn + 1) % 2;
		}
		else if (m_turn == COMPUTER) {
			std::cout << "Number of apples left: [ " << m_numTotalApples << " ]\n";
			std::cout << "***COMPUTER'S TURN***\n";
			switch (m_algorithmIndex) {
			case MODULO:
				pick = FindBestMoveWithModuloOperator(m_numTotalApples);
				break;
			case NAIVE_MINIMAX:
				pick = FindBestMoveWithMiniMax(m_numTotalApples);
				break;
			case ALPHA_BETA_MINIMAX:
				pick = FindBestMoveWithMiniMaxAlphaBeta(m_numTotalApples);
				break;
			default:
				std::cout << "Invalid Algorithm\n";
				return;
			}
			m_numTotalApples -= pick;
			m_turn = (m_turn + 1) % 2;
			std::cout << "Computer picks ( ";
			if (pick != 1)
				std::cout << pick << " ) apples.\n";
			else
				std::cout << pick << " ) apple.\n";
		}
		if (m_numTotalApples == 0) {
			std::cout << "Empty basket! \n";
			if (m_turn == COMPUTER)
				std::cout << "You lose.\n";
			else
				std::cout << "You win.\n";
		}
	}

	std::cout << "***Game Over***\n";
}

//-------------------- modulo ------------------------------
int AppleNim::FindBestMoveWithModuloOperator(int numTotalApples)
{
	for (int pick = 4; pick > 0; --pick)
		if ((numTotalApples - pick) % 5 == 1)
			return pick;
	return 1;
}

//-------------------- mini max with alpha beta pruning ------------------------------
int AppleNim::FindBestMoveWithMiniMaxAlphaBeta(int numTotalApples)
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
			int value = MiniMaxAlphaBeta(numTotalApples, false, 1, -APPLE_NIM_INFINITY, APPLE_NIM_INFINITY);
			if (value > best) {
				best = value;
				bestPick = pick;
			}
		}
		numTotalApples = numTotalApples + pick;
	}
#ifdef APPLE_NIM_DEBUG
	std::cout << "\nSearched " << m_nodesSearched << " nodes at a maximum depth of " << m_maxDepth << '\n';
#endif	
	return bestPick;
}

int AppleNim::MiniMaxAlphaBeta(int totalApples, bool maximizingPlayer, int depth, int alpha, int beta)
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
				int value = MiniMaxAlphaBeta(totalApples, false, depth + 1, alpha, beta);
				if (value > best) {
					best = value;
				}
			}
			totalApples = totalApples + pick;
			if (best > alpha) {
				alpha = best;
			}
			if (alpha >= beta)
				break;
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
				int value = MiniMaxAlphaBeta(totalApples, true, depth + 1, alpha, beta);
				if (value < best) {
					best = value;
				}
			}
			totalApples = totalApples + pick;
			if (best < beta) {
				beta = best;
			}
			if (beta <= alpha)
				break;

		}
		return best;
	}
}

//-------------------- naive mini max ------------------------------
int AppleNim::FindBestMoveWithMiniMax(int numTotalApples)
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
	std::cout << "\nSearched " << m_nodesSearched << " nodes at a maximum depth of " << m_maxDepth << '\n';
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
	//n.Init(26, HUMAN, MODULO);
	n.Init(26, HUMAN, ALPHA_BETA_MINIMAX);
	//n.Init(26, HUMAN, NAIVE_MINIMAX);
	
	n.GameLoop();
	return 0;
}

//Losing baskets 1, 6, 11, 16, 21, 26, 31
