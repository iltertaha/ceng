/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ceng.ceng351.bookdb;

/**
 *
 * @author Ilter taha Aktolga
 */



import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;
import java.util.ArrayList;



 public class BOOKDB implements IBOOKDB {
    private static String user = "2236891"; // TODO: Your userName
    private static String password = "<passwordhere>"; //  TODO: Your password
    private static String host = "<iphere>"; // host name
    private static String database = "db2236891"; // TODO: Your database name
    private static int port = 8084; // port
     
    private Connection con;

    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database;

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            this.con =  DriverManager.getConnection(url, this.user, this.password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        } 
    }

    @Override
    public int createTables(){
        int result;
        int inserted = 0;

        String queryCreateAuthorTable  = "create table author (" +
                                            "author_id int," +
                                            "author_name varchar(60)," +
                                            "primary key (author_id));";
        
        
        String queryCreatePublisherTable = "create table publisher (" +
                                            "publisher_id int," +
                                            "publisher_name varchar(50)," +
                                            "primary key (publisher_id));";
        
        
        String queryCreateBookTable = "create table book (" +
                                       "isbn char(13)," +
                                       "book_name varchar(120)," +
                                       "publisher_id int ," +
                                       "first_publish_year char(4)," +
                                       "page_count int ," +
                                       "category varchar(25)," +
                                       "rating float ," +
                                       "primary key (isbn)," +
                                       "foreign key (publisher_id) references publisher(publisher_id));";
        
        String queryCreateAuthorofTable = "create table author_of (" +
                                            "isbn char(13)," +
                                            "author_id int ," +
                                            "primary key(isbn,author_id) ," +
                                            "foreign key (isbn) references book(isbn)," +
                                            "foreign key (author_id) references author(author_id));";
        
        String queryCreatePhw1Table = "create table phw1 (" +
                                        "isbn char(13) ," +
                                        "book_name varchar(120) ,"+
                                        "rating float ," +
                                        "primary key (isbn));";
        
        try {
            Statement statement = this.con.createStatement();

            statement.executeUpdate(queryCreateAuthorTable);
            inserted++;
            statement.close();

        } 
        catch (SQLException e) {
            e.printStackTrace();
        }
        
        try {
            Statement statement = this.con.createStatement();

            statement.executeUpdate(queryCreatePublisherTable);
            inserted++;
            statement.close();

        } 
        catch (SQLException e) {
            e.printStackTrace();
        }
        
        try {
            Statement statement = this.con.createStatement();

            statement.executeUpdate(queryCreateBookTable);
            inserted++;
            statement.close();

        } 
        catch (SQLException e) {
            e.printStackTrace();
        }
        
        
        
        try {
            Statement statement = this.con.createStatement();

            statement.executeUpdate(queryCreateAuthorofTable);
            inserted++;
            statement.close();

        } 
        catch (SQLException e) {
            e.printStackTrace();
        }
        
        try {
            Statement statement = this.con.createStatement();

            statement.executeUpdate(queryCreatePhw1Table);
            inserted++;
            statement.close();

        } 
        catch (SQLException e) {
            e.printStackTrace();
        }
        
        
        
        
        return inserted;
    }
    
    @Override
    public int dropTables(){
        
        
        int dropped = 0;
        
        
        String queryDropBookTable = "drop table if exists book";
        
        String queryAuthorofTable = "drop table if exists author_of";
        
        String queryDropAuthorTable = "drop table if exists author";
        
        String queryDropPublisherTable = "drop table if exists publisher";
        
        
        
        
        
        String queryPhw1Table = "drop table if exists phw1";
        
        
        try {
            Statement statement = this.con.createStatement();
           
        statement.executeUpdate(queryAuthorofTable);
        dropped++;
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        
        try {
            Statement statement = this.con.createStatement();
           
        statement.executeUpdate(queryDropBookTable);
        dropped++;
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        try {
            Statement statement = this.con.createStatement();
           
        statement.executeUpdate(queryDropAuthorTable);
        dropped++;
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        
        try {
            Statement statement = this.con.createStatement();
           
        statement.executeUpdate(queryDropPublisherTable);
        dropped++;
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        try {
            Statement statement = this.con.createStatement();
           
        statement.executeUpdate(queryPhw1Table);
        dropped++;
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        
   
     
        return dropped;
        
        
        
    }
    
    
    @Override
    public int insertAuthor(Author[] authors){
        int inserted = 0;
        int result;
        
        for(int i=0; i< authors.length ; i++){
        
            try {
                Author auth = authors[i];
           
                String query = "insert into author values(?,?)";
                PreparedStatement stmt = this.con.prepareStatement(query);
                
                stmt.setInt(1,auth.getAuthor_id());
                stmt.setString(2,auth.getAuthor_name());
                
                
                
                result = stmt.executeUpdate();
                //System.out.println(result);
                
                
                stmt.close();
                
                inserted++;
              
                
            }
            catch(SQLException e){
            if (e.toString().contains("SQLIntegrityConstraintViolationException")){
                e.printStackTrace();
            }
            
            }
        
        }
        
                        
        return inserted;
    }
    
    @Override
    public int insertBook(Book[] books){
    int inserted = 0;
    int result;
    
    for(int i=0;i<books.length;i++){
        try{
            Book buk = books[i];
            
            String query = "insert into book values(?,?,?,?,?,?,?)";
            PreparedStatement stmt = this.con.prepareStatement(query);
            
            stmt.setString(1,buk.getIsbn());
            stmt.setString(2,buk.getBook_name());
            stmt.setInt(3,buk.getPublisher_id());
            stmt.setString(4,buk.getFirst_publish_year());
            stmt.setInt(5,buk.getPage_count());
            stmt.setString(6,buk.getCategory());
            stmt.setDouble(7,buk.getRating());
            
            
            stmt.executeUpdate();
            
            stmt.close();
            inserted++;
      
        }
        catch(SQLException e){
            if (e.toString().contains("SQLIntegrityConstraintViolationException")){
                e.printStackTrace();
            }
            
            }
        
        
    }
        return inserted;
    
    }
    
    @Override
    public int insertPublisher(Publisher[] publishers){
        int inserted = 0;
        int result;
        
        for(int i=0;i<publishers.length;i++){
        try{
            Publisher pub = publishers[i];
            
            String query = "insert into publisher values(?,?)";
            PreparedStatement stmt = this.con.prepareStatement(query);
            
            stmt.setInt(1,pub.getPublisher_id());
            stmt.setString(2,pub.getPublisher_name());
            
            stmt.executeUpdate();
            
            stmt.close();
            inserted++;
            
            }
        
        catch(SQLException e){
            if (e.toString().contains("SQLIntegrityConstraintViolationException")){
                e.printStackTrace();
            }
            
            }
        
        
        
    }
        return inserted;
    }
    
    
    @Override
    public int insertAuthor_of(Author_of[] author_ofs){
        int inserted = 0;
        int result;
        
        for(int i=0;i<author_ofs.length;i++){
        try{
            Author_of auth_of = author_ofs[i];
            
            String query = "insert into author_of values(?,?)";
            PreparedStatement stmt = this.con.prepareStatement(query);
            
            stmt.setString(1,auth_of.getIsbn());
            stmt.setInt(2,auth_of.getAuthor_id());
            
            stmt.executeUpdate();
            
            stmt.close();
            inserted++;
            
            }
        
        catch(SQLException e){
            if (e.toString().contains("SQLIntegrityConstraintViolationException")){
                e.printStackTrace();
            }
            
            }
        
        
    }
    
       return inserted;    
    
 }
    
    @Override
    public QueryResult.ResultQ1[] functionQ1(){
        ResultSet rs;
        
        ArrayList<QueryResult.ResultQ1> resultlist = new ArrayList<QueryResult.ResultQ1>();
        
        String query = "select distinct B.isbn, B.first_publish_year,B.page_count,P.publisher_name\n" +
                        "from book B,publisher P\n" +
                        "where B.publisher_id=P.publisher_id \n" +
                         "AND B.page_count = (select MAX(B2.page_count)\n" +
"					  from book B2)\n" +
                            "order by B.isbn;";
    
        try{
           Statement st = this.con.createStatement();
           rs = st.executeQuery(query);
           
           while(rs.next()){
               
               String isbn = rs.getString("isbn");
               String fpuby = rs.getString("first_publish_year");
               int pagecnt = rs.getInt("page_count");
               String pubname = rs.getString("publisher_name");
               
               
               QueryResult.ResultQ1 obj = new QueryResult.ResultQ1(isbn,fpuby,pagecnt,pubname);
               resultlist.add(obj);
           }
               st.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
        QueryResult.ResultQ1[] resarray =new QueryResult.ResultQ1[resultlist.size()];
        
        
        return resultlist.toArray(resarray);
        
        
        
    }
    
    @Override
    public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2){
        ResultSet rs;
        
        ArrayList<QueryResult.ResultQ2> resultlist = new ArrayList<QueryResult.ResultQ2>();
        
           
                
                
        try{
           
        PreparedStatement stmt =this.con.prepareStatement("select B.publisher_id, AVG(B.page_count) as pg_cnt \n" +
"from book B\n" +
"where B.publisher_id IN (select B1.publisher_id\n" +
"						from author_of AO1,author_of AO2,book B1\n" +
"						where AO1.isbn =AO2.isbn \n" +
"						AND AO1.author_id = ? \n" +
"                        AND AO2.author_id = ? \n" +
"                        And B1.isbn = AO1.isbn)\n" +
"\n" +
"GROUP BY B.publisher_id\n" +
"ORDER BY B.publisher_id;");
        
        stmt.setInt(1,author_id1);
        stmt.setInt(2,author_id2);
        
        rs = stmt.executeQuery();
           
           while(rs.next()){
               
               int publ_id = rs.getInt("publisher_id");
               double avg_page = rs.getDouble("pg_cnt");
               
               
               QueryResult.ResultQ2 obj = new QueryResult.ResultQ2(publ_id,avg_page);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
        
        QueryResult.ResultQ2[] resarray =new QueryResult.ResultQ2[resultlist.size()];
        
        
        return resultlist.toArray(resarray);
        
    }
    
   @Override
   public QueryResult.ResultQ3[] functionQ3(String author_name){
       ResultSet rs;
       
       ArrayList<QueryResult.ResultQ3> resultlist = new ArrayList<QueryResult.ResultQ3>();
       
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("SELECT B.book_name,B.category,B.first_publish_year\n" +
"FROM book B,author_of AO,author A\n" +
"WHERE A.author_name = ? \n" +
"AND A.author_id = AO.author_id\n" +
"AND AO.isbn = B.isbn\n" +
"AND B.first_publish_year = (SELECT min(B2.first_publish_year)\n" +
"							FROM book B2,author_of AO2\n" +
"							WHERE B2.isbn = AO2.isbn\n" +
"                            AND AO2.author_id = A.author_id) "
                + "ORDER BY B.book_name,B.category,B.first_publish_year;" );
        
        stmt.setString(1,author_name);
        
        
        rs = stmt.executeQuery();
           
           while(rs.next()){
               
               String bukname = rs.getString("book_name");
               String cate = rs.getString("category");
               String first_p = rs.getString("first_publish_year");
               
               
               
               QueryResult.ResultQ3 obj = new QueryResult.ResultQ3(bukname,cate,first_p);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       
       
       QueryResult.ResultQ3[] resarray =new QueryResult.ResultQ3[resultlist.size()]; 
        
       return resultlist.toArray(resarray);
   }
   
   @Override
   public QueryResult.ResultQ4[] functionQ4(){
       
       ResultSet rs;
       
       ArrayList<QueryResult.ResultQ4> resultlist = new ArrayList<QueryResult.ResultQ4>();
       
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("Select distinct temp.publisher_id,B.category \n" +
"FROM book B,(SELECT  P.publisher_id\n" +
"	FROM publisher P,book B\n" +
"	where P.publisher_id = B.publisher_id  \n" +
"    AND P.publisher_name LIKE \"%_ %_ %_\" \n" +
"	group by P.publisher_id\n" +
"	having AVG(B.rating) > 3.0 AND COUNT(*) > 3) as temp\n" +
"WHERE temp.publisher_id = B.publisher_id\n" +
"ORDER BY temp.publisher_id, B.category;");
        
        
        
        rs = stmt.executeQuery();
           
           while(rs.next()){
               
               int pid = rs.getInt("publisher_id");
               String cate = rs.getString("category");
               
               
               
               
               QueryResult.ResultQ4 obj = new QueryResult.ResultQ4(pid,cate);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       
       QueryResult.ResultQ4[] resarray =new QueryResult.ResultQ4[resultlist.size()]; 
        
       return resultlist.toArray(resarray);
       
   }
   
   @Override
   public QueryResult.ResultQ5[] functionQ5(int author_id){
       
       ResultSet rs;
       
       ArrayList<QueryResult.ResultQ5> resultlist = new ArrayList<QueryResult.ResultQ5>();
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("SELECT A.author_id,A.author_name\n" +
"FROM author A\n" +
"WHERE NOT EXISTS (SELECT P.publisher_id\n" +
"				  FROM publisher P,author_of AO,book B\n" +
"				  WHERE AO.isbn = B.isbn\n" +
"                  AND B.publisher_id = P.publisher_id\n" +
"                  AND AO.author_id = ? AND (P.publisher_id NOT in (SELECT P.publisher_id\n" +
"																  FROM publisher P2, author_of AO2, book B2\n" +
"                                                                  WHERE AO2.isbn =B2.isbn\n" +
"                                                                  AND B2.publisher_id = P.publisher_id\n" +
"                                                                  AND AO2.author_id =A.author_id)))\n" +
"order by author_id;\n" +
"                  ");
        
        stmt.setInt(1,author_id);
        
        
        rs = stmt.executeQuery();
           
           while(rs.next()){
               
               int autid = rs.getInt("author_id");
               String autname = rs.getString("author_name");
               
               
               
               QueryResult.ResultQ5 obj = new QueryResult.ResultQ5(autid,autname);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       QueryResult.ResultQ5[] resarray =new QueryResult.ResultQ5[resultlist.size()]; 
        
       return resultlist.toArray(resarray);
   }
    
    
    @Override
    public QueryResult.ResultQ6[] functionQ6(){
       ResultSet rs;
       
       ArrayList<QueryResult.ResultQ6> resultlist = new ArrayList<QueryResult.ResultQ6>();
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("SELECT A.author_id,B.isbn\n" +
"FROM author_of AO, book B,author A\n" +
"WHERE AO.isbn = B.isbn \n" +
"AND A.author_id= AO.author_id\n" +
"AND  not EXISTS (Select B1.publisher_id\n" +
"				FROM book B1,author_of AO1\n" +
"                WHERE AO1.isbn = B1.isbn\n" +
"                AND AO1.author_id <> AO.author_id\n" +
"                AND B.publisher_id = B1.publisher_id)\n" +
"                \n" +
"AND AO.author_id NOT IN (SELECT A3.author_id\n" +
"						FROM author_of AO3, book B3,author A3\n" +
"						WHERE AO3.isbn = B3.isbn \n" +
"                        AND A3.author_id=AO3.author_id\n" +
"						and exists(Select B4.publisher_id\n" +
"										FROM book B4,author_of AO4\n" +
"										WHERE AO4.isbn = B4.isbn\n" +
"										AND AO4.author_id <> AO3.author_id\n" +
"										AND B3.publisher_id = B4.publisher_id))\n" +
"\n" +
"order by A.author_id,B.isbn;");
        
        
        
        
        rs = stmt.executeQuery();
           
           while(rs.next()){
               
               int autid = rs.getInt("author_id");
               String isbnn = rs.getString("isbn");
               
               
               
               QueryResult.ResultQ6 obj = new QueryResult.ResultQ6(autid,isbnn);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       QueryResult.ResultQ6[] resarray =new QueryResult.ResultQ6[resultlist.size()]; 
        
       return resultlist.toArray(resarray);
    
    }

    @Override
    public QueryResult.ResultQ7[] functionQ7(double rating) {
        
        ResultSet rs;
       
       ArrayList<QueryResult.ResultQ7> resultlist = new ArrayList<QueryResult.ResultQ7>();
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("SELECT P.publisher_id,P.publisher_name\n" +
"FROM publisher P\n" +
"WHERE P.publisher_id in (\n" +
"	SELECT P2.publisher_id\n" +
"	FROM publisher P2,book B\n" +
"	WHERE P2.publisher_id =B.publisher_id\n" +
"	AND B.category = \"Roman\"\n" +
"	GROUP BY P2.publisher_id\n" +
"	HAVING ? < (SELECT AVG(B3.rating)\n" +
"				FROM book B3,publisher P3\n" +
"                where P3.publisher_id =B3.publisher_id\n" +
"                and P3.publisher_id = P2.publisher_id)\n" +
"    AND COUNT(*) > 1)\n" +
"ORDER BY P.publisher_id;");
        
        stmt.setDouble(1,rating);
        
        
        rs = stmt.executeQuery();
           
           while(rs.next()){
               
               int pub_id = rs.getInt("publisher_id");
               String pub_name = rs.getString("publisher_name");
               
               
               
               QueryResult.ResultQ7 obj = new QueryResult.ResultQ7(pub_id,pub_name);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       
       QueryResult.ResultQ7[] resarray =new QueryResult.ResultQ7[resultlist.size()]; 
        
       return resultlist.toArray(resarray);
        
    }

    @Override
    public QueryResult.ResultQ8[] functionQ8() {
        ResultSet rs,rs2;
       
       ArrayList<QueryResult.ResultQ8> resultlist = new ArrayList<QueryResult.ResultQ8>();
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("INSERT INTO phw1(isbn,book_name,rating)\n" +
"SELECT B.isbn,B.book_name,B.rating\n" +
"FROM book B,(SELECT B2.book_name,MIN(B2.rating) as min_rat\n" +
"			FROM book B2\n" +
"			GROUP BY B2.book_name\n" +
"            HAVING COUNT(*) > 1) as temp\n" +
"where B.book_name = temp.book_name\n" +
"AND B.rating = temp.min_rat");
        
        
        
        
        stmt.executeUpdate();
        
        PreparedStatement stmt2 = this.con.prepareStatement("SELECT isbn,book_name,rating FROM phw1;");
        
        rs2 = stmt2.executeQuery();
        
           
           while(rs2.next()){
               
               String isbnn = rs2.getString("isbn");
               String bookk = rs2.getString("book_name");
               double ratt = rs2.getDouble("rating");
               
               
               
               QueryResult.ResultQ8 obj = new QueryResult.ResultQ8(isbnn,bookk,ratt);
               resultlist.add(obj);
           }
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       
       QueryResult.ResultQ8[] resarray =new QueryResult.ResultQ8[resultlist.size()]; 
        
       return resultlist.toArray(resarray);
    }

    @Override
    public double functionQ9(String keyword) {
       ResultSet rs,rs2;
       
       double sumof=0;
       
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("UPDATE book B\n" +
"SET B.rating = B.rating + 1\n" +
"WHERE B.rating <= 4 AND B.book_name LIKE \"%"+keyword +"%\"");
     
        
        
        
        
        
        stmt.executeUpdate();
        
        PreparedStatement stmt2 = this.con.prepareStatement("SELECT SUM(B.rating) as sumof FROM book B");
        
        rs2 = stmt2.executeQuery();
        
           
            if(rs2.next()){
               
               sumof = rs2.getDouble("sumof");
               
               
               
               
           }
               
               
               
               
               
    
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       
       return sumof;
       }

    @Override
    public int function10() {
        ResultSet rs,rs2;
       
       int sumof=0;
       
       
       try{
           
        PreparedStatement stmt =this.con.prepareStatement("DELETE \n" +
"FROM publisher\n" +
"WHERE publisher_id NOT IN (SELECT B.publisher_id\n" +
"							FROM book B);");
     
        
        
        
        
        
        stmt.executeUpdate();
        
        PreparedStatement stmt2 = this.con.prepareStatement("SELECT count(*) as sumof \n" +
"FROM publisher");
        
        rs2 = stmt2.executeQuery();
        
           
            if(rs2.next()){
               
               sumof = rs2.getInt("sumof");
               
               
               
               
           }
               
               
               
               
               
    
               stmt.close();
           
        }
        catch(SQLException e){
            e.printStackTrace();
    }
       
       return sumof;
    }
    
    
   
    
 }
    
    
