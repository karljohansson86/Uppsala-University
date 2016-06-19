data Ordering1 = LT1 | EQ1 | GT1 

compare1 :: Int -> Int -> Ordering
compare1 a b 
	| a < b = LT
	| a == b = EQ
	| otherwise = GT

