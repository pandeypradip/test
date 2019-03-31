#include <xdatesthdr.h> 
#include <string.h> 
#include <sqludf.h> 

#define SQL_CLOBLOC             964 

int main(int argc, char *argv[]) 
{ 
 char varname [10] = "XDALOB12 ";    /* variation name           */ 
 char passfail [2] = "F";            /* pass of fail of variation*/ 
 char errmsg  [81] = " ";            /* error message for fail   */ 
 char testname[10] = "XDALOBS  ";    /* test case name           */ 
 char lib     [11] = "XDATESTS  ";   /* collection for report    */ 
 char file    [11] = "XDARESULTS";   /* table for reporting      */ 
 short int c1;                       /* c1 & c2 are used to      */ 
 short int c2;                       /* substr lib & file fields */ 

 char expcode  [8]; 
 char msginf  [81]; 
 char recode   [8]; 
 int int101 = 101; 
 struct extdvar *extdaptr; 

 short fail = 0; 
 int   cmdlen; 
 int   stmtlen; 
 long datalenary[6]; 
 short indicators[6]; 
 char SQLStmnt[4096]; 
 char                sqld2area[6][50000]; 

  /*initialization */ 
 c1 = strlen(lib); 
 c2 = strlen(file); 
 strcpy(expcode, "0      "); 
 strcpy(recode, "0"); 
 strcpy(msginf, " "); 


 datalenary[0] = 4096; 
 datalenary[1] = 1024; 
 datalenary[2] = 14336; 
 datalenary[3] = 64; 
 datalenary[4] = 4; 
 datalenary[5] = 23552; 

 INIT; 

/*-----------------------------------------------------------------*/ 
/* Connect to system                                               */ 
/*-----------------------------------------------------------------*/ 
 QxdaConnectEDRS(istr, "CDBI0100", &ostr, &ostrsize, 
                 "CDBO0100", &err); 
 if (err.ec.Bytes_Available != 0 || ostr.Connection_Handle <= 0) { 
   strcpy(recode, err.ec.Exception_Id); 
   strcpy(msginf, "      Connect failed  "); 
   strcat(msginf, err.Exception_Data); 
   fail = 1; 
 } 

/*-----------------------------------------------------------------*/ 
/* Create table                                                    */ 
/*-----------------------------------------------------------------*/ 

/* Col 1 - 4K CLOB */ 
/* Col 2 - 1K BLOB */ 
/* Col 3 - 14K NULL CAP DBCLOB */ 
/* Col 4 - 64 CHAR */ 
/* Col 5 - INT  */ 
/* Col 6 - 23K NULL CAP DBCLOB */ 

 strcpy(SQLStmnt,"CREATE TABLE XDALOBS/LOBTAB12 ("); 
 strcat(SQLStmnt,"COL1 CLOB(4 K) NOT NULL,"); 
 strcat(SQLStmnt,"COL2 BLOB(1 K) NOT NULL,"); 
 strcat(SQLStmnt,"COL3 DBCLOB(14 K),"); 
 strcat(SQLStmnt,"COL4 CHAR(64),"); 
 strcat(SQLStmnt,"COL5 INT,"); 
 strcat(SQLStmnt,"COL6 BLOB(23 K) ) \n"); 
 stmtlen = strlen(SQLStmnt) -1; 
 QxdaProcessImmediateEDRS(&ostr.Connection_Handle, 
                          SQLStmnt, &stmtlen, &myca, &err); 
 if (err.ec.Bytes_Available != 0 && fail == 0) { 
   strcpy(recode, err.ec.Exception_Id);             /*send msg. only if*/ 
   strcpy(msginf, "      Process Immed. failed  "); /*Connect worked   */ 
   strcat(msginf, err.Exception_Data); 
   fail = 1; 
 } 
 else 
    /* -601 means table already exists, thats ok... */ 
   if (myca.sqlcode < 0 && myca.sqlcode != -601 && fail == 0)  { 
     sprintf(recode, "%7d", myca.sqlcode); 
     strcpy(msginf, "      Create table failed  "); 
     fail = 1; 
   } 

 /*-----------------------------------------------------------------*/ 
 /* Insert data                                                     */ 
 /*-----------------------------------------------------------------*/ 
 qsq.Function = '2';                                       /* Prepare   */ 
 memcpy(qsq.SQL_Package_Name,"XDALOB12  ",10); 
 memcpy(qsq.Library_Name,"XDALOBS   ",10); 
 memcpy(qsq.Main_Pgm,"          ",10); 
 memcpy(qsq.Main_Lib,"          ",10); 
 memcpy(qsq.Statement_Name,"                  ",18); 
 memcpy(qsq.Cursor_Name,"CSRXDALOB12       ",18); 
 qsq.Open_Options = 0x80; 
 memcpy(qsq.Date_Format,"USA",3); 
 qsq.Date_Separator = '/'; 
 memcpy(qsq.Time_Format,"USA",3); 
 qsq.Time_Separator  = ':'; 
 memcpy(qsq.Naming_Option,"SYS",3); 
 qsq.Decimal_Point = '.'; 
 qsq.Commitment_Control   = 'C'; 

 inda2.sqld     = 0; 
 inda2.sqln     = 12; 
 inda2.sqldabc  = inda2.sqln * 80 + 16; 


 strcpy(SQLStmnt,"INSERT INTO XDALOBS/LOBTAB12 VALUES ( "); 
 strcat(SQLStmnt,  "'clob not null',"); 
 strcat(SQLStmnt, "BLOB('blob not null'),"); 
 strcat(SQLStmnt, "DEFAULT,"); 
 strcat(SQLStmnt, "'char null capable',"); 
 strcat(SQLStmnt, "101,"); 
 strcat(SQLStmnt, "BLOB('blob null capable') ) \n"); 

 qsq.Statement_Length  = strlen(SQLStmnt) - 1; 
 memcpy(qsq.Statement_Text, SQLStmnt,  qsq.Statement_Length); 

 exdopt = QXDA_CREATE_OBJECTS | QXDA_FIND_STMT; 

 QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &inda2, &myca, "SQLP0300", &qsq, 
                     &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strncpy(recode, err.ec.Exception_Id,7); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Insert prepared stmt. failed  "); 
   fail = 1; 
 } 

qsq.Function = '3';                                    /* Execute   */ 
memcpy(qsq.Statement_Name, exd_rvar.Statement_Name, 18); 
memcpy(qsq.Cursor_Name,"CSRXDALOB12       ",18); 

exdopt = QXDA_EXTDYN_NOOPTS; 

QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &inda2, &myca, "SQLP0300", &qsq, 
                     &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strcpy(recode, err.ec.Exception_Id); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Execute of insert failed  "); 
   fail = 1; 
 } 

 /*-----------------------------------------------------------------*/ 
 /* Select                                                          */ 
 /*-----------------------------------------------------------------*/ 
qsq.Function = '2';                                  /* prepare */ 
qsq.Blocking_Factor = 1; 

inda2.sqld     = 0; 
inda2.sqln     = 12; 
inda2.sqldabc  = 16 + (outda2.sqln * sizeof( outda2.unn[0].sqlvar));; 

strcpy(SQLStmnt,"SELECT COL4, COL2, COL3, COL1, "); 
strcat(SQLStmnt," COL5, COL6 FROM XDALOBS/LOBTAB12 \n"); 

qsq.Statement_Length  = strlen(SQLStmnt) - 1; 
memcpy(qsq.Statement_Text, SQLStmnt, qsq.Statement_Length); 

exdopt = QXDA_EXTDYN_NOOPTS; 

QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &inda2, &myca, "SQLP0300", &qsq, 
                     &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strncpy(recode, err.ec.Exception_Id,7); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Select prepared stmt. failed  "); 
   fail = 1; 
 } 

qsq.Function = '7';                                    /* Describe   */ 
qsq.Clause_For_Describe = 'N'; 

exdopt = QXDA_EXTDYN_NOOPTS; 

QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &inda2, &myca, "SQLP0300", &qsq, 
                     &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strcpy(recode, err.ec.Exception_Id); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Execute of insert failed  "); 
   fail = 1; 
 } 
 else { 
   if (fail == 0) { 
     if ( (inda2.sqld != 6) || 
          (inda2.unn[0].sqlvar.sqltype != 408) || 
          (inda2.unn[1].sqlvar.sqltype != 404) || 
          (inda2.unn[2].sqlvar.sqltype != 413) || 
          (inda2.unn[3].sqlvar.sqltype != 453) || 
          (inda2.unn[4].sqlvar.sqltype != 497) || 
          (inda2.unn[5].sqlvar.sqltype != 405) || 
          (inda2.unn[3].sqlvar.sqllen != 64) || 
          (inda2.unn[4].sqlvar.sqllen != 4) || 
          (memcmp(&(inda2.unn[6].sqlvar.sqltype), &(datalenary[0]), 4) != 0) || 
          (memcmp(&(inda2.unn[7].sqlvar.sqltype), &(datalenary[1]), 4) != 0) || 
          (memcmp(&(inda2.unn[8].sqlvar.sqltype), &(datalenary[2]), 4) != 0) || 
          (memcmp(&(inda2.unn[11].sqlvar.sqltype), &(datalenary[5]), 4) != 0) || 
          (inda2.sqln != 12) ) { 
                strcpy(msginf, "      The described DA is incorrect"); 
       fail = 1; 
     } 
   } 
 } 



qsq.Function = '4';                                  /* open cursor */ 

inda2.sqld     = 0; 
inda2.sqln     = 1; 
inda2.sqldabc  = 176; 

exdopt = QXDA_EXTDYN_NOOPTS; 

QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &inda2, &myca, "SQLP0300", &qsq, 
                     &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strcpy(recode, err.ec.Exception_Id); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Open cursor failed  "); 
   fail = 1; 
 } 

qsq.Function = '5'; 
strcpy((char *)outda2.sqldaid, "SQLDA 2 "); 
outda2.sqld     = 6; 
outda2.sqln     = 12; 
outda2.sqldabc  = 16 + (outda2.sqln * sizeof( outda2.unn[0].sqlvar)); 
/* 176*/ 

outda2.unn[0].sqlvar.sqlind = &(indicators[0]); 
outda2.unn[0].sqlvar.sqldata = (unsigned char *)sqld2area[0]; 
outda2.unn[0].sqlvar.sqllen = 0; 
outda2.unn[0].sqlvar.sqltype =453 ;//char 
outda2.unn[6].extdvar.len.sqllonglen = datalenary[0]; 
outda2.unn[6].extdvar.sqldatalen = &datalenary[0]; 

outda2.unn[1].sqlvar.sqlind = &(indicators[1]); 
outda2.unn[1].sqlvar.sqldata = (unsigned char *)sqld2area[1]; 
outda2.unn[1].sqlvar.sqllen = 0; 
outda2.unn[1].sqlvar.sqltype = 404; 
outda2.unn[7].extdvar.len.sqllonglen = datalenary[1]; 
outda2.unn[7].extdvar.sqldatalen = &datalenary[1]; 

outda2.unn[2].sqlvar.sqlind = &(indicators[2]); 
outda2.unn[2].sqlvar.sqldata = (unsigned char *)sqld2area[2]; 
outda2.unn[2].sqlvar.sqllen = 0; 
outda2.unn[2].sqlvar.sqltype = 413; 
outda2.unn[8].extdvar.len.sqllonglen = datalenary[2]; 
outda2.unn[8].extdvar.sqldatalen = &datalenary[2]; 

outda2.unn[3].sqlvar.sqlind = &(indicators[3]); 
outda2.unn[3].sqlvar.sqldata = (unsigned char *)sqld2area[3]; 
outda2.unn[3].sqlvar.sqllen = datalenary[3]; 
outda2.unn[3].sqlvar.sqltype = SQL_CLOBLOC; 

outda2.unn[4].sqlvar.sqlind = &(indicators[4]); 
outda2.unn[4].sqlvar.sqldata = (unsigned char *)sqld2area[4]; 
outda2.unn[4].sqlvar.sqllen = 4; 
outda2.unn[4].sqlvar.sqltype = 497; 
outda2.unn[10].extdvar.len.sqllonglen = datalenary[4]; 
outda2.unn[10].extdvar.sqldatalen = &datalenary[4]; 

outda2.unn[5].sqlvar.sqlind = &(indicators[5]); 
outda2.unn[5].sqlvar.sqldata = (unsigned char *)sqld2area[5]; 
outda2.unn[5].sqlvar.sqllen = 0; 
outda2.unn[5].sqlvar.sqltype = 405; 
outda2.unn[11].extdvar.len.sqllonglen = datalenary[5]; 
outda2.unn[11].extdvar.sqldatalen = &datalenary[5]; 


exdopt = QXDA_EXTDYN_NOOPTS; 

QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &outda2, &myca, "SQLP0300", 
                     &qsq, &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 


//////////////// 
 long my_Lob_Length=0; 
 long output_rec=0; 
 int getLobLenRC = sqludf_length((udf_locator*) &sqld2area[3], 
             &my_Lob_Length); 

 void *outputLob = malloc(my_Lob_Length); 

//note could break this into several calls to get a large lob by changing start and ending positions. 
 int getLobRC = sqludf_substr((udf_locator*) &sqld2area[3], //lob locator 
                            1,                        //start position 
                            my_Lob_Length,        //end position 
                            (unsigned char *) outputLob,  //put lob here 
                            &output_rec)  ;    //output length 
//cleanup when done: 
 int freeupRC = sqludf_free_locator((udf_locator*) &sqld2area[3]); 
 delete outputLob; 

//////////////// 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strcpy(recode, err.ec.Exception_Id); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Fetch failed  "); 
   fail = 1; 
 } 
 else 
   if (fail == 0) { 
     if ( (memcmp(sqld2area[0], "clob not null", datalenary[0]) != 0) || 
          (memcmp(sqld2area[1], "blob not null", datalenary[1]) != 0) || 
         (memcmp(sqld2area[3], "char null capable", 17) != 0) || 
         (memcmp(sqld2area[4], &int101, 4) != 0) || 
         (memcmp(sqld2area[5], "blob null capable", datalenary[5]) != 0) || 
         (datalenary[0]!= 13) || 
         (datalenary[1]!= 13) || 
         (indicators[2]!= -1) || 
          (datalenary[5]!= 17) ) { 
                strcpy(msginf, "      One or more fetched values is incorrect"); 
       fail = 1; 
     } 
   } 

 /*-----------------------------------------------------------------*/ 
 /* Close cursor                                                    */ 
 /*-----------------------------------------------------------------*/ 
qsq.Function = '6'; 

inda2.sqld     = 0; 
exdopt = QXDA_EXTDYN_NOOPTS; 

QxdaProcessExtDynEDRS(&ostr.Connection_Handle, &inda2, &myca, "SQLP0300", &qsq, 
                     &exd_rvar, &rvar_len, "EXDO0100", &exdopt, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strcpy(recode, err.ec.Exception_Id); 
 strcpy(msginf, "      Process Ext. Dyn. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 
else 
 if (myca.sqlcode != 0 && fail == 0) { 
   strcpy(recode, "1"); 
   sprintf(recode, "%d", myca.sqlcode); 
   strcpy(msginf, "      Close Cursor failed  "); 
   fail = 1; 
 } 

 /*-----------------------------------------------------------------*/ 
 /* Delete the SQL package                                          */ 
 /*-----------------------------------------------------------------*/ 

cmdlen = 34; 
QxdaProcessCommandEDRS(&ostr.Connection_Handle, 
                      "DLTSQLPKG SQLPKG(XDALOBS/XDALOB12)", &cmdlen, &err); 
if (err.ec.Bytes_Available != 0 && fail == 0) { 
 strcpy(recode, err.ec.Exception_Id); 
 strcpy(msginf, "      Process Cmd. failed  "); 
 strcat(msginf, err.Exception_Data); 
 fail = 1; 
} 

dscopt = QXDA_DISCONNECT_COMMIT; 
QxdaDisconnectEDRS(&ostr.Connection_Handle, &dscopt, &err); 

/* Builds errmsg field */ 
sprintf(errmsg, "exp: %s rcv: %.7s  %s", expcode, recode, msginf); 

if (fail) { 
 strcpy(passfail,"F"); 
} 
else { 
 strcpy(passfail,"P"); 
} 

} 
