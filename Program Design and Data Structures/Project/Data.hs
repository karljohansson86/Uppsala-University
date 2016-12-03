module Data where


  {-
  REPRESENTATION CONVENTION:  Square [Candidates] Value Row Column Box
    A Square represents a square in a Sudoku board pussle.
    Row, Column and Box represents the coordinates for the Square where:
      Row indicates in which row the Square is located
      Column indicates in which column the Square is located
      Box indicates in which box the Square is located.
    Value represents the Squares Value, 1-9 if the Square has a value, if a Square does not have a value yet it is represented by 0.
    [Candidates] represents a list with possible candidates for the Square. At the beginning while creating the Squares, the list of candidates is empty.
      Once the functions have calculated the possible candidates they are inserted into the list.
      If a Square has a value other than 0 the list of candidates will be a empty list.

  REPRESENTATION INVARIANT:
    The only allowed values of integers for Candidates, Row, Column, and Box are 1-9.
    The only allowed values of integers for Value are 0-9 where 0 represents a empty Square.
    The list [Candidates] must always be empty when the Value of a square is not equal to zero.
  -}
  data Square = Square [Int] Int Int Int Int
    deriving (Show, Eq)


  {-
  REPRESENTATION CONVENTION:
    A Board represents a Sudoku board.
    A Board has 9 Rows, 9 Columns and 9 Boxes. Represented by a 9x9 grid.
    A Box is represented by a 3x3 grid.
    Each Row, Column and Box has 9 Squares each.

    For a board to be considered solvable, all of the following conditions must hold true:
      A solvable board has 81 Squares.
      A well formed sudoku puzzle should only ever have a single solution. This in turn means that for a board to be solvable
        it's starting configuration must contain at least 17 non-zero entries. Source: http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php

  REPRESENTATION INVARIANT:
    The Squares within each row, box or column must never share the same Value (other than 0).
  -}
  type Board = [Square]


  {-
    Visual example of a Board without any values sorted after Box.
      Board [
      -- topLeft Box
      Square [] 0 1 1 1,Square [] 0 1 2 1,Square [] 0 1 3 1,
      Square [] 0 2 1 1,Square [] 0 2 2 1,Square [] 0 2 3 1,
      Square [] 0 3 1 1,Square [] 0 3 2 1,Square [] 0 3 3 1,

      -- topMid Box
      Square [] 0 1 4 2,Square [] 0 1 5 2,Square [] 0 1 6 2,
      Square [] 0 2 4 2,Square [] 0 2 5 2,Square [] 0 2 6 2,
      Square [] 0 3 4 2,Square [] 0 3 5 2,Square [] 0 3 6 2,

      -- topRight Box
      Square [] 0 1 7 3,Square [] 0 1 8 3,Square [] 0 1 9 3,
      Square [] 0 2 7 3,Square [] 0 2 8 3,Square [] 0 2 9 3,
      Square [] 0 3 7 3,Square [] 0 3 8 3,Square [] 0 3 9 3,

      -- midLeft Box
      Square [] 0 4 1 4,Square [] 0 4 2 4,Square [] 0 4 3 4,
      Square [] 0 5 1 4,Square [] 0 5 2 4,Square [] 0 5 3 4,
      Square [] 0 6 1 4,Square [] 0 6 2 4,Square [] 0 6 3 4,

      -- midMid Box
      Square [] 0 4 4 5,Square [] 0 4 5 5,Square [] 0 4 6 5,
      Square [] 0 5 4 5,Square [] 0 5 5 5,Square [] 0 5 6 5,
      Square [] 0 6 4 5,Square [] 0 6 5 5,Square [] 0 6 6 5,

      -- midRight Box
      Square [] 0 4 7 6,Square [] 0 4 8 6,Square [] 0 4 9 6,
      Square [] 0 5 7 6,Square [] 0 5 8 6,Square [] 0 5 9 6,
      Square [] 0 6 7 6,Square [] 0 6 8 6,Square [] 0 6 9 6,

      -- bottomLeft Box
      Square [] 0 7 1 7,Square [] 0 7 2 7,Square [] 0 7 3 7,
      Square [] 0 8 1 7,Square [] 0 8 2 7,Square [] 0 8 3 7,
      Square [] 0 9 1 7,Square [] 0 9 2 7,Square [] 0 9 3 7,

      -- bottomMid Box
      Square [] 0 7 4 8,Square [] 0 7 5 8,Square [] 0 7 6 8,
      Square [] 0 8 4 8,Square [] 0 8 5 8,Square [] 0 8 6 8,
      Square [] 0 9 4 8,Square [] 0 9 5 8,Square [] 0 9 6 8,

      -- bottomRight Box
      Square [] 0 7 7 9,Square [] 0 7 8 9,Square [] 0 7 9 9,
      Square [] 0 8 7 9,Square [] 0 8 8 9,Square [] 0 8 9 9,
      Square [] 0 9 7 9,Square [] 0 9 8 9,Square [] 0 9 9 9]
  -}


  -- Boards exposed to main
  exposedBoards = [
    (easyBoard, "Easy"),
    (mediumBoard, "Medium"),
    (hardBoard, "Hard"),
    (extremeBoard, "Extreme"),
    (worldHardestBoard, "Worlds hardest")]

  zeroBoard = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0] :: [Int]

  ninesBoard = [9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
    9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
    9,9,9,9,9,9,9,9,9,9,9] :: [Int]

  -- EASY BOARD FROM: http://www.247sudoku.com/sudokuEasy.php
  easyBoard = [0,8,0,0,0,0,0,0,3,0,0,7,0,6,0,2,8,0,3,2,6,8,0,0,0,0,7,1,0,5,4,0,
    0,0,2,6,0,0,4,0,0,2,0,0,0,8,0,0,7,0,6,4,9,0,0,4,0,0,0,3,0,7,5,5,0,0,1,8,7,3,
    0,0,0,0,0,0,2,0,8,0,0] :: [Int]

  mediumBoard = [0,0,5,0,7,0,0,9,3,0,9,0,3,0,0,5,0,7,0,0,2,1,0,0,0,0,0,6,0,0,0,
    0,3,0,0,0,2,0,1,0,0,8,0,3,0,0,7,0,0,0,0,0,0,0,8,0,0,0,0,5,0,0,0,3,0,0,4,0,6,
    2,0,0,0,0,0,0,8,0,0,0,0] :: [Int]

  hardBoard = [0,2,0,5,0,0,0,0,0,0,0,0,1,0,3,0,0,7,4,6,0,2,0,0,8,0,0,0,0,0,0,7,
    2,0,0,0,0,0,0,3,0,0,4,0,0,0,0,0,0,0,0,0,1,9,0,0,0,0,0,4,7,0,6,3,0,0,0,0,0,5,
    0,0,7,5,0,6,0,0,1,0,0] :: [Int]

  extremeBoard = [0,0,6,0,0,9,7,2,5,0,7,0,1,0,0,0,0,0,8,0,0,7,0,0,0,0,0,0,5,0,0,0,
    0,0,3,6,0,0,7,0,5,0,4,0,0,4,2,0,0,0,0,0,9,0,0,0,0,0,0,8,0,0,1,0,0,0,0,0,6,0,4,
    0,6,8,1,5,0,0,3,0,0] :: [Int]

  worldHardestBoard = [8,0,0,0,0,0,0,0,0,0,0,3,6,0,0,0,0,0,0,7,0,0,9,0,2,0,0,0,
    5,0,0,0,7,0,0,0,0,0,0,0,4,5,7,0,0,0,0,0,1,0,0,0,3,0,0,0,1,0,0,0,0,6,8,0,0,8,
    5,0,0,0,1,0,0,9,0,0,0,0,4,0,0] :: [Int]

  testBoard1 = [1,2,3,0,0,0,0,0,0,4,5,6,0,0,0,0,0,0,7,8,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0] :: [Int]

  testBoard4 = [0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,2,0,9,0,0,0,5,0,0,0,
    0,0,0,0,0,0,0,6,0,0,0] :: [Int]

  validSolution = [1,2,7,5,8,6,9,4,3,5,9,8,1,4,3,2,6,7,4,6,3,2,9,7,8,5,1,9,3,1,4,
    7,2,6,8,5,6,8,5,3,1,9,4,7,2,2,7,4,8,6,5,3,1,9,8,1,2,9,5,4,7,3,6,3,4,6,7,2,1,
    5,9,8,7,5,9,6,3,8,1,2,4] :: [Int]

  solvedExtremeBoard =
    [Square [] 3 1 1 1,Square [] 1 1 2 1,Square [] 6 1 3 1,Square [] 8 1 4 2,Square [] 4 1 5 2,Square [] 9 1 6 2,
    Square [] 7 1 7 3,Square [] 2 1 8 3,Square [] 5 1 9 3,
    Square [] 5 2 1 1,Square [] 7 2 2 1,Square [] 4 2 3 1,Square [] 1 2 4 2,Square [] 6 2 5 2,Square [] 2 2 6 2,
    Square [] 9 2 7 3,Square [] 8 2 8 3,Square [] 3 2 9 3,
    Square [] 8 3 1 1,Square [] 9 3 2 1,Square [] 2 3 3 1,Square [] 7 3 4 2,Square [] 3 3 5 2,Square [] 5 3 6 2,
    Square [] 1 3 7 3,Square [] 6 3 8 3,Square [] 4 3 9 3,
    Square [] 1 4 1 4,Square [] 5 4 2 4,Square [] 8 4 3 4,Square [] 4 4 4 5,Square [] 9 4 5 5,Square [] 7 4 6 5,
    Square [] 2 4 7 6,Square [] 3 4 8 6,Square [] 6 4 9 6,
    Square [] 9 5 1 4,Square [] 6 5 2 4,Square [] 7 5 3 4,Square [] 2 5 4 5,Square [] 5 5 5 5,Square [] 3 5 6 5,
    Square [] 4 5 7 6,Square [] 1 5 8 6,Square [] 8 5 9 6,
    Square [] 4 6 1 4,Square [] 2 6 2 4,Square [] 3 6 3 4,Square [] 6 6 4 5,Square [] 8 6 5 5,Square [] 1 6 6 5,
    Square [] 5 6 7 6,Square [] 9 6 8 6,Square [] 7 6 9 6,
    Square [] 2 7 1 7,Square [] 4 7 2 7,Square [] 9 7 3 7,Square [] 3 7 4 8,Square [] 7 7 5 8,Square [] 8 7 6 8,
    Square [] 6 7 7 9,Square [] 5 7 8 9,Square [] 1 7 9 9,
    Square [] 7 8 1 7,Square [] 3 8 2 7,Square [] 5 8 3 7,Square [] 9 8 4 8,Square [] 1 8 5 8,Square [] 6 8 6 8,
    Square [] 8 8 7 9,Square [] 4 8 8 9,Square [] 2 8 9 9,
    Square [] 6 9 1 7,Square [] 8 9 2 7,Square [] 1 9 3 7,Square [] 5 9 4 8,Square [] 2 9 5 8,Square [] 4 9 6 8,
    Square [] 3 9 7 9,Square [] 7 9 8 9,Square [] 9 9 9 9]

  easyBoardAfterSolveEasy =
    [Square [4,9] 0 1 1 1,Square [] 8 1 2 1,Square [1,9] 0 1 3 1,Square [2,5,9] 0 1 4 2,Square [1,4,5,7,9] 0 1 5 2,
    Square [1,4,5,9] 0 1 6 2,Square [1,5,6,9] 0 1 7 3,Square [1,4,5,6] 0 1 8 3,Square [] 3 1 9 3,
    Square [4,9] 0 2 1 1,Square [1,5,9] 0 2 2 1,Square [] 7 2 3 1,Square [3,5,9] 0 2 4 2,Square [] 6 2 5 2,
    Square [1,4,5,9] 0 2 6 2,Square [] 2 2 7 3,Square [] 8 2 8 3,Square [1,4,9] 0 2 9 3,
    Square [] 3 3 1 1,Square [] 2 3 2 1,Square [] 6 3 3 1,Square [] 8 3 4 2,Square [1,4,5,9] 0 3 5 2,
    Square [1,4,5,9] 0 3 6 2,Square [1,5,9] 0 3 7 3,Square [1,4,5] 0 3 8 3,Square [] 7 3 9 3,
    Square [] 1 4 1 4,Square [3,7,9] 0 4 2 4,Square [] 5 4 3 4,Square [] 4 4 4 5,Square [3,9] 0 4 5 5,
    Square [8,9] 0 4 6 5,Square [] 7 4 7 6,Square [] 2 4 8 6,Square [] 6 4 9 6,
    Square [6,7,9] 0 5 1 4,Square [3,6,7,9] 0 5 2 4,Square [] 4 5 3 4,Square [3,5,9] 0 5 4 5,Square [1,3,5,9] 0 5 5 5,
    Square [] 2 5 6 5,Square [1,5] 0 5 7 6,Square [1,3,5] 0 5 8 6,Square [1,8] 0 5 9 6,
    Square [] 8 6 1 4,Square [] 3 6 2 4,Square [] 2 6 3 4,Square [] 7 6 4 5,Square [1,5] 0 6 5 5,
    Square [] 6 6 6 5,Square [] 4 6 7 6,Square [] 9 6 8 6,Square [] 1 6 9 6,
    Square [2,6,9] 0 7 1 7,Square [] 4 7 2 7,Square [1,8,9] 0 7 3 7,Square [6,9] 0 7 4 8,Square [] 9 7 5 8,
    Square [] 3 7 6 8,Square [1,6] 0 7 7 9,Square [] 7 7 8 9,Square [] 5 7 9 9,
    Square [] 5 8 1 7,Square [6,9] 0 8 2 7,Square [] 9 8 3 7,Square [] 1 8 4 8,Square [] 8 8 5 8,
    Square [] 7 8 6 8,Square [] 3 8 7 9,Square [4,6] 0 8 8 9,Square [2,4] 0 8 9 9,
    Square [6,7] 0 9 1 7,Square [1,6,7] 0 9 2 7,Square [1,3] 0 9 3 7,Square [5,6] 0 9 4 8,Square [] 2 9 5 8,
    Square [4,5] 0 9 6 8,Square [] 8 9 7 9,Square [1,4,6] 0 9 8 9,Square [4,9] 0 9 9 9]

  easyBoardAfterSolveWithConstraints =
    [Square [4,9] 0 1 1 1,Square [] 8 1 2 1,Square [1,9] 0 1 3 1,Square [] 2 1 4 2,Square [] 7 1 5 2,
    Square [1,4,5,9] 0 1 6 2,Square [1,5,6,9] 0 1 7 3,Square [1,4,5,6] 0 1 8 3,Square [] 3 1 9 3,
    Square [4,9] 0 2 1 1,Square [] 5 2 2 1,Square [] 7 2 3 1,Square [] 3 2 4 2,Square [] 6 2 5 2,
    Square [1,4,5,9] 0 2 6 2,Square [] 2 2 7 3,Square [] 8 2 8 3,Square [1,4,9] 0 2 9 3,
    Square [] 3 3 1 1,Square [] 2 3 2 1,Square [] 6 3 3 1,Square [] 8 3 4 2,Square [1,4,5,9] 0 3 5 2,
    Square [1,4,5,9] 0 3 6 2,Square [1,5,9] 0 3 7 3,Square [1,4,5] 0 3 8 3,Square [] 7 3 9 3,
    Square [] 1 4 1 4,Square [3,7,9] 0 4 2 4,Square [] 5 4 3 4,Square [] 4 4 4 5,Square [3,9] 0 4 5 5,
    Square [] 8 4 6 5,Square [7] 0 4 7 6,Square [] 2 4 8 6,Square [] 6 4 9 6,
    Square [6,7,9] 0 5 1 4,Square [3,6,7,9] 0 5 2 4,Square [] 4 5 3 4,Square [3,5,9] 0 5 4 5,Square [1,3,5,9] 0 5 5 5,
    Square [] 2 5 6 5,Square [1,5,7] 0 5 7 6,Square [] 3 5 8 6,Square [] 8 5 9 6,
    Square [] 8 6 1 4,Square [3] 0 6 2 4,Square [] 2 6 3 4,Square [] 7 6 4 5,Square [] 5 6 5 5,
    Square [] 6 6 6 5,Square [] 4 6 7 6,Square [] 9 6 8 6,Square [1] 0 6 9 6,
    Square [] 2 7 1 7,Square [] 4 7 2 7,Square [] 8 7 3 7,Square [6,9] 0 7 4 8,Square [9] 0 7 5 8,
    Square [] 3 7 6 8,Square [1,6,9] 0 7 7 9,Square [] 7 7 8 9,Square [] 5 7 9 9,
    Square [] 5 8 1 7,Square [6,9] 0 8 2 7,Square [2,9] 0 8 3 7,Square [] 1 8 4 8,Square [] 8 8 5 8,
    Square [] 7 8 6 8,Square [] 3 8 7 9,Square [4,6] 0 8 8 9,Square [] 2 8 9 9,
    Square [6,7,9] 0 9 1 7,Square [1,3,6,7,9] 0 9 2 7,Square [1,3,9] 0 9 3 7,Square [5,6,9] 0 9 4 8,Square [] 2 9 5 8,
    Square [] 4 9 6 8,Square [] 8 9 7 9,Square [1,4,6] 0 9 8 9,Square [1,4,9] 0 9 9 9]

  easyBoardAfterSolveWithForce =
    [Square [] 4 1 1 1,Square [] 8 1 2 1,Square [] 1 1 3 1,Square [] 2 1 4 2,Square [] 7 1 5 2,
    Square [] 9 1 6 2,Square [] 6 1 7 3,Square [] 5 1 8 3,Square [] 3 1 9 3,
    Square [] 9 2 1 1,Square [] 5 2 2 1,Square [] 7 2 3 1,Square [] 3 2 4 2,Square [] 6 2 5 2,
    Square [] 1 2 6 2,Square [] 2 2 7 3,Square [] 8 2 8 3,Square [] 4 2 9 3,
    Square [] 3 3 1 1,Square [] 2 3 2 1,Square [] 6 3 3 1,Square [] 8 3 4 2,Square [] 4 3 5 2,
    Square [] 5 3 6 2,Square [] 9 3 7 3,Square [] 1 3 8 3,Square [] 7 3 9 3,
    Square [] 1 4 1 4,Square [] 9 4 2 4,Square [] 5 4 3 4,Square [] 4 4 4 5,Square [] 3 4 5 5,
    Square [] 8 4 6 5,Square [] 7 4 7 6,Square [] 2 4 8 6,Square [] 6 4 9 6,
    Square [] 6 5 1 4,Square [] 7 5 2 4,Square [] 4 5 3 4,Square [] 9 5 4 5,Square [] 1 5 5 5,
    Square [] 2 5 6 5,Square [] 5 5 7 6,Square [] 3 5 8 6,Square [] 8 5 9 6,
    Square [] 8 6 1 4,Square [] 3 6 2 4,Square [] 2 6 3 4,Square [] 7 6 4 5,Square [] 5 6 5 5,
    Square [] 6 6 6 5,Square [] 4 6 7 6,Square [] 9 6 8 6,Square [] 1 6 9 6,
    Square [] 2 7 1 7,Square [] 4 7 2 7,Square [] 8 7 3 7,Square [] 6 7 4 8,Square [] 9 7 5 8,
    Square [] 3 7 6 8,Square [] 1 7 7 9,Square [] 7 7 8 9,Square [] 5 7 9 9,
    Square [] 5 8 1 7,Square [] 6 8 2 7,Square [] 9 8 3 7,Square [] 1 8 4 8,Square [] 8 8 5 8,
    Square [] 7 8 6 8,Square [] 3 8 7 9,Square [] 4 8 8 9,Square [] 2 8 9 9,
    Square [] 7 9 1 7,Square [] 1 9 2 7,Square [] 3 9 3 7,Square [] 5 9 4 8,Square [] 2 9 5 8,
    Square [] 4 9 6 8,Square [] 8 9 7 9,Square [] 6 9 8 9,Square [] 9 9 9 9]

  hiddenTwin = [4,0,9,6,2,0,0,0,0,1,6,0,0,4,5,0,2,0,7,5,2,0,0,0,0,4,0,2,0,0,5,0,
    0,4,9,0,6,4,5,0,9,0,1,0,2,0,9,0,2,0,4,0,0,0,0,2,0,0,0,0,3,7,4,0,0,0,4,0,0,2,
    1,9,0,0,4,0,3,2,5,0,0] :: [Int]
