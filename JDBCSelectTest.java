import java.sql.*;
import java.util.*;
import java.math.*;
import java.io.*;

public class JDBCSelectTest {
  public static void main (String[] args) {
    Connection connection=null;
    try {

        FileInputStream file = new FileInputStream("ink.properties");
        Properties properties = new Properties();
        properties.load(file);
        String driver = properties.getProperty("ink.driver");
        String url = properties.getProperty("ink.url");
        String login = properties.getProperty("ink.login");
        String password = properties.getProperty("ink.password");
        String owner = properties.getProperty("ink.owner");
        // For SQL Server database
		Class.forName(driver);
		System.out.println("url :" + url);
		System.out.println("login :" + login);
		System.out.println("password :" + password);
        connection = DriverManager.getConnection(url, login, password);

        DatabaseMetaData databaseMetaData = connection.getMetaData();
    	System.out.println(" Database Name =  " + databaseMetaData.getDatabaseProductName() );
        System.out.println(" Database Version =  " + databaseMetaData.getDatabaseProductVersion() );
        System.out.println(" Database Driver Name =  " + databaseMetaData.getDriverName() );
        System.out.println(" Database Driver Version =  " + databaseMetaData.getDriverVersion() );
        System.out.println(" Database Driver Major Version =  " + databaseMetaData.getDriverMajorVersion());
        System.out.println(" Database Driver Minor Version =  " + databaseMetaData.getDriverMinorVersion());
	    System.out.println(" Database URL =  " + databaseMetaData.getURL() );

        int resultSetType = ResultSet.TYPE_FORWARD_ONLY;
	    int resultSetConcurrency = ResultSet.CONCUR_READ_ONLY;

	String sqlStatement =  "SELECT GLASID,GLKCO,GLBC,GLSBL,GLSUB,GLPN,GLBRE,GLDOI,GLEXA,GLU,GLCO,GLAID,GLLT,GLEXTL,GLEXR," +
	                       "GLR2,GLJELN,GLALT5,GLICUT,GLALT3,GLPOST,GLICU,GLPO,GLDCT,GLUM,GLCRCD,GLAA,GLMCU,GLCTRY, " +
                               "GLANI,GLSBLT,GLFY,GLDOC,GLOBJ,GLDGJ FROM crpdta.F0911 " +
 	                       "WHERE ((GLAID = ? AND GLPOST = ?  AND GLLT = ?  AND GLALT3 = ?  AND GLBRE = ?  )) " +
	 	               " ORDER BY GLDCT ASC  , GLDOC ASC  , GLKCO ASC  , GLDGJ DESC  , GLJELN ASC  , GLLT ASC  , GLEXTL ASC";


         PreparedStatement ps = connection.prepareStatement(sqlStatement, resultSetType, resultSetConcurrency);

         ps.setString(1, "00854401");
         ps.setString(2, "P");
         ps.setString(3, "AA");
         ps.setString(4, " ");
         ps.setString(5, " ");

        System.out.println(" start SQL with 5 where clause :" + sqlStatement);

        long starttime = System.currentTimeMillis();
        ResultSet rs = ps.executeQuery();
        long endtime = System.currentTimeMillis();
        long diff = endtime - starttime;
        System.out.println("Time consumed: " + diff + " milliseconds.");

	String sqlStatement2 =  "SELECT GLASID,GLKCO,GLBC,GLSBL,GLSUB,GLPN,GLBRE,GLDOI,GLEXA,GLU,GLCO,GLAID,GLLT,GLEXTL,GLEXR," +
	                        "GLR2,GLJELN,GLALT5,GLICUT,GLALT3,GLPOST,GLICU,GLPO,GLDCT,GLUM,GLCRCD,GLAA,GLMCU,GLCTRY, " +
                                "GLANI,GLSBLT,GLFY,GLDOC,GLOBJ,GLDGJ FROM crpdta.F0911 " +
 	                        "WHERE ((GLALT3 = ?  AND GLBRE = ?  AND GLLT = ?  AND GLPOST = ? )) " +
	 	                " ORDER BY GLDCT ASC  , GLDOC ASC  , GLKCO ASC  , GLDGJ DESC  , GLJELN ASC  , GLLT ASC  , GLEXTL ASC";


         PreparedStatement ps2 = connection.prepareStatement(sqlStatement2, resultSetType, resultSetConcurrency);

         ps2.setString(1, " ");
         ps2.setString(2, " ");
         ps2.setString(3, "AA");
         ps2.setString(4, "P");

         System.out.println(" start SQL with 4 where clause :" + sqlStatement2);

        long starttime2 = System.currentTimeMillis();
        ResultSet rs2 = ps2.executeQuery();
        long endtime2 = System.currentTimeMillis();
        long diff2 = endtime2 - starttime2;
        System.out.println("Time consumed: " + diff2 + " milliseconds.");

        if (rs != null)
        rs.close();
        {
            rs.close();
        }
        if (ps != null)
        {
            ps.close();
        }

        if (rs2 != null)
        rs2.close();
        {
            rs2.close();
        }
        if (ps2 != null)
        {
            ps2.close();
        }

	    if (connection!=null)
        {
	      connection.close();
        }
    }
    catch(SQLException se){
	      se.printStackTrace();
	}
    catch (Exception e) {
      e.printStackTrace();
    }
  }
}