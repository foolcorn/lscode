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

真正执行prepare的语句，