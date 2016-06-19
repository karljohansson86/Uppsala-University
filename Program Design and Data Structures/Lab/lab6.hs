my_init :: [a] -> [a]
my_init [x] = []
my_init (x:xs) = x: my_init xs

from_decimals :: [Integer] -> Integer 
from_decimals [] = 0
from_decimals (x:xs) = 
	from_decimals_aux (x:xs) 0

from_decimals_aux [] acc  = acc
from_decimals_aux (x:xs) acc = 
	from_decimals_aux (xs) (acc + (x*10^(length (x:xs)-1))) 
	from_decimals_aux (xs) (acc*10 + x)


square_of_even1 :: [Integer] -> [Integer]
square_of_even1 [] = []
square_of_even1 (x:xs) 
	| x `mod` 2 == 0 = (x*x): square_of_even1 xs
	| otherwise = square_of_even1 xs

square_of_even2 :: [Integer] -> [Integer]
square_of_even2 xs = [x*x | x <-xs , x `mod` 2 == 0]









insertion_sort_aux sorted [] = sorted
insertion_sort_aux sorted (h:t) =
	insertion_sort_aux ( insert h sorted ) t
insertion_sort xs = insertion_sort_aux [] xs

insert k [] = [k]
insert k (h:t) =
	if k < h then k:h:t else h:( insert k t)




merge_sort :: [ Integer ] -> [ Integer ]
merge_sort [] = []
merge_sort [x] = [x]
merge_sort xs =
	let
		(xs1 , xs2) = split xs
	in
		merge ( merge_sort xs1) ( merge_sort xs2)

merge :: [ Integer ] -> [ Integer ] -> [ Integer ]
merge [] ys = ys
merge xs [] = xs
merge (x:xs) (y:ys)
	| y < x = y : merge (x:xs) ys
	| otherwise = x : merge xs (y:ys)

split :: [a] -> ([a], [a])
split xs =
	let
		l = length xs `div ` 2
	in
		( take l xs , drop l xs)

{-
length (insertion_sort [1..1000]) - 0,67sec
length (insertion_sort [1..2000]) - 2.73 secs
length (insertion_sort [1..3000]) - 6.17 secs
length (insertion_sort [1..4000]) - 11.25 secs
length (insertion_sort [1..5000]) - 17.56 secs
length (insertion_sort [1..6000]) - 25.86 secs
length (insertion_sort [1..7000]) - 36.00 secs
length (insertion_sort [1..8000]) - 35.58 secs
length (insertion_sort [1..9000]) - 67.12 secs
length (insertion_sort [1..10000]) - 64.42 secs


length (merge_sort [1..10000]) - 0.23 secs
length (merge_sort [1..100000]) - 2.48 secs
length (merge_sort [1..1000000]) - 28.97 secs
-}