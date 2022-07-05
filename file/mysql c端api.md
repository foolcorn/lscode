## mysql c端api



### 1.1 初始化函数

```
MYSQL *mysql_init(MYSQL *mysql)
```

可以传null 

返回mysql句柄指针



### 1.2 设置连接选项

```
int mysql_options(MYSQL *mysql, enum mysql_option option, const char *arg)
```

示例

my_bool reconnect = true;
mysql_options(m_mysql, MYSQL_OPT_RECONNECT, &reconnect);
mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");



### 1.3 连接数据库

```
MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, 
                          const char *passwd, const char *db, unsigned int port, 
                          const char *unix_socket, unsigned long client_flag)
```



- mysql：数据库句柄
- host:主机
- user 用户名---mysql root
- passwd  密码
- db： 连接的数据库名
- port：端口，一般填0，mysql默认3306
- unix_socket:本地套接字，一般填null
- client_flag 连接标志，一般填0

返回值 成功返回connet句柄，失败返回null



--------------------------------------------------------------以下为登陆之后---------------------------------------------------------------

### 1.4 设定登陆字符集

```
int mysql_set_character_set(MYSQL *mysql, char *csname)
```



- mysql：connect连接句柄
- csname：字符集名称：utf8



### 1.5 执行sql，增删改查都可以

```
int mysql_query(MYSQL *mysql, const char *query)
```

- mysql：connect连接句柄
- query：sql语句

返回值：

- 成功：返回0
- 失败：返回非0



### 1.6 读取结果集

```
MYSQL_RES *mysql_store_result(MYSQL *mysql)
```

如果读取失败返回null

有几个api可以检查结果集的具体问题，类似perror

- mysql_error(MYSQL *mysql)：若有错则返回非空字符串
- mysql_errno(MYSQL *mysql)：有错返回非0
- mysql_field_count(MYSQL *mysql)：有错返回0

```
MYSQL_RES* res = mysql_store_result(m_mysql);
if (!res) {
    log_error("mysql_store_result failed: %s", mysql_error(m_mysql));
    return NULL;
}
```



### 1.7 释放结果集

void mysql_free_result(MYSQL_RES *result)

```
if (m_res) {
    mysql_free_result(m_res);
    m_res = NULL;
}
```



### 1.8获取查询的显示列长度

```
unsigned int mysql_num_fields(MYSQL_RES *result)
```



### 1.9 获取结果集的列信息

```
MYSQL_FIELD *mysql_fetch_field(MYSQL_RES *result)
```

每次调用返回一个列，需要一直调用直到返回null

每次select查询后都会复位mysql_fetch_field的偏移，mysql_field_seek()也可以调整偏移量



### 1.10 取出一行信息

```
MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)
```

检索结果集的下一行，直到出错或者返回null

行内列数可以用 mysql_num_fields 获得，



### 1.11 现实受影响的行数

```
my_ulonglong mysql_affected_rows(MYSQL *mysql)
```

如果进行了增删改，会返回更改了行数

- \>0：受影响的行数
- =0：未更改未查询或者查询无匹配
- =-1：查询返回错误



```c
/* gcc -o connect mysql.c -lmysqlclient -I/usr/include/mysql -L/usr/lib64/mysql/ */
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <unistd.h>
#define _USER_      "root"          /* 用户名 */
#define _HOST_      "127.0.0.1"     /* host */
#define _PASSWD_    "123456"        /* root 用户名密码 */
#define _DB_        "teamtalk"         /* 库名 */
#define _PORT_      3306            /* port */
/* 打印信息   结果集 */
void show_information( MYSQL_RES * res, MYSQL *mysql )
{
    int     i;
    MYSQL_ROW   row;            /*  */
    /* unsigned long *lengths; */
    unsigned int    num_fields;
    MYSQL_FIELD *field;         /* 字段名 */
    /* 数据的长度unsigned int mysql_num_fields(MYSQL_RES *result) */
    num_fields = mysql_num_fields( res );
    /*
     * 返回列长unsigned long *mysql_fetch_lengths(MYSQL_RES *result)
     * lengths  = mysql_fetch_lengths(res);
     */
    printf( "num_fields = [%d]\n", num_fields );
    /* 字段名 */
    for ( i = 0; i < num_fields, field = mysql_fetch_field( res ); i++ )
    {
        printf( " %s\t", field->name );
    }
    printf( "\n" );
    printf( "--------------------------------------------------------------\n" );
    while ( (row = mysql_fetch_row( res ) ) )
    {
        for ( i = 0; i < num_fields; i++ )
        {
            printf( "%s \t", row[i] ? row[i] : NULL );
        }
        printf( "\n" );
    }
    /* 释放结果集 */
    mysql_free_result( res );
    printf( "--------------------------------------------------------------\n" );
    /* 显示受影响的行数 */
    printf( "  %ld rows in set\n", (long) mysql_affected_rows( mysql ) );
}
int main( int argc, char *argv[] )
{
    int     ret, i;
    MYSQL       *mysql;
    MYSQL_RES   * res;
    char        strsql[512] = { 0 };
    /* 1，mysql_init 初始化 */
    mysql = mysql_init( NULL );
    if ( mysql == NULL )
    {
        printf( "mysql_init error\n" );
        return(-1);
    }
    /* 2，mysql_real_connect链接 */
    mysql = mysql_real_connect( mysql,
                    _HOST_,
                    _USER_,
                    _PASSWD_,
                    _DB_,
                    0,
                    NULL,
                    0 );
    if ( mysql == NULL )
    {
        printf( "mysql_real_connect error\n" );
        return(-1);
    }
    printf( "connect mysql ok\n" );
    /* 处理字符问题的函数 */
    if ( mysql_set_character_set( mysql, "utf8" ) != 0 )
    {
        printf( " mysql_set_character_set error\n" );
        mysql_close( mysql );
        exit( 1 );
    }
    /* ===========================输入sql语句====================== */
    while ( 1 )
    {
        memset( strsql, 0x00, sizeof(strsql) );
        write( STDOUT_FILENO, "yoursql>", 8 );
        // select * from IMUser;
        /* 读取client输入的sql字符串 */
        read( STDIN_FILENO, strsql, sizeof(strsql) );
        /* 退出的处理 */
        if ( strncasecmp( strsql, "quit", 4 ) == 0 )
        {
            printf( "client 退出处理\n" );
            break;
        }
        /* 输入sql的 */
        ret = mysql_query( mysql, strsql );
        if ( ret )
        {
            printf( "mysql_query error\n" );
            continue;
        }
        res = mysql_store_result( mysql );
        /*有结果集的处理 */
        if ( res != NULL )
        {
            show_information( res, mysql );
        }else  {
            /* 显示受影响的行数 */
            printf( " Query OK, %ld row affected\n", (long) mysql_affected_rows( mysql ) );
        }
    }
    /* 关闭mysql */
    mysql_close( mysql );
    return(0);
}
```



### 2.1 预处理初始化

```
MYSQL_STMT *mysql_stmt_init(MYSQL *mysql)
```

返回stmt句柄，如果出错返回null，后期应由mysql_stmt_close



### 2.2 插入预处理

```
int mysql_stmt_prepare(MYSQL_STMT *stmt, const char *query, unsigned long length)
```

- stmt：预处理句柄
- query：sql语句，结尾没有；或者\g（允许使用?作为参数占位符）
- length：query的长度

如果成功返回0

错误返回非0



### 2.3 获取参数

```
unsigned long mysql_stmt_param_count(MYSQL_STMT *stmt)
```

根据stmt获得prepare sql中的参数（?）数量



### 2.4 参数绑定

```
my_bool mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bind)
```

MYSQL_BIND是一个数组结构，bind是数组指针

数组的每个元素和每个？一 一对应



### 2.5 预处理执行

```
int mysql_stmt_execute(MYSQL_STMT *stmt)
```

真正执行prepare的语句。

执行成功，返回0

出现错误，返回非0



### 2.6 预处理的行数

```
my_ulonglong mysql_stmt_affected_rows(MYSQL_STMT *stmt)
```

如果执行了增删改语句，返回更新的行数

如果执行了查询语句，返回结果集的行数

- \>0：受影响的行数
- =0：未更改未查询或者查询无匹配
- =-1：查询返回错误





### 2.7 预处理插入demo

```c
/* gcc -o insert mysql_insert.c -lmysqlclient -I/usr/include/mysql -L/usr/lib64/mysql/ */
/* 预处理方式api处理 -- insert */
#include <stdio.h>
#include "mysql.h"
#include <stdlib.h>
#include <string.h>
#define _HOST_      "localhost"                                     /* 主机 */
#define _USER_      "root"                                          /* mysql用户,非主机 */
#define _PASSWD_    "123456"                                        /* 密码 */
#define _DBNAME_    "teamtalk"                                      /* 库名 */
#define STRING_SIZE 50
#define DROP_SAMPLE_TABLE   "DROP TABLE IF EXISTS test_table"       /* if EXISTS 好处 是如果表不存在,执行不会报错 */
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
                                                 col2 VARCHAR(40),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)"
#define INSERT_SAMPLE       "INSERT INTO test_table(col1,col2,col3) VALUES(?,?,?)"
void prepare_insert( MYSQL *mysql );
int main()
{
    /* 1.初始化 */
    MYSQL * mysql = NULL;
    mysql = mysql_init( NULL );
    if ( mysql == NULL )
    {
        printf( "mysql init err\n" );
        exit( 1 );
    }
    /* 2.连接 */
    mysql = mysql_real_connect( mysql, _HOST_, _USER_, _PASSWD_, _DBNAME_, 0, NULL, 0 );
    if ( mysql == NULL )
    {
        printf( "mysql_real_connect connect err\n" );
        exit( 1 );
    }
    printf( "welcome to mysql \n" );
    prepare_insert( mysql );
    /* 3.关闭 */
    mysql_close( mysql );
    return(0);
}
void prepare_insert( MYSQL *mysql )
{
    MYSQL_STMT  *stmt;          /* 预处理的句柄 -- 标识进程内唯一的一个预处理的sql */
    MYSQL_BIND  bind[3];        /* 绑定变量 */
    my_ulonglong    affected_rows;
    int     param_count;
    short       small_data;
    int     int_data;
    char        str_data[STRING_SIZE];
    unsigned long   str_length;
    my_bool     is_null;
    if ( mysql_query( mysql, DROP_SAMPLE_TABLE ) )          /*删除表 */
    {
        fprintf( stderr, " DROP TABLE failed\n" );
        fprintf( stderr, " %s\n", mysql_error( mysql ) );
        exit( 0 );
    }
    if ( mysql_query( mysql, CREATE_SAMPLE_TABLE ) )        /* 创建表 */
    {
        fprintf( stderr, " CREATE TABLE failed\n" );
        fprintf( stderr, " %s\n", mysql_error( mysql ) );
        exit( 0 );
    }
    /* Prepare an INSERT query with 3 parameters */
    /* (the TIMESTAMP column is not named; the server */
    /*  sets it to the current date and time) */
    stmt = mysql_stmt_init( mysql );                                                /* 预处理的初始化 */
    if ( !stmt )
    {
        fprintf( stderr, " mysql_stmt_init(), out of memory\n" );
        exit( 0 );
    }
    if ( mysql_stmt_prepare( stmt, INSERT_SAMPLE, strlen( INSERT_SAMPLE ) ) )       /* insert 语句 的预处理 */
    {
        fprintf( stderr, " mysql_stmt_prepare(), INSERT failed\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    fprintf( stdout, " prepare, INSERT successful\n" );
    /* Get the parameter count from the statement */
    param_count = mysql_stmt_param_count( stmt );   /* 获得参数个数 */
    fprintf( stdout, " total parameters in INSERT: %d\n", param_count );
    if ( param_count != 3 )                         /* validate parameter count */
    {
        fprintf( stderr, " invalid parameter count returned by MySQL\n" );
        exit( 0 );
    }
    /* Bind the data for all 3 parameters */
    memset( bind, 0, sizeof(bind) );
    /* INTEGER PARAM */
    /* This is a number type, so there is no need to specify buffer_length */
    bind[0].buffer_type = MYSQL_TYPE_LONG;      /* 对应int类型 */
    bind[0].buffer      = (char *) &int_data;   /* 内存地址的映射 */
    bind[0].is_null     = 0;
    bind[0].length      = 0;
    /* STRING PARAM */
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer      = (char *) str_data;    /* char 100 */
    bind[1].buffer_length   = STRING_SIZE;
    bind[1].is_null     = 0;
    bind[1].length      = &str_length;
    /* SMALLINT PARAM */
    bind[2].buffer_type = MYSQL_TYPE_SHORT;
    bind[2].buffer      = (char *) &small_data;
    bind[2].is_null     = &is_null;             /* 是否为null的指示器 */
    bind[2].length      = 0;
    /* Bind the buffers */
    if ( mysql_stmt_bind_param( stmt, bind ) )      /* 绑定变量 参数绑定 */
    {
        fprintf( stderr, " mysql_stmt_bind_param() failed\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    /* 第一波赋值 */
    int_data = 10;                                  /* integer */
    strncpy( str_data, "MySQL", STRING_SIZE );      /* string  */
    str_length = strlen( str_data );
    /* INSERT INTO test_table(col1,col2,col3) VALUES(10,'MySQL',null) */
    /* INSERT SMALLINT data as NULL */
    is_null = 1;                                    /* 指示插入的第三个字段是否为null */
    /* insert into test_table(col1,col2,col3) values(10,'MySQL',null); */
    /* Execute the INSERT statement - 1*/
    if ( mysql_stmt_execute( stmt ) )               /* 预处理的执行,第一次执行 */
    {
        fprintf( stderr, " mysql_stmt_execute(), 1 failed\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    /* Get the total number of affected rows */
    affected_rows = mysql_stmt_affected_rows( stmt );       /* 预处理的影响条数 */
    fprintf( stdout, " total affected rows(insert 1): %lu\n",
         (unsigned long) affected_rows );
    if ( affected_rows != 1 )                               /* validate affected rows */
    {
        fprintf( stderr, " invalid affected rows by MySQL\n" );
        exit( 0 );
    }
    /* 第二波赋值 */
    int_data = 1000;
    strncpy( str_data, "The most popular Open Source database", STRING_SIZE );
    str_length  = strlen( str_data );
    small_data  = 1000; /* smallint */
    is_null     = 1;    /* reset */
    /* INSERT INTO test_table(col1,col2,col3) VALUES(1000,'The most popular Open Source database',1000) */
    /* insert into test_table(col1,col2,col3) values(1000,'The most popular Open Source database',1000); */
    /* Execute the INSERT statement - 2*/
    if ( mysql_stmt_execute( stmt ) ) /* 第二次执行 */
    {
        fprintf( stderr, " mysql_stmt_execute, 2 failed\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    /* Get the total rows affected */
    affected_rows = mysql_stmt_affected_rows( stmt );
    fprintf( stdout, " total affected rows(insert 2): %lu\n",
         (unsigned long) affected_rows );
    if ( affected_rows != 1 ) /* validate affected rows */
    {
        fprintf( stderr, " invalid affected rows by MySQL\n" );
        exit( 0 );
    }
    /* Close the statement */
    if ( mysql_stmt_close( stmt ) )
    {
        fprintf( stderr, " failed while closing the statement\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
}



```

### 2.8 预处理查询demo

```
/* gcc -o select mysql_select.c -lmysqlclient -I/usr/include/mysql -L/usr/lib64/mysql/ */
/* SELECT id, name, sex FROM IMUser; 查询语句的使用 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#define _USER_      "root"          /* 用户名 */
#define _HOST_      "127.0.0.1"     /* host */
#define _PASSWD_    "123456"        /* root 用户名密码 */
#define _DB_        "teamtalk"      /* 库名 */
#define _PORT_      3306            /* port */
/* ============================stmt ====================================== */
#define STRING_SIZE 50
/* mysql 查询语句 */
#define SELECT_SAMPLE "SELECT id, name, sex FROM IMUser"
void seach_information( MYSQL *mysql )
{
    int     i, count = 0;;
    MYSQL_STMT  *stmt;                  /*  */
    MYSQL_BIND  bind[3];                /* 条件变量 */
    MYSQL_RES   *prepare_meta_result;   /* 结果集 */
    unsigned long   length[3];
    int     param_count, column_count, row_count;
    int     int_data;               /* 序号 */
    char        str_name[STRING_SIZE];  /* 姓名 */
    char        str_loc[STRING_SIZE];
    my_bool     is_null[3];             /* 参数的控制 */
    my_ulonglong    row_unm;
    /*
     * ===========================init start=========================
     * 初始化sql预处理语句
     */
    stmt = mysql_stmt_init( mysql );
    if ( !stmt )
    {
        fprintf( stderr, " mysql_stmt_init(), out of memory\n" );
        exit( 0 );
    }
    /*
     * 绑定参数my_bool mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bind)
     * mysql_stmt_bind_param(stmt, bind);
     */
    if ( mysql_stmt_prepare( stmt, SELECT_SAMPLE, strlen( SELECT_SAMPLE ) ) )
    {
        fprintf( stderr, " mysql_stmt_prepare(), SELECT failed\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    fprintf( stdout, " prepare, SELECT successful\n" );
    fprintf( stdout, " total parameters in SELECT: %d\n", param_count );
    param_count = mysql_stmt_param_count( stmt );
    if ( param_count != 0 ) /* validate parameter count */
    {
        fprintf( stderr, " invalid parameter count returned by MySQL\n" );
        exit( 0 );
    }
    prepare_meta_result = mysql_stmt_result_metadata( stmt );
    if ( !prepare_meta_result )
    {
        fprintf( stderr,
             " mysql_stmt_result_metadata(), returned no meta information\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    column_count = mysql_num_fields( prepare_meta_result );
    fprintf( stdout, " total columns in SELECT statement: %d\n", column_count );
    if ( column_count != 3 ) /* validate column count */
    {
        fprintf( stderr, " invalid column count returned by MySQL\n" );
        exit( 0 );
    }
    /* mysql_stmt_execute(stmt); //发送占位符到mysql数据库上？？？？ */
    if ( mysql_stmt_execute( stmt ) )
    {
        fprintf( stderr, " mysql_stmt_execute(), failed\n" );
        fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
        exit( 0 );
    }
    /* 初始化参数 */
    memset( bind, 0, sizeof(bind) );
    /* INTEGER COLUMN */
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer      = (char *) &int_data;
    bind[0].is_null     = &is_null[0];
    bind[0].length      = &length[0];
    /* varchar    -- MYSQL_TYPE_VAR_STRING */
    /* name COLUMN */
    bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    bind[1].buffer      = (char *) str_name;
    bind[1].buffer_length   = STRING_SIZE;
    bind[1].is_null     = &is_null[1];
    bind[1].length      = &length[1];
    /* loc COLUMN */
    bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
    bind[2].buffer      = (char *) &str_loc;
    bind[2].buffer_length   = STRING_SIZE;
    bind[2].is_null     = &is_null[2];
    bind[2].length      = &length[2];
    /* 绑定数据 */
    mysql_stmt_bind_result( stmt, bind );
    mysql_stmt_store_result( stmt ); /*  */
    /* init row */
    row_count = 0;
    /* 查询的结果 */
    row_unm = mysql_stmt_num_rows( stmt );
    /*
     * 打印数据
     * mysql_stmt_fetch(stmt);
     * mysql_stmt_fetch(stmt);
     */
    printf( "row_unm = %ld\n", row_unm );
    for ( i = 0; i < row_unm; i++ )
    {
        mysql_stmt_fetch( stmt );
        row_count++;
        fprintf( stdout, "  row %d\n", row_count );
        /* column 1 */
        fprintf( stdout, "   column1 (integer)  : " );
        if ( is_null[0] )
            fprintf( stdout, " NULL\n" );
        else
            fprintf( stdout, " %d(%ld)\n", int_data, length[0] );
        /* column 2 */
        fprintf( stdout, "   column2 (string)   : " );
        if ( is_null[1] )
            fprintf( stdout, " NULL\n" );
        else
            fprintf( stdout, " %s(%ld)\n", str_name, length[1] );
        /* column 3 */
        fprintf( stdout, "   column3 (string) : " );
        if ( is_null[2] )
            fprintf( stdout, " NULL\n" );
        else
            fprintf( stdout, " %s(%ld)\n", str_loc, length[2] );
        fprintf( stdout, "\n" );
        /* break; */
    }
    /* 释放内存 */
    /* Validate rows fetched */
    // if ( row_count != 10 )
    // {
    //  fprintf( stderr, " MySQL failed to return all rows\n" );
    //  exit( 0 );
    // }
    /* Free the prepared result metadata */
    mysql_free_result( prepare_meta_result );
    /* Close the statement */
    mysql_stmt_close( stmt );
}
/* ===================================================================== */
int main( int argc, char *argv[] )
{
    int     ret, i;
    MYSQL       *mysql;
    MYSQL_RES   * res;
    MYSQL_ROW   row;    /*  */
    /* unsigned long *lengths; */
    unsigned int    num_fields;
    MYSQL_FIELD *field; /* 字段名 */
    /* 1，mysql_init 初始化 */
    mysql = mysql_init( NULL );
    if ( mysql == NULL )
    {
        printf( "mysql_init error\n" );
        return(-1);
    }
    /* 2，mysql_real_connect链接 */
    mysql = mysql_real_connect( mysql,
                    _HOST_,
                    _USER_,
                    _PASSWD_,
                    _DB_,
                    0,
                    NULL,
                    0 );
    if ( mysql == NULL )
    {
        printf( "mysql_real_connect error\n" );
        return(-1);
    }
    printf( "connect mysql ok\n" );
    /* 设置一下字符编码utf8 */
    if ( mysql_set_character_set( mysql, "utf8" ) )
    {
        printf( "   mysql_set_character_set error\n");
        return(-1);
    }
    seach_information( mysql );
    /* 关闭mysql */
    mysql_close( mysql );
    return(0);
}
```

