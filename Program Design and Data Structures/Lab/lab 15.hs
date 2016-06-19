
{-
    Representation Convention: A suit of a card
    Representation Invariant: Spades, Hearts, Clubs or Diamonds are the only valid suits
-}
data Suits =  Spades 
			| Hearts
			| Clubs 
			| Diamonds

{-
    Representation Convention: An Integer, 2-10 represents 2-10 
    	11 = Jack ; 12 = Queen; 13 = King; 14 = Ace
    Representation Invariant: An Integer between value 2-14.

    Valid Value: 2 ,3 ,4 
    Invalid Value: -1, 0, 15
-}
type Value = Integer


{-
    Representation Convention: A playing card 
    Representation Invariant: A suit and a value

    Valid Card: Card Spades 12 , Card Hearts 3
-}
data Card = Card Suits Value 


{- greaterCard card card
   PRE:  Valid type Card
   POST: Bool
   PURPOSE: Checks if on card is greater than another. 
   EXAMPLES: 
   greaterCards (Card Spades 12) (Card Hearts 10) = True
-}
greaterCard :: Card -> Card -> Bool
greaterCard (Card a b) (Card c d)
	| b > d = True
	| b < d = False
	| otherwise = compareSuits a c

{- compareSuits suits suits
   PRE:  True
   POST: Bool
   PURPOSE: Returns true or false if the suit has greater suit value than the other.
   EXAMPLES: 
   compareSuits Spades Hearts  = True
   compareSuits Spades Spades  = False
-}
compareSuits :: Suits -> Suits -> Bool
compareSuits Spades _ = True
compareSuits Hearts Spades = False
compareSuits Hearts Hearts = False
compareSuits Hearts _ = True
compareSuits Clubs Diamonds = False
compareSuits Clubs _ = True
compareSuits Diamonds _ = False


{- (+?) (Just a) (Just b)
   PRE:  True
   POST: Returns a Just (a+b) if a and b are of type Int. Nothing if any of the arguments are of value Nothing
   EXAMPLES: 
   
-}
(+?) :: Maybe Int -> Maybe Int -> Maybe Int
(+?) Nothing (Just _) = Nothing
(+?) (Just _) Nothing = Nothing
(+?) Nothing Nothing = Nothing
(+?) (Just a) (Just b) = Just (a+b)


{- (+?) (Just a) (Just b)
   PRE:  True
   POST: Returns a Just (a+b) if a and b are of type Int. Nothing if
   PURPOSE: Returns true or false if the suit has greater suit value than the other.
   EXAMPLES: 
   compareSuits Spades Hearts  = True
   compareSuits Spades Spades  = False
-}
(-?) :: Maybe Int -> Maybe Int -> Maybe Int
(-?) Nothing (Just _) = Nothing
(-?) (Just _) Nothing = Nothing
(-?) Nothing Nothing = Nothing
(-?) (Just a) (Just b) = Just (a + b)


{- (+?) (Just a) (Just b)
   PRE:  True
   POST: Returns a Just (a+b) if a and b are of type Int. Nothing if
   PURPOSE: Returns true or false if the suit has greater suit value than the other.
   EXAMPLES: 
   compareSuits Spades Hearts  = True
   compareSuits Spades Spades  = False
-}
(*?) :: Maybe Int -> Maybe Int -> Maybe Int
(*?) (Just a) (Just b) = Just (a*b)


{- (+?) (Just a) (Just b)
   PRE:  b cant be 0
   POST: Returns a Just (a+b) if a and b are of type Int. Nothing if
   PURPOSE: Returns true or false if the suit has greater suit value than the other.
   EXAMPLES: 
   compareSuits Spades Hearts  = True
   compareSuits Spades Spades  = False
-}
(/?) :: Maybe Int -> Maybe Int -> Maybe Double
(/?) (Just a) (Just 0) = Nothing
(/?) (Just a) (Just b) = Just (a/b)


