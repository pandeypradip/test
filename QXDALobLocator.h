typedef _Packed struct xda_sqlda2
{
   unsigned char   sqldaid[8];
            long   sqldabc;
            short  sqln;
            short  sqld;
            union {
                struct sqlvar sqlvar;
                struct extdvar extdvar;
            } unn[256];
} xda_sqlda2_t;

=====================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <mih/cpybytes.h>
#include <qusec.h>
#include <qsqprced.h>
#include <qxdaedrs.h>
#include <unistd.h>
//#include "space.h"

#define MAXSQLVAR 256

typedef struct {
  Qus_EC_t  ec;
   char      Exception_Data[60];
} err_t;

struct extdvar {
    struct {
       long sqllonglen;
       char reserve1[28];
    } len;
    long * sqldatalen;
    struct sqldistinc_type {
       short         length;
       unsigned char data[30];
    } sqldatatype_name;
} extdvar;


typedef _Packed struct xda_sqlda2
{
   unsigned char   sqldaid[8];
            long   sqldabc;
            short  sqln;
            short  sqld;
            union {
                struct sqlvar sqlvar;
                struct extdvar extdvar;
            } unn[256];
} xda_sqlda2_t;

typedef _Packed struct xda_sqlda
{
   unsigned char   sqldaid[8];
            long   sqldabc;
            short  sqln;
            short  sqld;
            struct sqlvar sqlvar[256];
 } xda_sqlda_t;


typedef _Packed struct xda_sqlp0300
{
  char    Function;
  char    SQL_Package_Name[10];
  char    Library_Name[10];
  char    Main_Pgm[10];
  char    Main_Lib[10];
  char    Statement_Name[18];
  char    Cursor_Name[18];
  char    Open_Options;
  char    Clause_For_Describe;
  char    Commitment_Control;
  char    Date_Format[3];
  char    Date_Separator;
  char    Time_Format[3];
  char    Time_Separator;
  char    Naming_Option[3];
  char    Decimal_Point;
  short   Blocking_Factor;
  short   Scrollable_Option;
  short   Position_Option;
  int     Relative_Record;
  int     Rows_To_Insert;
  char    Direct_Map;
  char    Reuse_DA;
  char    Name_Check;
  char    Use_Ptrs;
  char    With_Hold;
  char    User_Defined_Field[18];
  char Close_File_Name[10];
  char Close_Library_Name[10];
  char    Reopen;
  char    Use_Perf;
  char    Reserved;
  unsigned short int Max_Scale;
  char    Max_Precision;
  char    Min_Divide_Scale;
  int     Statement_Text_CCSID;
  _SYSPTR package_syp;
  _SYSPTR main_program_syp;
  short   Statement_Length;
  char    Statement_Text[5000];
} xda_sqlp0300_t;


typedef _Packed struct xda_sqlp0310
{
  char    Function;
  char    SQL_Package_Name[10];
  char    Library_Name[10];
  char    Main_Pgm[10];
  char    Main_Lib[10];
  char    Statement_Name[18];
  char    Cursor_Name[18];
  char    Open_Options;
  char    Clause_For_Describe;
  char    Commitment_Control;
  char    Date_Format[3];
  char    Date_Separator;
  char    Time_Format[3];
  char    Time_Separator;
  char    Naming_Option[3];
  char    Decimal_Point;
  short   Blocking_Factor;
  short   Scrollable_Option;
  short   Position_Option;
  int     Relative_Record;
  int     Rows_To_Insert;
  char    Direct_Map;
  char    Reuse_DA;
  char    Name_Check;
  char    Use_Ptrs;
  char    With_Hold;
  char    User_Defined_Field[18];
  char    Close_File_Name[10];
  char    Close_Library_Name[10];
  char    Reopen;
  char    Use_Perf;
  char    Reval;
  unsigned short int Max_Scale;
  char    Max_Precision;
  char    Min_Divide_Scale;
  int     Statement_Text_CCSID;
  _SYSPTR package_syp;
  _SYSPTR main_program_syp;
  //add to be up to date
  int     Statement_Offset;
  char    Hex_Literal_Option;
  char    Reserved[7];
  int     Length_Of_Additional_Fields;
  int     Connection_Handle;
  char    Auto_Commit;
  short   Statement_Length;
  char    Statement_Text[5000];

} xda_sqlp0310_t;


typedef _Packed struct xda_sqlp0410
{
  char    Function;
  char    SQL_Package_Name[10];
  char    Library_Name[10];
  char    Main_Pgm[10];
  char    Main_Lib[10];
  char    Statement_Name[18];
  char    Cursor_Name[18];
  char    Open_Options;
  char    Clause_For_Describe;
  char    Commitment_Control;
  char    Date_Format[3];
  char    Date_Separator;
  char    Time_Format[3];
  char    Time_Separator;
  char    Naming_Option[3];
  char    Decimal_Point;
  short   Blocking_Factor;
  short   Scrollable_Option;
  short   Position_Option;
  int     Relative_Record;
  int     Rows_To_Insert;
  char    Direct_Map;
  char    Reuse_DA;
  char    Name_Check;
  char    Use_Ptrs;
  char    With_Hold;
  char    User_Defined_Field[18];
  char    Close_File_Name[10];
  char    Close_Library_Name[10];
  char    Reopen;
  char    Use_Perf;
  char    Reval;
  unsigned short Max_Scale;
  char    Max_Precision;
  char    Min_Divide_Scale;
  int     Statement_Text_CCSID;
  _SYSPTR package_syp;
  _SYSPTR main_program_syp;
  char Sort_Sequence_Table[10];
  char Sort_Sequence_Library[10];
  char Language_ID[10];
  char Allow_Copy_Data;
  char Allow_Blocking;
  int     Statement_Offset;
  char    Hex_Literal_Option;
  char    Reserved[7];
  int     Length_Of_Additional_Fields;
  int     Connection_Handle;
  char    Auto_Commit;
  short   Statement_Length;  /* short for the statement length --- key to this test */
  char    Statement_Text[5000];
} xda_sqlp0410_t;

err_t     err;
char *conntype;
char *sys;

/*The input structures below are defined because we know amount of storage,*/
/*including variable character fields */
char  instruct[750];
char  instruct2[1000];
char  instruct3[800];

int   ostrsize;
int   dscopt;
int   exdopt;
Qxda_CDBI0100_t *istr = (Qxda_CDBI0100_t *)instruct;
Qxda_CDBI0200_t *istr2 = (Qxda_CDBI0200_t *) instruct2;
Qxda_CDBI0200_t *istr3 = (Qxda_CDBI0200_t *) instruct3;
Qxda_CDBO0100_t  ostr;
Qsq_sqlca_t      myca;
xda_sqlda_t      inda, outda = {0};
xda_sqlda2_t      inda2, outda2 = {0};
xda_sqlda_t *outdaPtr = &outda;
xda_sqlda_t *indaPtr = &inda;
xda_sqlp0310_t   qsq;
Qxda_EXDO0100_t  exd_rvar;
int              rvar_len = sizeof(Qxda_EXDO0100_t);
char             sqldtaara[500000] = {0};
char            *dtap = sqldtaara;
char             sqlindara[2000] = {0};
short           *indp = (short *)sqlindara;
char            userProfile[11] = "marda     ";
char            password[9] = "poo32jab";


void INIT() { //initialize a local connection
  memset(istr, 0x00, sizeof(istr));
  err.ec.Bytes_Provided = sizeof(err_t); //The bytes provide must be initialized or error will occur
  sys = "SYSNAME";
  istr->Connection_Type = 'L';
  if ((sys != NULL)  && (istr->Connection_Type != 'L'))
     strcpy(istr->Server_Name, sys);
  else
     memset(istr->Server_Name, ' ', 256);
  istr->Commitment_Control = 'C';
  _CPYBYTES(istr->Commit_Scope, "*JOB      ", 10);
  istr->Allow_Suspend = 'N'; //can not be enabled if thread safety is enabled
  istr->SQLDA_Cache_Size = 0;//DA caching should be disabled always
  istr->Offset_Job_Data = sizeof(Qxda_CDBI0100_t);
  istr->Length_Job_Data = 7;
  _CPYBYTES((char *)istr + istr->Offset_Job_Data, "CONNECT", 7);
  istr->Offset_Suspend_Data = istr->Offset_Job_Data + istr->Length_Job_Data;
  istr->Length_Suspend_Data = 7;
  _CPYBYTES((char *)istr + istr->Offset_Suspend_Data, "CONNECT", 7);
  ostrsize = sizeof(Qxda_CDBO0100_t); //set the size of the connection output structure
}

void INIT3() //initialize a connection with TCP-IP sockets
{
  memset(istr2, 0x00, sizeof(istr2));
  sys = "RCHASMOS";
  err.ec.Bytes_Provided = sizeof(err_t); //The bytes provide must be initialized or error will occur
  istr2->Connection_Type = 'T';
  if ((istr2->Connection_Type == 'T'))
     strcpy(istr2->Server_Name, sys);
  else
     memset(istr2->Server_Name, ' ', 256);

  istr2->Commitment_Control = 'C'; //set commitment control to appropriate level
  _CPYBYTES(istr2->Commit_Scope, "*JOB      ", 10);
  istr2->SQL_Hex_Constants = '0';
  istr2->Allow_Suspend = 'N'; //can not be enabled if thread safety is enabled
  istr2->SQLDA_Cache_Size = 0;//DA caching should be disabled always
  istr2->Offset_Job_Data = sizeof(Qxda_CDBI0200_t);
  istr2->Length_Job_Data = 7;
  _CPYBYTES((char *)istr2 + istr2->Offset_Job_Data, "CONNECT", 7);
  istr2->Offset_Suspend_Data = istr2->Offset_Job_Data + istr2->Length_Job_Data;
  istr2->Length_Suspend_Data = 7;
  _CPYBYTES((char *)istr2 + istr2->Offset_Suspend_Data, "CONNECT", 7);
  istr2->Offset_Profile = istr2->Offset_Suspend_Data + istr2->Length_Suspend_Data;
  istr2->Length_Profile = strlen(userProfile);
  _CPYBYTES((char *)istr2 + istr2->Offset_Profile, userProfile, istr2->Length_Profile);
  istr2->Offset_Password = istr2->Offset_Profile + istr2->Length_Profile;
  istr2->Length_Password = strlen (password);
  _CPYBYTES((char *)istr2 + istr2->Offset_Password, password, istr2->Length_Password);
  ostrsize = sizeof(Qxda_CDBO0100_t);  //set the size of the connection output structure
}

void INIT3(void *instructure)
{

Qxda_CDBI0200_t *istr2 = (Qxda_CDBI0200_t *) instructure;
  err.ec.Bytes_Provided = sizeof(err_t);

  istr2->Connection_Type = 'T';

  memcpy(istr2->Server_Name, "RCHASJLB", 8);
  istr2->Commitment_Control = 'C';
  memcpy(istr2->Commit_Scope, "*JOB      ", 10);
  istr2->SQL_Hex_Constants = '0';
  memset((&istr2->SQL_Hex_Constants), 0x00, 1);
  istr2->Allow_Suspend = 'N';
  istr2->Server_Job_CCSID = 37;
  istr2->SQLDA_Cache_Size = 10;
  istr2->Offset_Job_Data = sizeof(Qxda_CDBI0200_t);
  istr2->Length_Job_Data = 7;
  memcpy((char *)istr2 + istr2->Offset_Job_Data, "CONNECT", 7);
  istr2->Offset_Suspend_Data = istr2->Offset_Job_Data + istr2->Length_Job_Data;
  istr2->Length_Suspend_Data = 7;
  memcpy((char *)istr2 + istr2->Offset_Suspend_Data, "CONNECT", 7);
  istr2->Offset_Profile = istr2->Offset_Suspend_Data + istr2->Length_Suspend_Data;
  istr2->Length_Profile = strlen(userProfile);
  memcpy((char *)istr2 + istr2->Offset_Profile, userProfile, istr2->Length_Profile);
  istr2->Offset_Password = istr2->Offset_Profile + istr2->Length_Profile;
  istr2->Length_Password = strlen (password);
  memcpy((char *)istr2 + istr2->Offset_Password, password, istr2->Length_Password);
  ostrsize = sizeof(Qxda_CDBO0100_t);
}

void INIT2()
{
  memset(istr, 0x00, sizeof(istr));
  err.ec.Bytes_Provided = sizeof(err_t); //set the bytes provide or an error will occur
  sys = "RCHASJLB";
  istr->Connection_Type = 'U';
  if ((sys != NULL)  && (istr->Connection_Type == 'T'))
     strcpy(istr->Server_Name, sys);
  else
     memset(istr->Server_Name, ' ', 256);
  istr->Commitment_Control = 'C'; //set appropriate commitment control level
  _CPYBYTES(istr->Commit_Scope, "*JOB      ", 10);
  istr->SQL_Hex_Constants = '0';
  istr->Allow_Suspend = 'N'; //can not be enabled if thread safety is enabled
  istr->SQLDA_Cache_Size = 10;//DA cache should be disabled always
  istr->Offset_Job_Data = sizeof(Qxda_CDBI0100_t);
  istr->Length_Job_Data = 7;
  _CPYBYTES((char *)istr + istr->Offset_Job_Data, "CONNECT", 7);
  istr->Offset_Suspend_Data = istr->Offset_Job_Data + istr->Length_Job_Data;
  istr->Length_Suspend_Data = 7;
  _CPYBYTES((char *)istr + istr->Offset_Suspend_Data, "CONNECT", 7);
  /*
  istr2->Offset_Profile = istr2->Offset_Suspend_Data + istr2->Length_Suspend_Data;
  istr2->Length_Profile = strlen(userProfile);
  memcpy((char *)istr2 + istr2->Offset_Profile, userProfile, istr2->Length_Profile);
  istr2->Offset_Password = istr2->Offset_Profile + istr2->Length_Profile;
  istr2->Length_Password = strlen (password);
  memcpy((char *)istr2 + istr2->Offset_Password, password, istr2->Length_Password);
  */
  ostrsize = sizeof(Qxda_CDBO0100_t); //set the size of the connection output structure
}


// We put this in the 'space.C' module because its a good generic
// place for utility programs and BATS sucks.
//
// #pragma linkage (RESULTS, OS)           /* link RESULTS program     */
//
// void extern RESULTS (char *varname,     /* declare RESULTS program  */
//                      char *passfail,    /* pass or fail of var      */
//                      char *errmsg,      /* error message for fail   */
//                      char *testname,    /* testcase name            */
//                      char *lib,         /* collection for report    */
//                      char *file,        /* table for reporting      */
//                      short int c1,      /* length of libname        */
//                      short int c2);     /* length of table name     */

