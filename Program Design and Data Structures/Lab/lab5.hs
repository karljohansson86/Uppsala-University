my_replicate :: Integer->a->[a]
my_replicate 0 _ = []
my_replicate n value = value: my_replicate (n-1) value

from_to :: Integer->Integer->[Integer]
from_to low high
	| ((high-low)+1) <= 0 = []
	| otherwise = low:from_to (low+1) high



search_string :: String -> String -> Integer
search_string main [] = -1
search_string main sub = 
	let search_string_aux main sub position
		| length main < length sub = -1
		| take (length sub) main == sub = position 
		| otherwise = search_string_aux (drop 1 main) sub (position+1) 

	in search_string_aux main sub 0



test1 x =
  let
    a = x + x
    b = a + a
  in
    b + b
-- 7 st
-- b + b
-- (a+a) +  (a+a)
-- ((x+x) +(x+x)) + ((x+x)+(x+x))



test2 x =
  let
    f x = x + x
    g x = f x + f x
  in
    g x + g x
-- 7 st
-- g x + g x 
-- ((f x)+ (f x)) + ((f x)+ (f x))
-- ((x+x)+(x+x)) + ((x+x)+(x+x))


test3 x =
  let
    f y = y + y
    g z =
      let
        a = f z
      in
        a + a
  in
    g x + g x
-- 7 st 
-- g x + g x 
-- (a+a) + (a+a)
-- ((y+y)+(y+y)) + ((y+y)+(y+y))



-- kallar på en funktion (f z) som inte 
--"finns" i den rymden/not in scope
{-test4 x =
  let
    g z = f z + f z
  in
    let
      f y = y + y
    in
      g x + g x -}
-- 0 st
