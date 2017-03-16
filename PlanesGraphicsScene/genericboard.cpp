#include "genericboard.h"
#include "playareagridsquare.h"

#include <QDebug>

GenericBoard::GenericBoard(PlaneGrid& grid, int squareWidth) : m_Grid(grid), m_SquareWidth(squareWidth)
{
    m_Scene = new CustomGraphicsScene();
    m_View = new QGraphicsView(m_Scene);
}

void GenericBoard::reset()
{
    clearBoard();
    m_CurStage = GameStages::BoardEditing;
    generateBoardItems();
    displayPlanes();
}

///@todo: deleted the items from m_SceneItems
void GenericBoard::clearBoard()
{
    m_Scene->clear();
}

void GenericBoard::generateBoardItems()
{
    ///generate player scene items
    int rows = m_Grid.getRowNo() + 2 * m_PaddingEditingBoard;
    int cols = m_Grid.getColNo() + 2 * m_PaddingEditingBoard;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (i < m_PaddingEditingBoard || abs(i - rows) <= m_PaddingEditingBoard
                    || j < m_PaddingEditingBoard || abs (j - cols) <= m_PaddingEditingBoard) {
                GridSquare* br = new GridSquare(i, j, m_SquareWidth);
                m_Scene->addItem(br);
                br->setPos(j * m_SquareWidth, i * m_SquareWidth);
                m_SceneItems[std::make_pair(i, j)] = br;
            } else {
                PlayAreaGridSquare* pabr = new PlayAreaGridSquare(i, j, m_SquareWidth);
                m_Scene->addItem(pabr);
                pabr->setPos(j * m_SquareWidth, i * m_SquareWidth);
                m_SceneItems[std::make_pair(i,j)] = pabr;
            }
        }
}

///@todo: deal with overlapping planes
void GenericBoard::displayPlanes() {
    int colorStep = (m_MaxPlaneBodyColor - m_MinPlaneBodyColor) / m_Grid.getPlaneNo();
    for (int i = 0; i < m_Grid.getPlaneNo(); i++) {
        Plane pl;
        int greyLevel = m_MinPlaneBodyColor + i * colorStep;
        if (!m_Grid.getPlane(i, pl))
            continue;
        if (i != m_SelectedPlane)
            showPlane(pl, QColor(greyLevel, greyLevel, greyLevel));
        else
            showSelectedPlane(pl);
    }
}

void GenericBoard::hidePlanes()
{
    int rows = m_Grid.getRowNo() + 2 * m_PaddingEditingBoard;
    int cols = m_Grid.getColNo() + 2 * m_PaddingEditingBoard;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            m_SceneItems[std::make_pair(i, j)]->clearPlaneOptions();
}

///shows the guesses on the grid
void GenericBoard::displayGuesses() {
    for (GuessPoint gp : m_GuessList) {
        showGuessPoint(gp);
    }
}

void GenericBoard::showPlane(const Plane &pl, const QColor& color)
{
    Q_UNUSED(color)
    QPoint head = pl.head();
    auto headGridSquareIndex = std::make_pair(head.y() + m_PaddingEditingBoard, head.x() + m_PaddingEditingBoard);
    m_SceneItems[headGridSquareIndex]->setType(GridSquare::Type::PlaneHead);
    PlanePointIterator ppi(pl);
    ///ignore the plane head
    ppi.next();
    while (ppi.hasNext()) {
        QPoint pt = ppi.next();
        auto pointGridSquareIndex = std::make_pair(pt.y() + m_PaddingEditingBoard, pt.x() + m_PaddingEditingBoard);
        m_SceneItems[pointGridSquareIndex]->setType(GridSquare::Type::Plane);
        m_SceneItems[pointGridSquareIndex]->setColor(color);
    }
}

void GenericBoard::showSelectedPlane(const Plane &pl)
{
    QPoint head = pl.head();
    auto headGridSquareIndex = std::make_pair(head.y() + m_PaddingEditingBoard, head.x() + m_PaddingEditingBoard);
    m_SceneItems[headGridSquareIndex]->setSelected(true);
    PlanePointIterator ppi(pl);
    ///ignore the plane head
    ppi.next();
    while (ppi.hasNext()) {
        QPoint pt = ppi.next();
        auto pointGridSquareIndex = std::make_pair(pt.y() + m_PaddingEditingBoard, pt.x() + m_PaddingEditingBoard);
        m_SceneItems[pointGridSquareIndex]->setSelected(true);
    }
}


void GenericBoard::updateEditorBoard()
{
    hidePlanes();
    if (m_Grid.doPlanesOverlap() || m_Grid.isPlaneOutsideGrid())
        emit planePositionNotValid(true);
    else
        emit planePositionNotValid(false);
    displayPlanes();
}

void GenericBoard::showMove(const GuessPoint& gp)
{
    m_GuessList.push_back(gp);
    hidePlanes();
    displayPlanes();
    displayGuesses();
}

void GenericBoard::showGuessPoint(const GuessPoint &gp)
{
    qDebug() << "show guess point " << gp.m_row << " " << gp.m_col;
    GridSquare::GameStatus st = GridSquare::GameStatus::TestedNotPlane;
    if (gp.isHit())
        st = GridSquare::GameStatus::PlaneGuessed;
    if (gp.isDead())
        st = GridSquare::GameStatus::PlaneHeadGuessed;
    ///not sure about -1 here
    PlayAreaGridSquare* pags = dynamic_cast<PlayAreaGridSquare*>(m_SceneItems[std::make_pair(gp.m_col + m_PaddingEditingBoard, gp.m_row + m_PaddingEditingBoard)]);
    if (!pags) {
        qDebug() << "Dynamic cast did not succeed !";
        return;
    }
    pags->showGuesses(true);
    pags->setGameStatus(st);
}
