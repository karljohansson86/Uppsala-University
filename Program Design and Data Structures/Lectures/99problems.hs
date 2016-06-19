myLast xs = last xs

my_Last :: [a] -> a
my_Last [] = error "empty list"
my_Last [x] = x
my_Last (_:xs) = my_Last xs

myButLast [] = error "empty list"
myButLast [x1,x2] = x1
myButLast (x:xs) = myButLast xs

elementAt [] _ = error "empty list"
elementAt (x:xs) at 
	| at == 1 = x
	|otherwise = elementAt xs (at-1)
	
myLength [] = 0
myLength (_:xs) = 1 + myLength xs

my_length_aux acc [] = acc
my_length_aux acc (_:xs) = my_length_aux (acc+1) xs
my_length_acc xs = my_length_aux 0 xs

myReverse [] = []
myReverse (xs) = last xs :myReverse (init xs)

--not efficant!?
myReverse2 [] = []
myReverse2 (x:xs) = (myReverse2 xs) ++ [x]

reverse_aux acc [] = acc
reverse_aux acc (h:t) = reverse_aux (h:acc) t
reverse_3 xs = reverse_aux [] xs

isPalindrome [] = True
isPalindrome xs 
	| take ((length xs) `div` 2) xs == take ((length xs) `div` 2) (reverse xs) = True
	|otherwise =  False

my_null [] = True
my_null _ = False

my_head (x:_) = x
my_tail (_:xs) = xs

my_last [] = error "empty list"
my_last (_:xs) = my_head $ reverse xs

my_init [] = error "empty list"
my_init xs = reverse $ my_tail $ reverse xs


[] +++ ys = ys
(x:xs) +++ ys = x:(xs +++ ys)


element e [] = False 
element e (x:xs) = x==e || element e xs

take_element [] _ _ = []
take_element xs position n 
	| myLength xs < (position+n) = error ""

{-
[x*x | x<-[1..100], x<=10]

[x|x <- "IdontLIKEFROGS", x `elem` ['A'..'Z']]

[(x,y) | x <- [1..3], y <- [2..4], x<y]
-}

-- sorting algorithms

insert k [] = [k]
insert k (x:xs) = if k <= x then k:x:xs else x: (insert k xs)

insertion_sort_aux sorted [] = sorted
insertion_sort_aux sorted (x:xs) = 
	insertion_sort_aux ( insert x sorted ) xs
insertion_sort l = insertion_sort_aux [] l

almost_insertion_sort [] = []
almost_insertion_sort (h:t) =
	insert h ( almost_insertion_sort t)




compress [] = []
compress [x] = [x]
compress (x:y:xs)
	| x == y = compress (x:xs)
	| otherwise = x : compress (y:xs) 



pack :: (Eq a) => [a] -> [[a]]
pack [] = []
pack [x] = [[x]]
pack (x:xs) 
	| x `elem` (head (pack xs)) = (x:(head (pack xs))):(tail (pack xs))
    | otherwise = [x]:(pack xs)
