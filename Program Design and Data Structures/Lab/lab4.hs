string_of_integer :: Integer -> String
string_of_integer 0 = "0"
string_of_integer 1 = "1"
string_of_integer 2 = "2"
string_of_integer 3 = "3"
string_of_integer 4 = "4"
string_of_integer 5 = "5"
string_of_integer 6 = "6"
string_of_integer 7 = "7"
string_of_integer 8 = "8"
string_of_integer 9 = "9"

string_of_integer n = 
	if n<0 then "-" ++ string_of_integer (n*(-1)) 
	else string_of_integer (n `div`10) ++ string_of_integer (n `mod` 10)



search_string :: String -> String -> Integer
search_string main [] = -1
search_string main sub = 
	search_string_aux main sub 0

--search_string_aux :: String -> String -> Integer -> Integer
search_string_aux main sub position
	| length main < length sub = -1
	| take (length sub) main == sub = position 
	| otherwise = search_string_aux (drop 1 main) sub (position+1) 






severian vodalus =
	if vodalus == "" 
	then 0
	else severian (take (length vodalus - 1) vodalus) * 2 +
		if drop (length vodalus - 1) vodalus == "0"
			then 0
			else 1



