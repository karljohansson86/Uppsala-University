{-
    Representation Convention: A binary Tree with each node having a value/Int and two children(BStree). If a BStree is empty it is represented by Void
    Representation Invariant: A binary search tree. Left tree represent values smaller than the node value and the right tree represents higher values. 
    	Can not be more than one instance of a value.
-}
data BSTree = Void | Node Int BSTree BSTree deriving (Show)


{- delete BStree arg1  
   PRE:  TRUE
   POST: A new BStree but with the node that has the same value as arg1 deleted. 
    EXAMPLES: 
  		delete (Node 3 (Node 1 (Node 0 Void Void) (Node 2 Void Void)) (Node 7 (Node 5 (Node 4 Void Void) (Node 6 Void Void)) (Node 8 Void Void))) 7 =
			Node 3 (Node 1 (Node 0 Void Void) (Node 2 Void Void)) (Node 6 (Node 5 (Node 4 Void Void) Void) (Node 8 Void Void))
	Variant: Height of the BStree.
-}
delete :: BSTree -> Int -> BSTree
delete Void _ = Void
delete (Node int left right) value 
	| int < value = Node int left (delete right value)
delete (Node int left right) value
	| int > value = Node int (delete left value) right
delete (Node int Void right) value = right
delete (Node int left Void) value = left
delete (Node int left right) value = (Node (findAux left 0) (delete left (findAux left 0)) right)

{-
delete (Node int Void Void) value = (Node int Void Void)
delete (Node int left Void) value = (Node int left Void)
delete (Node int Void right) value = (Node int Void right)
delete (Node int Void right) value 
 	| int == value = 
delete (Node int left Void) value

delete (Node int left right) value 
	| int == value = Node (findAux left 0) (deleteNode left (findAux left 0) ) right 
	| int < value = Node int left (delete right value)
	| int > value = Node int (delete left value) right
-}

{- findAux (Node int left right) startValue 
   PRE:  TRUE
   POST: The highest node value in the BSTree.
   EXAMPLES: 
  		findAux (Node 5 (Node 4 Void Void) (Node 6 Void Void)) 0 = 6
   Variant: Height of the BStree.
-}
findAux :: BSTree -> Int -> Int
findAux (Node int Void Void) startValue = int
findAux (Node int left right) startValue
	| int > startValue = findAux right int


{- deleteNode (Node int (Node int1 left1 right1) (Node int2 left2 right2)) value 
   PRE:  TRUE
   POST: A BStree with the node with the highest value is deleted.
   EXAMPLES: 
  		deleteNode (Node 5 (Node 4 Void Void) (Node 6 Void Void)) 6 = 
  			Node 5 (Node 4 Void Void) Void
   Variant: Height of the BStree.

deleteNode (Node int Void Void) value = (Node int Void Void)
deleteNode (Node int (Node int1 left1 right1) (Node int2 left2 right2)) value 
	| int1 == value = (Node int Void (Node int2 left2 right2))
	| int2 == value = (Node int (Node int1 left1 right1) Void)
	| otherwise =  Node int (Node int1 left1 right1) (deleteNode (Node int2 left2 right2) value) 
-}





--(Node 3 (Node 1 (Node 0 Void Void) (Node 2 Void Void)) (Node 7 (Node 5 (Node 4 Void Void) (Node 6 Void Void)) (Node 8 Void Void)))
