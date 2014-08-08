inifile
=======

一个轻量级的INI文件解析库,使用gtest和valgrind做过测试

使用很简单
生成一个ini文件`test.ini`
```
>cat > test.ini
#this is commit
;this is commit
[COMMON]
DB = mysql
PASSWD=root

```
支持以“#”或者“;"开头作为注释

首先指定头文件和命名空间
然后使用open函数打开ini文件
getSection可以获取段
getValue可以获取指定段的指定项的值


```c++
#include "inifile.h"
using namespace inifile;

filepath = "test.ini";
 
IniFile ini;
ini.open(filepath);

//获取段 
IniSection * sec = ini.getSection("COMMON");
//获取指定段的指定项的值
string db_name = ini.getValue("COMMON","DB");

```
