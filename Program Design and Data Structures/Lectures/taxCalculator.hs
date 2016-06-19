import Test.HUnit

{-a floating point representing the amount of income in kronor. >= 0-}
type Income = Float 
{-
1000.00 represent 1000kr
30000.00 represent 30000kr-}

type Tax = Float

{- Function calculates the tax due for the amount of input
 using the table(on slide 3)
 calculate 18000 = 0.0
 calculate 18001 = 0.31
 calculate 433900 = 128929.0
 calculate 433901 = 128929.51
 calculate 615700 = 221646.49
 calculate 615701 = 221647.05 -}

calculate :: Income -> Tax
calculate income = 	
	if income <= 18000 then 0
	else if income <= 433900 then (income- 18000) * 0.31 + calculate 18000
	else if income <= 615700 then (income- 433900) * 0.51 + calculate 433900
	else (income- 615700) * 0.56 + calculate 615700


test1 = TestCase $ assertEqual "Test 18000" 0.0 (calculate 18000)
test2 = TestCase $ assertEqual "Test 18001" 0.31 (calculate 18001)
test3 = TestCase $ assertEqual "Test 433900" 128929.0 (calculate 433900)
test4 = TestCase $ assertEqual "Test 433901" 128929.51 (calculate 433901)
test5 = TestCase $ assertEqual "Test 615700" 221646.49 (calculate 615700)
test6 = TestCase $ assertEqual "Test 615701" 221647.05 (calculate 615701)

runTests = runTestTT $ TestList [ test1,test2,test3,test4,test5,test6]

