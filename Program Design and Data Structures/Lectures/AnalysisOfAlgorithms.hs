sumList [] = 0
sumList (x:xs) = x + sumList(xs)

{- 
n=size of list
t= time
T(n) = t(0) + n* t(add)
t(0)	n = 0
t(patternmatch) + t(add) + t(n-1)	n > 0

t(n) = t(0) + n* t(add)

-}

myHead [] = error "error"
myHead (x:_) = x
{-
T(n) =
t(error) 	n = 0
t(pattern) 	n > 0
-}

myInit [x] = []
myInit (x:xs) = x: myInit xs
{-
-}


myInit2 [x] = []
myInit2 [x,y] = [x]
myInit2 (x:xs) = x: myInit2 xs ++ myInit2 (tail xs)


{-
T(n) = t+ 2T(n-1) + T(n-2)
-}

hanoi :: Int -> String -> String -> String -> String
hanoi 0 from via to = ""
hanoi n from via to =
	hanoi (n-1) from to via ++ from ++ "->" ++ to ++ " " ++
		hanoi (n-1) via from to


bar [] = 0
bar (a : as) = bar as + bar as

power b 0 = 1
power b n 
	| even n = power b (n `div` 2) * power b (n `div` 2)
	| otherwise = b * power b (n `div` 2) * power b (n `div` 2)


ge 0 = 0
ge n = ge(n-1) + (2*n -1)



partition _ [] = ([], [])
partition p (x:xs) =
	let
		(lows, highs) = partition p xs
	in
		if x < p
		then (x:lows, highs)
		else (lows, x:highs)

quicksort [] = []
quicksort (x:xs) =
	let
		(lows, highs) = partition x xs
	in
		quicksort lows ++ x : quicksort highs

mee 0 = 1
mee n = mee (n-1) + n

add1 x = x+1

ntimes 0 f = id
ntimes n f = f . ntimes (n-1) f

twice f = f . f

