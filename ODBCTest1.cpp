// ODBCTestCase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <sqlext.h>
#include <sql.h> 
#include <odbcss.h>
#include <stdlib.h>
#include <stdio.h>


//SQLCHAR sql[] = "SELECT  ILITM  FROM testdb.dbo.F4111  WHERE  ( ILLITM = 'B-3995' )  ORDER BY ILUKID ASC";
SQLCHAR sql[] = "SELECT  ILITM  FROM CUSTOMER.JDE.F4111  WHERE  ( ILLITM = 'B-3995' )  ORDER BY ILUKID ASC";

void GetSQLError();
long InitializeEnvironment();
long Connect();
long Disconnect();
int  ExecuteProcedure();
int  is_timeout(SQLHANDLE handle,SQLSMALLINT type);

SQLHENV     ghEnvironment = NULL;
SQLHDBC     ghConnection  = NULL;
HSTMT       ghStatement   = NULL;


int main(int argc, char* argv[])
{
	if(InitializeEnvironment() == 0)
	{
		if(Connect() == 0)
		{
			ExecuteProcedure();
			Disconnect();
		}

		SQLFreeHandle(SQL_HANDLE_ENV, ghEnvironment);
	}
	
	return 0;
}


/**********************************************
 * InitializeEnvironment
 **********************************************/
long InitializeEnvironment()
{
	
	if (!SQL_SUCCEEDED(SQLSetEnvAttr(NULL,
									 SQL_ATTR_CONNECTION_POOLING,
									 (SQLPOINTER)SQL_CP_ONE_PER_DRIVER,
									 SQL_IS_INTEGER)))
	{
		GetSQLError();
		return 8;
	}
	

	if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, NULL, &ghEnvironment)))
	{
		GetSQLError();
		return 8;
	}
	if(!SQL_SUCCEEDED(SQLSetEnvAttr(ghEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC2, SQL_IS_INTEGER)))
	{
		GetSQLError();
		return 8;
	}
	if (!SQL_SUCCEEDED(SQLSetEnvAttr(ghEnvironment, 
									 SQL_ATTR_CP_MATCH,
									 (SQLPOINTER) SQL_CP_STRICT_MATCH , 
									 //(SQLPOINTER) SQL_CP_RELAXED_MATCH , 
									 SQL_IS_INTEGER)))
	{
		GetSQLError();
		return 8;
	}

	return 0;   
}
/**********************************************
 * Connect
 **********************************************/
long Connect()
{
	SQLCHAR     lszOutConnectString[1024];
	SQLSMALLINT llReturnLength;
	SQLAllocHandle(SQL_HANDLE_DBC, ghEnvironment, &ghConnection);

	if(!SQL_SUCCEEDED(SQLDriverConnect(ghConnection, 
		NULL,
		(SQLCHAR*)"DSN=DENOSCL38 JAS;UID=JDE;PWD=JDE",
//		(SQLCHAR*)"DSN=DENOSPRO1 2005;UID=JDE;PWD=JDE;",
		SQL_NTS,
		lszOutConnectString,
		1024,
		&llReturnLength,
		SQL_DRIVER_NOPROMPT)))
	{
		GetSQLError();
		return 8;
	}

	SQLAllocStmt(ghConnection, &ghStatement);

	return 0;
}
/**********************************************
 * Disconnect
 **********************************************/
long Disconnect()
{
	if(ghStatement)
	{
		if(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, ghStatement)))
		{
			GetSQLError();
			return 8;
		}
		ghStatement = NULL;
	}
 
	if(ghConnection)
	{
		::SQLDisconnect(ghConnection);

		if(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_DBC, ghConnection)))
		{
			GetSQLError();
			return 8;
		}
		ghConnection = NULL;
	}
	return 0;
}
/**********************************************
 * ExecuteProcedure
 **********************************************/
int ExecuteProcedure()
{
	RETCODE llDbRetCd;
	int count = 0;
	SQLUSMALLINT   RowStatusArray[30];
	SQLUINTEGER    NumRowsFetched = 0;

	SQLINTEGER  mlIndicator = 0; 
	SQLRETURN   lnSqlRetCd = SQL_SUCCESS; 
	::SQLFreeStmt(ghStatement, SQL_CLOSE);
	::SQLFreeStmt(ghStatement, SQL_UNBIND);

//	llDbRetCd = SQLSetStmtAttr(ghStatement,SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, SQL_IS_UINTEGER);
	llDbRetCd = SQLSetStmtAttr(ghStatement,SQL_SOPT_SS_CURSOR_OPTIONS, (SQLPOINTER)SQL_CO_FFO_AF, SQL_IS_UINTEGER);

//	SQLSetStmtAttr(ghStatement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)20, 0);
	SQLSetStmtAttr(ghStatement, SQL_ATTR_ROW_ARRAY_SIZE,(SQLPOINTER)30, 0);
	SQLSetStmtAttr(ghStatement, SQL_ATTR_ROW_STATUS_PTR, RowStatusArray, 0);
	SQLSetStmtAttr(ghStatement, SQL_ATTR_ROWS_FETCHED_PTR, &NumRowsFetched, 0);
    SQLSetStmtAttr(ghStatement, SQL_ROWSET_SIZE,(SQLPOINTER)30,0);
    SQLSetStmtAttr(ghStatement, SQL_BIND_TYPE,(SQLPOINTER)20,0);

	/*****************************************************************/
	/* Bind return Value
	/*****************************************************************/
	char lszReturnBuf[30][20];
	SDWORD lSts;

	llDbRetCd = SQLBindCol(ghStatement, 1, SQL_C_TCHAR, &lszReturnBuf, 20, &lSts);

	if ((llDbRetCd != SQL_SUCCESS) && (llDbRetCd != SQL_SUCCESS_WITH_INFO))
	{
		GetSQLError();
		return 8;
	}

	/*****************************************************************/
	/* Prepare Procedure
	/*****************************************************************/
	llDbRetCd = SQLPrepare(ghStatement, (SQLCHAR*)sql, SQL_NTS);
	if((llDbRetCd != SQL_SUCCESS) && (llDbRetCd != SQL_SUCCESS_WITH_INFO))
	{
		GetSQLError();
		return 8;
	}

	/*****************************************************************/
	/* Execute Procedure
	/*****************************************************************/
	SQLSetStmtAttr(ghStatement, SQL_QUERY_TIMEOUT, (SQLPOINTER)1, SQL_IS_UINTEGER);
	while(1)
	{
		llDbRetCd = SQLExecute(ghStatement);
		if(llDbRetCd == SQL_SUCCESS || llDbRetCd == SQL_NO_DATA)
			break;

		if(is_timeout(ghStatement,SQL_HANDLE_STMT))
		{
			printf("NumRowFetched %d\n",NumRowsFetched);
			SQLCloseCursor(ghStatement);
			Sleep(500);
			continue;
		}
		break;
	}

	SQLSetStmtAttr(ghStatement, SQL_QUERY_TIMEOUT, (SQLPOINTER)0, SQL_IS_UINTEGER);
	/*****************************************************************/
	/* Fetch Result
	/*****************************************************************/
	do
	{
		for (int i = 0; i < NumRowsFetched; i++) 
		{
			if (RowStatusArray[i] == SQL_ROW_SUCCESS || RowStatusArray[i] == SQL_ROW_SUCCESS_WITH_INFO) 
			{
				printf("Value# %d =  %s\n",++count,lszReturnBuf[i]);
			}
			else
			{
				printf("Row %d is invalid\n",i);
			}
		}

		NumRowsFetched = 0;
	}
	while((llDbRetCd = SQLFetchScroll(ghStatement,SQL_FETCH_NEXT,0))!=  SQL_NO_DATA);

	if ((llDbRetCd != SQL_SUCCESS) && (llDbRetCd != SQL_NO_DATA))
	{
		GetSQLError();
		return 8;
	}

	return 0;
}

/**********************************************
 * GetSQLError
 **********************************************/
void GetSQLError()
{
long    llDbErrCd = 0;
short   llRetMsgLen = 0;
char    lszSqlErrMsg[255];
char    lszSqlMsg[255];

   SQLError(ghEnvironment,
            ghConnection,
            ghStatement,
            (SQLCHAR*) lszSqlErrMsg,
            &llDbErrCd,
            (SQLCHAR*) lszSqlMsg,
            255,
            &llRetMsgLen);
	printf(lszSqlErrMsg);
	printf(lszSqlMsg);
}

int is_timeout(SQLHANDLE handle,SQLSMALLINT type)
{
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT len;
    SQLRETURN ret;

     do
    {
      ret = SQLGetDiagRec(type, handle, ++i, state, &native, text,
      sizeof(text), &len );
      if (SQL_SUCCEEDED(ret))
      printf("%s:%ld:%ld:%s\n", state, i, native, text);
    }
    while( ret == SQL_SUCCESS );

	if(stricmp("S1T00",(const char*)state) == 0 || stricmp("HYT00",(const char*)state) == 0)
		return 1;

	return 0;
}
