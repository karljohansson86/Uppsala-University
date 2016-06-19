import System.Random

type AssocList a b = [(a,b)]

lookup1 :: Eq a => a -> AssocList a b -> Maybe b
lookup1 _ [] = Nothing
lookup1 k ((x,y):xs) = if x == k then Just y else lookup k xs


data Snarp = Grob | Str String | Gister Snarp Integer

data Tree a = Node a [Tree a] 
  deriving Show

foo (Node a ts) = a : foos ts 

foos [] = []
foos (t:ts) = foo t ++ foos ts


broken = do
  putStrLn $ "Enter "
    ++ "your " ++ "name:"
  let x = getLine
  y <- putStrLn "Thank you"
  x

{-
data Stack a = [] | Stack a [Stack b]
empty :: Stack a
isEmpty :: Stack a -> Bool
push :: a -> Stack a -> Stack a
top :: Stack a -> a
pop :: Stack a -> Stack a -}

threeCoins :: (Bool, Bool, Bool)
threeCoins = 
  let gen = (mkStdGen 20)
  in (fst (random gen),fst (random gen),fst (random gen))
