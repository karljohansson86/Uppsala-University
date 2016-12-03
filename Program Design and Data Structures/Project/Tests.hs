module Tests where
  import Test.HUnit
  import Helpers
  import Main
  import BoardConstructors
  import Data



  -- TEST CASES FOR
  -- ==============
  -- *    MAIN    *
  -- ==============


  -- ALT START
  -- =========
  testM1 = TestCase $ assertEqual "altStart:A solvable board is inserted and solved"
    (altStart extremeBoard) testBoardExtremeSolved

  testM2 = TestCase $ assertEqual "altStart: A unsolvable board is inserted"
    (altStart ninesBoard) []


  -- SOLVE
  -- =====
  testM3 = TestCase $ assertEqual "solve: A empty board inserted"
    (solve []) []

  testM4 = TestCase $ assertEqual "solve: A board which can be solved solely by using solveEasy and solveWithConstraints inserted"
    (solve (makeBoard extremeBoard)) testBoardExtremeSolved

  testM5 = TestCase $ assertEqual "solve: A board which can be solved only by using solveWithForce inserted"
    (solve (makeBoard worldHardestBoard)) testBoardWorldHardestSolved


  -- SOLVE EASY
  -- ==========
  testM6 = TestCase $ assertEqual "solveEasy: Value inserted by missingInRow"
    (solveEasy testCaseMissingInRow testCaseMissingInRow) [Square [] 1 1 1 1,Square [] 2 1 2 1,Square [] 3 1 3 1,
                                                          Square [] 4 1 4 2,Square [] 5 1 5 2,Square [] 6 1 6 2,
                                                          Square [] 7 1 7 3,Square [] 8 1 8 3,Square [] 9 1 9 3]

  testM7 = TestCase $ assertEqual "solveEasy: Value inserted by missingInCol"
    (solveEasy testCaseMissingInCol testCaseMissingInCol) [Square [] 1 1 1 1,Square [] 2 2 1 1,Square [] 3 3 1 1,
                                                          Square [] 4 4 1 4,Square [] 5 5 1 4,Square [] 6 6 1 4,
                                                          Square [] 7 7 1 7,Square [] 8 8 1 7,Square [] 9 9 1 7]

  testM8 = TestCase $ assertEqual "solveEasy: Value inserted by missingInBox"
    (solveEasy testCaseMissingInBox testCaseMissingInBox) [Square [] 1 1 1 1,Square [] 2 1 2 1,Square [] 3 1 3 1,
                                                          Square [] 4 2 1 1,Square [] 5 2 2 1,Square [] 6 2 3 1,
                                                          Square [] 7 3 1 1,Square [] 8 3 2 1,Square [] 9 3 3 1]

  testM9 = TestCase $ assertEqual "solveEasy: Value inserted by possibleCand"
    (solveEasy testcasepossibleCand testcasepossibleCand) testcasepossibleCandSolved

  testM10 = TestCase $ assertEqual "solveEasy: A board with no empty squares inserted"
    (solveEasy [Square [] 1 1 1 1, Square [] 2 1 2 1] [Square [] 1 1 1 1, Square [] 2 1 2 1]) [Square [] 1 1 1 1, Square [] 2 1 2 1]

  testM11 = TestCase $ assertEqual "solveEasy: No values can be inserted by solveEasy"
    (solveEasy [Square [1,2,3,4,5,6,7] 8 1 1 1, Square [1,2,3,4,5,6,7] 9 1 2 1] [Square [1,2,3,4,5,6,7] 8 1 1 1, Square [1,2,3,4,5,6,7] 9 1 2 1]) [Square [1,2,3,4,5,6,7] 8 1 1 1, Square [1,2,3,4,5,6,7] 9 1 2 1]


  -- SOLVE WITH CONSTRAINTS
  -- =====================
  testM12 = TestCase $ assertEqual "solveWithConstraints: Value inserted by uniqueSolutionsBox"
    (solveWithConstraints consTest) consSolved

  testM13 = TestCase $ assertEqual "solveWithConstraints: Value inserted by uniqueSolutionRow"
    (solveWithConstraints consRow) consRowSolved

  testM14 = TestCase $ assertEqual "solveWithConstraints: Value inserted with missingInCol"
    (solveWithConstraints consCol) consColSolved

  testM15 = TestCase $ assertEqual "solveWithConstraints: A board with no empty squares inserted"
   (solveWithConstraints [Square [] 1 1 1 1, Square [] 2 1 2 1]) [Square [] 1 1 1 1, Square [] 2 1 2 1]

  testM16 = TestCase $ assertEqual "solveWithConstraints: No values can be inserted by solveWithConstraints"
    (solveWithConstraints [Square [1,2,3,4,5,6,7] 8 1 1 1, Square [1,2,3,4,5,6,7] 9 1 2 1]) [Square [1,2,3,4,5,6,7] 8 1 1 1, Square [1,2,3,4,5,6,7] 9 1 2 1]

  --SOLVE WITH FORCE
  -- ================
  -- In the solveWithForce test, we show how each recursion can evaluate in two different results.

  testM17 = TestCase $ assertEqual "solveWithForce: Result of the recursion which returns the valid solution to the board "
    (solveWithForce easyBoardAfterSolveWithConstraints) easyBoardAfterSolveWithForce

  testM18 = TestCase $ assertEqual "solveWithForce: Result of the recursion which returns one of the many invalid solution to the board"
    (solveWithForce solveWithForceInvalid) []








  -- TEST CASES FOR
  -- ==============
  -- *   HELPERS  *
  -- ==============


  -- INROW
  -- =====
  testH1 = TestCase $ assertEqual "inRow: Return all squares in row 1"
    (inRow 1 testBoardEasy)
    [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,
     Square [] 0 1 5 2,Square [] 0 1 6 2,Square [] 0 1 7 3,Square [] 0 1 8 3,
     Square [] 3 1 9 3]

  testH2 = TestCase $ assertEqual "inRow: No squares in row 0"
    (inRow 0 testBoardEasy) []


  -- INCOL
  -- =====
  testH3 = TestCase $ assertEqual "inCol: Return all squares in col 1"
    (inCol 1 testBoardEasy)
    [Square [] 0 1 1 1,Square [] 0 2 1 1,Square [] 3 3 1 1,Square [] 1 4 1 4,
     Square [] 0 5 1 4,Square [] 8 6 1 4,Square [] 0 7 1 7,Square [] 5 8 1 7,
     Square [] 0 9 1 7]

  testH4 = TestCase $ assertEqual "inCol: No squares in col 0"
    (inCol 0 testBoardEasy) []


  -- INBOX
  -- =====
  testH5 = TestCase $ assertEqual "inBox: Return all squares in box 1"
    (inBox 1 testBoardEasy)
    [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 0 1 3 1,Square [] 0 2 1 1,
     Square [] 0 2 2 1,Square [] 7 2 3 1,Square [] 3 3 1 1,Square [] 2 3 2 1,
     Square [] 6 3 3 1]

  testH6 = TestCase $ assertEqual "inBox: No squares in box 0"
    (inBox 0 testBoardEasy) []


  -- MISSING NUMBERS
  -- ===============
  testH7 = TestCase $ assertEqual "missingNumbers: Numbers 4-9 missing"
    (missingNumbers [1..3]) [4..9]

  testH8 = TestCase $ assertEqual "missingNumbers: No numbers missing in complete list"
    (missingNumbers [1..9]) []


  -- GET VALUES
  -- ==========
  testH9 = TestCase $ assertEqual "getValues: Ignore squares with zero value"
    (getValues [Square [6..9] 0 1 1 1, Square [] 5 5 5 5, Square [] 4 4 4 4, Square [] 3 3 3 3]) [5,4,3]

  testH10 = TestCase $ assertEqual "getValues: Empty squares should be removed"
    (getValues [Square [1,3] 0 2 2 2, Square [5,6,9] 0 6 6 6, Square [1,4] 0 8 8 8]) []


  -- VALID BOARD
  -- ===========
  testH11 = TestCase $ assertEqual "validBoard: Invalid/incomplete solution returns empty list"
    (validBoard testBoardEasy) []

  testH12 = TestCase $ assertEqual "validBoard: Correctly solved board should return true"
    (validBoard testBoardExtremeSolved) testBoardExtremeSolved


  -- DUPE IN RCB
  -- ===========
  testH13 = TestCase $ assertEqual "dupeInRCB: A list with unique entries is not invalid"
    (dupeInRCB [1..9]) False

  testH14 = TestCase $ assertEqual "dupeInRCB: Duplicates are not allowed in a row, col or box"
    (dupeInRCB ([1..9] ++ [1..9])) True


  -- SOLVED SUDOKO
  -- ==============
  testH15 = TestCase $ assertEqual "solvedSudoku: Solved board inserted"
    (solvedSudoku testBoardExtremeSolved) True

  testH16 = TestCase $ assertEqual "solvedSudoku: Unsolved board inserted"
    (solvedSudoku testBoardEasy) False


  -- REPLACE SQUARE
  -- ==============
  testH17 = TestCase $ assertEqual "replaceSquare: No square to replace in inserted board"
    (replaceSquare [Square [] 0 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1] (Square [] 3 1 0 1)) [Square [] 0 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1]

  testH18 = TestCase $ assertEqual "replaceSquare: Square replaced in inserted board "
    (replaceSquare [Square [] 0 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1] (Square [] 3 1 1 1)) [Square [] 3 1 1 1,Square [] 1 1 2 1,Square [] 9 1 3 1]


  -- BEST GUESS
  -- ==========
  testH19 = TestCase $ assertEqual "bestGuess: The first square is the only option"
    (bestGuess [Square [1,2] 0 1 1 1, Square [] 2 1 2 1]) (Square [1,2] 0 1 1 1)

  testH20 = TestCase $ assertEqual "bestGuess: The squares have an equal amount of candidates but the first is chosen"
    (bestGuess [Square [1,2] 0 1 1 1, Square [3,4] 0 1 2 1]) (Square [1,2] 0 1 1 1)

  testH21 = TestCase $ assertEqual "bestGuess: The second square is the best option"
    (bestGuess [Square [1,2,3] 0 1 1 1, Square [1,2] 0 1 2 1]) (Square [1,2] 0 1 2 1)


  -- VALID GUESS
  -- ===========
  testH22 = TestCase $ assertEqual "validGuess: The input board is a valid board"
    (validGuess (makeBoard validSolution) 1 1 1)  (makeBoard validSolution)

  testH23 = TestCase $ assertEqual "validGuess: Duplicate values in the first row"
    (validGuess testBoardInvalidEasy 1 1 1) []


  -- OTHER CANDIDATES
  -- =================
  testH24 = TestCase $ assertEqual "otherCandidates: Square exist in the board so it's candidates is ignored"
    (otherCandidates [Square [1,2,3] 0 1 1 1,Square [3,4,5,6] 0 1 2 1] (Square [3,4,5,6] 0 1 2 1)) [1,2,3]

  testH25 = TestCase $ assertEqual "otherCandidates: Square dosen't exist in board so all candidates are returned"
    (otherCandidates [Square [1,2,3] 0 1 1 1,Square [3,4,5,6] 0 1 2 1]  (Square [1..6] 0 1 2 1)) [1,2,3,3,4,5,6]


  -- UNIQUE CANDIDATES
  -- =================
  testH26 = TestCase $ assertEqual "uniqueCandidates: 1..5 are unique to the first list"
    (uniqueCandidates [1..9] [6..9]) [1,2,3,4,5]

  testH27 = TestCase $ assertEqual "uniqueCandidates: No unique elements in the first list"
    (uniqueCandidates [6..9] [1..9]) []


  -- FIND CANDIDATES
  -- ===============
  testH28 = TestCase $ assertEqual "findCandidates: Inserted usolved squares now have all possible candidates"
    (findCandidates [Square [] 0 1 1 1,Square [] 1 1 2 1] [Square [] 0 1 1 1,Square [] 1 1 2 1]) ([Square [2,3,4,5,6,7,8,9] 0 1 1 1,Square [] 1 1 2 1])

  testH29 = TestCase $ assertEqual "findCandidates: No unsolved squares inserted"
    (findCandidates [Square [] 4 1 1 1,Square [] 1 1 2 1] [Square [] 2 1 1 1,Square [] 1 1 2 1]) [Square [] 2 1 1 1,Square [] 1 1 2 1]




  -- TEST CASES FOR
  -- ==============
  -- * BOARDCONST *
  -- ==============


  -- MAKEBOARD
  -- ===========

  -- NOT TESTABLE DUE TO ERROR HANDLING !!
  -- =====================================
  -- MORE INFORMATION IN THE DOCUMENTATION

  {-
    testBErr = TestCase $ assertEqual "makeBoard: a list with less than 17 non-zero numbers inserted "
      (makeBoard zeroBoard) []

    testBErr2 = TestCase $ assertEqual "makeBoard: a list whose length differs from 81 inserted"
      (makeBoard (easyBoard ++ [1])) []
  -}

  testB1 = TestCase $ assertEqual "makeBoard: a list of 81 numbers is inserted which of atleast 17 is non-zero"
    (makeBoard easyBoard) testBoardEasy


  -- BOARD TO LIST
  -- ==============
  testB2 = TestCase $ assertEqual "boardToList: Returns all values of a board"
    (boardToList testBoardEasy) easyBoard


  testB3 = TestCase $ assertEqual "boardToList: Empty board"
    (boardToList []) []


  -- BOARD TO STRING
  -- ===============
  testB4 = TestCase $ assertEqual "boardToString: Should return the string \"123456789\""
    (boardToString [1..9]) "123456789"

  testB5 = TestCase $ assertEqual "boardToString: Zeros converted to dots"
    (boardToString [0,0,0]) "..."




  mainTests      = TestList [testM1, testM2, testM3, testM4, testM5, testM6, testM7,
                             testM8, testM9, testM10, testM11, testM12, testM13, testM14,
                             testM15, testM16, testM17, testM18]

  helpersTests   = TestList [testH1, testH2, testH3, testH4, testH5, testH6, testH7,
                             testH8, testH9, testH10, testH11, testH12, testH13, testH14,
                             testH15, testH16, testH17, testH18, testH19, testH20, testH21,
                             testH22, testH23, testH24, testH25, testH26, testH27, testH28,
                             testH29]

  boardConsTests = TestList [testB1, testB2, testB3, testB4, testB5]

  -- RUN ALL TESTS
  runtests = runTestTT $ TestList [helpersTests, mainTests, boardConsTests]




  -- BOARDS FOR TESTING
  -- ==================

  testBoardEasy =
    [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,
     Square [] 0 1 5 2,Square [] 0 1 6 2,Square [] 0 1 7 3,Square [] 0 1 8 3,
     Square [] 3 1 9 3,Square [] 0 2 1 1,Square [] 0 2 2 1,Square [] 7 2 3 1,
     Square [] 0 2 4 2,Square [] 6 2 5 2,Square [] 0 2 6 2,Square [] 2 2 7 3,
     Square [] 8 2 8 3,Square [] 0 2 9 3,Square [] 3 3 1 1,Square [] 2 3 2 1,
     Square [] 6 3 3 1,Square [] 8 3 4 2,Square [] 0 3 5 2,Square [] 0 3 6 2,
     Square [] 0 3 7 3,Square [] 0 3 8 3,Square [] 7 3 9 3,Square [] 1 4 1 4,
     Square [] 0 4 2 4,Square [] 5 4 3 4,Square [] 4 4 4 5,Square [] 0 4 5 5,
     Square [] 0 4 6 5,Square [] 0 4 7 6,Square [] 2 4 8 6,Square [] 6 4 9 6,
     Square [] 0 5 1 4,Square [] 0 5 2 4,Square [] 4 5 3 4,Square [] 0 5 4 5,
     Square [] 0 5 5 5,Square [] 2 5 6 5,Square [] 0 5 7 6,Square [] 0 5 8 6,
     Square [] 0 5 9 6,Square [] 8 6 1 4,Square [] 0 6 2 4,Square [] 0 6 3 4,
     Square [] 7 6 4 5,Square [] 0 6 5 5,Square [] 6 6 6 5,Square [] 4 6 7 6,
     Square [] 9 6 8 6,Square [] 0 6 9 6,Square [] 0 7 1 7,Square [] 4 7 2 7,
     Square [] 0 7 3 7,Square [] 0 7 4 8,Square [] 0 7 5 8,Square [] 3 7 6 8,
     Square [] 0 7 7 9,Square [] 7 7 8 9,Square [] 5 7 9 9,Square [] 5 8 1 7,
     Square [] 0 8 2 7,Square [] 0 8 3 7,Square [] 1 8 4 8,Square [] 8 8 5 8,
     Square [] 7 8 6 8,Square [] 3 8 7 9,Square [] 0 8 8 9,Square [] 0 8 9 9,
     Square [] 0 9 1 7,Square [] 0 9 2 7,Square [] 0 9 3 7,Square [] 0 9 4 8,
     Square [] 2 9 5 8,Square [] 0 9 6 8,Square [] 8 9 7 9,Square [] 0 9 8 9,
     Square [] 0 9 9 9]

  testBoardInvalidEasy =
    [Square [] 0 1 1 1,Square [] 8 1 2 1,Square [] 8 1 3 1,Square [] 0 1 4 2,
     Square [] 0 1 5 2]

  testBoardExtremeSolved =
    [Square [] 3 1 1 1,Square [] 1 1 2 1,Square [] 6 1 3 1,Square [] 8 1 4 2,
    Square [] 4 1 5 2,Square [] 9 1 6 2,Square [] 7 1 7 3,Square [] 2 1 8 3,
    Square [] 5 1 9 3,Square [] 5 2 1 1,Square [] 7 2 2 1,Square [] 4 2 3 1,
    Square [] 1 2 4 2,Square [] 6 2 5 2,Square [] 2 2 6 2,Square [] 9 2 7 3,
    Square [] 8 2 8 3,Square [] 3 2 9 3,Square [] 8 3 1 1,Square [] 9 3 2 1,
    Square [] 2 3 3 1,Square [] 7 3 4 2,Square [] 3 3 5 2,Square [] 5 3 6 2,
    Square [] 1 3 7 3,Square [] 6 3 8 3,Square [] 4 3 9 3,Square [] 1 4 1 4,
    Square [] 5 4 2 4,Square [] 8 4 3 4,Square [] 4 4 4 5,Square [] 9 4 5 5,
    Square [] 7 4 6 5,Square [] 2 4 7 6,Square [] 3 4 8 6,Square [] 6 4 9 6,
    Square [] 9 5 1 4,Square [] 6 5 2 4,Square [] 7 5 3 4,Square [] 2 5 4 5,
    Square [] 5 5 5 5,Square [] 3 5 6 5,Square [] 4 5 7 6,Square [] 1 5 8 6,
    Square [] 8 5 9 6,Square [] 4 6 1 4,Square [] 2 6 2 4,Square [] 3 6 3 4,
    Square [] 6 6 4 5,Square [] 8 6 5 5,Square [] 1 6 6 5,Square [] 5 6 7 6,
    Square [] 9 6 8 6,Square [] 7 6 9 6,Square [] 2 7 1 7,Square [] 4 7 2 7,
    Square [] 9 7 3 7,Square [] 3 7 4 8,Square [] 7 7 5 8,Square [] 8 7 6 8,
    Square [] 6 7 7 9,Square [] 5 7 8 9,Square [] 1 7 9 9,Square [] 7 8 1 7,
    Square [] 3 8 2 7,Square [] 5 8 3 7,Square [] 9 8 4 8,Square [] 1 8 5 8,
    Square [] 6 8 6 8,Square [] 8 8 7 9,Square [] 4 8 8 9,Square [] 2 8 9 9,
    Square [] 6 9 1 7,Square [] 8 9 2 7,Square [] 1 9 3 7,Square [] 5 9 4 8,
    Square [] 2 9 5 8,Square [] 4 9 6 8,Square [] 3 9 7 9,Square [] 7 9 8 9,
    Square [] 9 9 9 9]

  testBoardWorldHardestSolved =
    [Square [] 8 1 1 1,Square [] 1 1 2 1,Square [] 2 1 3 1,Square [] 7 1 4 2,
    Square [] 5 1 5 2,Square [] 3 1 6 2,Square [] 6 1 7 3,Square [] 4 1 8 3,
    Square [] 9 1 9 3,Square [] 9 2 1 1,Square [] 4 2 2 1,Square [] 3 2 3 1,
    Square [] 6 2 4 2,Square [] 8 2 5 2,Square [] 2 2 6 2,Square [] 1 2 7 3,
    Square [] 7 2 8 3,Square [] 5 2 9 3,Square [] 6 3 1 1,Square [] 7 3 2 1,
    Square [] 5 3 3 1,Square [] 4 3 4 2,Square [] 9 3 5 2,Square [] 1 3 6 2,
    Square [] 2 3 7 3,Square [] 8 3 8 3,Square [] 3 3 9 3,Square [] 1 4 1 4,
    Square [] 5 4 2 4,Square [] 4 4 3 4,Square [] 2 4 4 5,Square [] 3 4 5 5,
    Square [] 7 4 6 5,Square [] 8 4 7 6,Square [] 9 4 8 6,Square [] 6 4 9 6,
    Square [] 3 5 1 4,Square [] 6 5 2 4,Square [] 9 5 3 4,Square [] 8 5 4 5,
    Square [] 4 5 5 5,Square [] 5 5 6 5,Square [] 7 5 7 6,Square [] 2 5 8 6,
    Square [] 1 5 9 6,Square [] 2 6 1 4,Square [] 8 6 2 4,Square [] 7 6 3 4,
    Square [] 1 6 4 5,Square [] 6 6 5 5,Square [] 9 6 6 5,Square [] 5 6 7 6,
    Square [] 3 6 8 6,Square [] 4 6 9 6,Square [] 5 7 1 7,Square [] 2 7 2 7,
    Square [] 1 7 3 7,Square [] 9 7 4 8,Square [] 7 7 5 8,Square [] 4 7 6 8,
    Square [] 3 7 7 9,Square [] 6 7 8 9,Square [] 8 7 9 9,Square [] 4 8 1 7,
    Square [] 3 8 2 7,Square [] 8 8 3 7,Square [] 5 8 4 8,Square [] 2 8 5 8,
    Square [] 6 8 6 8,Square [] 9 8 7 9,Square [] 1 8 8 9,Square [] 7 8 9 9,
    Square [] 7 9 1 7,Square [] 9 9 2 7,Square [] 6 9 3 7,Square [] 3 9 4 8,
    Square [] 1 9 5 8,Square [] 8 9 6 8,Square [] 4 9 7 9,Square [] 5 9 8 9,
    Square [] 2 9 9 9]

  testCaseMissingInRow =
    [Square [] 0 1 1 1,Square [] 2 1 2 1,Square [] 3 1 3 1,
    Square [] 4 1 4 2,Square [] 5 1 5 2,Square [] 6 1 6 2,
    Square [] 7 1 7 3,Square [] 8 1 8 3,Square [] 9 1 9 3]

  testCaseMissingInCol =
    [Square [] 0 1 1 1,Square [] 2 2 1 1,Square [] 3 3 1 1,
    Square [] 4 4 1 4,Square [] 5 5 1 4,Square [] 6 6 1 4,
    Square [] 7 7 1 7,Square [] 8 8 1 7,Square [] 9 9 1 7]

  testCaseMissingInBox =
    [Square [] 0 1 1 1,Square [] 2 1 2 1,Square [] 3 1 3 1,
    Square [] 4 2 1 1,Square [] 5 2 2 1,Square [] 6 2 3 1,
    Square [] 7 3 1 1,Square [] 8 3 2 1,Square [] 9 3 3 1]

  testcasepossibleCandSolved =
    [Square [3,4,5,6,7,8,9] 0 1 1 1,Square [2,3,4,5,6,7,8,9] 0 1 2 1,Square [2,3,4,5,6,7,8] 0 1 3 1,
    Square [2,3,5,7,8,9] 0 1 4 2,Square [2,3,4,5,7,9] 0 1 5 2,Square [] 1 1 6 2,Square [2,3,4,5,6,7,8,9] 0 1 7 3,
    Square [2,3,4,5,6,7,8,9] 0 1 8 3,Square [2,3,4,5,6,8,9] 0 1 9 3,Square [1,3,4,5,6,7,8,9] 0 2 1 1,
    Square [1,2,3,4,5,6,7,8,9] 0 2 2 1,Square [1,2,3,4,5,6,7,8] 0 2 3 1,Square [2,3,5,7,8,9] 0 2 4 2,
    Square [2,3,4,5,7,9] 0 2 5 2,Square [2,4,5,7,8,9] 0 2 6 2,Square [1,2,3,4,5,6,7,8,9] 0 2 7 3,
    Square [1,2,3,4,5,6,7,8,9] 0 2 8 3,Square [1,2,3,4,5,6,8,9] 0 2 9 3,Square [1,3,4,5,7,8,9] 0 3 1 1,
    Square [1,2,3,4,5,7,8,9] 0 3 2 1,Square [1,2,3,4,5,7,8] 0 3 3 1,Square [2,3,5,7,8,9] 0 3 4 2,
    Square [2,3,4,5,7,9] 0 3 5 2,Square [] 6 3 6 2,Square [1,2,3,4,5,7,8,9] 0 3 7 3,Square [1,2,3,4,5,7,8,9] 0 3 8 3,
    Square [1,2,3,4,5,8,9] 0 3 9 3,Square [1,3,5,6,7,8] 0 4 1 4,Square [1,3,5,6,7,8] 0 4 2 4,Square [1,3,5,6,7,8] 0 4 3 4,
    Square [] 4 4 4 5,Square [1,2,3,5,6,7,9] 0 4 5 5,Square [2,5,7,9] 0 4 6 5,Square [1,2,3,5,6,8,9] 0 4 7 6,
    Square [1,2,3,5,6,8,9] 0 4 8 6,Square [1,2,3,5,6,8,9] 0 4 9 6,Square [1,3,4,5,6,7] 0 5 1 4,
    Square [1,3,4,5,6,7] 0 5 2 4,Square [1,3,4,5,6,7] 0 5 3 4,Square [1,2,3,5,6,7,9] 0 5 4 5,Square [] 8 5 5 5,
    Square [2,5,7,9] 0 5 6 5,Square [1,2,3,4,5,6,9] 0 5 7 6,Square [1,2,3,4,5,6,9] 0 5 8 6,Square [1,2,3,4,5,6,9] 0 5 9 6,
    Square [] 2 6 1 4,Square [1,3,4,5,6,8] 0 6 2 4,Square [] 9 6 3 4,Square [1,3,5,6] 0 6 4 5,Square [1,3,5,6] 0 6 5 5,
    Square [] 5 6 6 5,Square [1,3,4,6,8] 0 6 7 6,Square [1,3,4,6,8] 0 6 8 6,Square [] 7 6 9 6,Square [1,3,4,5,6,7,8,9] 0 7 1 7,
    Square [1,2,3,4,5,6,7,8,9] 0 7 2 7,Square [1,2,3,4,5,6,7,8] 0 7 3 7,Square [1,2,5,6,7,8,9] 0 7 4 8,
    Square [1,2,4,5,6,7,9] 0 7 5 8,Square [2,4,7,8,9] 0 7 6 8,Square [1,2,3,4,5,6,7,8,9] 0 7 7 9,
    Square [1,2,3,4,5,6,7,8,9] 0 7 8 9,Square [1,2,3,4,5,6,8,9] 0 7 9 9,Square [1,4,5,6,7,8,9] 0 8 1 7,
    Square [1,2,4,5,6,7,8,9] 0 8 2 7,Square [1,2,4,5,6,7,8] 0 8 3 7,Square [1,2,5,6,7,8,9] 0 8 4 8,
    Square [1,2,4,5,6,7,9] 0 8 5 8,Square [] 3 8 6 8,Square [1,2,4,5,6,7,8,9] 0 8 7 9,Square [1,2,4,5,6,7,8,9] 0 8 8 9,
    Square [1,2,4,5,6,8,9] 0 8 9 9,Square [1,3,4,5,6,7,8,9] 0 9 1 7,Square [1,2,3,4,5,6,7,8,9] 0 9 2 7,Square [1,2,3,4,5,6,7,8] 0 9 3 7,
    Square [1,2,5,6,7,8,9] 0 9 4 8,Square [1,2,4,5,6,7,9] 0 9 5 8,Square [2,4,7,8,9] 0 9 6 8,Square [1,2,3,4,5,6,7,8,9] 0 9 7 9,
    Square [1,2,3,4,5,6,7,8,9] 0 9 8 9,Square [1,2,3,4,5,6,8,9] 0 9 9 9]

  testcasepossibleCand =
    [Square [] 0 1 1 1,Square [] 0 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,
    Square [] 0 1 5 2,Square [] 1 1 6 2,Square [] 0 1 7 3,Square [] 0 1 8 3,
    Square [] 0 1 9 3,Square [] 0 2 1 1,Square [] 0 2 2 1,Square [] 0 2 3 1,
    Square [] 0 2 4 2,Square [] 0 2 5 2,Square [] 0 2 6 2,Square [] 0 2 7 3,
    Square [] 0 2 8 3,Square [] 0 2 9 3,Square [] 0 3 1 1,Square [] 0 3 2 1,
    Square [] 0 3 3 1,Square [] 0 3 4 2,Square [] 0 3 5 2,Square [] 6 3 6 2,
    Square [] 0 3 7 3,Square [] 0 3 8 3,Square [] 0 3 9 3,Square [] 0 4 1 4,
    Square [] 0 4 2 4,Square [] 0 4 3 4,Square [] 4 4 4 5,Square [] 0 4 5 5,
    Square [] 0 4 6 5,Square [] 0 4 7 6,Square [] 0 4 8 6,Square [] 0 4 9 6,
    Square [] 0 5 1 4,Square [] 0 5 2 4,Square [] 0 5 3 4,Square [] 0 5 4 5,
    Square [] 8 5 5 5,Square [] 0 5 6 5,Square [] 0 5 7 6,Square [] 0 5 8 6,
    Square [] 0 5 9 6,Square [] 2 6 1 4,Square [] 0 6 2 4,Square [] 9 6 3 4,
    Square [] 0 6 4 5,Square [] 0 6 5 5,Square [] 0 6 6 5,Square [] 0 6 7 6,
    Square [] 0 6 8 6,Square [] 7 6 9 6,Square [] 0 7 1 7,Square [] 0 7 2 7,
    Square [] 0 7 3 7,Square [] 0 7 4 8,Square [] 0 7 5 8,Square [] 0 7 6 8,
    Square [] 0 7 7 9,Square [] 0 7 8 9,Square [] 0 7 9 9,Square [] 0 8 1 7,
    Square [] 0 8 2 7,Square [] 0 8 3 7,Square [] 0 8 4 8,Square [] 0 8 5 8,
    Square [] 3 8 6 8,Square [] 0 8 7 9,Square [] 0 8 8 9,Square [] 0 8 9 9,
    Square [] 0 9 1 7,Square [] 0 9 2 7,Square [] 0 9 3 7,Square [] 0 9 4 8,
    Square [] 0 9 5 8,Square [] 0 9 6 8,Square [] 0 9 7 9,Square [] 0 9 8 9,
    Square [] 0 9 9 9]

  consTest =
    [Square [] 2 1 1 1,Square [] 0 1 2 1,Square [] 0 1 3 1,Square [] 5 1 4 2,
    Square [] 0 1 5 2,Square [] 0 1 6 2,Square [] 4 1 7 3,Square [] 9 1 8 3,
    Square [] 0 1 9 3,Square [] 6 2 1 1,Square [] 0 2 2 1,Square [] 5 2 3 1,
    Square [] 0 2 4 2,Square [] 9 2 5 2,Square [] 0 2 6 2,Square [] 1 2 7 3,
    Square [] 0 2 8 3,Square [] 2 2 9 3,Square [] 0 3 1 1,Square [] 9 3 2 1,
    Square [] 0 3 3 1,Square [] 2 3 4 2,Square [] 0 3 5 2,Square [] 4 3 6 2,
    Square [] 0 3 7 3,Square [] 0 3 8 3,Square [] 0 3 9 3]

  consSolved =
    [Square [] 2 1 1 1,Square [] 1 1 2 1,Square [1,3,7,8] 0 1 3 1,Square [] 5 1 4 2,
    Square [1,3,6,7,8] 0 1 5 2,Square [1,3,6,7,8] 0 1 6 2,Square [] 4 1 7 3,
    Square [] 9 1 8 3,Square [3,6,7,8] 0 1 9 3,Square [] 6 2 1 1,Square [] 4 2 2 1,
    Square [] 5 2 3 1,Square [3,7,8] 0 2 4 2,Square [] 9 2 5 2,Square [3,7,8] 0 2 6 2,
    Square [] 1 2 7 3,Square [3,7,8] 0 2 8 3,Square [] 2 2 9 3,Square [1,3,7,8] 0 3 1 1,
    Square [] 9 3 2 1,Square [1,3,7,8] 0 3 3 1,Square [] 2 3 4 2,Square [1,3,6,7,8] 0 3 5 2,
    Square [] 4 3 6 2,Square [3,5,6,7,8] 0 3 7 3,Square [3,5,6,7,8] 0 3 8 3,Square [] 5 3 9 3]

  consRow =
    [Square [] 2 1 1 1,Square [] 0 1 2 1,Square [] 7 1 3 1,Square [] 5 1 4 2,Square [] 0 1 5 2,
    Square [] 0 1 6 2,Square [] 4 1 7 3,Square [] 9 1 8 3,Square [] 6 1 9 3,Square [] 6 2 1 1,
    Square [] 0 2 2 1,Square [] 3 2 3 1,Square [] 8 2 4 2,Square [] 9 2 5 2,Square [] 0 2 6 2,
    Square [] 1 2 7 3,Square [] 8 2 8 3,Square [] 2 2 9 3,Square [] 1 3 1 1,Square [] 9 3 2 1,
    Square [] 8 3 3 1,Square [] 0 3 4 2,Square [] 0 3 5 2,Square [] 6 3 6 2,Square [] 0 3 7 3,
    Square [] 0 3 8 3,Square [] 5 3 9 3]

  consRowSolved =
    [Square [] 2 1 1 1,Square [] 0 1 2 1,Square [] 7 1 3 1,Square [] 5 1 4 2,Square [] 1 1 5 2,
    Square [1,3] 0 1 6 2,Square [] 4 1 7 3,Square [] 9 1 8 3,Square [] 6 1 9 3,Square [] 6 2 1 1,
    Square [] 5 2 2 1,Square [] 3 2 3 1,Square [] 8 2 4 2,Square [] 9 2 5 2,Square [] 7 2 6 2,
    Square [] 1 2 7 3,Square [] 8 2 8 3,Square [] 2 2 9 3,Square [] 1 3 1 1,Square [] 9 3 2 1,
    Square [] 8 3 3 1,Square [2,3,4,7] 0 3 4 2,Square [2,3,4,7] 0 3 5 2,Square [] 6 3 6 2,
    Square [3,7] 0 3 7 3,Square [3,7] 0 3 8 3,Square [] 5 3 9 3]

  consCol =
    [Square [] 0 1 1 1,Square [] 0 1 2 1,Square [] 0 1 3 1,Square [] 0 1 4 2,Square [] 0 1 5 2,
    Square [] 0 1 6 2,Square [] 6 1 7 3,Square [] 8 1 8 3,Square [] 3 1 9 3,Square [] 6 2 1 1,
    Square [] 4 2 2 1,Square [] 0 2 3 1,Square [] 0 2 4 2,Square [] 7 2 5 2,Square [] 3 2 6 2,
    Square [] 0 2 7 3,Square [] 0 2 8 3,Square [] 9 2 9 3,Square [] 3 3 1 1,Square [] 0 3 2 1,
    Square [] 9 3 3 1,Square [] 0 3 4 2,Square [] 6 3 5 2,Square [] 0 3 6 2,Square [] 7 3 7 3,
    Square [] 4 3 8 3,Square [] 5 3 9 3,Square [] 5 4 1 4,Square [] 9 4 2 4,Square [] 6 4 3 4,
    Square [] 0 4 4 5,Square [] 0 4 5 5,Square [] 0 4 6 5,Square [] 8 4 7 6,Square [] 5 4 8 6,
    Square [] 0 4 9 6,Square [] 8 5 1 4,Square [] 0 5 2 4,Square [] 3 5 3 4,Square [] 4 5 4 5,
    Square [] 5 5 5 5,Square [] 6 5 6 5,Square [] 9 5 7 6,Square [] 0 5 8 6,Square [] 2 5 9 6,
    Square [] 0 6 1 4,Square [] 0 6 2 4,Square [] 0 6 3 4,Square [] 0 6 4 5,Square [] 0 6 5 5,
    Square [] 0 6 6 5,Square [] 4 6 7 6,Square [] 3 6 8 6,Square [] 6 6 9 6,Square [] 9 7 1 7,
    Square [] 6 7 2 7,Square [] 0 7 3 7,Square [] 0 7 4 8,Square [] 0 7 5 8,Square [] 0 7 6 8,
    Square [] 3 7 7 9,Square [] 0 7 8 9,Square [] 8 7 9 9,Square [] 7 8 1 7,Square [] 3 8 2 7,
    Square [] 0 8 3 7,Square [] 6 8 4 8,Square [] 8 8 5 8,Square [] 0 8 6 8,Square [] 0 8 7 9,
    Square [] 9 8 8 9,Square [] 0 8 9 9,Square [] 0 9 1 7,Square [] 2 9 2 7,Square [] 8 9 3 7,
    Square [] 0 9 4 8,Square [] 0 9 5 8,Square [] 0 9 6 8,Square [] 0 9 7 9,Square [] 6 9 8 9,
    Square [] 0 9 9 9]

  consColSolved =
    [Square [1,2] 0 1 1 1,Square [] 5 1 2 1,Square [1,2,5,7] 0 1 3 1,Square [1,2,5,9] 0 1 4 2,
    Square [1,2,4,9] 0 1 5 2,Square [1,2,4,5,9] 0 1 6 2,Square [] 6 1 7 3,Square [] 8 1 8 3,
    Square [] 3 1 9 3,Square [] 6 2 1 1,Square [] 4 2 2 1,Square [1,2,5] 0 2 3 1,
    Square [] 8 2 4 2,Square [] 7 2 5 2,Square [] 3 2 6 2,Square [1,2] 0 2 7 3,
    Square [1,2] 0 2 8 3,Square [] 9 2 9 3,Square [] 3 3 1 1,Square [] 8 3 2 1,
    Square [] 9 3 3 1,Square [1,2,8] 0 3 4 2,Square [] 6 3 5 2,Square [1,2,8] 0 3 6 2,
    Square [] 7 3 7 3,Square [] 4 3 8 3,Square [] 5 3 9 3,Square [] 5 4 1 4,Square [] 9 4 2 4,
    Square [] 6 4 3 4,Square [1,2,3,7] 0 4 4 5,Square [1,2,3] 0 4 5 5,Square [1,2,7] 0 4 6 5,
    Square [] 8 4 7 6,Square [] 5 4 8 6,Square [1,7] 0 4 9 6,Square [] 8 5 1 4,
    Square [1,7] 0 5 2 4,Square [] 3 5 3 4,Square [] 4 5 4 5,Square [] 5 5 5 5,
    Square [] 6 5 6 5,Square [] 9 5 7 6,Square [1,7] 0 5 8 6,Square [] 2 5 9 6,
    Square [1,2] 0 6 1 4,Square [1,7] 0 6 2 4,Square [1,2,7] 0 6 3 4,Square [1,2,7,8,9] 0 6 4 5,
    Square [1,2,9] 0 6 5 5,Square [1,2,7,8,9] 0 6 6 5,Square [] 4 6 7 6,Square [] 3 6 8 6,
    Square [] 6 6 9 6,Square [] 9 7 1 7,Square [] 6 7 2 7,Square [1,4,5] 0 7 3 7,Square [1,2,5,7] 0 7 4 8,
    Square [1,2,4] 0 7 5 8,Square [1,2,4,5,7] 0 7 6 8,Square [] 3 7 7 9,Square [1,2,7] 0 7 8 9,Square [] 8 7 9 9,
    Square [] 7 8 1 7,Square [] 3 8 2 7,Square [1,4,5] 0 8 3 7,Square [] 6 8 4 8,Square [] 8 8 5 8,
    Square [1,2,4,5] 0 8 6 8,Square [1,2,5] 0 8 7 9,Square [] 9 8 8 9,Square [1,4] 0 8 9 9,
    Square [] 4 9 1 7,Square [] 2 9 2 7,Square [] 8 9 3 7,Square [1,3,5,7,9] 0 9 4 8,Square [1,3,4,9] 0 9 5 8,
    Square [1,4,5,7,9] 0 9 6 8,Square [1,5] 0 9 7 9,Square [] 6 9 8 9,Square [1,4,7] 0 9 9 9]

  solveWithForceInvalid =
    [Square [4,9] 8 1 1 1,Square [] 8 1 2 1,Square [1,9] 0 1 3 1,Square [] 2 1 4 2,Square [] 7 1 5 2,
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
