

import Control.Monad
import Data.Char
import System.IO
import System.Random

main = do
	withFile "myfile.txt" ReadMode (\handle -> do
		contents <- hGetContents handle
		putStr $ shortLinesOnly contents)

{-
	handle <- openFile "myfile.txt" ReadMode
	contents <- hGetContents handle
	putStr $ shortLinesOnly contents
-}



	--WriteMode , ReadWriteMode , AppendMode

shortLinesOnly = unlines . filter (\line -> length line < 10) . lines 



--RandomNo
threeCoins :: StdGen ->(Bool,Bool,Bool)
threeCoins gen =
	let 
		(firstCoin, newGen) = random gen
		(secondCoin, newGen2) = random newGen
		(thirdCoin, newGen3) = random newGen2
	in (firstCoin, secondCoin, thirdCoin)





twoCoins :: IO (Bool,Bool)
twoCoins = do
	gen <- newStdGen
	let coin1 = fst $ random gen 
	gen2 <- newStdGen
	let coin2 =  fst $ random gen2 
	return $ (coin1, coin2)








