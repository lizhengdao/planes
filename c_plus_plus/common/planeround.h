#ifndef __PLANE_ROUND_JAVAFX_
#define __PLANE_ROUND_JAVAFX_

#include "planegrid.h"
#include "computerlogic.h"
#include "gamestatistics.h"
#include "guesspoint.h"
#include "planeroundoptions.h"


struct PlayerGuessReaction {
	//victory for computer or player
	bool m_RoundEnds = false;
	//who won
	bool m_isPlayerWinner = false;
	//is draw
	bool m_isDraw = false;
	//if no victory then a computer move is generated
	bool m_ComputerMoveGenerated = false;
	//which computer move was generated 
	GuessPoint m_ComputerGuess;
	GameStatistics m_GameStats;

	PlayerGuessReaction() : m_ComputerGuess(0, 0) { }
};

class PlaneRound {
public:
	enum class GameStages { GameNotStarted = 0, BoardEditing = 1, Game = 2};

	PlaneRound(int rowNo, int colNo, int planeNo);
	~PlaneRound();

	//inits a new round
	void initRound();
	//switches to the state GameNotStarted
	void roundEnds();

	//checks if we are in state GameNotStarted
	bool didRoundEnd() {
		return m_State == GameStages::GameNotStarted;
	}

	/**
	@param[in] gp - the player's guess together with its evaluation
	@param[out] pgr - response to the player's guess: the computer's guess, if the game ended, winner, game statistics
	Plays a step in the game, as triggered by the player's guess gp.
	*/
	void playerGuess(const GuessPoint& gp, PlayerGuessReaction& pgr);

	/**
	@param[in] row, col - coordinates of player's guess
	Check if a guess was already made at this position.
	*/
	int playerGuessAlreadyMade(int row, int col);


	/**
		@param[in] row, col - coordinates of player's guess
		@param[out] guessRes - the evaluation of the player's guess
		@param[out] pgr - response to the player's guess: the computer's guess, if the game ended, winner, game statistics
		Plays a step in the game, as triggered by the the player's guess coordinates.
	*/
	void playerGuessIncomplete(int row, int col, GuessPoint::Type& guessRes, PlayerGuessReaction& pgr);

	/**
	Rotate the plane and return false if the current plane configuration is valid.
	*/
	bool rotatePlane(int idx);
	/**
	Move the plane left and return false if the current plane configuration is valid.
	*/
	bool movePlaneLeft(int idx);
	/**
	Move the plane right and return false if the current plane configuration is valid.
	*/
	bool movePlaneRight(int idx);
	/**
	Move the plane upwards and return false if the current plane configuration is valid.
	*/
	bool movePlaneUpwards(int idx);
	/**
	Move the plane downwards and return false if the current plane configuration is valid.
	*/
	bool movePlaneDownwards(int idx);

	void doneEditing();

	PlaneGrid* playerGrid() { return m_PlayerGrid; }
	PlaneGrid* computerGrid() { return m_ComputerGrid; }
	ComputerLogic* computerLogic() { return m_computerLogic; }

	int getRowNo() const {
		return m_rowNo;
	}

	int getColNo() const {
		return m_colNo;
	}

	int getPlaneNo() const {
		return m_planeNo;
	}

	int getPlaneSquareType(int i, int j, bool isComputer);

	int getPlayerGuessesNo() {
		return int(m_playerGuessList.size());
	}

	int getComputerGuessesNo() {
		return int(m_computerGuessList.size());
	}

	GuessPoint getPlayerGuess(int idx) {
		if (idx < 0 || idx >= int(m_playerGuessList.size()))
			return GuessPoint(-1, -1, GuessPoint::Miss);
		else
			return m_playerGuessList[idx];
	}

	GuessPoint getComputerGuess(int idx) {
		if (idx < 0 || idx >= int(m_computerGuessList.size()))
			return GuessPoint(-1, -1, GuessPoint::Miss);
		else
			return m_computerGuessList[idx];
	}

	int getCurrentStage() {
		return int(m_State);
	}

	/**
	Sets the computer skill. When this is during a game reject the change.
	**/
	bool setComputerSkill(int computerSkill);

	/**
	Sets the computer skill. When this is during a game reject the change.
	**/
	bool setShowPlaneAfterKill(bool showPlane);

	int getComputerSkill() {
		return m_RoundOptions.m_ComputerSkillLevel;
	}
	bool getShowPlaneAfterKill() {
		return m_RoundOptions.m_ShowPlaneAfterKill;
	}

private:
	//update game statistics
	void updateGameStats(const GuessPoint& gp, bool isComputer);
	//tests whether all of the planes have been guessed
	bool enoughGuesses(PlaneGrid* pg, const std::vector<GuessPoint>& guessList) const;
	//based on the available information makes the next move for the computer
	GuessPoint guessComputerMove();
	//resets the round
	void reset();
	//check to see if there is a winner
	bool roundEnds(bool& isPlayerWinner, bool& isComputerWinner);

	void updateGameStatsAndGuessListPlayer(const GuessPoint& gp);
	void updateGameStatsAndReactionComputer(PlayerGuessReaction& pgr);

private:
	//whether the computer or the player moves first
	bool m_isComputerFirst = false;
	//the  game statistics
	GameStatistics m_gameStats;

	PlaneRoundOptions m_RoundOptions;

	//the player and computer's grid
	PlaneGrid* m_PlayerGrid;
	PlaneGrid* m_ComputerGrid;

	//the list of guesses for computer and player
	std::vector<GuessPoint> m_computerGuessList;
	std::vector<GuessPoint> m_playerGuessList;

	//the computer's strategy
	ComputerLogic* m_computerLogic;

	GameStages m_State = GameStages::GameNotStarted;

	//size of the grid and number of planes
	int m_rowNo = 10;
	int m_colNo = 10;
	int m_planeNo = 3;
};


#endif