


{- sizeList StringList
   PRE:  TRUE
   POST: List with Integers that represent the lenght of each word in the stringList.
   EXAMPLES: 
   sizeList ["hej","hej","hello"] = [3,3,5]
-}
sizeList :: [String] -> [Int]
sizeList list = map length list




{- multiples Multiplier IntList
   PRE:  TRUE
   POST: List with Integers that only contains the Integers divisible by the Multiplier.
   EXAMPLES: 
   multiples 5  [1,2,3,4,5,6,7,8,9,10] = [5,10]
-}
multiples :: Int -> [Int] -> [Int]
multiples multiplier list = filter (\x -> mod x multiplier == 0) list




{- myConcat StringList
   PRE:  TRUE
   POST: Returns a string with all the strings in StringList concatinated to one string.
   EXAMPLES: 
   myConcat ["hej","pa","dig"] = "hejpadig"
-}
myConcat :: [String] -> String
myConcat list = foldr (++) [] list




{- bowman function List
   PRE:  List must no be empty. Elements in list needs to be of same "type" as the function. In example (+) would not work with a list of strings.
   POST: Returns a value from applying the function to each element in the list.
   EXAMPLES: 
   bowman (+) [1,2,3,4] = 10
-}
bowman :: (a -> a -> a) -> [a] -> a
bowman poole (floyd:hal) =
    if null hal
        then floyd
        else poole floyd (bowman poole hal)

-- bowman (*) [2.5, 2.0, 0.2]