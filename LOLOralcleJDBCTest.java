import java.sql.*;
import java.util.*;


public class LOLOralcleJDBCTest {

    static String URL = "jdbc:oracle:thin:@tnsname";
    static String USER = "user";
    static String PASS = "pass";
    static String query1 = "SELECT * FROM PDDTA.F47036";
    static String query2 = "SELECT * FROM PDDTA.F4072";
    static String query3 = "SELECT * FROM PDDTA.F4101";

    public static void main(String[] args)
    {
        try
        {
            Class.forName("oracle.jdbc.driver.OracleDriver").newInstance();
            long startTime = System.currentTimeMillis();
            Connection conn = DriverManager.getConnection(URL,USER,PASS);
            long endTime = System.currentTimeMillis();
            long diff = endTime - startTime;
            System.out.println("DriverManager.getConnection took " + diff + " milliseconds.");
            DatabaseMetaData dmd = conn.getMetaData();
            System.out.println("DriverVersion:      ["+dmd.getDriverVersion()+"]");
            System.out.println("DriverMajorVersion: ["+dmd.getDriverMajorVersion()+"]");
            System.out.println("DriverMinorVersion: ["+dmd.getDriverMinorVersion()+"]");
            System.out.println("DriverName:         ["+dmd.getDriverName()+"]");
            System.out.println("URL:         ["+dmd.getURL()+"]");
            System.out.println("UserName:         ["+dmd.getUserName()+"]");
            System.out.println(dmd.getDatabaseProductName() );
            System.out.println(dmd.getDatabaseProductVersion() );
            Statement stmt = conn.createStatement();
            /*runQuery(stmt, query0);*/
            runQuery(stmt, query1);
            runQuery(stmt, query2);
            runQuery(stmt, query3);
            if(stmt != null) stmt.close();
            if(conn != null) conn.close();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    public static void runQuery(Statement stmt, String query)
    {
        try
        {
            long startTime = System.currentTimeMillis();
	        ResultSet rs = stmt.executeQuery(query);
	        long endTime = System.currentTimeMillis();
	        long diff = endTime - startTime;
            System.out.println("query <" + query + ">  took " + diff + " milliseconds.");
            if(rs != null) rs.close();
        }
        catch (Exception e)
        {
                e.printStackTrace();
        }
    }
}
