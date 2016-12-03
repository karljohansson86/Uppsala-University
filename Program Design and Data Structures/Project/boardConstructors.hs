module BoardConstructors where
  import Data.List
  import Helpers
  import Data

  {- makeBoard numbers
    PURPOSE: Converts a list of integers into a sudoku board.
    PRE: The list must contain 81 integers, 17 of these must be non-zero. Every integer must be in the range of 0-9.
    POST: A full sudoku board as described by our datatypes Board and Square. [] if any of the preconditions are violated.
    EXAMPLES:
      makeBoard [0,8,0,0,0,0,0,0,3 ...] =
        [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,Square []
        0 1 5 2,Square [] 0 1 6 2,Square [] 0 1 7 3,Square [] 0 1 8 3,Square [] 3 1 9 3 ...]
  -}

  makeBoard :: [Int] -> Board
  makeBoard numbers
    | length numbers /= 81 = error "A board must contain exactly 81 entries"
    | length nonZeros < 17 = error "A board must contain at least 17 non-zero values"
    | otherwise            = newBoard
    where
      newBoard = makeBoardAux numbers 1 1 1
      nonZeros = filter (0 /=) numbers
  {- makeBoardAux numbers
    PURPOSE: Convert a list of integers into a sudoku board.
    PRE: True
    POST: A full sudoku board as described by our datatypes Board and Square.
    EXAMPLE:
      makeBoard [0,8,0,0,0,0,0,0,3 ...] =
        [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,Square []
        0 1 5 2,Square [] 0 1 6 2,Square [] 0 1 7 3,Square [] 0 1 8 3,Square [] 3 1 9 3 ...]
    VARIANT: The length of the list numbers
  -}
      makeBoardAux :: [Int] -> Int -> Int -> Int -> Board
      makeBoardAux [] _ _ _ = []
      makeBoardAux (x:xs) row col box
        | col == 3 = (Square [] x row col box ) : (makeBoardAux xs row (col+1) (box+1))
        | col == 6 = (Square [] x row col box ) : (makeBoardAux xs row (col+1) (box+1))
        | col == 9 && (row == 3 || row == 6) = (Square [] x row col box) : (makeBoardAux xs (row+1) 1 (box+1))
        | col == 9 = (Square [] x row col box ) : (makeBoardAux xs (row+1) 1 (box-2))
        | otherwise = (Square [] x row col box ) : (makeBoardAux xs row (col+1) box)


  {- boardToList board
    PURPOSE: To convert a Board into a list of integers.
    PRE: True
    POST: A list of integers where every entry represents the value of a square.
    EXAMPLE:
      boardToList [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,Square []
      0 1 5 2,Square [] 0 1 6 2,Square [] 0 1 7 3,Square [] 0 1 8 3,Square [] 3 1 9 3 ...] = [0,8,0,0,0,0,0,0,3 ...]
    VARIANT: The length of board.
  -}
  boardToList :: Board -> [Int]
  boardToList [] = []
  boardToList ((Square _ x _ _ _):xs) = x : boardToList xs


  {- boardToString numbers
    PURPOSE: Converts a list of integers into a String
    PRE: True
    POST: A String, where 1-9 are simply "1" and "2" and so on, and 0 are represented as "."
    EXAMPLES:
      boardToString [1,2,4,0,2] = "124.2"
      boardToString [1] = "1"
      boardToString [] = ""
    VARIANT: The length of the list numbers
  -}
  boardToString :: [Int] -> String
  boardToString [] = ""
  boardToString (x:xs) = (if x == 0 then "." else show x) ++ boardToString xs


  {- printBoard board
    PURPOSE: Print a board in a readable style
    PRE: True
    POST: A board printed with vertical and horizontal lines to help readability
    SIDE EFFECTS: Printing to the screen/terminal
    EXAMPLE:
      printBoard (makeBoard [0,8,0,0,0,0,0,0,3,0,0,7,0,6,0,2,8,0,3,2,6,8,0,0,0,0,7,1,0,5,4,0,
      0,0,2,6,0,0,4,0,0,2,0,0,0,8,0,0,7,0,6,4,9,0,0,4,0,0,0,3,0,7,5,5,0,0,1,8,7,3,
      0,0,0,0,0,0,2,0,8,0,0]) =

      . 8 .  |  . . .  |  . . 3
      . . 7  |  . 6 .  |  2 8 .
      3 2 6  |  8 . .  |  . . 7
      ------ + ------- + ------
      1 . 5  |  4 . .  |  . 2 6
      . . 4  |  . . 2  |  . . .
      8 . .  |  7 . 6  |  4 9 .
      ------ + ------- + ------
      . 4 .  |  . . 3  |  . 7 5
      5 . .  |  1 8 7  |  3 . .
      . . .  |  . 2 .  |  8 . .
  -}
  printBoard :: Board -> IO ()
  printBoard board = do
    let sudoku = boardToString (boardToList board)
    putStrLn $ intersperse ' ' (take 3 sudoku) ++ "  |  " ++ intersperse ' ' (take 3 (drop 3 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 6 sudoku))
    putStrLn $ intersperse ' ' (take 3 (drop 9 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 12 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 15 sudoku))
    putStrLn $ intersperse ' ' (take 3 (drop 18 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 21 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 24 sudoku))
    putStrLn $ "------ + ------- + ------"
    putStrLn $ intersperse ' ' (take 3 (drop 27 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 30 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 33 sudoku))
    putStrLn $ intersperse ' ' (take 3 (drop 36 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 39 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 42 sudoku))
    putStrLn $ intersperse ' ' (take 3 (drop 45 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 48 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 51 sudoku))
    putStrLn $ "------ + ------- + ------"
    putStrLn $ intersperse ' ' (take 3 (drop 54 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 57 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 60 sudoku))
    putStrLn $ intersperse ' ' (take 3 (drop 63 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 66 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 69 sudoku))
    putStrLn $ intersperse ' ' (take 3 (drop 72 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 75 sudoku)) ++ "  |  " ++ intersperse ' ' (take 3 (drop 78 sudoku))
    putStrLn ""
