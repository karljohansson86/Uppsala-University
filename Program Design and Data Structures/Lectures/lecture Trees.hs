{-
data FBTree a = Leaf a
	| Node (FBTree a) a (FBTree a)


mirror :: FBTree a -> FBTree a
mirror (Leaf x) = (Leaf x)
mirror (Node l x r) = Node (mirror r) x (mirror l) 

listInOrder :: FBTree a -> [a]
listInOrder (Leaf a) = [a]
listInOrder (Node l a r) = listInOrder l ++ [a] ++ listInOrder r
-}

{-
data Tree a = Node a [Tree a] deriving (Show)

treeSum :: Tree Int -> Int
treeSum (Node a []) = a
treeSum (Node a ts) = a + treeSumList ts 

treeSumList :: [Tree Int] -> Int
treeSumList [] = 0
treeSumList (t:ts) = treeSum t + treeSumList ts
-}

{-
data BTree a = Void | Node a (BTree a) (BTree a) deriving (Show)

insert :: BTree a -> a -> BTree a
insert t a = Node a Void t

find :: Eq a => BTree a -> a -> Bool
find Void v = False
find (Node x l r) v = x==v || find l v || find r v
-}


{- Represent Invariant: 
-}

{-
data BSTree = Void | Node Int BSTree BSTree deriving (Show)

insert :: BSTree -> Int -> BSTree
insert Void v = Node v Void Void
insert (Node x l r) v  
	| x==v = Node x l r
	| x>v = Node x (insert l v) r
	| x<v = Node x l (insert r v)

find :: BSTree -> Int -> Bool
find Void _ = False
find (Node x l r) v 
	| x==v = True
	| x < v = find r v
	| x > v = find l v
-}

--Red/Black tree

data Color = Red | Black
data RBTree = Void | Node Color Int RBTree RBTree deriving(Show) 

insert :: RBTree -> Int -> RBTree
insert t v =
		let
			ins Void = Node Red v Void Void
			ins (Node c x l r) 	| x == v = Node c x l r
								| x > v = balance c x (ins l) r
								| x < v = balance c x l ( ins r)
			Node _ x l r = ins t
		in Node Black x l r


balance :: Color -> Int -> RBTree -> RBTree -> RBTree
balance Black z (Node Red y (Node Red x a b) c) d =
	Node Red y (Node Black x a b) (Node Black z c d)

balance Black z (Node Red x a (Node Red y b c)) d =
	Node Red y (Node Black x a b) (Node Black z c d)

balance Black x a (Node Red z (Node Red y b c) d) =
	Node Red y ( Node Black x a b) ( Node Black z c d)

balance Black x a ( Node Red y b (Node Red z c d)) =
	Node Red y (Node Black x a b) (Node Black z c d)

balance c v l r = Node c v l r
