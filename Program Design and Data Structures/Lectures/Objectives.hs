import Data.Char

{-
main = do 
	print 20
	print 5
	print "hello"
-}
l = [ print 4, print 65]

doall [] = return ()
doall (p:ps) = 
	do
		p
		doall ps


main = do
	putStrLn "What is your first name?"
	firstName <- getLine
	putStrLn "What is your last name?"
	lastName <- getLine
	let bigFirstName = map toUpper firstName
	putStrLn $ "Hello " ++  bigFirstName ++ " " ++ lastName ++ "!"
	return firstName
