#ifndef _INIFILE_H
#define _INIFILE_H

#include <map>
#include <vector>
#include <string>
#include <string.h>

using namespace std;
namespace inifile
{

struct IniItem
{
	string key;
	string value;
};
class IniSection
{
public:
	IniSection(const char * name):name_(name){}
	~IniSection(){ items_.clear(); }
public:	
	typedef vector<IniItem>::iterator iterator;
	iterator begin() {return items_.begin();}
	iterator end() {return items_.end();}
public:
	const char * get(const char *key)
	{
		for(iterator it = items_.begin(); it != items_.end(); ++it){
			if (it->key == key){
				return it->value.c_str();
			}
		}

		return NULL;
	}
	bool add_from_raw_string(const char *content,char c = '=');
	bool parse(const char *content,string &key,string &value,char c = '=');


private:
	string name_;
	vector<IniItem> items_;
};

class IniFile
{
public:	
	IniFile(){}
	~IniFile(){release();}

public:
	typedef map<string,IniSection *>::iterator iterator;

	iterator begin() {return sections_.begin();}
	iterator end() {return sections_.end();}
public:
	int open(const char *filename);

	IniSection *getSection(const char *sct = NULL);
	const char *getValue(const char *sct,const char *key);
protected:
	void release();
private:
	map<string,IniSection *> sections_;


};

}

#endif
