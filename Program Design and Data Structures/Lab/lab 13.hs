{-
1 head  = 				[a] - > a   , poly

2 tail = 				[a] -> [a]   , poly

3 \x -> x = 			t -> t , poly

4 (,) = 				a -> b -> (a, b)  , poly

5 (:) = 				a -> [a] -> [a] ,no? needs to be of same type
						can't be different in the list. Otherwise yes 

6 [[]] =  				[[t]]  , poly

7 tail [[]] = 			:: [[t]] ,  

8 id : [] = 			:: [a -> a] , 

9 id id	= 				:: a -> a  , poly

10 head [id] "foo" = 	:: [Char]  (head på [id] och sen id på "foo") 
						
-}


flip_arguments :: (t1 -> t2 -> t) -> t2 -> t1 -> t
flip_arguments f arg1 arg2 = f arg2 arg1 


difference_quotient :: Fractional a => (a -> a) -> a -> a -> a
difference_quotient f x h = (f (x+h) - f x)/ h


-- (\x y z -> x z y)

