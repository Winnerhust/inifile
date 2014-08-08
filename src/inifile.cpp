#ifndef _INIFILE_CPP
#define _INIFILE_CPP

#include "stringutil.h"
#include "inifile.h"
#include <stdlib.h>
#include <stdio.h>
namespace inifile{
using namespace stringutil;

int INI_BUF_SIZE=2048;

/**-----------IniSection------------------**/
bool IniSection::parse(const char *content,string &key,string &value,char c/*= '='*/)
{
	int i = 0;
	int len = strlen(content);

	while(i < len && content[i] != c){
		++i;
	}
	if(i >= 0 && i < len){
		key = string(content,i);
		value = string(content+i+1,len-i-1);
		return true;
	}

	return false;
}
bool IniSection::add_from_raw_string(const char *content,char c /*= '=' */)
{
	struct IniItem item;

	if(parse(content,item.key,item.value,c)){
		items_.push_back(item);
		return true;
	}

	return false;
}

/**-----------IniFile------------------**/
int IniFile::open(const char *filename)
{	
	release();

	int buf_size = INI_BUF_SIZE;
	IniSection *section = NULL;
	char *buf =(char *) malloc(INI_BUF_SIZE*sizeof(char));
	FILE *fp = fopen(filename,"r");
	
	if(fp == NULL || buf == NULL){
		return -1;
	}

	char *p = buf;

	while(fgets(p,INI_BUF_SIZE-1,fp) != NULL){
		int plen = strlen(p);

		if( plen > 0 && p[plen-1] != '\n' && !feof(fp)){
	
			buf_size *= 2;

			if((buf = (char *)realloc(buf,buf_size)) == NULL){
				fprintf(stderr,"no enough memory!exit!\n");
				exit(-1);
			}

			p = buf + buf_size/2;
			continue;
		}

		p = buf;

		trimright(buf,'\n');
		trimright(buf,'\r');
		trim(buf);

		if(buf[0] == ';' || buf[0] == '#' || buf[0] == '\0') continue;
		
		if(buf[0] == '['){
			section = NULL;
			char *sct = strrchr(buf,']');
			
			if(sct == NULL){
				fclose(fp);
				fprintf(stderr,"没有找到匹配的]\n");
				return -1;
			}
			int len = sct-1-buf;
			if(len <= 0){
				fprintf(stderr,"段为空\n");
				continue;
			}
			string s(buf+1,len);

			if(getSection(s.c_str()) != NULL){
				fclose(fp);
				fprintf(stderr,"此段已存在:%s\n",s.c_str());
				return -1;
			}
			
			section = new IniSection(s.c_str());
			sections_[s] = section;
			continue;
		}
		if(section == NULL){
			section = getSection("");
			if ( section == NULL ){//默认段
				section = new IniSection("");
				sections_[""] = section;
			}
		}

		section->add_from_raw_string(buf);

	}
	
	fclose(fp);

	free(buf);
}


IniSection *IniFile::getSection(const char *sct /*= NULL*/)
{
	if(sct != NULL){
		iterator it = sections_.find(sct);
		if(it != sections_.end()){
			return it->second;
		}
	}else if(sections_.size() == 1){
		return sections_.begin()->second;
	}

	return NULL;
}

const char *IniFile::getValue(const char *sct,const char *key)
{
	IniSection * sect = getSection(sct);

	if(sect != NULL){
		return sect->get(key);
	}

	return NULL;
}

void IniFile::release()
{
	for(iterator i = sections_.begin(); i != sections_.end(); ++i){
		delete i->second;
	}

	sections_.clear();
}
}
#endif
