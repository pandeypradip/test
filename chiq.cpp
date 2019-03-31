#include <stdio.h>
#include <string.h>
#include <oci.h>
#include <time.h>



OCIEnv *envhp; /* Environment handle */
OCIError *errhp; /* Error handle */
OCIServer *srvhp; /* Server handle */
OCISvcCtx *svchp; /* Service context handle */
OCISession *usrhp; /* Session handle */
OCIStmt *stmthp; /* Statement handle */
   
OCIDefine *def1p = 0;
OCIDefine *def2p = 0;
OCIDefine *def3p = 0;
OCIDefine *def4p = 0;
OCIDefine *def5p = 0;
OCIDefine *def6p = 0;
OCIDefine *def7p = 0;
OCIDefine *def8p = 0;


/*text *selectstmt = (text *)"SELECT * FROM QA7334.F98741 ORDER BY ESEVSK ASC";*/
text *selectstmt = (text *)"SELECT ESEVSPEC, ESEVSEQ, ESJDEVERS, ESMRGMOD, ESMRGOPT, ESFFU1, ESFFU2, ESEVSK FROM PD812.F98741 ORDER BY ESEVSK ASC";

static          ub4 indsk[9] = {0}; 
static          ub4 rlsk[9]  = {0}; 
static          ub4 rcsk[9]  = {0}; 

static			sword status;

text errbuf[512];
ub4 buflen;
ub4 errcode;

#define BATCH_SIZE 50

struct demo 
{ 
	sword v_no1[15];
	sword v_no2[15];
	text v_str1[11];
	text v_avc1[2];
	text v_avc2[2];
	text v_avc3[2];
	text v_avc4[2];
	text v_str2[37];
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
   int  i =0; 
   int rowFetched=0;
   ub4 attrSize;
   int rowsTotal = 0;
   int previousRowsTotal = 0;
   int rowsFetched = 0;
   time_t     now;
   struct tm  *ts;
   char       buf[80];
		
   printf("Setting up environment and connecting...\n");

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
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp, (OCIDefine **)&def1p, (OCIError *)errhp, (ub4)1,(dvoid *)&demo_array[0].v_no1,(sword)sizeof(int), SQLT_INT, (dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp,(OCIDefine **)&def2p,(OCIError *)errhp,(ub4)2,(dvoid *)&demo_array[0].v_no2,(sword)sizeof(int), SQLT_INT, (dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp,(OCIDefine **)&def3p,(OCIError *)errhp,(ub4)3,(dvoid *)&demo_array[0].v_str1,(sb4)20,SQLT_STR,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp, (OCIDefine **)&def4p, (OCIError *)errhp, (ub4)4,(dvoid *)&demo_array[0].v_avc1,(sb4)20,SQLT_AVC,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp,(OCIDefine **)&def5p,(OCIError *)errhp,(ub4)5,(dvoid *)&demo_array[0].v_avc2,(sb4)20,SQLT_AVC,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp,(OCIDefine **)&def6p,(OCIError *)errhp,(ub4)6,(dvoid *)&demo_array[0].v_avc3,(sb4)20,SQLT_AVC,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp, (OCIDefine **)&def7p, (OCIError *)errhp, (ub4)7,(dvoid *)&demo_array[0].v_avc4,(sb4)20,SQLT_STR,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));
   checkerr(errhp, OCIDefineByPos((OCIStmt *)stmthp,(OCIDefine **)&def8p,(OCIError *)errhp,(ub4)8,(dvoid *)&demo_array[0].v_str2,(sb4)20,SQLT_STR,(dvoid *)0,(ub2 *)0,(ub2 *)0,(ub4)OCI_DEFAULT));

   checkerr(errhp, OCIDefineArrayOfStruct( def1p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[0], rlsk[0], rcsk[0])); 
   checkerr(errhp, OCIDefineArrayOfStruct( def2p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[1], rlsk[1], rcsk[1])); 
   checkerr(errhp, OCIDefineArrayOfStruct( def3p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[2], rlsk[2], rcsk[2])); 
   checkerr(errhp, OCIDefineArrayOfStruct( def4p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[3], rlsk[3], rcsk[3]));
   checkerr(errhp, OCIDefineArrayOfStruct( def5p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[4], rlsk[4], rcsk[4])); 
   checkerr(errhp, OCIDefineArrayOfStruct( def6p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[5], rlsk[5], rcsk[5]));
   checkerr(errhp, OCIDefineArrayOfStruct( def7p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[6], rlsk[6], rcsk[6])); 
   checkerr(errhp, OCIDefineArrayOfStruct( def8p, (OCIError *)errhp, sizeof(demo_array[0]), indsk[7], rlsk[7], rcsk[7]));
   
   printf("execute\n");
   checkerr(errhp, OCIStmtExecute((OCISvcCtx *)svchp,(OCIStmt *)stmthp,(OCIError *)errhp,(ub4)0,(ub4)0,(OCISnapshot *)NULL,(OCISnapshot *)NULL,(ub4)OCI_DEFAULT));
    
   printf("Fetch \n");
   time(&now);
   ts = localtime(&now);
   strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
   printf("%s\n", buf);

   do
   {
       /*checkerr(errhp, status = OCIStmtFetch2(stmthp, errhp, (ub4) BATCH_SIZE, (ub2) OCI_FETCH_NEXT, (sb4)0, OCI_DEFAULT));*/
	   status = OCIStmtFetch2(stmthp, errhp, (ub4) BATCH_SIZE, (ub2) OCI_FETCH_NEXT, (sb4)0, OCI_DEFAULT);
       OCIErrorGet ((dvoid *)errhp,(ub4)1,(text *)NULL,(sb4 *)&errcode,(text *)errbuf,(ub4)sizeof(errbuf),(ub4)OCI_HTYPE_ERROR);
	   if ((strncmp( (const char *)errbuf, "ORA-01406", 9) == 0) || (strncmp( (const char *)errbuf, "ORA-01455", 9) == 0))
       {
		   status = OCI_SUCCESS;
	   }
	   else
	   {
          printf("OCIStmtFetch2 : Error buffer - %s\n", errbuf);
          printf("OCIStmtFetch2 : Error code   - %d\n", errcode);
	   }
	   previousRowsTotal = rowsTotal;
       checkerr(errhp, OCIAttrGet(stmthp, OCI_HTYPE_STMT, &rowsTotal, &attrSize,  OCI_ATTR_ROW_COUNT, errhp));
       rowsFetched = rowsTotal-previousRowsTotal;
       printf("Fetch return value: %d, Rows Fetched: %d \n", status, rowsFetched);
		
	   for( i = 0; i < rowsFetched; i++) { 
          printf(" Rows returned = %d \n", ++rowFetched);
		  /*printf(" Rows returned for v_no1 %d \n", demo_array[i].v_no1); 
		  printf(" Rows returned for v_no2 %s \n", demo_array[i].v_no2); 
          printf(" Rows returned for v_str1 %d \n", demo_array[i].v_str1); 
		  printf(" Rows returned for v_avc1 %s \n", demo_array[i].v_avc1);
          printf(" Rows returned for v_avc2 %d \n", demo_array[i].v_avc2); 
		  printf(" Rows returned for v_avc3 %s \n", demo_array[i].v_avc3);
          printf(" Rows returned for v_avc4 %d \n", demo_array[i].v_avc4); 
		  printf(" Rows returned for v_str2 %s \n", demo_array[i].v_str2);*/
      }                
      printf("\n");
	}
	while (status == OCI_SUCCESS);
	printf(" After Fetch \n");
	time(&now);
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
    printf("%s\n", buf);
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
