#include <stdio.h>
#include <string.h>
#include <oci.h>


OCIEnv *envhp; /* Environment handle */
OCIError *errhp; /* Error handle */
OCIServer *srvhp; /* Server handle */
OCISvcCtx *svchp; /* Service context handle */
OCISession *usrhp; /* Session handle */
OCIStmt *stmthp; /* Statement handle */
    
OCIBind *bnd1p = 0;
OCIBind *bnd2p = 0;
OCIBind *bnd3p = 0;
OCIBind *bnd4p = 0;
OCIBind *bnd5p = 0;
OCIBind *bnd6p = 0;

OCIBind *bnd7p = 0;
OCIBind *bnd8p = 0;
OCIBind *bnd9p = 0;
OCIBind *bnd10p = 0;
OCIBind *bnd11p = 0;
OCIBind *bnd12p = 0;


    
OCIDefine *def1p = 0;
OCIDefine *def2p = 0;


text *selectstmt = (text *)"SELECT SDKCOO, SDDOCO FROM TESTDTA.F4211 WHERE  "
						   "( SDLTTR >= :C1 AND SDLTTR <= :C2 AND SDAN8 = :N1 AND SDNXTR = :C3 AND SDMCU "
						   "LIKE :C4 AND SDSO15 <> :C5 )  UNION SELECT SDKCOO, SDDOCO "
						   "FROM TESTDTA.F42119 WHERE  ( SDLTTR >= :C6 AND SDLTTR <= :C7 AND "
						   "SDAN8 = :N2 AND SDNXTR = :C8 AND SDMCU LIKE :C9 AND SDSO15 <> :C10 )"
						   "ORDER BY 1 ASC , 2 ASC";

static          ub4 indsk[2] = {0,0}; 
static          ub4 rlsk[2]  = {0,0}; 
static          ub4 rcsk[2]  = {0,0}; 


static			sword status;

text errbuf[512];
ub4 buflen;
ub4 errcode;

#define BATCH_SIZE 50

struct demo 
{ 
	sword v_no;
	text v_str[20];
}; 


typedef struct demo demoArray; 

demoArray demo_array[BATCH_SIZE]; 

static void checkerr(OCIError *errhp, sword status)
{
    switch (status)
		
    {
    case OCI_SUCCESS:
		break;
		
    case OCI_SUCCESS_WITH_INFO:
		printf("Error - OCI_SUCCESS_WITH_INFO\n");
		break;
		
    case OCI_NEED_DATA:
		printf("Error - OCI_NEED_DATA\n");
		break;
		
    case OCI_NO_DATA:
		printf("In check Error - OCI_NO_DATA\n");
		break;
		
    case OCI_ERROR:
		OCIErrorGet ((dvoid *)errhp,(ub4)1,(text *)NULL,(sb4 *)&errcode,(text *)errbuf,(ub4)sizeof(errbuf),(ub4)OCI_HTYPE_ERROR);
		printf("Error - %s\n", errbuf);
		break;
		
    case OCI_INVALID_HANDLE:
		printf("Error - OCI_INVALID_HANDLE\n");
		break;
		
    case OCI_STILL_EXECUTING:
		printf("Error - OCI_STILL_EXECUTING\n");
		break;
		
    case OCI_CONTINUE:
		printf("Error - OCI_CONTINUE\n");
		break;
		
    default:
		break;
		
    }
    
}

int main(int argc,char *argv[])
{
  
/*	text *dbname = (text *)"jdenaq85.rar.ncsus.jnj.com";
    text *username = (text *)"dba_backup";
    text *password = (text *)"shepard_2";

	text *dbname = (text *)"c1bkalle";
    text *username = (text *)"JDE";
    text *password = (text *)"JDE";
*/

    text *c1 = (text *) "520";
    text *c2 = (text *) "999";
	sword n1 = 60005.000000;
	text *c3 = (text *) "560";
	text *c4 = (text *) "%USNAL%";
	text *c5 = (text *) "1";

    
    ub4  i =0;
    ub4 attrSize;
    int rowsTotal = 0;
	int previousRowsTotal = 0;
	int rowsFetched = 0;
	
	
    printf("Setting up environment and connecting...\n");

/*
    OCIInitialize((ub4)OCI_OBJECT,(dvoid *)0,(dvoid * (*)(void *,unsigned int))0,(dvoid * (*)(void *,void *,unsigned int))0,(void (*)(void *,void *))0);
    
    OCIEnvInit((OCIEnv **)&envhp,(ub4)OCI_DEFAULT,(size_t)0,(dvoid **)0);
*/
    if(OCIEnvCreate((OCIEnv **) &envhp,(ub4)OCI_THREADED, (dvoid *)0,(dvoid * (*)(dvoid *, size_t)) 0,(dvoid * (*)(dvoid *, dvoid *, size_t))0,(void (*)(dvoid *, dvoid *)) 0,(size_t) 0, (dvoid **) 0 ))
		printf("FAILED: OCIEnvCreate()\n");
  
    OCIHandleAlloc((dvoid *)envhp,(dvoid **)&errhp,(ub4)OCI_HTYPE_ERROR,(size_t)0,(dvoid **)0);
    OCIHandleAlloc((dvoid *)envhp,(dvoid **)&srvhp,(ub4)OCI_HTYPE_SERVER, (size_t)0,(dvoid **)0);
    OCIHandleAlloc((dvoid *)envhp,(dvoid **)&svchp,(ub4)OCI_HTYPE_SVCCTX,(size_t)0,(dvoid **)0);
    OCIHandleAlloc((dvoid *)envhp,(dvoid **)&usrhp,(ub4)OCI_HTYPE_SESSION,(size_t)0,(dvoid **)0);
    checkerr(errhp,OCIServerAttach((OCIServer *)srvhp,(OCIError *)errhp,(text *)argv[1],(sb4)strlen((char *)argv[1])+1,(ub4)OCI_DEFAULT));
    OCIAttrSet((dvoid *)svchp,(ub4)OCI_HTYPE_SVCCTX,(dvoid *)srvhp,(ub4)0,(ub4)OCI_ATTR_SERVER,(OCIError *)errhp);
    OCIAttrSet((dvoid *)usrhp,(ub4)OCI_HTYPE_SESSION,(dvoid *)argv[2],(ub4)strlen((const char *)argv[2]),(ub4)OCI_ATTR_USERNAME,(OCIError *)errhp);
    OCIAttrSet((dvoid *)usrhp,(ub4)OCI_HTYPE_SESSION,(dvoid *)argv[3],(ub4)strlen((const char *)argv[3]),(ub4)OCI_ATTR_PASSWORD,(OCIError *)errhp);
    checkerr(errhp,OCISessionBegin((OCISvcCtx *)svchp,(OCIError *)errhp,(OCISession *)usrhp, OCI_CRED_RDBMS,OCI_DEFAULT));
    OCIAttrSet((dvoid *)svchp,(ub4)OCI_HTYPE_SVCCTX, (dvoid *)usrhp,(ub4)0, (ub4)OCI_ATTR_SESSION,(OCIError *)errhp);
    
    printf("Set up done. Connected.\n");
    
    checkerr(errhp, OCIHandleAlloc((dvoid *)envhp, (dvoid **)&stmthp, (ub4)OCI_HTYPE_STMT, (size_t)0,(dvoid **)0));
    checkerr(errhp,OCIStmtPrepare((OCIStmt *)stmthp, (OCIError *)errhp,(CONST text*)selectstmt,(ub4)strlen((char*)selectstmt),(ub4)OCI_NTV_SYNTAX,(ub4)OCI_DEFAULT));
    
    /* Define the output variables for the select list items */
    checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp, (OCIDefine **)&def1p, (OCIError *)errhp, (ub4)2,(dvoid *)&demo_array[0].v_no,(sword)sizeof(int), SQLT_INT, (dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp,(OCIDefine **)&def2p,(OCIError *)errhp,(ub4)1,(dvoid *)&demo_array[0].v_str,(sb4)20,SQLT_STR,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));

	checkerr(errhp, OCIDefineArrayOfStruct( def1p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[0], rlsk[0], rcsk[0])); 
	checkerr(errhp, OCIDefineArrayOfStruct( def2p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[1], rlsk[1], rcsk[1])); 

	
    /* Bind the input variable for the select statement */
    printf("binding\n");

    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd1p,(OCIError *)errhp,(ub4)1,(dvoid *)c1,(sb4)strlen((char *)c1)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd2p,(OCIError *)errhp,(ub4)2,(dvoid  *)c2,(sb4)strlen((char *)c2)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd3p,(OCIError *)errhp,(ub4)3,(dvoid *)&n1,sizeof(sword),SQLT_INT,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd4p,(OCIError *)errhp,(ub4)4,(dvoid *)c3,(sb4)strlen((char *)c3)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd5p,(OCIError *)errhp,(ub4)5,(dvoid  *)c4,(sb4)strlen((char *)c4)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd6p,(OCIError *)errhp,(ub4)6,(dvoid  *)c5,(sb4)strlen((char *)c5)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));

    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd7p,(OCIError *)errhp,(ub4)7,(dvoid *)c1,(sb4)strlen((char *)c1)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd8p,(OCIError *)errhp,(ub4)8,(dvoid  *)c2,(sb4)strlen((char *)c2)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd9p,(OCIError *)errhp,(ub4)9,(dvoid *)&n1,sizeof(sword),SQLT_INT,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd10p,(OCIError *)errhp,(ub4)10,(dvoid *)c3,(sb4)strlen((char *)c3)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd11p,(OCIError *)errhp,(ub4)11,(dvoid  *)c4,(sb4)strlen((char *)c4)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));
    checkerr(errhp, OCIBindByPos((OCIStmt *)stmthp,(OCIBind **)&bnd12p,(OCIError *)errhp,(ub4)12,(dvoid  *)c5,(sb4)strlen((char *)c5)+1,SQLT_STR,(ub2 *)0,(ub2 *)0,(ub2 *)0,(ub4)0,(ub4 *)0,(ub4)OCI_DEFAULT));

    
    printf("execute\n");
    checkerr(errhp, OCIStmtExecute((OCISvcCtx *)svchp,(OCIStmt *)stmthp,(OCIError *)errhp,(ub4)0,(ub4)0,(OCISnapshot *)NULL,(OCISnapshot *)NULL,(ub4)OCI_DEFAULT));
    
	printf("Fetch \n");
	
	do
	{
        checkerr(errhp, status = OCIStmtFetch(stmthp, errhp, (ub4) BATCH_SIZE, (ub2) OCI_FETCH_NEXT, OCI_DEFAULT));
		
		previousRowsTotal = rowsTotal;
        checkerr(errhp, OCIAttrGet(stmthp, OCI_HTYPE_STMT, &rowsTotal, &attrSize,  OCI_ATTR_ROW_COUNT, errhp));
        rowsFetched = rowsTotal-previousRowsTotal;
		printf("Fetch return value: %d, Rows Fetched: %d \n", status, rowsFetched);
		
		for( i = 0; i < rowsFetched; i++) { 
			printf(" Rows returned for v_no %d \n", demo_array[i].v_no); 
			printf(" Rows returned for v_str %s \n", demo_array[i].v_str); 
		}                
		printf("\n");
	}
	while (status == OCI_SUCCESS);
    
	printf(" After Fetch \n");
	OCIErrorGet ((dvoid *)errhp,(ub4)1,(text *)NULL,(sb4 *)&errcode,(text *)errbuf,(ub4)sizeof(errbuf),(ub4)OCI_HTYPE_ERROR);
    printf("Error buffer - %s\n", errbuf);
    printf("Error code   - %d\n", errcode);
	printf("\n");
	printf("********* Done ********* \n");
    checkerr(errhp,OCISessionEnd((OCISvcCtx *)svchp,(OCIError *)errhp,(OCISession *)usrhp,(ub4)OCI_DEFAULT));
    
    checkerr(errhp,OCIServerDetach((OCIServer *)srvhp,(OCIError *)errhp,(ub4)OCI_DEFAULT));
    
    OCIHandleFree((dvoid *)srvhp,(ub4)OCI_HTYPE_SERVER);
    OCIHandleFree((dvoid *)svchp,(ub4)OCI_HTYPE_SVCCTX);
    OCIHandleFree((dvoid *)usrhp,(ub4)OCI_HTYPE_SESSION);
    OCIHandleFree((dvoid *)errhp,(ub4)OCI_HTYPE_ERROR);
    OCIHandleFree((dvoid *)envhp,(ub4)OCI_HTYPE_ENV);

	return 0;
    
}
