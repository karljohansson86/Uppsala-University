module Helpers where
  import Data.List
  import Data

  -- HELPER FUNCTIONS
  -- ================

  {- inRow num board
    PURPOSE: To get all squares in a specific row.
    PRE: True
    POST: A Board containing all squares found in the row specificed by num in board.
    EXAMPLES:
      inRow 1 [Square [] 0 1 1 1,Square [] 1 2 2 1,Square [] 0 3 5 2] =
        [Square [] 0 1 1 1]
      inRow 1 [] = []
    VARIANT: The length of board.
  -}
  inRow :: Int -> Board -> Board
  inRow num [] = []
  inRow num (s@(Square _ _ rowNum _ _):xs)
    | (num == rowNum) = s : inRow num xs
    | otherwise       = inRow num xs

  {- inCol num board
    PURPOSE: To get all squares in a specific column.
    PRE: True
    POST: A Board containing all squares found in the column specificed by num in board.
    EXAMPLES:
      inCol 1 [Square [] 0 1 1 1,Square [] 1 2 2 1,Square [] 0 3 5 2] =
        [Square [] 0 1 1 1]
      inCol 1 [] = []
    VARIANT: The length of board.
  -}
  inCol :: Int -> Board -> Board
  inCol num [] = []
  inCol num (c@(Square _ _ _ colNum _):xs)
    | (num == colNum) = c : inCol num xs
    | otherwise       = inCol num xs

  {- inBox num board
    PURPOSE: To get all squares in a specific box.
    PRE: True
    POST: A Board containing all squares found in the box specificed by num in board.
    EXAMPLES:
      inBox 1 [Square [] 0 1 1 1,Square [] 1 2 2 1,Square [] 0 3 5 2] =
        [Square [] 0 1 1 1,Square [] 1 2 2 1]
      inBox 1 [] = []
    VARIANT: The length of board.
  -}
  inBox :: Int -> Board -> Board
  inBox num [] = []
  inBox num (b@(Square _ _ _ _ boxNum):xs)
   | (num == boxNum) = b : inBox num xs
   | otherwise       = inBox num xs

  {- missingNumbers numbers
    PURPOSE: Finds the numbers 1 to 9 that's missing in a list.
    PRE: True
    POST: A list containing all the numbers missing 1 to 9 missing from the list numbers.
    EXAMPLES:
      missingNumbers [1,2,3] = [4,5,6,7,8,9]
      missingNumbers [] = [1,2,3,4,5,6,7,8,9]
  -}
  missingNumbers :: [Int] -> [Int]
  missingNumbers list = filter (flip notElem list) [1..9]

  {- getValues board
    PURPOSE: To get all non-zero values from a specific board.
    PRE: True
    POST: A list containing all non-zero values in board.
    EXAMPLE:
      getValues [Square [] 0 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1] = [1,9]
    VARIANT: The length of board.
  -}
  getValues :: Board -> [Int]
  getValues [] = []
  getValues ((Square _ 0 _ _ _):xs)     = getValues xs
  getValues ((Square _ value _ _ _):xs) = value : getValues xs

  {- validBoard board
    PURPOSE: Checks if a board is a valid solution
    PRE: True
    POST: The input board if board is a valid sulution, i.e. it contains 1-9 in
      each row, column and box according the rules of the game, otherwise [].
    EXAMPLES:
      validBoard (makeBoard hardBoard) = []
      validBoard (makeBoard validSolution) = validSolution
    VARIANT: Size of a regular sudoku board, 9 represents the 9 rows, colums and
      boxes of a board.
  -}
  validBoard :: Board -> Board
  validBoard []    = []
  validBoard board =
    let
      {- validBoardAux um board
        PURPOSE: Checks if a board is a valid solution
        PRE: True
        POST: The input board if board is a valid sulution, i.e. it contains 1-9 in
          each row, column and box according the rules of the game, otherwise [].
        EXAMPLES:
          validBoard 1 (makeBoard hardBoard) = []
          validBoard 1 (makeBoard validSolution) = validSolution
        VARIANT: Size of a regular sudoku board, 9 represents the 9 rows, colums and
          boxes of a board.
      -}
      validBoardAux :: Int -> Board -> Board
      validBoardAux 9 board = board
      validBoardAux num board
      -- recursively check if rows/columns/boxes are valid (Values = 1..9)
        | row && col && box = validBoardAux (num+1) board
        | otherwise = []
        where
          row = sort (getValues(inRow num board)) == [1..9]
          col = sort (getValues(inCol num board)) == [1..9]
          box = sort (getValues(inBox num board)) == [1..9]
    in
      validBoardAux 1 board

  {- dupeInRCB numbers
    PURPOSE: Checks if a list contains duplicate values.
    PRE: True
    POST: Bool, True if it contains duplicates otherwise False.
    EXAMPLES:
     dupeInRCB [1..9] = False
     dupeInRCB [1,1,2,3,4,5,6,7,8,9] = True
    VARIANT: The length of numbers.
  -}
  dupeInRCB :: [Int] -> Bool
  dupeInRCB numbers =
    let
      {- dupeInRCBAux numbers
        PURPOSE: Checks if a list contains duplicate values.
        PRE: True
        POST: Bool, True if it contains duplicates otherwise False.
        EXAMPLES:
          dupeInRCBAux (group $ sort [1..9]) = False
          dupeInRCBAux (group $ sort [1,1,2,3,4,5,6,7,8]) = True
        VARIANT: The length of numbers.
      -}
      dupeInRCBAux :: [[Int]] -> Bool
      dupeInRCBAux [] = False
      dupeInRCBAux (x:xs)
        | length x > 1 = True
        | otherwise    = dupeInRCBAux xs
    in
      dupeInRCBAux (group $ sort numbers)

  {-
    solvedSudoku board
    PURPOSE:  To check if the board contains only non-zero entries.
    PRE:      True
    POST:     True if every square has a value that is not 0 otherwise False.
    EXAMPLES:
      solvedSudoku [Square [1,2,3] 0 1 1 1,Square [3,4,5,6] 0 1 2 1] = False
      solvedSudoku [Square [] 1 1 1 1,Square [] 2 1 2 1] = True
    VARIANT: The length of board.
  -}
  solvedSudoku :: Board -> Bool
  solvedSudoku [] = True
  solvedSudoku ((Square _ n _ _ _):xs)
    | n == 0    = False
    | otherwise = solvedSudoku xs

  {-
    replaceSquare board square
    PURPOSE:  Updates an existing square in a board
    PRE:      True
    POST:     A Board where the input square is updated if found within that board.
    EXAMPLES:
      replaceSquare [Square [] 0 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1] (Square [] 3 1 1 1) =
        [Square [] 3 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1]
    VARIANT: The length of board.
  -}
  replaceSquare :: Board -> Square -> Board
  replaceSquare [] _ = []
  replaceSquare (squareOld@(Square _ _ r1 c1 b1):xs) squareNew@(Square _ _ r2 c2 b2)
    | (r1,c1,b1) == (r2,c2,b2)         = squareNew : xs
    | otherwise                        = squareOld : replaceSquare xs squareNew

  {-
    bestGuess square
    PURPOSE: To get the square with the least amount of possible candidates.
    PRE: True
    POST: A square with the least amount of possible candidates.
    EXAMPLES:
      bestGuess [Square [1,3] 0 1 1 1,Square [1,3,4] 0 1 2 1 ...,Square [] 1 9 9 9] = Square [1,3] 0 1 1 1
  -}
  bestGuess :: Board -> Square
  bestGuess board =
    let
      {-
        bestGuessAux square board
        PURPOSE: To check which square has the least amount of possible candidates.
        PRE: True
        POST: A Square with the least amount of possible candidates
        EXAMPLES:
          bestGuessAux (Square [1,3] 0 1 1 1) [Square [1,3,4] 0 1 2 1] = Square [1,3] 0 1 1 1
          bestGuessAux (Square [1,3] 0 1 1 1) [Square [1,4] 0 1 2 1] = Square [1,3] 0 1 1 1
          bestGuessAux (Square [1,3] 0 1 1 1) [Square [] 2 1 2 1] = Square [1,3] 0 1 1 1
        VARIANT: The length of board.
      -}
      bestGuessAux :: Square -> Board -> Square
      bestGuessAux square [] = square
      bestGuessAux square@(Square cand2 val2 _ _ _) (newSquare@(Square cand1 val1 _ _ _):xs)
        | val1 /= 0                                = bestGuessAux square xs
        | val2 /= 0 || length cand1 < length cand2 = bestGuessAux newSquare xs
        | otherwise                                = bestGuessAux square xs
    in
      bestGuessAux (head board) board

  {-
    validGuess board r c b
    PURPOSE: Checks if a board contains duplicates in a given row, col and box.
    PRE: True
    POST: The Board board if no duplicates were found. Otherwise it returns a empty list.
    EXAMPLES:
      validGuess [Square [] 6 1 1 1,Square [] 6 1 2 1, ... ,Square [] 1 9 9 9] 1 1 1 = []  (two 6:s in the same row and box)
      validGuess (makeBoard validSolution) 1 1 1 = (makeBoard validSolution)
  -}
  validGuess :: Board -> Int -> Int -> Int -> Board
  validGuess board r c b
    | not (dupeInRCB $ getValues $ inRow r board) &&
      not (dupeInRCB $ getValues $ inCol c board) &&
      not (dupeInRCB $ getValues $ inBox b board) = board
    | otherwise = []

  {-
    otherCanditates board square
    PURPOSE:  Extracts the candidates from all squares that are not identical to the provided square.
    PRE:      True
    POST:     A list with of candidates from all squares that are not identical to the provided square.
    EXAMPLES:
     otherCanditates [Square [1,2,3] 0 1 1 1,Square [3,4,5,6] 0 1 2 1]  (Square [3,4,5,6] 0 1 2 1) = [1,2,3]
    VARIANT: The length of board.
  -}
  otherCandidates :: Board -> Square -> [Int]
  otherCandidates [] _ = []
  otherCandidates (x@(Square cand _ _ _ _):xs) square
    | x == square = otherCandidates xs square
    | otherwise   = cand ++ otherCandidates xs square

  {-
    uniqueCandidates first second
    PURPOSE:  Returns the elements that are unique to the first list
    PRE:      True
    POST:     A list with the elements that are unique to the first list
    EXAMPLES:
      uniqueCandidates [1..9] [6..9] = [1,2,3,4,5]
  -}
  uniqueCandidates :: [Int] -> [Int] -> [Int]
  uniqueCandidates first second = filter (flip notElem second) first

  {-
    findCandidates original board
    PURPOSE:  Find all the candidates for every unsolved square in a board
    PRE:      True
    POST:     A board with all unsolved squares now containing possible candidates for that square in their list cand
    EXAMPLES:
      findCandidates [Square [] 0 1 1 1,Square [] 1 1 2 1] [Square [] 0 1 1 1,Square [] 1 1 2 1] =
        [Square [2,3,4,5,6,7,8,9] 0 1 1 1,Square [] 1 1 2 1]
    VARIANT: The length of board.
  -}
  findCandidates :: Board -> Board -> Board
  findCandidates original [] = []
  findCandidates original board@(square@(Square cand x row col box):xs)
    | x /= 0 = square : findCandidates original xs
    | otherwise = (Square possibleCand x row col box) : findCandidates original xs
    where
      possibleCand = missingNumbers $ 
        (getValues $ inRow row original) ++
        (getValues $ inCol col original) ++
        (getValues $ inBox box original)
