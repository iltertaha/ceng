

    type Mapping = [(String, String, String)]
    data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)
    
    writeExpression :: (AST, Mapping) -> String
    evaluateAST :: (AST, Mapping) -> (AST, String)
    -- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
    -- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES -- 

    writeExpression (EmptyAST,veri) = ""
    writeExpression (ASTNode a l r,veri) 
        
        | length veri >= 1   = "let " ++ checklist_more veri ++ " in "  ++ writeExpressionHelper (ASTNode a l r,veri)
        | otherwise = writeExpressionHelper (ASTNode a l r,veri)
        


        where
            writeExpressionHelper (EmptyAST,veri) = ""
            writeExpressionHelper (ASTNode a l r,veri)
                | a == "num"       = getvalue_num (l)
                | a == "str"       = getvalue_str (l) 
                | a == "plus"      = "(" ++ writeExpressionHelper (l,veri) ++ "+" ++ writeExpressionHelper (r,veri) ++ ")"
                | a == "times"     = "(" ++ writeExpressionHelper (l,veri) ++ "*" ++ writeExpressionHelper (r,veri) ++ ")"
                | a == "negate"    = "(" ++ "-" ++ writeExpressionHelper (l,veri) ++ ")"
                | a == "cat"       = "(" ++ writeExpressionHelper (l,veri) ++ "++" ++ writeExpressionHelper (r,veri) ++ ")"
                | a == "len"       = "(" ++ "length " ++ writeExpressionHelper (l,veri) ++ ")"
                | otherwise = a

                where  
                    getvalue_str (ASTNode x _ _) = show x {- show u kaldır -}
                    getvalue_num (ASTNode x _ _) = x

    
            checklist_more t@((x,y,z):xs)
                | length t == 0 = ""
                | length t == 1 = x ++ "=" ++ num_str_decider ((x,y,z):xs)
                | length t > 1  = x ++ "=" ++ num_str_decider ((x,y,z):xs) ++ ";" ++ checklist_more xs
            
            num_str_decider t@((x,y,z):xs)
                | y == "num" = show (read z::Int)
                | y == "str" = show (z)

    evaluateAST ((ASTNode a l r) , veri)  = ((treeInsert (ASTNode a l r)), result (treeInsert (ASTNode a l r)) ) 
        where 
            
            
            treeInsert EmptyAST = EmptyAST   
            treeInsert (ASTNode a l r)   
                | (is_contain a veri) == 1 = ASTNode (get_sub_cat a veri)  (value_fixer (get_sub_val a veri) l) EmptyAST  
                | otherwise                = ASTNode a (treeInsert l) (treeInsert r)  
                  


            getvalue_str (ASTNode x _ _) = show x
            getvalue_num (ASTNode x _ _) = x

            is_contain x ((q,w,e):ys) = if x == q then 1 else is_contain x ys
            is_contain x [] = -1

            get_sub_val x ((q,w,e):ys) = if x == q then e else get_sub_val x ys
            get_sub_val x [] = ""

            get_sub_cat x ((q,w,e):ys) = if x == q then w else get_sub_cat x ys
            get_sub_cat x [] = ""
            
            value_fixer x (ASTNode a l r) = (ASTNode x EmptyAST EmptyAST)
            value_fixer x EmptyAST = ASTNode x EmptyAST EmptyAST
{- ------------------------------------------------------------------------------------------ -}

            result (EmptyAST) = ""
            result (ASTNode a l r)
                | a == "num"       = (getvalue_num (l))
                | a == "str"       = (getvalue_num (l))
                | a == "plus"      = show ((+) (read_as_int (result l)) (read_as_int (result r)))
                | a == "times"     = show ((*) (read_as_int (result l)) (read_as_int (result r)))
                | a == "negate"    = show ((*) (read_as_int (result l)) (-1))
                | a == "cat"       = (++) (result l) (result r)
                | a == "len"       = show (length (result l))
                | otherwise = ""


            read_as_int x = read x::Integer
            read_as_str x = read x::String
            
            getevalue_str (ASTNode x _ _) = show x
            getevalue_num (ASTNode x _ _) = x

                
            

            
    
          
