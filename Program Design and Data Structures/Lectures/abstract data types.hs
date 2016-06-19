

module Table (empty, inc, search, delete, iterate) where
-- Assume there is a type Table k v.
empty :: Table k 
inc :: Eq k => Table k -> k -> Table k 
search :: Table k -> k -> Maybe Int
delete :: Table k -> k -> Table k 
iterate6 :: Table k -> ((k,Int) -> b -> b) -> b -> b



-- stars = add (add empty "Brad" 11) "LeBron" 26

--REPRESENTATION CONVENTION
--REPRESENTATION INVARIANT: no key appears more than once
data Table k = Table [(k,Int)] deriving (Show)

empty = Table []

inc (Table l) k = Table (inc' l k)
	where
		inc' [] k = [(k,1)]
		inc' ((k',v'):l) k  
			| k == k' = (k',v'+1):l
			| otherwise = (k',v') : inc' l k

search = undefined
delete = undefined
iterate6 = undefined

myadd ( Table t) k v = Table ((k, v): t)


----- 
type Stack a = [a]
--RC : first element of list is top of the stack, 2nd element is under top, etc
--RI : None


isEmpty :: Stack a -> Bool
push :: Stack a -> a -> Stack a
pop :: Stack a -> Stack a
top :: Stack a -> a


isEmpty [] = True
isEmpty _ = False
push s e = e:s
pop (_ : s) = s
top (t:_) = t







