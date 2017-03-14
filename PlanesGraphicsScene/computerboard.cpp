#include "computerboard.h"
#include "playareagridsquare.h"

#include <QDebug>


ComputerBoard::ComputerBoard(PlaneGrid& grid, int squareWidth) : GenericBoard(grid, squareWidth) {
    connect(m_Scene, SIGNAL(gridSquareClicked(int, int)), this, SLOT(gridSquareClicked(int, int)));
}

void ComputerBoard::gridSquareClicked(int row, int col)
{
    QPoint qp(row, col);
    GuessPoint::Type tp = m_Grid.getGuessResult(qp);

    qDebug() << "Guess " << tp;

    //the m_grid object returns whether is a miss, hit or dead
    //with this data builda guess point object
    GuessPoint gp(qp.x(), qp.y(), tp);

    //verify if the guess point is not already in the list
    //emit a signal that the guess has been made
    if(m_GuessList.indexOf(gp)==-1)
    {
        m_GuessList.append(gp);
        //to not let the user draw while the computer is thinking
//        m_currentMode = Editor;
        emit guessMade(gp);
        hidePlanes();
        displayPlanes();
        displayGuesses();
    }
}
