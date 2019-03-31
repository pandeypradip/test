package oralclejdbctest;

import java.sql.*;
import java.util.*;


public class oralclejdbctest {
    
    static String URL = "jdbc:oracle:thin:@(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = dene1lx3)(PORT = 1521)))(CONNECT_DATA =(SERVICE_NAME = orcl.dene1lx3)))";
    static String USER = "jde";
    static String PASS = "jde";
    public oralclejdbctest() {
    }

    public static void main(String[] args) {
        try
                {
                        Class.forName("oracle.jdbc.driver.OracleDriver").newInstance();
                        long starttime1 = System.currentTimeMillis();
                        Connection conn = DriverManager.getConnection(URL,USER,PASS);
                        long endtime1 = System.currentTimeMillis();
                        long diff1 = endtime1 - starttime1;
                        System.out.println("DriverManager.getConnection took " + diff1 + " milliseconds.");
                        DatabaseMetaData dmd = conn.getMetaData();
                        System.out.println("DriverVersion:      ["+dmd.getDriverVersion()+"]");
                        System.out.println("DriverMajorVersion: ["+dmd.getDriverMajorVersion()+"]");
                        System.out.println("DriverMinorVersion: ["+dmd.getDriverMinorVersion()+"]");
                        System.out.println("DriverName:         ["+dmd.getDriverName()+"]");
                        System.out.println("URL:         ["+dmd.getURL()+"]");
                        System.out.println("UserName:         ["+dmd.getUserName()+"]");
                        System.out.println(dmd.getDatabaseProductName() );
                        System.out.println(dmd.getDatabaseProductVersion() );
                        //Properties props = System.getProperties();
                        //System.out.println( props.toString() );
                        Statement stmt = conn.createStatement();
                        long starttime2 = System.currentTimeMillis();
                        ResultSet rs = stmt.executeQuery("select * FROM SY812.F986101");
                        long endtime2 = System.currentTimeMillis();
                        long diff2 = endtime2 - starttime2;
                        System.out.println("stmt.executeQuery took " + diff2 + " milliseconds.");
                        if(rs != null) rs.close();
                        if(conn != null) conn.close();

                }
                catch (Exception e)
                {
                        e.printStackTrace();
                }
    }
}
