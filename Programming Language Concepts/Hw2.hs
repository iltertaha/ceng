module Hw2 where

data ASTResult = ASTError String | ASTJust (String, String, Int) deriving (Show, Read)
data ASTDatum = ASTSimpleDatum String | ASTLetDatum String deriving (Show, Read)
data AST = EmptyAST | ASTNode ASTDatum AST AST deriving (Show, Read)

isNumber :: String -> Bool
eagerEvaluation :: AST -> ASTResult
normalEvaluation :: AST -> ASTResult 
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT isNumber, eagerEvaluation and normalEvaluation FUNCTIONS ACCORDING TO GIVEN SIGNATURES -- 

isNumber [] = False
isNumber t@(x:word)
    | x =='-' && length t == 1 = False
    | x == '-' = isNumber_helper word
    | otherwise = isNumber_helper t
    where
        isNumber_helper "" = True
        isNumber_helper (x:word)
            | x == '0' = True && isNumber_helper word
            | x == '1' = True && isNumber_helper word
            | x == '2' = True && isNumber_helper word
            | x == '3' = True && isNumber_helper word
            | x == '4' = True && isNumber_helper word
            | x == '5' = True && isNumber_helper word
            | x == '6' = True && isNumber_helper word
            | x == '7' = True && isNumber_helper word
            | x == '8' = True && isNumber_helper word
            | x == '9' = True && isNumber_helper word
            | otherwise = False


eagerEvaluation EmptyAST = ASTJust ("","",0)
eagerEvaluation (ASTNode (ASTSimpleDatum a) l r)



    | a == "num"  = if isNumber(getvalue_num l) == True
                    then ASTJust (getvalue_num l , "num",0)
                        else ASTError ("the value " ++ "'" ++ getvalue_num l ++ "'"  ++ " is not a number!")
    
    | a == "str"  = ASTJust (getvalue_num l , "str",0)

    | a == "plus" = if isError (eagerEvaluation l) then returnError (eagerEvaluation l)
                    else if (isError (eagerEvaluation r)) then returnError (eagerEvaluation r)
                    else if tuple_type (eagerEvaluation l) == "num" && tuple_type (eagerEvaluation r) == "num" 
                        then ASTJust ( show ((read (tuple_value (eagerEvaluation l))::Int) + (read (tuple_value (eagerEvaluation r))::Int)),"num",(tuple_step (eagerEvaluation l)) + tuple_step (eagerEvaluation r) + 1 ) 
                    else ASTError ("plus operation is not defined between " ++ tuple_type (eagerEvaluation l) ++ " and " ++ (tuple_type (eagerEvaluation r)) ++ "!")
    
    
    | a == "times" = if isError (eagerEvaluation l) then returnError (eagerEvaluation l)
                    else if (isError (eagerEvaluation r)) then returnError (eagerEvaluation r)
                    else if tuple_type (eagerEvaluation l) == "num" && tuple_type (eagerEvaluation r) == "num" 
                        then ASTJust ( show ((read (tuple_value (eagerEvaluation l))::Int) * (read (tuple_value (eagerEvaluation r))::Int)),"num",(tuple_step (eagerEvaluation l)) + tuple_step (eagerEvaluation r) + 1 ) 
                    else ASTError ("times operation is not defined between " ++ tuple_type (eagerEvaluation l) ++ " and " ++ (tuple_type (eagerEvaluation r)) ++ "!")
    
                    
    | a == "cat" = if isError (eagerEvaluation l) then returnError (eagerEvaluation l)
                    else if (isError (eagerEvaluation r)) then returnError (eagerEvaluation r)
                    else if tuple_type (eagerEvaluation l) == "str" && tuple_type (eagerEvaluation r) == "str"
                        then ASTJust ( (tuple_value (eagerEvaluation l)) ++ (tuple_value (eagerEvaluation r)) ,"str",(tuple_step (eagerEvaluation l)) + tuple_step (eagerEvaluation r) + 1 ) 
                    else ASTError ("cat operation is not defined between " ++ tuple_type (eagerEvaluation l) ++ " and " ++ (tuple_type (eagerEvaluation r)) ++ "!" )
    
    | a == "negate" = if isError (eagerEvaluation l) then returnError (eagerEvaluation l)
                        else if tuple_type (eagerEvaluation l) == "num" 
                        then ASTJust ( show ((read (tuple_value (eagerEvaluation l))::Int) * (-1) ),"num",(tuple_step (eagerEvaluation l)) + 1 ) 
                    else ASTError ("negate operation is not defined on str!")

    | a == "len" = if isError (eagerEvaluation l) then returnError (eagerEvaluation l)
                        else if tuple_type (eagerEvaluation l) == "str" 
                        then ASTJust ( show (length (tuple_value (eagerEvaluation l)) ),"num",(tuple_step (eagerEvaluation l)) + 1 ) 
                else ASTError ("len operation is not defined on num!")
    
eagerEvaluation (ASTNode (ASTLetDatum a) l r)
    | isError (eagerEvaluation l) == True = returnError (eagerEvaluation l)
    | otherwise                           = (eager_step_modifier (eagerEvaluation ((treefixer a (tuple_value (eagerEvaluation l)) (tuple_type (eagerEvaluation l)) r))) (tuple_step (eagerEvaluation l))) 

getvalue_num (ASTNode (ASTSimpleDatum x) _ _ ) = x
tuple_value (ASTJust (x, _ , _)) = x
tuple_type (ASTJust (_,x,_)) = x
tuple_step (ASTJust (_ , _ , x )) = x

isError (ASTError _) = True
isError _         = False
returnError (ASTError x)=ASTError x

treefixer x val tip  EmptyAST = EmptyAST
treefixer x val tip  (ASTNode (ASTSimpleDatum a) l r) 
    | x == a    = (ASTNode (ASTSimpleDatum tip) (ASTNode (ASTSimpleDatum val) EmptyAST EmptyAST ) EmptyAST) 
    | otherwise = (ASTNode (ASTSimpleDatum a) (treefixer x val tip l) (treefixer x val tip r))

treefixer x val tip  (ASTNode (ASTLetDatum a) l r) 
    |  x == a       = (treefixer a (tuple_value (eagerEvaluation l)) (tuple_type (eagerEvaluation l)) r)
    | otherwise     = (ASTNode (ASTLetDatum a) (treefixer x val tip l) (treefixer x val tip r))
{- --------------------------- -} 


treecount x val tip  (ASTNode (ASTSimpleDatum a) l r) 
    | x == a    = (ASTNode (ASTSimpleDatum tip) (ASTNode (ASTSimpleDatum val) EmptyAST EmptyAST ) EmptyAST) 
    | otherwise = (ASTNode (ASTSimpleDatum a) (treefixer x val tip l) (treefixer x val tip r))

eager_step_modifier (ASTJust ( a , b , c)) ekstra = ASTJust ( a , b , c + ekstra)

{- --------------- END OF EAGER EVAL ----------------}



normalEvaluation (EmptyAST) = ASTJust ("0","0",0)
normalEvaluation (ASTNode (ASTSimpleDatum a) l r)



    | a == "num"  = if isNumber(getvalue_num l) == True
                    then ASTJust (getvalue_num l , "num",0)
                        else ASTError ("the value " ++ "'" ++ getvalue_num l ++ "'"  ++ " is not a number!")
    
    | a == "str"  = ASTJust (getvalue_num l , "str",0)

    | a == "plus" = if isError (normalEvaluation l) then returnError (normalEvaluation l)
                    else if (isError (normalEvaluation r)) then returnError (normalEvaluation r)
                    else if tuple_type (normalEvaluation l) == "num" && tuple_type (normalEvaluation r) == "num" 
                        then ASTJust ( show ((read (tuple_value (normalEvaluation l))::Int) + (read (tuple_value (normalEvaluation r))::Int)),"num",(tuple_step (normalEvaluation l)) + tuple_step (normalEvaluation r) + 1 ) 
                    else ASTError ("plus operation is not defined between " ++ tuple_type (normalEvaluation l) ++ " and " ++ (tuple_type (normalEvaluation r)) ++ "!")
    
    
    | a == "times" = if isError (normalEvaluation l) then returnError (normalEvaluation l)
                    else if (isError (normalEvaluation r)) then returnError (normalEvaluation r)
                    else if tuple_type (normalEvaluation l) == "num" && tuple_type (normalEvaluation r) == "num" 
                        then ASTJust ( show ((read (tuple_value (normalEvaluation l))::Int) * (read (tuple_value (normalEvaluation r))::Int)),"num",(tuple_step (normalEvaluation l)) + tuple_step (normalEvaluation r) + 1 ) 
                    else ASTError ("times operation is not defined between " ++ tuple_type (normalEvaluation l) ++ " and " ++ (tuple_type (normalEvaluation r)) ++ "!")
    
                    
    | a == "cat" = if isError (normalEvaluation l) then returnError (normalEvaluation l)
                    else if (isError (normalEvaluation r)) then returnError (normalEvaluation r)
                    else if tuple_type (normalEvaluation l) == "str" && tuple_type (normalEvaluation r) == "str"
                        then ASTJust ( (tuple_value (normalEvaluation l)) ++ (tuple_value (normalEvaluation r)) ,"str",(tuple_step (normalEvaluation l)) + tuple_step (normalEvaluation r) + 1 ) 
                    else ASTError ("cat operation is not defined between " ++ tuple_type (normalEvaluation l) ++ " and " ++ (tuple_type (normalEvaluation r)) ++ "!")
    
    | a == "negate" = if isError (normalEvaluation l) then returnError (normalEvaluation l)
                        else if tuple_type (normalEvaluation l) == "num" 
                        then ASTJust ( show ((read (tuple_value (normalEvaluation l))::Int) * (-1) ),"num",(tuple_step (normalEvaluation l)) + 1 ) 
                    else ASTError ("negate operation is not defined on str!")

    | a == "len" = if isError (normalEvaluation l) then returnError (normalEvaluation l)
                        else if tuple_type (normalEvaluation l) == "str" 
                        then ASTJust ( show (length (tuple_value (normalEvaluation l)) ),"num",(tuple_step (normalEvaluation l)) + 1 ) 
                else ASTError ("len operation is not defined on num!")
    
normalEvaluation (ASTNode (ASTLetDatum a) l r)
    | isError (normalEvaluation l) == True = returnError (normalEvaluation l)
    | otherwise                            = normalEvaluation (treefixernormal a l r)




treefixernormal x sol_agac EmptyAST = EmptyAST
treefixernormal x sol_agac (ASTNode (ASTSimpleDatum a) l r) 
    | x == a    = sol_agac
    | otherwise = (ASTNode (ASTSimpleDatum a) (treefixernormal x sol_agac l) (treefixernormal x sol_agac r))

treefixernormal x sol_agac (ASTNode (ASTLetDatum a) l r) 
    | x == a    = treefixernormal x l r
    | otherwise =  (ASTNode (ASTLetDatum a) (treefixernormal x sol_agac l) (treefixernormal x sol_agac r)) 

{- -------------END OF NORMALEVAL-------------- -} 

