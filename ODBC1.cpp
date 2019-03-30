// ODBC1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

SQLHENV 		henv;
SQLHDBC 		hdbc;
SQLHSTMT		hstmt;
SQLRETURN		retcode;
SQLUSMALLINT    *pnRowStatusBuf;
SQLULEN         nNumRowsFetched;
unsigned long        nRowByteLen = 3700;   
unsigned long        nRowSetSize = 37;   


SQLCHAR sql[] = "SELECT  *  FROM CUSTOMER.JDE.F4111  WHERE  ( ILLITM = 'B-3995' )  ORDER BY ILUKID ASC";

void GetSQLError();

int main(int argc, char* argv[])
{
	SQLCHAR 		szStatus[10] = "1234";
	SQLINTEGER		cbStatus =	SQL_NTS;
	
	pnRowStatusBuf = new SQLUSMALLINT[6000];
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
		
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 
			
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
			{
				SQLCHAR buf[1024];
				SQLSMALLINT len;
				retcode = SQLDriverConnect(hdbc,NULL, (SQLCHAR*) "DSN=DENOSCL38 JAS CUST 2005;UID=JDE;PWD=JDE;", SQL_NTS,
					(SQLCHAR*)buf, 1024,&len,SQL_DRIVER_NOPROMPT);

				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
				{
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 

			        retcode  = SQLSetStmtAttr(hstmt
                               , SQL_SOPT_SS_CURSOR_OPTIONS, (SQLPOINTER)SQL_CO_FFO_AF
                               , SQL_IS_UINTEGER);
/*
			        retcode  = SQLSetStmtAttr(hstmt
                               , SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY
                               , SQL_IS_UINTEGER);
*/

					if(!SQL_SUCCEEDED(retcode))
						GetSQLError();

					retcode = SQLSetStmtAttr(hstmt
						, SQL_ATTR_ROW_STATUS_PTR
						, (SQLPOINTER)pnRowStatusBuf
						, SQL_IS_POINTER);
					if(!SQL_SUCCEEDED(retcode))
						GetSQLError();

					retcode = SQLSetStmtAttr(hstmt
						, SQL_ATTR_ROWS_FETCHED_PTR
						, (SQLPOINTER)&nNumRowsFetched
						, SQL_IS_POINTER);
					if(!SQL_SUCCEEDED(retcode))
						GetSQLError();

					retcode = SQLSetStmtAttr(hstmt
						, SQL_ATTR_ROW_ARRAY_SIZE
						, (SQLPOINTER)nRowSetSize
						, SQL_IS_UINTEGER);
					if(!SQL_SUCCEEDED(retcode))
						GetSQLError();

					retcode = SQLSetStmtAttr(hstmt
						, SQL_ROWSET_SIZE
						, (SQLPOINTER)nRowSetSize
						, SQL_IS_UINTEGER);
					if(!SQL_SUCCEEDED(retcode))
						GetSQLError();

					retcode = SQLSetStmtAttr(hstmt
						, SQL_BIND_TYPE
						, (SQLPOINTER)nRowByteLen
						, SQL_IS_UINTEGER);
					if(!SQL_SUCCEEDED(retcode))
						GetSQLError();

					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
					{
						if(!SQL_SUCCEEDED(SQLPrepare(hstmt,sql,SQL_NTS)))
						{
							GetSQLError();
						}

						if(!SQL_SUCCEEDED(SQLExecute(hstmt)))
						{
							GetSQLError();
						}

						while(SQL_SUCCEEDED(SQLFetchScroll(hstmt,SQL_FETCH_NEXT,37)))
						{
							printf("Fetched %d\n",nNumRowsFetched);
						}

						GetSQLError();

						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}
					
					SQLDisconnect(hdbc);
				}
				
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
			
		}
		
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
	
	return 0;
}

void GetSQLError()
{
long    llDbErrCd = 0;
short   llRetMsgLen = 0;
char    lszSqlErrMsg[255];
char    lszSqlMsg[255];

   SQLError(henv,
            hdbc,
            hstmt,
            (SQLCHAR*) lszSqlErrMsg,
            &llDbErrCd,
            (SQLCHAR*) lszSqlMsg,
            255,
            &llRetMsgLen);
	printf(lszSqlErrMsg);
	printf(lszSqlMsg);
}
