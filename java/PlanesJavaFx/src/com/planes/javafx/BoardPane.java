package com.planes.javafx;

import java.util.HashMap;
import java.util.Map;
import javafx.beans.binding.Bindings;
import javafx.event.EventHandler;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;

class BoardPane extends Pane 
{
	class PositionBoardPane {
		private int x = 0;
		private int y = 0;
		public PositionBoardPane(int i, int j) {
			x = i;
			y = j;
		}
		
		@Override
		public boolean equals(final Object other) {
			if (other == null)
				return false;
			if (this == other)
				return true;
			if (this.getClass() != other.getClass())
				return false;
			
			final PositionBoardPane pos = (PositionBoardPane)other;
			return this.x == pos.x && this.y == pos.y;
		}
		
		@Override
		public int hashCode() {
			return 100 * x + y;
		}
		
		public int x() {
			return x;
		}
		
		public int y() {
			return y;
		}
	}
	
	public enum GameStages {
		GameNotStarted, BoardEditing, Game
	}
	
	public void selectPlane() {
		int planeNo = m_PlaneRound.getPlaneNo();
		m_SelectedPlane = (m_SelectedPlane + 1) % planeNo;
	}
	
	public void movePlaneLeft() {
		m_PlaneRound.movePlaneLeft(m_SelectedPlane);
	}
	
	public void movePlaneRight() {
		m_PlaneRound.movePlaneRight(m_SelectedPlane);
	}
	
	public void movePlaneUpwards() {
		m_PlaneRound.movePlaneUpwards(m_SelectedPlane);
	}
	
	public void movePlaneDownwards() {
		m_PlaneRound.movePlaneDownwards(m_SelectedPlane);
	}
	
	public void rotatePlane() {
		m_PlaneRound.rotatePlane(m_SelectedPlane);
	}
	
	public void doneClicked() {
		m_CurStage = GameStages.Game;
	}
	
	public void updateBoard() {
		System.out.println("Update board");
        int gRows = m_PlaneRound.getRowNo();
        int gCols = m_PlaneRound.getColNo();
        int planeNo = m_PlaneRound.getPlaneNo();
        int colorStep = (m_MaxPlaneBodyColor - m_MinPlaneBodyColor) / planeNo;
		
		for (int i = 0; i < gRows + 2 * m_Padding; i++) {
			for (int j = 0; j < gCols + 2 * m_Padding; j++) {
				Canvas c = m_GridSquares.get(new PositionBoardPane(i, j));
				GraphicsContext gc = c.getGraphicsContext2D();
				//compute the color of the square
				Color squareColor = null;
				 
				if (i < m_Padding || i >= gRows + m_Padding || j < m_Padding || j >= gCols + m_Padding) {
					squareColor = Color.YELLOW;
				} else {
					squareColor = Color.AQUA;
				}
				
				if (!m_IsComputer || (m_IsComputer && m_CurStage == GameStages.GameNotStarted)) {
					int type = m_PlaneRound.getPlaneSquareType(i - m_Padding, j - m_Padding, m_IsComputer);
					switch (type) {					
						//intersecting planes
						case -1:
							squareColor = Color.RED;
							break;								
						//plane head
						case -2:
							squareColor = Color.GREEN;
							break;			
						//not a plane	
						case 0:
							break;					
						//plane but not plane head
						default:
							if ((type - 1) == m_SelectedPlane) {
								squareColor = Color.BLUE;
							} else {
								int grayCol = m_MinPlaneBodyColor + type * colorStep;
								squareColor = Color.rgb(grayCol, grayCol, grayCol);
							}
							break;						
					}
				}
				
				//antialiasing
			    gc.setFill(Color.WHITE);
			    gc.fillRect(0, 0, c.getWidth(), c.getHeight());
					
				//draw the background of the square
				gc.setFill(squareColor);
				gc.fillRect(c.getWidth() / 10, c.getHeight() / 10 , c.getWidth() * 8 / 10, c.getHeight() * 8 / 10);
				gc.fillRoundRect(c.getWidth() / 10, c.getHeight() / 10, c.getWidth() * 8 / 10, c.getHeight() * 8 / 10, 5, 5);
			}
		} //display background of square; double for loop
		
		int count = 0;
		
		if (m_IsComputer)
			count = m_PlaneRound.getComputerGuessesNo();
		else
			count = m_PlaneRound.getPlayerGuessesNo();
		
		for (int i = 0; i < count; i++) {
			int row = 0;
			int col = 0;
			int type = 0;
			Canvas c = null;
			
			if (m_IsComputer) {
				row = m_PlaneRound.getPlayerGuessRow(i);
				col = m_PlaneRound.getPlayerGuessCol(i);
				type = m_PlaneRound.getPlayerGuessType(i);	
			} else {
				row = m_PlaneRound.getComputerGuessRow(i);
				col = m_PlaneRound.getComputerGuessCol(i);
				type = m_PlaneRound.getComputerGuessType(i);
			}

			c = m_GridSquares.get(new PositionBoardPane(row + m_Padding, col + m_Padding));	
			GraphicsContext gc = c.getGraphicsContext2D();
			int width = (int)c.getWidth();
			int height = (int)c.getHeight();
			gc.setFill(Color.RED);
			gc.setStroke(Color.RED);
			
			//enum Type {Miss = 0, Hit = 1, Dead = 2};
			switch (type) {
				case 0:
					//draw red circle
					gc.fillOval(width / 4, width / 4, width / 2, height / 2);
					break;
				case 1:
					//draw triangle
					gc.beginPath();
				    gc.moveTo(0, height / 2);
				    gc.lineTo(width / 2, 0);
				    gc.lineTo(width, height / 2);
				    gc.lineTo(width / 2, height);
				    gc.lineTo(0, height / 2);
				    gc.fill();
				    gc.closePath();
				    gc.stroke();
					break;
				case 2:
					//draw X
				    gc.strokeLine(0, 0, width, width);
				    gc.strokeLine(0, width, width, 0);						
					break;
			}
		}
	}
	
	public BoardPane(PlaneRoundJavaFx planeRound, RightPane rightPane, boolean isComputer) {
		m_PlaneRound = planeRound;
		m_IsComputer = isComputer;
		m_RightPane = rightPane;
		final GridPane gridPane = new GridPane();
		
        // In order to see the GridPane extends with the LeftPane, remove it further
        gridPane.setGridLinesVisible(true);
        // Those 2 following lines enable the gridpane to stretch/shrink according the LeftPane

        gridPane.prefWidthProperty().bind(Bindings.min(this.widthProperty(), this.heightProperty()));
        gridPane.prefHeightProperty().bind(Bindings.min(this.widthProperty(), this.heightProperty()));
        gridPane.layoutXProperty().bind(Bindings.divide(Bindings.subtract(this.widthProperty(), Bindings.min(this.widthProperty(), this.heightProperty())), 2.0));
        gridPane.layoutYProperty().bind(Bindings.divide(Bindings.subtract(this.heightProperty(), Bindings.min(this.widthProperty(), this.heightProperty())), 2.0));
        
        int gRows = m_PlaneRound.getRowNo();
        int gCols = m_PlaneRound.getColNo();
        
    	m_ClickedHandler = new EventHandler<MouseEvent>() {
                @Override
                public void handle(final MouseEvent e) {            
                    if (m_CurStage != GameStages.Game)
                    	return;
                	Object obj = e.getSource();  
                    if (obj instanceof Canvas)
                    {
                        PositionBoardPane position = (PositionBoardPane)(((Canvas) obj).getUserData());
                    	System.out.println("Clicked on position " + position.x() + " " + position.y());
                    	m_PlaneRound.playerGuess(position.y() - m_Padding, position.x() - m_Padding);
                    	int playerWins = m_PlaneRound.playerGuess_StatNoPlayerWins();
                    	int playerMoves = m_PlaneRound.playerGuess_StatNoPlayerMoves();
                    	int playerHits = m_PlaneRound.playerGuess_StatNoPlayerHits();
                    	int playerMisses = m_PlaneRound.playerGuess_StatNoPlayerMisses();
                    	int playerDead = m_PlaneRound.playerGuess_StatNoPlayerDead();
                    	int computerWins = m_PlaneRound.playerGuess_StatNoComputerWins();
                    	int computerMoves = m_PlaneRound.playerGuess_StatNoComputerMoves();
                    	int computerHits = m_PlaneRound.playerGuess_StatNoComputerHits();
                    	int computerMisses = m_PlaneRound.playerGuess_StatNoComputerMisses();
                    	int computerDead = m_PlaneRound.playerGuess_StatNoComputerDead();
                    	m_RightPane.updateStats(playerWins, playerMoves, playerHits, playerMisses, playerDead, 
                    			computerWins, computerMoves, computerHits, computerMisses, computerDead);
                    	updateBoard();
                    }
                }
            };
        
        m_GridSquares = new HashMap<PositionBoardPane, Canvas>();
        
		for (int i = 0; i < gRows + 2 * m_Padding; i++) {
			for (int j = 0; j < gCols + 2 * m_Padding; j++) {
				Canvas c = new Canvas();
			    c.widthProperty().bind(Bindings.min(this.widthProperty(), this.heightProperty()).divide(gCols + 2 * m_Padding));
			    c.heightProperty().bind(Bindings.min(this.widthProperty(), this.heightProperty()).divide(gRows + 2 * m_Padding));
				gridPane.add(c,  i,  j);
				PositionBoardPane position = new PositionBoardPane(i, j);
				m_GridSquares.put(position, c);
				if (m_IsComputer && i >= m_Padding && i < gRows + m_Padding && j >= m_Padding && j < gCols + m_Padding) {	
					c.setUserData(position);
					c.setOnMouseClicked(m_ClickedHandler);
				}
			}
		}
	    
		//AnchorPane.setTopAnchor(gridPane, 10.0);
		//AnchorPane.setLeftAnchor(gridPane, 10.0);
		
		this.getChildren().add(gridPane);
	}
	
	private Map<PositionBoardPane, Canvas> m_GridSquares;
	private PlaneRoundJavaFx m_PlaneRound;
	RightPane m_RightPane;
	private int m_Padding = 3;
	private boolean m_IsComputer = false;
	private int m_MinPlaneBodyColor = 0;
	private int m_MaxPlaneBodyColor = 200;	
	private GameStages m_CurStage = GameStages.BoardEditing;
	private int m_SelectedPlane = 0;
	private EventHandler<MouseEvent> m_ClickedHandler;
	
}	//BoardPane	