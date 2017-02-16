using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.IO.Pipes;
using FullSailAFI.GamePlaying.CoreAI;

namespace FullSailAFI.GamePlaying
{
    public class StudentAI : Behavior
    {
        TreeVisLib treeVisLib;  // lib functions to communicate with TreeVisualization
        bool visualizationFlag = false;  // turn this on to use tree visualization (which you will have to implement via the TreeVisLib API)
                                         // WARNING: Will hang program if the TreeVisualization project is not loaded!

        public StudentAI()
        {
            if (visualizationFlag == true)
            {
                if (treeVisLib == null)  // should always be null, but just in case
                    treeVisLib = TreeVisLib.getTreeVisLib();  // WARNING: Creation of this object will hang if the TreeVisualization project is not loaded!
            }
        }

        //
        // This function starts the look ahead process to find the best move
        // for this player color.
        //
        public ComputerMove Run(int _nextColor, Board _board, int _lookAheadDepth)
        {
            ComputerMove nextMove = GetBestMove(_nextColor, _board, _lookAheadDepth);

            return nextMove;
        }

        //
        // This function uses look ahead to evaluate all valid moves for a
        // given player color and returns the best move it can find. This
        // method will only be called if there is at least one valid move
        // for the player of the designated color.
        //
        private ComputerMove GetBestMove(int color, Board board, int depth)
        {
            //TODO: the lab
            ComputerMove m_bestMove = null;
            Board newState = new Board();

            List<ComputerMove> Moves = new List<ComputerMove>();
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (board.IsValidMove(color, i, j))
                        Moves.Add(new ComputerMove(i, j));
                }
            }

            for (int i = 0; i < Moves.Count; i++)
            {
                newState.Copy(board);
                newState.MakeMove(color, Moves[i].row, Moves[i].col);

                if (newState.IsTerminalState() || depth == 0)
                    Moves[i].rank = Evaluate(newState);
                else
                {
                    Moves[i].rank = GetBestMove(GetNextPlayer(color, newState), newState, depth - 1).rank;
                }

                //white
                if (color == 1)
                {
                    if (m_bestMove == null || Moves[i].rank > m_bestMove.rank)
                        m_bestMove = Moves[i];
                }
                //black
                else
                {
                    if (m_bestMove == null || Moves[i].rank < m_bestMove.rank)
                        m_bestMove = Moves[i];
                }
            }
            return m_bestMove;

        }

        #region Recommended Helper Functions

        private int Evaluate(Board _board)
        {
            //TODO: determine score based on position of pieces
            int m_Score = 0;

            for (int m_Row = 0; m_Row < 8; m_Row++)
            {
                for (int m_Column = 0; m_Column < 8; m_Column++)
                {
                    if (((m_Row == 0) && (m_Column == 0)) || ((m_Row == 7) && (m_Column == 0))
                        || ((m_Row == 0) && (m_Column == 7)) || ((m_Row == 7) && (m_Column == 7)))
                    {
                        m_Score += _board.GetSquareContents(m_Row, m_Column) * 100;
                    }

                    else if (m_Row == 0 || m_Row == 7 || m_Column == 0 || m_Column == 7)
                    {
                        m_Score += _board.GetSquareContents(m_Row, m_Column) * 10;
                    }
                    else
                    {
                        m_Score += _board.GetSquareContents(m_Row, m_Column);
                    }
                }
            }

            if (_board.IsTerminalState())
            {
                if (m_Score > 0)
                    m_Score += 10000;
                else if (m_Score < 0)
                    m_Score -= 10000;
            }

            return m_Score;
        }

        private int GetNextPlayer(int _player, Board _gameState)
        {
            if (_gameState.HasAnyValidMove(-_player))
                return -_player;
            return _player;
        }
        #endregion

    }
}
