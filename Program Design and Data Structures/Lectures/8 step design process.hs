
{- 
Representation Convention: Number represents position on Board
Representation Invariant: Number is greater that or equal to 1
-}
type Position = Int

{-
Representation Convention: Number represents a dice roll
Representation Invariant: Number is between 1 and 6, inclusive
-}
type Roll = Int

{-
Representation Convention: Denotes the contents of a square on the 
board position associated with Snake or LAdder indicates where the 
moves go to

Representation Invariant: position, if present, is greater than 
or equal to 1
-}
data Square = Empty | Snake Position | Ladder Position
-- data List = Nil | Cons Int List
{- Ladder 15 is a ladder that ends in square 15
-}


{-
Representation Convention: The first element gives the size of the board (>1). 
	Second element gives squares of the board

Representation Invariant: 
	First element is equal to the length of the second element
	For snakes and ladders, the position must be on the board  
	(>=1 <=size of board).
	Snakes go down --- if (snake p) is in position n then p < n.
	Ladders go up  --- 
	Snakes and ladders can't have the same position.
	No chains of snakes and ladders
-}
type Board = (Int, [Square])
{-
Invalid board: contains a chain
(9,[Empty, Ladder 6,Empty,
	Snake 1, Ladder 9, Empty,
	Snake 2, Empty, Empty])

Valid board:
(9,[Empty, Ladder 6,Empty,
	Snake 1, Ladder 9, Empty,
	Snake 3, Empty, Empty])

-}


{-
move board position roll
PURPOSE: compute next position for the given 
board based on the value of the roll
PRE: position is on the board(position >=1 and <= size(board))
POST: Result is the new position based on the current position and 
	the roll, for the given board, based on the rules of the game.
	Result is on the board
EXAMPLES: 
	move (9,[Empty, Ladder 6,Empty,
			Snake 1, Ladder 9, Empty,
			Snake 3, Empty, Empty]) 1 + 4 = 9


	move(9,[Empty, Ladder 6,Empty,
		Snake 1, Ladder 9, Empty,
		Snake 3, Empty, Empty]) 6 + 4 = 9

	move(9,[Empty, Ladder 6,Empty,
		Snake 1, Ladder 9, Empty,
		Snake 3, Empty, Empty]) 3 + 1 = 1

-}
move :: Board -> Position -> Roll -> Position
move _ _ roll | (roll < 1 || roll > 6) = error "Broken Dice"
move (size, squares) position roll = 
	if (position + roll) > size then size
	else case squares !! (position + roll-1) of 
		Empty -> position + roll
		(Snake p) -> p
		(Ladder p) -> p


-- case lookup squares of
--lookUp2 :: [Square] -> position -> Square
--lookUp2  squares position = squares !!  (position-1)



{-
foo :: [Int] -> [Int]
foo [] = 
foo (x:xs) = 

bar :: Bool -> OtherType
bar True = ...
bar False = ...

bar b = if b then ...
		else ...

bar :: Int -> OtherType
bar 

render :: Square ->  IO ()
render Empty = 
render Snake p) =
render (Ladder p) = 

-}