{- PKD Assignment 3 (Quadtrees), 2014/15

   Prepared by Tjark Weber

   Test cases originally developed by Karl Sundequist B.
     (karl.sundequist@it.uu.se)
 -}

{- Group 35
   Student 1 Karl Johansson
   Student 2 Isabelle Enlund Astrom

   Karl Johansson discussed the insert function/algorithm with Marcus Andersson group 19 in the beggining of the assignment. 
   We did not write any code together.
-}

import Test.HUnit -- provides testing framework
import System.Random


{- 
  REPRESENTATION CONVENTION: 
  A rectangle (Rect x1 y1 x2 y2), where (x1,y1) represent the upper left corner and (x2,y2) represents the lower right corner of the rectangle.
  
  REPRESENTATION INVARIANT: 
  x1 < x2 and y1 > y2. Smallest rectangle is 1*1. 
  A rectangle contains all the points (x,y) when  x1 <= x < x2 and y1 <= y < top.
-}
data Rectangle = Rect Integer Integer Integer Integer
  deriving (Eq,Show)

 
{- 
  REPRESENTATION CONVENTION: 
  A QuadTree is either empty, represented by EmptyQuadTree or a non-empty QuadTree is (Qt maxRect@(Rect x1 y1 x2 y2) (horizontal) (vertical) qTL qTR qBL qBR).
  The maxRect Rectangle respresent the area covered by the quadtree. 
  Horizontal is a list that contains the rectangles the horzontal center line "y" intersects, (y1 + y2) div 2
  Vertical is a list that contains the rectangles the vertical center line "x" intersects, (x1 + x2) div 2. 
    If both the centerline x and y intersects a rectangle it is stored in the Vertical list.
  The QaudTree has 4 subtrees topLeft topRight bottomLeft bottomRight which can either be a EmptyQuadTree or a QuadTree.
  topLeft Rectangle respresent the area covered by rectangle (Rect x1 y1 (centerline x) (centerline y+1))
  topRight Rectangle respresent the area covered by rectangle (Rect (centerline x+1) y1 x2 (centerline y+1))
  bottomLeft Rectangle respresent the area covered by rectangle (Rect x1 (centerline y) (centerline x) y2))
  bottomRight Rectangle respresent the area covered by rectangle (Rect (centerline x+1) (centerline y) x2 y2))
  +1 according to assignmentfile, points on the centerline will be stored in the vertical or horizontal list and that means that for the tofLeft subQuadTree we
  have to raise the "y2" point with +1 so it doesn't covers the points on the centerline. Same applies for the other subQuadtree as per above.
  REPRESENTATION INVARIANT: -- 
-}
data QuadTree = EmptyQuadTree
              | Qt Rectangle [Rectangle] [Rectangle]
                   QuadTree QuadTree QuadTree QuadTree
  deriving (Eq,Show)



{- 
  emptyQtree maxRect@(Rect left top right bottom)
  PURPOSE:  To create a quadtree that covers the rectangle area (left,top) corner to (right,bottom) corner, called extent
  PRE:   TRUE
  POST: A quadtree extent = maxRect and 2 empty lists and 4 EmptyQuadTree.
  EXAMPLES: 
    emptyQtree (Rect 0 10 10 0) =
      Qt (Rect 0 10 10 0) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree
-}
emptyQtree :: Rectangle -> QuadTree
emptyQtree (Rect left top right bottom)  = 
  (Qt (Rect left top right bottom) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree)


{- 
  insert (Qt maxRect@(Rect maxL maxT maxR maxB) h v qTL qTR qBL qBR) newRect@(Rect l t r b)
  PURPOSE:  To insert a rectangle into a quadtree
  PRE:  newRect dimensions must be inside the maxRect dimensions  
  POST: QuadTree Qt with rectangle newRect sorted into appropriate list either vertical or horizontal in the Qt or the subtrees qTL qTR qBL qBR
  EXAMPLES: 
    insert (Qt (Rect 0 10 10 0) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) (Rect 0 6 2 4) =
      Qt (Rect 0 10 10 0) [Rect 0 6 2 4] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree  -- sorted in horizontal list
    insert (Qt (Rect 0 10 10 0) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) (Rect 0 2 6 0)
      Qt (Rect 0 10 10 0) [] [Rect 0 2 6 0] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree  -- sorted in vertical list
    insert (Qt (Rect 0 10 10 0) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) (Rect 4 6 6 4)
      Qt (Rect 0 10 10 0) [] [Rect 4 6 6 4] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree  -- sorted in vertical list

  VARIANT: Height/depth of the quadtree. 
-}
insert :: QuadTree -> Rectangle -> QuadTree
insert EmptyQuadTree _ = EmptyQuadTree
insert (Qt maxRect@(Rect maxL maxT maxR maxB) h v qTL qTR qBL qBR) newRect@(Rect l t r b)  
  | l <= x && x < r                             = (Qt maxRect h (newRect:v) qTL qTR qBL qBR) -- rectangle on the vertical line x 
  | t > y && y >= b                             = (Qt maxRect (newRect:h) v qTL qTR qBL qBR) -- rectangle on the horizontal line y 
  | maxL <= l && r <= x && maxT >= t && b >= y  = (Qt maxRect h v (insert (emptySubTree qTL tL)  newRect) qTR qBL qBR) -- rectangle in TopLeft QuadTree
  | x <= l && r <= maxR && maxT >= t && b >= y  = (Qt maxRect h v qTL (insert (emptySubTree qTR tR) newRect) qBL qBR) -- rectangle in TopRight QuadTree
  | maxL <= l && r <= x && y >= t && b >= maxB  = (Qt maxRect h v qTL qTR (insert (emptySubTree qBL bL) newRect) qBR) -- rectangle in BottomLeft QuadTree
  | x <= l && r <= maxR && t <= y && b >= maxB  = (Qt maxRect h v qTL qTR qBL (insert (emptySubTree qBR bR) newRect)) -- rectangle in BottomRight QuadTree
  where 
    x = (maxL+maxR) `div` 2 -- vertical line 
    y = (maxT+maxB) `div` 2 -- horizontal line

    -- Represents the QuadTrees qTL qTR qBL qBR with new extent
    tL = (Qt (Rect maxL maxT x (y+1)) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) -- new TopLeft QuadTree 
    tR = (Qt (Rect (x+1) maxT maxR (y+1)) [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) -- new TopRight QuadTree
    bL = (Qt (Rect maxL y x maxB)[] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) -- new BottomLeft QuadTree
    bR = (Qt (Rect (x+1) y maxR maxB)[] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) -- new BottomRight QuadTree

    {- emptySubTree QuadTree1 QuadTree2
    PURPOSE:  To check if QuadTree1 is empty
    PRE: TRUE
    POST: If QuadTree1 is empty the function returns QuadTree2 otherwise it returns QuadTree1
    -}
    emptySubTree :: QuadTree -> QuadTree -> QuadTree
    emptySubTree EmptyQuadTree tree2 = tree2
    emptySubTree tree1 _ = tree1


{- 
  query (Qt maxRect@(Rect maxL maxT maxR maxB) h v qTL qTR qBL qBR) x y
  PURPOSE:  To check if any of the rectangles stored in the Quadtree Qt contains the point (x,y)
  PRE:   True
  POST: A list with the rectangles that contains the point (x,y) otherwise an empty list. 
  EXAMPLES: 
    query (Qt (Rect 0 10 10 0) [] [Rect 4 10 10 6] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) 7 7 = [Rect 4 10 10 6]
    query (Qt (Rect 0 10 10 0) [] [Rect 4 10 10 6] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) 11 11 = [] -- point outside of extent
  VARIANT: Height/depth of the quadtree. 
-}
query :: QuadTree -> Integer -> Integer -> [Rectangle]
query EmptyQuadTree _ _ = []
query (Qt _ [] [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) _ _ = []
query (Qt _  h [] EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) x y = inList h x y
query (Qt _  [] v EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) x y = inList v x y
query (Qt _  h v EmptyQuadTree EmptyQuadTree EmptyQuadTree EmptyQuadTree) x y = inList (h ++ v) x y
query (Qt maxRect@(Rect maxL maxT maxR maxB) h v qTL qTR qBL qBR) pointX pointY 
  --Checks within which QuadTree the point is located. 
  | maxL <= pointX && pointX < x && maxT > pointY && pointY >= y = inList (h ++ v) pointX pointY  ++ query qTL pointX pointY -- TopLeft  
  | x <= pointX && pointX < maxR && maxT > pointY && pointY >= y = inList (h ++ v) pointX pointY ++ query qTR pointX pointY  -- TopRight
  | maxL <= pointX && pointX < x && y > pointY && pointY >= maxB = inList (h ++ v) pointX pointY ++ query qBL pointX pointY -- BottomLeft
  | x <= pointX && pointX < maxR && y > pointY && pointY >= maxB = inList (h ++ v) pointX pointY ++ query qBR pointX pointY  -- BottomRight
  | otherwise = []
  where
    x = (maxL+maxR) `div` 2 -- vertical line 
    y = (maxT+maxB) `div` 2 -- horizontal line

{- inList (rectangle:list) x y
  PURPOSE:  Checks and sorts if the point (x,y) is within the rectangles stored in the list
  PRE: TRUE
  POST: A list with the rectangles that contains the point (x,y) otherwise an empty list.
  EXAMPLE:
    inList [] 1 1 = []
    inList [Rect 0 1 1 0] 1 1 = []
    inList [Rect 0 1 1 0] 0 0 = [Rect 0 1 1 0]
    inList [Rect 0 1 1 0 , Rect 0 5 5 0] 0 0 = [Rect 0 1 1 0,Rect 0 5 5 0]
  VARIANT: Size/lenght of the list.
-}
inList :: [Rectangle] -> Integer -> Integer -> [Rectangle]
inList [] _ _ = []
inList (rectangle:list) x y
  | inRectangle rectangle x y = rectangle : inList list x y
  | otherwise = inList list x y
  where 
  {- inRectangle rectangle@(Rect l t r b) x y
    PURPOSE:  Checks if the point x,y is within the rectangle
    PRE: TRUE
    POST: True if point (x,y) is within the rectangle otherwise False
  EXAMPLE:  
    inRectangle (Rect 0 1 1 0) 0 0 = True
    inRectangle (Rect 0 1 1 0) 1 1 = False
  -}
    inRectangle :: Rectangle -> Integer -> Integer -> Bool 
    inRectangle (Rect l t r b) x y 
      | (l <= x && x < r && b <= y && y < t) = True
      | otherwise = False



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



-- Av Albin Stjerna



test12 = TestCase $
         (let extent = (Rect (-25) 25 25 (-25))
              targetRect =  (Rect 0 1 1 0)
              validRectangle (Rect left top right bottom) = left < right && bottom < top
              randomContainedRect (Rect left top right bottom) gen =
                  let (newLeft, ngen) = randomR (left, (right-1)) gen
                      (newRight, ngen1) = randomR (newLeft, (right-1)) ngen
                      (newBottom, ngen2) = randomR (bottom, (top-1)) ngen1
                      (newTop, lastGen) = randomR (newBottom, (top-1)) ngen2
                  in
                    ((Rect newLeft newTop newRight newBottom), lastGen)

              randomContainedRects rect gen=
                let (newRect, newGen) = randomContainedRect rect gen
                in
                  if validRectangle newRect then newRect : (randomContainedRects rect newGen)
                  else randomContainedRects rect newGen
          in do
            gen <- newStdGen
            let rects = take 10000 $ randomContainedRects extent gen
            let qt = createQtree extent (targetRect : rects)
            let findings = query qt 0 0 -- should find our targetRect
            assertBool "Contains target rectangle" (targetRect `elem` findings))


-- for running all the tests
runtests = runTestTT $ TestList [test1, test2, test3, test4, test5, test12]
