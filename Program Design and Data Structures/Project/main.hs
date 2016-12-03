import System.Environment
import Control.Monad
import Data.List
import Data.Char

import Data -- Our data types
import Helpers -- Helper functions
import BoardConstructors -- Functions required to represent boards in the terminal



-- The interface for the compiled version.
-- Provides access to the boards exposed in exposedBoards.

main = do
  args <- getArgs

  let firstArg            = args !! 0
      firstChar           = firstArg !! 0
      exposedBoardsLength = length exposedBoards

  if length args == 1 &&
     length firstArg == 1 &&
     (digitToInt firstChar) `elem` [1..exposedBoardsLength]

    then do
      let selectedBoard = exposedBoards !! ((digitToInt (firstArg !! 0)) - 1)
      putStrLn $ "\nSolving board: " ++ (snd selectedBoard)
      start (fst selectedBoard)

    else do
      putStrLn "\nInvalid or no argument. Here are the available boards:\n"
      putStrLn "1 = Easy board"
      putStrLn "2 = Medium board"
      putStrLn "3 = Hard board"
      putStrLn "4 = Extreme board"
      putStrLn "5 = Worlds hardest board"

      putStrLn "\nWindows:"
      putStrLn "To solve the Easy board run: main 1"
      putStrLn "\nUnix:"

      putStrLn "To solve the Easy board run: ./main 1"
      putStrLn "With timing information run: time ./main 1\n"



{-
  start source
  PURPOSE:      To solve the sudoku puzzle board and print the solution
  PRE:          True
  POST:         A printed solution of the board if solvable otherwise a error message saying "Sorry, couldnt find a valid solution."
  SIDE EFFECTS: Printing to the screen/terminal
  EXAMPLES:
    start extremeBoard =

      Solution found, hooray!

      3 1 6  |  8 4 9  |  7 2 5
      5 7 4  |  1 6 2  |  9 8 3
      8 9 2  |  7 3 5  |  1 6 4
      ------ + ------- + ------
      1 5 8  |  4 9 7  |  2 3 6
      9 6 7  |  2 5 3  |  4 1 8
      4 2 3  |  6 8 1  |  5 9 7
      ------ + ------- + ------
      2 4 9  |  3 7 8  |  6 5 1
      7 3 5  |  9 1 6  |  8 4 2
      6 8 1  |  5 2 4  |  3 7 9
-}
start :: [Int] -> IO ()
start source
  | (validBoard solution /= []) = do
      putStrLn "\nSolution found, hooray!\n"
      printBoard solution
  | otherwise                   = do
      putStrLn "\nSorry, couldnt find a unique valid solution.\n"
  where
    board    = makeBoard source
    solution = solve board

{-
  altStart source
  PURPOSE:  Testable version of start
  PRE:      True
  POST:     A solved board if the board is solvable otherwise a empty list
  EXAMPLES:
    altStart extremeBoard = solvedExtremeBoard
-}
altStart :: [Int] -> Board
altStart source
  | (validBoard solution /= []) = solution
  | otherwise                   = []
  where
    board    = makeBoard source
    solution = solve board



{-
  solve board
  PURPOSE:  To solve the sudoku puzzle board using our 3 algorithms solveEasy,solveWithConstraints and solveWithForce.
  PRE:      Board has to have a unique solution otherwise
  POST:     A solved/valid Board with all squares solved, a error message if the inserted board is invalid or a empty list if the inserted board is a empty list.
  EXAMPLES: solve (makeBoard extremeBoard) = solvedExtremeBoard
            solve [] = []
-}
solve :: Board -> Board
solve [] = []
solve board
  | solvedSudoku solvedC = solvedC
  | (board == solvedE) &&
    (board == solvedC) &&
    (solvedE == solvedC) = solveWithForce solvedC
  | otherwise            = solve solvedC
  where
    solvedE = solveEasy board board
    solvedC = solveWithConstraints solvedE



{-
  solveEasy original board
  PURPOSE:  To solve a board completely or as much of the board as possible by checking if a square only has one possible candidate,
            if so it's inserted as the value x of that square.
  PRE:      True
  POST:     A Board containing new values in some squares if function could solve any.
            Each unsolved square now has the possible candidates for that square insert in its list for candidates.
  EXAMPLES:
    solveEasy (makeBoard easyBoard) (makeBoard easyBoard) = easyBoardAfterSolveEasy (copy of Board in SudokuBoard.hs file)
  VARIANT: Lenght of the list/Board board.
-}
-- solves a square in a board using a simple but fast algorithm, if a square only have one potential candidate, its printed on the board
solveEasy :: Board -> Board -> Board
solveEasy original [] = []
solveEasy original board@(square@(Square cand x row col box):xs)
  | x /= 0 = square : solveEasy original xs
  -- Checks for easy solution
  | length missingInRow == 1 = solvedByRow : solveEasy (replaceSquare original solvedByRow) xs -- solveEasy original xs
  | length missingInCol == 1 = solvedByCol : solveEasy (replaceSquare original solvedByCol) xs -- solveEasy original xs
  | length missingInBox == 1 = solvedByBox : solveEasy (replaceSquare original solvedByBox) xs -- solveEasy original xs
  | length possibleCand == 1 = solvedByCnd : solveEasy (replaceSquare original solvedByCnd) xs -- solveEasy original xs
  -- No solution found, try the next square. Store the candidates
  | otherwise = (Square possibleCand x row col box) : solveEasy original xs
  where
    -- Calculates the entries present and missing for current row, col and box
    alreadyInRow = getValues $ inRow row original
    alreadyInCol = getValues $ inCol col original
    alreadyInBox = getValues $ inBox box original

    missingInRow = missingNumbers alreadyInRow
    missingInCol = missingNumbers alreadyInCol
    missingInBox = missingNumbers alreadyInBox

    solvedByRow = Square [] (head missingInRow) row col box
    solvedByCol = Square [] (head missingInCol) row col box
    solvedByBox = Square [] (head missingInBox) row col box

    -- Tries to find a solution using the sole candidate principle
    -- https://www.kristanix.com/sudokuepic/sudoku-solving-techniques.php
    possibleCand = missingNumbers $ alreadyInRow ++ alreadyInCol ++ alreadyInBox
    solvedByCnd = Square [] (head possibleCand) row col box



{-
  solveWithConstraints original
  PURPOSE:  To solve a board completely or as much of the board as possible by calculating if a square has any unique candidates.
  PRE:      True
  POST:     A Board containing new values in some squares if the function could solve any. Otherwise it returns the same Board board without any changes.
  EXAMPLES:
    solveWithConstraints (makeBoard easyBoard) = easyBoardAfterSolveWithConstraints (copy of Board in SudokuBoard.hs file)
-}
--solves a square in a board using a more complex algorithm by removing possible candidates to a square
solveWithConstraints :: Board -> Board
solveWithConstraints original =
  let
    {-
      solveWithConstraintsAux original board
      PURPOSE:  Attempts solve a square by calculating if the square has any unique candidates by comparing it with
                  the candidates of other squares in the row, column and box
      PRE:      True
      POST:     A Board board with a value inserted in x (if the function could solve any) for each square in the board.
                If no changes it returns the same Board board without any changes.
      EXAMPLES:
        solveWithConstraintsAux (makeBoard easyBoard) (makeBoard easyBoard) = easyBoardAfterSolveWithConstraints (copy of Board in SudokuBoard.hs file)
      VARIANT: Lenght of the list/Board board.
    -}
    solveWithConstraintsAux :: Board -> Board -> Board
    solveWithConstraintsAux original [] = []
    solveWithConstraintsAux original board@(square@(Square cand x row col box):xs)
      | x /= 0 = square : solveWithConstraintsAux original xs
      -- Checks for easy solution
      | length uniqueSolutionsRow == 1 = solvedByRow : solveWithConstraintsAux original xs
      | length uniqueSolutionsCol == 1 = solvedByCol : solveWithConstraintsAux original xs
      | length uniqueSolutionsBox == 1 = solvedByBox : solveWithConstraintsAux original xs
      | otherwise = square : solveWithConstraintsAux original xs
      where
        uniqueSolutionsRow = uniqueCandidates cand $ otherCandidates (inRow row original) square
        uniqueSolutionsCol = uniqueCandidates cand $ otherCandidates (inCol col original) square
        uniqueSolutionsBox = uniqueCandidates cand $ otherCandidates (inBox box original) square

        solvedByRow = Square [] (head uniqueSolutionsRow) row col box
        solvedByCol = Square [] (head uniqueSolutionsCol) row col box
        solvedByBox = Square [] (head uniqueSolutionsBox) row col box

    candidatesBoard = findCandidates original original

  in
    solveWithConstraintsAux candidatesBoard candidatesBoard



{-
  solveWithForce board square
  PURPOSE: Attempts to solve the board by first finding a suitable empty square and then inserts each possible candidates into the board.
    solveWithForce recurses over each possible candidate and calls the function solve with a new board where the current candidate is inserted.
  PRE: True
  POST: A solved valid board if all "guesses" were correct or a empty list if any guess was incorrect.
  EXAMPLES:
    solveWithForce easyBoardAfterSolveWithConstraints (Square [4,9] 0 1 1 1) = easyBoardAfterSolveWithForce (copy of Board in SudokuBoard.hs file)
  VARIANT: Length of list containing the candidates for the square.
-}
solveWithForce :: Board -> Board
solveWithForce []    = []
solveWithForce board =
  let
    solveWithForceAux :: Board -> Square -> Board
    solveWithForceAux [] _ = []
    solveWithForceAux _ (Square [] _ _ _ _) = []
    solveWithForceAux board (Square (c:cs) num row col box) =
      let
        insBoard = replaceSquare board (Square [] c row col box)
        newBoard = validGuess insBoard row col box
      in
        solveWithForceAux newBoard (Square cs num row col box) ++ validBoard (solve newBoard)
  in
    solveWithForceAux board (bestGuess board)
