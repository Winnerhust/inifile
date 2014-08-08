#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <string>
#include <vector>
using namespace std;

namespace stringutil
{
	/*去掉str后面的c字符*/
	void trimleft(char *str,char c=' ');
	/*去掉str前面的c字符*/
	void trimright(char *str,char c=' ');
	/*去掉str前面和后面的空格符,Tab符等空白符*/
	void trim(char *str);

}

#endif
