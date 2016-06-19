
{-
    Representation Convention: Represents a fraction number
    Representation Invariant: Denominator is the snd Integer. Denominator is not zero. Denomitor > 0
-}
type Fraction = (Integer,Integer)
{-	Valid Fraction: (0,2) (1,1) (-1,6)
	Invalid Fraction: (5,0)
-}


{-
	addFraction (num1,den1) (num2 den2)
	TYPE: Fraction -> Fraction -> Fraction
	PURPOSE: Add two fractions of the same base together to create a larger number.
	PRE: Needs to be two valid fractions. snd fraction1 == snd fraction2
	POST: ((num1+num2), dem1)
	EXAMPLES:
	addFraction (3,5) (5,3) = (8,3)
	addFraction (-1,6) (5,6) = (4,6)
	addFraction (-1,6) (-5,6) = (-6,6)
	addFraction (1,1) (1,1) = (2,1)
-}
addFraction :: Fraction -> Fraction -> Fraction
addFraction (num1,den1) (num2, den2) = undefined


{-
	subFraction (num1,den1) (num2 den2)
	TYPE: Fraction -> Fraction -> Fraction
	PURPOSE: Subtract one fraction from another number of the same base to create a smaller number.
	PRE: Needs to be two valid fractions. snd fraction1 == snd fraction2
	POST: ((num1+num2), dem1)
	EXAMPLES:
	subFraction (3,5) (5,3) = (-2,3)
	subFraction (-1,6) (5,6) = (-4,6)
	subFraction (-1,6) (-5,6) = (4,6)
	subFraction (1,1) (1,1) = (0,1) ?
-}
subFraction:: Fraction -> Fraction -> Fraction
subFraction (num1,den1) (num2, den2) = undefined


{-
	multFraction (num1,den1) (num2 den2)
	TYPE: Fraction -> Fraction -> Fraction
	PURPOSE: Multiply the fraction number to create a new fraction number
	PRE: Both arguments need to be a valid fraction
	POST: ((num1*num2), (den1*den2))
	EXAMPLES:
	multFraction (3,5) (5,3) = (15,15)
	multFraction (-1,6) (5,6) = (-5,36)
	multFraction (-1,6) (-5,6) = (5,36)
	multFraction (1,1) (1,1) = (1,1)
-}
multFraction:: Fraction -> Fraction -> Fraction
multFraction (num1,den1) (num2, den2) = undefined


{-
	divFraction (num1,den1) (num2 den2)
	TYPE: Fraction -> Fraction -> Fraction
	PURPOSE: Devide one fractional number with another to create a new number
	PRE: Both arguments need to be a valid fraction. num2 can't be zero
	POST: ((num1*den2), (den1*num2))
	EXAMPLES:
	divFraction (3,5) (5,3) = (9,25)
	divFraction (-1,6) (5,6) = (-6,30)
	divFraction (-1,6) (-5,6) = (6,30)
	divFraction (1,1) (1,1) = (1,1)

-}
divFraction:: Fraction -> Fraction -> Fraction
divFraction (num1,den1) (num2, den2) = undefined
	| num2 < 0 = ...
	| otherwise = ...

{-
	convertBase (num1,den1) multiply
	TYPE: Fraction -> Integer -> Fraction
	PURPOSE: Convert the fraction to the same base for addition and subraction.
	PRE: First argument needs to be a valid fraction and multiply > 0
	POST:((num1*multiply),(den1*multiply))
	convertBase (3,5) 4 = (12,20)
	convertBase (-3,5) 4 = (-12,20)

-}
convertBase :: Fraction -> Integer -> Fraction
convertBase (num1,den1) multiply = undefined

