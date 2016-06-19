swap :: (Integer, Integer) -> (Integer, Integer)
swap (a,b) = (b,a)

add []  ys = ys
add (x:xs) ys = x : add xs ys

{-

Svaret på nr 2.1


T++(n)	= t0 	if n=0
		= 

T++(0) = 1
T++(n) = n +


 (xs) ys = 1 : (xs-1) +
 
T: + T++(n-1)

T(0) = 1
T(1) = T: + T++(0)
T(2) = T: + T++ (1) = 2T: + T++(0) = 2T + 1 


2.2:
0 1 2 3 4 5 6 7  8  9
0 1 1 2 3 5 8 13 21 34

Tfib (0) = 0
Tfib (1) = 1
Tfib (n) = Tfib(n-1) + Tfib(n-2) + Tadd

Tfib (0) = 1
Tfib (1) = 1
Tfib (2) = Tfib(2-1) + Tfib(2-2) + Tadd
		 = 1 + 1 + Tadd = 2 + Tadd
Tfib (3) = Tfib(3-1) + Tfib(3-2) + Tadd
		 = Tfib(2) + Tfib (1) + Tadd 
		 = 2 + Tadd + 1 + Tadd
		 = 3 + 2Tadd

Tfib (4) = Tfib(4-1) +Tfib(4-2) + Tadd
		 = Tfib (3) + Tfib (2) + Tadd
		 = 3 + 2Tadd + 2 + Tadd + Tadd
		 = 5 + 4Tadd

Tfib (5) = Tfib (5-1) + Tfib (5-2) + Tadd
		 = Tfib (4) + Tfib (3) + Tadd
		 = 5 + 4Tadd + 3 + 2Tadd + Tadd
		 = 8 + 7Tadd

F(n)	= F(n-1) + F(n-2)

F(0)	= 0
F(1)	= 1
F(2)	= F(1) + F(0) = 1 + 0 = 1
F(3)	= F(2) + F(1) = 1 + 1 = 2
F(4)	= F(3) + F(2) = 2 + 1 = 3
F(5)	= F(4) + F(3) = 3 + 2 = 5
F(6)	= 

-}



my_uncurry f (a,b) = f a b 

my_curry f a b = f (a,b)

negate1 [] = []
negate1 (x:xs) = (-1)*x: negate1 xs

map_abs [] = []
map_abs (x:xs) = abs x : map_abs xs

filter_even [] = []
filter_even (x:xs) 
	| even x = x: filter_even xs
	| otherwise = filter_even xs

filter_odd [] = []
filter_odd (x:xs) 
	| odd x = x: filter_odd xs
	| otherwise = filter_odd xs


bubble_select [] = []
bubble_select [a] = [a]
bubble_select (a:b:t) =
	if a > b then b:( bubble_select (a:t))
	else a:( bubble_select (b:t))

bubblesort [] = []
bubblesort (h:t) = bubble_select (h:( bubblesort t))

bubblesort [1,2,3,4] =
	bubble_select (1:( bubblesort [2,3,4]))
		bubble_select (2:( bubblesort [3,4]))
			bubble_select (3:( bubblesort [4]))
				bubble_select (4:( bubblesort []))
					bubble_select (4:[])

					bubble_select (4:[]) = 
						[4]	
				