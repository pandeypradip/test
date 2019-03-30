import java.io.*;
import java.sql.*;
import java.util.Date;
import java.math.*;
import java.util.*;


public class OralcleJDBCTest {

//	static String URL = "jdbc:oracle:thin:@//denoscl38:1521/c1bkalle";

	static String URL = null,USER = null,PASS = null;


  public static void main(String args[]) throws Exception
  {

  	try
  	{
  		if(args.length != 4)
  		{
			System.out.println("Number of parameters passed not correct. progname <connect-url> <user> <pass> <schema>");
			return;
  		}

  		URL = args[0];
  		USER = args[1];
  		PASS = args[2];

		//Class.forName("com.microsoft.jdbc.sqlserver.SQLServerDriver");
		 Class.forName("oracle.jdbc.driver.OracleDriver");
                //DriverManager.registerDriver(new oracle.jdbc.driver.OracleDriver());
		Connection conn = DriverManager.getConnection(URL,USER,PASS);
		DatabaseMetaData dmd = conn.getMetaData();

		System.out.println("DriverVersion:      ["+dmd.getDriverVersion()+"]");
		System.out.println("DriverMajorVersion: ["+dmd.getDriverMajorVersion()+"]");
		System.out.println("DriverMinorVersion: ["+dmd.getDriverMinorVersion()+"]");
		System.out.println("DriverName:         ["+dmd.getDriverName()+"]");
		System.out.println("URL:         ["+dmd.getURL()+"]");
		System.out.println("UserName:         ["+dmd.getUserName()+"]");
		System.out.println(dmd.getDatabaseProductName() );
		System.out.println(dmd.getDatabaseProductVersion() );
		Properties props = System.getProperties();
		System.out.println( props.toString() );
		conn.close();

  	}
	catch (SQLException e)
   	{
	  	e.printStackTrace();
   	}
   }
}
