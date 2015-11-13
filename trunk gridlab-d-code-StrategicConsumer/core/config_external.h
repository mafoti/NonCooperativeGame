// config.h automatically created 16-Mar-15 16:14:53.72 
// ..\utilities\configure: config=config_external.h 
// ..\utilities\configure: search=C:\ 
// ..\utilities\configure: source=.. 
// 
// MYSQL configuration 
// 
// ..\utilities\configure: found C:\PROGRA~1\MySQL\MYSQLC~1\include\mysql.h/ 
#define HAVE_MYSQL "C:\PROGRA~1\MySQL\MYSQLC~1" 
#define MYSQL_INCLUDE "C:\PROGRA~1\MySQL\MYSQLC~1\include" 
#ifdef _DEBUG
#define MYSQL_LIB "C:\PROGRA~1\MySQL\MYSQLC~1\lib\debug" 
#else
#define MYSQL_LIB "C:\PROGRA~1\MySQL\MYSQLC~1\lib\opt" 
#endif 
// 
// MATLAB x64 configuration 
// 
// ..\utilities\configure: found C:\PROGRA~1\MATLAB\R2011a\extern\include\mex.h 
#if defined _M_X64 || defined _M_IA64 
#define MATLAB_X64 
#endif 
 
#ifdef MATLAB_X64 
#define MATLAB_INCLUDE "C:\PROGRA~1\MATLAB\R2011a\extern\include" 
#define MATLAB_LIB "C:\PROGRA~1\MATLAB\R2011a\extern\lib\win64\MICROS~1" 
#define HAVE_MATLAB "C:\PROGRA~1\MATLAB\R2011a" 
#endif 
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: MATLAB target extern\lib\platform\libmx.lib not found  
// ..\utilities\configure: PowerWorld SimAuto target RegSimAuto.exe not found  
