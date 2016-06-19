
module Main where

import Debug.Trace
import Test.HUnit

foo x = x * x

test1 = TestCase (assertEqual "for (foo 3)," 9 (foo 3))
test2 = TestCase (assertBool "foo 10 > 0" (foo 10 > 0))

test3 = TestCase (assertEqual "positives [0]" [] (positives [0]))  
test4 = TestCase (assertEqual "seconds [1,2,3] " [1,3] (seconds [1,2,3]))

runtests = runTestTT $ TestList [TestLabel "test1" test1, TestLabel "test2" test2, TestLabel "test3" test3, TestLabel "test4" test4]


{- PURPOSE: Keep only positives numbers

   PRE: (none)
   POST: the positive numbers of the input list, in same order -}
positives [] = []
positives (a : as) = trace ("list: " ++ show (a : as)) $ if a > 0 then a : positives as -- Should be a > 0 
                     else positives as

--main = putStrLn $ "positives [-1,1,2,3] " ++ show (positives [-1,1,2,3])


{- PURPOSE: Extract the elements of in even indexed positions (indexing starts at 0)
   PRE: (none)
   POST: the elements in the even positions of the input list in same order -}
seconds [] = []
seconds [x] = [x] -- should be a pattermatch with only 1 element in the list / uneven elements in list
seconds (a : _ : as) = trace ("list: " ++ show (a:as)) $ a : seconds as
main = putStrLn $ "seconds [1,2,3] " ++ show (seconds [1,2,3])



fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = trace ("n: " ++ show n) $ fib (n - 1) + fib (n - 2)
 
--main = putStrLn $ "fib 4: " ++ show (fib 4)

