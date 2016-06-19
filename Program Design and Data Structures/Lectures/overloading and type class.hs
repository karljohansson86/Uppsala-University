
data Bit = O | I 
	deriving (Show)

instance Num Bit where
	O + x = x
	x + O = x
	I + I = O

	O * _ = O


{-
import Prelude hiding(elem)
data Colour = Red | Blue | Yellow
	deriving (Eq, Read, Show)

data Tree a = Leaf a | Branch (Tree a) (Tree a)
	deriving Eq

elem :: Eq a => a -> [a] -> Bool
elem x [] = False
elem x (y:ys) = x == y || elem x ys







instance Eq Colour where 
	(==) = eqColour
-}




--elemColour :: Colour -> [Colour] -> Bool
--elemColour x [] = False
--elemColour x (y:ys) = eqColour x y || elemColour x ys
{-
elemGen :: (t1 -> t -> Bool) -> t1 -> [t] -> Bool
elemGen eq x [] = False
elemGen eq x (y:ys) = eq x y || elemGen eq x ys


elemColour = elemGen eqColour
-}



{-
---------------------
class EQ a where 
	(===) :: a -> a -> Bool
	(/==) :: a -> a -> Bool
	x /== y = not (x === y)
	x === y = not (x /== y)  ---infinite loop

instance EQ Colour where
	(===) = eqColour


instance EQ a => EQ [a] where
	[] === [] = True
	(a:as) === (b:bs) = a === b && as === bs
	_ === _ = False

instance (EQ a, EQ b) => EQ (a,b) where
	(a,b) === (c,d) = a === c && b === d

-}