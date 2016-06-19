my_last [x1,x2,x3,x4,x5] = (x1,x2,x3,x4,x5)
my_last (_:xs) = my_last xs 

my_five xs position antal  = take antal (drop position xs)

tot [x] = x
tot (x:xs) = x + tot xs

indivisible n low high = 
	low > high ||
	n `mod` low /= 0 && indivisible n (low+1) high

prime n = 
	n > 1 && indivisible n 2 (n-1)

fib 0 = 0
fib 1 = 1
fib n = fib(n-1) + fib (n-2)

fast_fib 0 = (0,1)
fast_fib n = let (a,b) = fast_fib (n-1) in (b, a+b)

fib_ny n = fst (fast_fib n)

delete value [] = []

delete value xs
	|head xs == value = delete value (drop 1 xs)
	|otherwise = head xs:delete value (drop 1 xs)
	
	--| (x:xs) == value = delete value xs
	--| otherwise x:delete value xs


