{- PKD Assignment 3 (Quadtrees), 2014/15

   Prepared by Tjark Weber

   Test cases originally developed by Karl Sundequist B.
     (karl.sundequist@it.uu.se)
 -}

{- Group 35
   Student 1 Karl Johansson
   Student 2 (Name)
 -}

import Test.HUnit -- provides testing framework


{- TODO: comment according to our Coding Convention 

We assume the precondition bottom < top
and left < right for any rectangle
-}
data Rectangle = Rect Integer Integer Integer Integer
  deriving (Eq,Show)

{- TODO: comment according to our Coding Convention -}
data QuadTree = EmptyQuadTree
              | Qt Rectangle [Rectangle] [Rectangle]
                   QuadTree QuadTree QuadTree QuadTree
  deriving (Eq,Show)

{- TODO: comment according to our Coding Convention -}
emptyQtree :: Rectangle -> QuadTree
emptyQtree (Rect leftB leftT rightT rightB) = Qt (Rect leftB leftT rightT rightB) [] []
                   EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree

{- TODO: comment according to our Coding Convention 
PRE - under the precondition that all points of r
are inside the extent of q-}
insert :: QuadTree -> Rectangle -> QuadTree
insert (Qt maxDims@(Rect maxLeftX maxLeftY maxRightX maxRightY) h v tL tR bL bR) newRect
    | onCenterLine2 maxDims newRect = (Qt maxDims h (newRect:v) tL tR bL bR)
    | onCenterLine1 maxDims newRect = (Qt maxDims (newRect:h) v tL tR bL bR)
    | otherwise = case (whichQT maxDims newRect ) of
      1 -> (Qt maxDims h v (insert (Qt (Rect maxLeftX maxLeftY x y) [] []
                    EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) newRect) tR bL bR)
      2 -> (Qt maxDims h v tL (insert (Qt (Rect x maxLeftY maxRightX y) [] []
                    EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) newRect) bL bR)
      3 -> (Qt maxDims h v tL tR (insert (Qt (Rect maxLeftX y x maxRightY) [] []
                    EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) newRect) bR)
      4 -> (Qt maxDims h v tL tR bL (insert (Qt (Rect x y maxRightX maxRightY) [] []
                    EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) newRect) )
  where
    y = (maxLeftY + maxRightY) `div` 2
    x = (maxLeftX + maxRightX) `div` 2

------delas till Int , dvs rest delen kommer bli större
onCenterLine1 :: Rectangle -> Rectangle -> Bool 
onCenterLine1 (Rect maxLeftX maxLeftY maxRightX maxRightY) (Rect leftX leftY rightX rightY)
  | (leftX <=  (maxRightX - x) && x < rightX) = True  --(maxRightX - x) x
  | otherwise = False
  where
    x = (maxLeftX + maxRightX) `div` 2

onCenterLine2 :: Rectangle -> Rectangle -> Bool 
onCenterLine2 (Rect maxLeftX maxLeftY maxRightX maxRightY) (Rect leftX leftY rightX rightY)
  | (leftY > y && rightY <=  (maxRightY + y)) = True -- (maxRightY - y)
  | otherwise = False
  where
    y = (maxLeftY + maxRightY) `div` 2

whichQT :: Rectangle -> Rectangle -> Int
whichQT (Rect maxLeftX maxLeftY maxRightX maxRightY) (Rect leftX leftY rightX rightY)
  | rightX < x && rightY > y = 1 --topleft
  | leftX > x && rightY > y = 2 --topright
  | rightX < x && leftY < y = 3 --bleft
  | leftX > x && leftY < y = 4 --bright
  where
    y = (maxLeftY + maxRightY) `div` 2
    x = (maxLeftX + maxRightX) `div` 2




{- TODO: comment according to our Coding Convention -}
query :: QuadTree -> Integer -> Integer -> [Rectangle]
query (Qt maxDims [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) _ _ = []
query (Qt maxDims h [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) x y = inList h x y
query (Qt maxDims [] v EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) x y = inList v x y

query (Qt maxDims@(Rect maxLeftX maxLeftY maxRightX maxRightY) h v tL tR bL bR) x y =
  case (whichQT2 maxDims x y ) of
    1 -> (inList h x y ++  (inList v x y)) ++ (query tL x y )
    2 -> (inList h x y ++  (inList v x y)) ++ (query tR x y )
    3 -> (inList h x y ++  (inList v x y)) ++ (query bL x y )
    4 -> (inList h x y ++  (inList v x y)) ++ (query bR x y )
    5 -> []

  where
    centerY = (maxLeftY + maxRightY) `div` 2
    centerX = (maxLeftX + maxRightX) `div` 2
 {- inList q x y


inList (Qt maxDims [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) x y = []
inList (Qt maxDims@(Rect maxLeftX maxLeftY maxRightX maxRightY) h v tL tR bL bR) x y = undefined
-}


inList :: [Rectangle] -> Integer -> Integer -> [Rectangle]
inList [] _ _ = []
inList (a:as) x y 
  | compareElement a x y = (a) : (inList as x y)
  | otherwise = inList as x y


compareElement :: Rectangle -> Integer -> Integer -> Bool
compareElement (Rect leftX leftY rightX rightY) x y 
  | (leftX <= x && x <= rightX) = True
  | (leftY >= y && y >= rightY) = True
  | otherwise = False

whichQT2 :: Rectangle -> Integer -> Integer -> Integer 
whichQT2 (Rect maxLeftX maxLeftY maxRightX maxRightY) x y
  | x < centerX && y > centerY = 1 --topLeft
  | x > centerX && y > centerY = 2 --topRight
  | x < centerX && y < centerY = 3 --bottomLeft
  | x > centerX && y < centerY = 4 --bottomRight
  | otherwise = 5
  where
    centerY = (maxLeftY + maxRightY) `div` 2
    centerX = (maxLeftX + maxRightX) `div` 2



-- test cases

{- createQtree e rs
   PRE: The extent e and all rectangles in rs are non-empty, and the extent
        contains all rectangles in rs.
   POST: A quadtree with the extent e containing all rectangles in rs.
 -}
createQtree :: Rectangle -> [Rectangle] -> QuadTree
createQtree e rs = foldl insert (emptyQtree e) rs

test1 = TestCase $ assertBool "non-overlapping rectangles"
  (let
    q = createQtree (Rect 0 6 10 0) [Rect 0 2 4 1, Rect 7 5 10 4]
  in
    -- testing one point in each of the two rectangles
    [Rect 0 2 4 1] == query q 2 1 && [Rect 7 5 10 4] == query q 8 4)

test2 = TestCase $ assertBool "overlapping rectangles stored in the same node"
  (let
    q = createQtree (Rect 0 6 10 0) [Rect 0 3 10 0, Rect 0 6 3 0]
    -- testing a point in both of the rectangles
    result = query q 1 1
  in
    result == [Rect 0 3 10 0, Rect 0 6 3 0] ||
      result == [Rect 0 6 3 0, Rect 0 3 10 0])

test3 = TestCase $ assertBool "overlapping rectangles stored in different nodes"
  (let
    q = createQtree (Rect 0 10 10 0) [Rect 4 10 10 6, Rect 8 10 10 8]
    -- testing a point in both of the rectangles
    result = query q 9 9
  in
    result == [Rect 4 10 10 6, Rect 8 10 10 8] ||
      result == [Rect 8 10 10 8, Rect 4 10 10 6])

test4 = TestCase $ assertBool "non-overlapping rectangles"
  (let
    q = createQtree (Rect 0 6 6 0) [Rect 0 2 2 0, Rect 4 6 6 5]
  in
    -- testing a point in neither of the rectangles
    query q 3 4 == [])

test5 = TestCase $ assertBool "each quadrant"
  (let
    qBL = createQtree (Rect 0 5 5 0) [Rect 0 2 2 0]
    qBR = createQtree (Rect 0 5 5 0) [Rect 3 2 5 0]
    qTL = createQtree (Rect 0 5 5 0) [Rect 0 5 2 3]
    qTR = createQtree (Rect 0 5 5 0) [Rect 3 5 5 3]
    {- getQuads t
       PRE: True
       POST: All quadrants of t, in the order in which they appear in the
             datatype definition; if t is empty, then four empty quadrants.
     -}
    getQuads :: QuadTree -> (QuadTree, QuadTree, QuadTree, QuadTree)
    getQuads EmptyQuadTree = (EmptyQuadTree, EmptyQuadTree, EmptyQuadTree, EmptyQuadTree)
    getQuads (Qt _ _ _ a b c d) = (a, b, c, d)
  in
    -- make sure they work before we dig into them!
    query qBL 1 1 == [Rect 0 2 2 0] &&
    query qBR 4 1 == [Rect 3 2 5 0] &&
    query qTL 1 4 == [Rect 0 5 2 3] &&
    query qTR 4 4 == [Rect 3 5 5 3] &&
    -- whether everything except top-left quadrant of qTL is empty
    let
      (q1, q2, q3, q4) = getQuads qTL
    in
      q1 /= EmptyQuadTree && q2 == EmptyQuadTree && q3 == EmptyQuadTree && q4 == EmptyQuadTree &&
    -- whether everything except top-right quadrant of qTR is empty
    let
      (q1, q2, q3, q4) = getQuads qTR
    in
      q1 == EmptyQuadTree && q2 /= EmptyQuadTree && q3 == EmptyQuadTree && q4 == EmptyQuadTree &&
    -- whether everything except bottom-left quadrant of qBL is empty
    let
      (q1, q2, q3, q4) = getQuads qBL
    in
      q1 == EmptyQuadTree && q2 == EmptyQuadTree && q3 /= EmptyQuadTree && q4 == EmptyQuadTree &&
    -- whether everything except bottom-right quadrant of qBR is empty
    let
      (q1, q2, q3, q4) = getQuads qBR
    in
      q1 == EmptyQuadTree && q2 == EmptyQuadTree && q3 == EmptyQuadTree && q4 /= EmptyQuadTree)

-- for running all the tests
runtests = runTestTT $ TestList [test1, test2, test3, test4, test5]
