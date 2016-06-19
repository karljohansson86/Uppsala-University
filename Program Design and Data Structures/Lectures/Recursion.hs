sum33 :: Integer->Integer->Integer
sum33 a b =
	if b < a then 0 else a + sum33 (a+1) b

power b 0 = 1
power b n = b * power b (n-1)

factorial 0 = 1
factorial 1 = 1
factorial n = n * factorial (n-1)

steps 0 = 0
steps 1 = 1
steps n = steps(n-1) + steps(n-2)

hanoi :: Integer->String->String->String->String
hanoi 0 _ _ _ = ""
hanoi n from via to =
	hanoi (n-1) from to via ++
	from ++ "->" ++ to ++ " " ++
	hanoi (n-1) via from to

fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2)

fast_fib 0 = (0, 1)
fast_fib n = let (a, b) = fast_fib (n-1) in (b, a+b)

type WordTally = [(String, Int)]


element e [] counter = counter
element e (x:xs) counter
	|x==e = element e xs (counter+1)
	|otherwise = element e xs (counter)


superaustin =  ["pride","and","prejudice","by","jane","austen","chapter","1","it","is","a","truth","universally",
	"acknowledged","that","a","single","man","in","possession","of","a","good","fortune","must","be","in","want",
	"of","a","wife","however","little","known","the","feelings","or","views","of","such","a","man","may","be","on",
	"his","first","entering","a","neighbourhood","this","truth","is","so","well","fixed","in","the","minds","of",
	"the","surrounding","families","that","he","is","considered","the","rightful","property","of","some","one",
	"or","other","of","their","daughters","my","dear","mr","bennet","said","his","lady","to","him","one","day",
	"have","you","heard","that","netherfield","park","is","let","at","last","mr","bennet","replied","that","he",
	"had","not","but","it","is","returned","she","for","mrs","long","has","just","been","here","and","she","told",
	"me","all","about","it","mr","bennet","made","no","answer","do","you","not","want","to","know","who","has",
	"taken","it","cried"]
{-
wordCount :: Document -> WordTally
wordCount (x:y:xs) =
	wordCountAux (x:xs) x 0 : wordCountAux (y:xs) y 0

wordCountAux (x:xs) element counter
	| element == x = wordCountAux xs element (counter+1)
	| otherwise = wordCountAux xs element counter

wordCountAux [] _ _ = []
wordCountAux [x] _ _ = (element, counter) -}