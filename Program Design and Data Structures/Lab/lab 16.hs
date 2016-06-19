
{-
    Representation Convention: Birthyear where BC is represented by negative numbers
    Representation Invariant: -
-}
type Year = Int

{-
    Representation Convention: Name of a person
    Representation Invariant: -
-}
type Name = String
 

{-
    Representation Convention: A binary Tree with each node having name, birth year and gender and represent a familytree of that person. FamilyTree represent the persons parents
    Representation Invariant: -
-}
data FamilyTree = Void 
				| Woman Name Year FamilyTree FamilyTree 
				| Man Name Year FamilyTree FamilyTree 



{- oldestWoman (gender name birth left right)
   PRE:  TRUE
   POST: A tuple containing the name and birth of the oldest woman else Nothing.
   EXAMPLES: 
   oldestWoman (Man "Arn" 1150 Void Void) = Nothing
   oldestWoman (Woman "Anna" 1992 Void Void) = Just ("Anna", 1992)
   oldestWoman (Woman "Anna" 1992 (Woman "Maria" 1989 Void Void) (Man "Arn" 1150 Void Void)) = Just ("Maria", 1989)
-}
oldestWoman :: FamilyTree -> Maybe (Name,Year)
oldestWoman Void = Nothing
oldestWoman (Man _ _ Void Void) = Nothing
oldestWoman (Woman name birth Void Void) = Just (name,birth)
oldestWoman (Man name birth left right) =  (oldestWomanAux (oldestWoman left) (oldestWoman right))
oldestWoman (Woman name birth left right) = oldestWomanAux (Just (name,birth)) (oldestWomanAux (oldestWoman left) (oldestWoman right))



{- oldestWomanAux (name1,birth1)@x (name2,birth2)@y
   PRE:  TRUE
   POST: A tuple containing the name and birth else Nothing. (If same age of two women returns first tuple)
   EXAMPLES: 
   oldestWomanAux Nothing Nothing = Nothing
   oldestWomanAux (name1,birth1) Nothing = (name1,birth1)
   oldestWomanAux ("Anna", 1992) ("Emma", 1992)= Just ("Anna", 1992)
 -}
oldestWomanAux :: Maybe (Name,Year) -> Maybe (Name,Year) -> Maybe (Name,Year)
oldestWomanAux Nothing Nothing = Nothing
oldestWomanAux Nothing x =  x 
oldestWomanAux x Nothing = x
oldestWomanAux (Just (name1,birth1)) (Just (name2,birth2)) 
	| birth1 <= birth2 = Just (name1,birth1)
	| birth1 > birth2 = Just (name2,birth2)




data Company a = Leaf a
			   | Node (Company a) (Company a)