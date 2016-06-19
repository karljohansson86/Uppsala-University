-- DO NOT MODIFY THE FOLLOWING LINE
module CompLing(wordCount, adjacentPairs, pairsCount, neighbours, mostCommonNeighbour) where
-- DO NOT MODIFY THE PREVIOUS LINE

import Test.HUnit -- provides testing framework
import PandP      -- provide sample text to play with (variable austin)

type WordTally = [(String, Int)]
type Document = [String]
type Pairs = [(String, String)]
type PairsTally = [((String, String), Int)]


wordCount :: Document -> WordTally
wordCount [] = []
wordCount (x:xs) =
	 (wordCountAux (x:xs) x 0)

wordCountAux (x:xs) element counter =
	(x, (countword x (x:xs) 0)) : wordCount (delete [x] xs)

countword word [] counter = counter
countword word (x:xs) counter
	|x==word = countword word xs (counter+1)
	|otherwise = countword word xs (counter)

delete word [] = []
delete word (x:xs)
	| [x] == word = delete word xs
	| otherwise = x:delete word xs


{-
Function to find all adjacent pairs.
The function takes a list of type Document which is a List containing Strings and put the adjacent pairs in tuples
The function returns a list of the type Pairs which is a List containing tuples
-}
adjacentPairs :: Document -> Pairs
--Control if the list is empty or if it only contains one element
adjacentPairs [] = []
adjacentPairs [x] = []
adjacentPairs (x:y:xs) = (x,y) : adjacentPairs (y:xs)
{-}
fastAdjacentPairs :: Document -> Pairs
fastAdjacentPairs [] = []
fastAdjacentPairs [a,b] = [a,b]
fastAdjacentPairs (x:xs) = 
	let (a, b) = fastAdjacentPairs xs 
	in (b, a)

fast_fib 0 = (0, 1)
fast_fib n = let (a, b) = fast_fib (n-1) in (b, a+b)
-}

{-
pairsCount ....
PURPOSE: 
PRE: position is on the board(position >=1 and <= size(board))
POST: Result is the new position based on the current position and 
	the roll, for the given board, based on the rules of the game.
	Result is on the board
EXAMPLES:
-}

pairsCount :: Pairs -> PairsTally
pairsCount [] = []
pairsCount (word:document) = 
	
	let 
		findPairs :: (String, String) -> Pairs -> Int -> Int
		findPairs find [] count = count
		findPairs find (pair:document) count 
			| equal find pair 	= findPairs find document (count+1)
			| otherwise 		= findPairs find document count

		equal :: (String, String) -> (String, String) -> Bool
		equal (a,b) (c,d) =
			(a == c || a == d) && (b == c || b == d)

		deletePairs :: (String, String) -> Pairs -> Pairs
		deletePairs find [] = []
		deletePairs find (pair:document)
			| equal find pair   = deletePairs find document
			| otherwise         = pair: deletePairs find document
	in
		(word, findPairs word (word:document) 0): 
			pairsCount (deletePairs word document)


{-
neighbours (pair:document) find ... 

PURPOSE: 	Given a list with Pairs and Integers(a PairsTally) and a String, the function return the Strings and 
			the number of occurences that it was in the same pair as the given String
			
			If the list doesn't contain the word the functions returns a empty list

PRE: 		Valid type PairsTally 
			
POST: 		The result is a list with Strings and Integers(a Wordtally). 
			Representing the the Strings that was in the same pair as the given String and the number of occurences 
		

EXAMPLES: 	[("pride","and"),("and","prejudice"),("prejudice","by")] "and" =
				[("pride","and"),("and","prejudice")]


-}
neighbours :: PairsTally -> String -> WordTally
neighbours [] find = []
neighbours (pair:document) find 
	let 	 

		equalNeighbours :: String -> ((String, String), Int) -> Bool
		equalNeighbours (a) ((b,c),_) =
			(a == b || a == c) 

		deleteNeighbours :: String -> ((String, String), Int) -> (String, Int) 
		deleteNeighbours (a) ((b,c),(x)) =
			if a == b then ((c),x) else ((b),x)

	in
		| equalNeighbours find pair = 
			deleteNeighbours find pair: neighbours document find 
		| otherwise = neighbours document find 









--data Maybe a = Just a | Nothing
mostCommonNeighbour :: PairsTally -> String -> Maybe String
mostCommonNeighbour (x:xs) word =
	mostCommon(listWithNeighbours (x:xs) word) 0 "" 


listWithNeighbours [] word = []
listWithNeighbours (x:xs) word
	| wordInPair x word = (deleteWord x word): listWithNeighbours xs word
	| otherwise 		= listWithNeighbours xs word
	
wordInPair ((a,b),_) word =
	(word == a || word == b)

deleteWord ((a,b),value) word 
	| word == a 	= (b,value)
	| otherwise		= (a,value)

mostCommon [] 0 acc				= Nothing
mostCommon [] highestValue acc 	= Just acc 
mostCommon ((a,value):xs) highestValue acc
	| value > highestValue		= mostCommon xs value a
	| otherwise					= mostCommon xs highestValue acc

-- test cases

-- wordCount
test1 = TestCase $ assertEqual "wordCount []" [] (wordCount [])
test2 = TestCase $ assertBool "wordCount [\"a\",\"b\",\"a\"]" (elem ("a",2) (wordCount ["a","b","a"]))

-- adjacentPairs
test3 = TestCase $ assertEqual "adjacentPairs [\"foo\"]" [] (adjacentPairs ["foo"]) 
-- small example with 4 words

-- pairsCount
test4 = TestCase $ assertBool "pairsCount simple" (elem (("a","b"), 2) (pairsCount [("a","b"),("c","d"),("a","b")]))
test5 = TestCase $ assertBool "pairsCount tricky" (let x = pairsCount (adjacentPairs ["a","b","a","c"]) in elem (("a","b"), 2) x || elem (("b","a"), 2) x)

-- neighbours
test6 = TestCase $ assertEqual "neighbours left" [("b",2)] (neighbours [(("a","b"),2),(("c","d"),1)] "a") 
test7 = TestCase $ assertEqual "neighbours left" [("a",2)] (neighbours [(("a","b"),2),(("c","d"),1)] "b") 

-- mostCommonNeighbour
test8 = TestCase $ assertEqual "mostCommonNeighbour text \"the\"" (Just "fun") (mostCommonNeighbour input "the") 
  where input = [(("the", "fun"),4),(("the","foot"),3),(("dog","power"),2)]

test9 = TestCase $ assertEqual "mostCommonNeighbour text \"spam\"" Nothing (mostCommonNeighbour input "spam")
  where input = [(("the", "fun"),4),(("the","foot"),3),(("dog","power"),2)]

-- testing the PandP.austin text
test10 = TestCase $ assertEqual "mostCommonNeighbour of \"bennet\"" 
            (Just "mr") (mostCommonNeighbour (pairsCount $ adjacentPairs $ austin) "bennet") 

-- for running all the tests
runtests = runTestTT $ TestList [test1, test2, test3, test4, test5, test6, test7,test8,test9,test10]




