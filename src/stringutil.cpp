#ifndef _STRINGUTIL_CPP
#define _STRINGUTIL_CPP

//#include <iostream>
//#include <stdio.h>
#include <string.h>
//#include <ctype.h>
#include "stringutil.h"
//#include <algorithm>
namespace stringutil
{
	void trimleft(char *str,char c/*=' '*/)
	{
		if(str == NULL){
			return ;
		}
		//trim head
	
		int len = strlen(str);

		int i = 0;
		while(str[i] == c && str[i] != '\0'){
			i++;
		}
		if(i != 0){
			memmove(str,str+i,len-i);
			str[len-i]=0;
		}
	}
	
	void trimright(char *str,char c/*=' '*/)
	{
		if(str == NULL){
			return ;
		}

		//trim tail
		int len = strlen(str);

		for(int i = len - 1; i >= 0; --i ){
			if(str[i] == c){
				str[i]='\0';
			}else{
				break;
			}
		}
	}

	void trim(char *str)
	{
		if(str == NULL){
			return ;
		}
		//trim head
	
		int len = strlen(str);

		int i = 0;
		while(isspace(str[i]) && str[i] != '\0'){
			i++;
		}
		if(i != 0){
			memmove(str,str+i,len-i);
			str[len-i]=0;
		}

		//trim tail
		len = strlen(str);

		for(int i = len - 1; i >= 0; --i ){
			if(isspace(str[i])){
				str[i]='\0';
			}else{
				break;
			}
		}
	}

}//end of namespace stringutil
#endif
