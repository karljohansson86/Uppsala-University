import Control.Exception
import System.Exit 
import Control.Monad
import System.IO
import System.Random

{- Move
  Representation Convention: represents a move. The first element of the tuple
    is the pile to move from, the second element is the amount to move.
  Representation Invariant: The first element refers to a valid pile. The second >= 0
-}
type Move = (Int, Int) 


{- Game State
  Representation Convention: represents the number of stones in the three piles.
  Representation Convention: each element of the tuple >= 0.
-}
type GameState = (Int, Int, Int)


type Player = String

{- readMove
   Purpose: Reads a move from standard input
   Post: A move object 
   Side-effects: Reads one or more lines from standard input 
-}
readMove :: IO Move -- reads input from
readMove = do
  catch (do
    line <- getLine 
    evaluate (read line))  -- evaluate required to force conversion of line to Move
    ((\_ -> do   -- exception handler
       putStrLn "Invalid input. Correct format: (pileNumber,amount)"
       readMove) :: SomeException -> IO Move)

{- validMove gs m
   Purpose: Determine whether a move is valid in a given game state
   Pre: None
   Post: True if and only if the move refers to a pile and the number 
    of elements to remove is greater than 0 and less than number of
    elements in the pile
-}
validMove :: GameState -> Move -> Bool
validMove (a, _, _) (1, n) = a >= n && n > 0 && n <= 3
validMove (_, b, _) (2, n) = b >= n && n > 0 && n <= 3
validMove (_, _, c) (3, n) = c >= n && n > 0 && n <= 3
validMove _ _ = False


{- victory gs
   Purpose: Determine whether a game state is the winning position.
   Pre: None
   Post: True when all stone piles are empty
-}
victory :: GameState -> Bool
victory (0, 0, 0) = True
victory _ = False

{- playmove state move 
   Purpose: To update the game state after making a move
   Pre: validMove state move
   Post: A valid game state
-}
playMove :: GameState -> Move -> GameState
playMove (a, b, c) (1, n) = (a - n, b, c)
playMove (a, b, c) (2, n) = (a, b - n, c)
playMove (a, b, c) (3, n) = (a, b, c - n)


{- gameState
   Purpose: Generate a fresh game state
   Pre: None
   Post: A valid game state
   Side effect: 
-}
genGameState :: IO GameState
genGameState = randomGameState

{- randomGameState
   Purpose: Generate a fresh random game state
   Pre: None
   Post: A valid game state
   Side effect: 
-}
randomGameState :: IO (Int,Int,Int)
randomGameState = do
  randoms <- sequence [randomRIO (1,20),randomRIO (1,20),randomRIO (1,20)]
  return $ (randoms !! 0, randoms !! 1, randoms !! 2)

{- printGameState gs
   Purpose: To print a game state
   Pre: None
   Post: True
   Side-effect: Displays game state to standard output
-}
printGameState :: GameState -> IO ()
printGameState (a, b, c) = do
  putStrLn $ "Pile 1 contains " ++ (show a) ++ " stones."
  putStrLn $ "Pile 2 contains " ++ (show b) ++ " stones."
  putStrLn $ "Pile 3 contains " ++ (show c) ++ " stones."


{- printMove player move
   Purpose: To print a move for a given player
   Pre: None
   Post: 
   Side-effect: Displays move on standard output
-}
printMove :: Player -> Move -> IO ()
printMove player (pile, amount) = putStrLn $ player ++ " removes " ++ (show amount) ++ " stones from pile " ++ (show pile)


{- main
   Purpose: run the game
   Pre: None
   Post: True
   Side-effects: Quite a lot, actually
-}
main :: IO ()
main = do 
  putStrLn "Welcome to Nim."
  gameState <- genGameState 
  play gameState

{- play gs
   Purpose: Play the g
   Pre: gs is valid and not the victory state
   Post: True -- it never returns
   Side-effect: The game interaction 
-}
play gameState = do
  printGameState gameState
  newGameState <- playerMove gameState
  if victory newGameState then do
    putStrLn "Player won!"
    putStrLn ""
    playAgain
  else do
    newNewGameState <- computerMove newGameState  
    if victory newNewGameState then do
      putStrLn "Computer won!"
      putStrLn ""
      playAgain
    else
      play newNewGameState      


{- playAgain 
   Purpose: Check if player wants to play another game
   Pre: TRUE
   Post: 
   Side-effect: 
-}
playAgain = do
  putStrLn "Do you want to play another game? Y/N"
  answer <- getLine
  when (answer == "N" || answer == "n") $ exitSuccess
  when (answer == "Y" || answer == "y") $ main
  putStrLn "Invalid input, try again"
  playAgain

{- playerMove gs
   Purpose: Perform the player's move
   Pre: gs is valid and not the victory state
   Post: a new game state
   Side-effect: Displays a description of the players's move
-}
playerMove :: GameState -> IO GameState
playerMove gameState = do
  putStrLn "Your move."
  move <- readMove
  printMove "Player" move
  if validMove gameState move then 
    return $ playMove gameState move
  else do
    putStrLn "Invalid Move."
    playerMove gameState

{- computerMove gs
   Purpose: Perform the computer's move
   Pre: gs is valid and not the victory state
   Post: a new game state
   Side-effect: Displays a description of the computer's move
-} 
computerMove :: GameState -> IO GameState
computerMove gameState = do 
  let move = calculateComputerMove gameState
  printMove "Computer" move
  return $ playMove gameState move
  
  
{- calculateComputerMove gs
   Purpose: Calculate the (best) next move for the computer
   Pre: None
   Post: A valid move for gs
-}
calculateComputerMove :: GameState -> Move
calculateComputerMove (a,b,c) | a > 0     = (1, (if a<=3 then a else 3))
                              | b > 0     = (2, (if b<=3 then b else 3))
                              | otherwise = (3, (if c<=3 then c else 3))
       