#include <iostream>
#include <gtest/gtest.h>
#include "inifile.h"
#include "stringutil.h"

using namespace std;
using namespace stringutil;
using namespace inifile;

TEST(stringutil,trim)
{
	char buf[64] = "   aaa    ";
	char * p = buf;
	trim(p);

	EXPECT_EQ(p,string("aaa"));

	strcpy(buf, "   aaa");
	p = buf;
	trim(p);
	EXPECT_EQ(p,string("aaa"));

	strcpy(buf, "aaa    ");
	p = buf;
	trim(p);
	EXPECT_EQ(p,string("aaa"));
}

TEST(stringutil,trimleft)
{
	char buf[64] = "   aaa    ";
	char * p = buf;
	trimleft(p);

	EXPECT_EQ(p,string("aaa    "));

	strcpy(buf, "   aaa");
	p = buf;
	trimleft(p);
	EXPECT_EQ(p,string("aaa"));

	strcpy(buf, "aaa    ");
	p = buf;
	trimleft(p);
	EXPECT_EQ(p,string("aaa    "));
}

TEST(stringutil,trimright)
{
	char buf[64] = "   aaa    ";
	char * p = buf;
	trimright(p);
	
	EXPECT_EQ(p,string("   aaa"));

	strcpy(buf, "   aaa");
	p = buf;
	trimright(p);
	EXPECT_EQ(p,string("   aaa"));

	strcpy(buf, "aaa    ");
	p = buf;
	trimright(p);
	EXPECT_EQ(p,string("aaa"));
}

////////////////////////////

TEST(IniSection,pasre)
{
	IniSection section("COMMON");
	string s = "DB=sys";
	string key,value;

	EXPECT_EQ(section.parse(s.c_str(),key,value),true);
	EXPECT_EQ(key,"DB");
	EXPECT_EQ(value,"sys");
	
	s= "DB";
	key = value = "";
	
	EXPECT_EQ(section.parse(s.c_str(),key,value),false);
	EXPECT_EQ(key,"");
	EXPECT_EQ(value,"");

	s= "DB=";
	key = value = "";
	
	EXPECT_EQ(section.parse(s.c_str(),key,value),true);
	EXPECT_EQ(key,"DB");
	EXPECT_EQ(value,"");

	s= "=sys";
	key = value = "";
	
	EXPECT_EQ(section.parse(s.c_str(),key,value),true);
	EXPECT_EQ(key,"");
	EXPECT_EQ(value,"sys");
}

TEST(IniSection,add_from_raw_string)
{
	IniSection section("COMMON");
	string key,value;

	string s = "DB=sys";
	EXPECT_EQ(section.add_from_raw_string(s.c_str()),true);

	
	s="DB=user";
	EXPECT_EQ(section.add_from_raw_string(s.c_str()),true);


	s="PASSWD=tt";
	EXPECT_EQ(section.add_from_raw_string(s.c_str()),true);
	
	for(IniSection::iterator it = section.begin(); it != section.end(); it++){
		if(it->key == "DB"){
			EXPECT_EQ((it->value=="user")||(it->value)=="sys",true);
			cout<<"value:"<<it->value<<endl;
		}else if(it->key == "PASSWD"){
			EXPECT_EQ(it->value,"tt");
		}
	}
}

TEST(IniSection,get)
{
	string key,value;

	IniSection section("COMMON");

	string s = "DB=sys";
	EXPECT_EQ(section.add_from_raw_string(s.c_str()),true);

	s="PASSWD=tt";
	EXPECT_EQ(section.add_from_raw_string(s.c_str()),true);
	
	EXPECT_EQ(section.get("DB"),string("sys"));
	EXPECT_EQ(section.get("PASSWD"),string("tt"));
}

//////////////////////

TEST(IniFile,getSection_and_getValue)
{
	//create a new ini file
	char filepath[] = "test/test.ini";
	FILE *fp = fopen(filepath,"w");
	char content[]=" USER=root \r\n [COMMON] \n DB=sys   	\nPASSWD=tt   \n#commit   \n ;--------- \n[DEFINE] \nname=cxy\n";
	fwrite(content,1,sizeof(content),fp);
	fclose(fp);

	//test
	IniFile ini;
	ini.open(filepath);

	EXPECT_NE(ini.getSection(""),(IniSection *)NULL);
	EXPECT_NE(ini.getSection("COMMON"),(IniSection *)NULL);
	EXPECT_NE(ini.getSection("DEFINE"),(IniSection *)NULL);

	
	EXPECT_EQ(ini.getValue("","USER"),string("root"));
	EXPECT_EQ(ini.getValue("COMMON","DB"),string("sys"));
	EXPECT_EQ(ini.getValue("COMMON","PASSWD"),string("tt"));
	EXPECT_EQ(ini.getValue("DEFINE","name"),string("cxy"));
	EXPECT_EQ(ini.getValue("DEFINE","tt"),(char *)NULL);

}

TEST(IniFile,reopen)
{
	//create a new ini file
	char filepath[] = "test/test.ini";
	FILE *fp = fopen(filepath,"w");
	char content[]=" USER=root \r\n [COMMON] \n DB=sys   	\nPASSWD=tt   \n#commit   \n ;--------- \n[DEFINE] \nname=cxy\n";
	fwrite(content,sizeof(char),strlen(content),fp);
	fclose(fp);

	//test
	IniFile ini;
	ini.open(filepath);

	EXPECT_NE(ini.getSection(""),(IniSection *)NULL);
	EXPECT_NE(ini.getSection("COMMON"),(IniSection *)NULL);
	EXPECT_NE(ini.getSection("DEFINE"),(IniSection *)NULL);

	
	EXPECT_EQ(ini.getValue("","USER"),string("root"));
	EXPECT_EQ(ini.getValue("COMMON","DB"),string("sys"));
	EXPECT_EQ(ini.getValue("COMMON","PASSWD"),string("tt"));
	EXPECT_EQ(ini.getValue("DEFINE","name"),string("cxy"));
	EXPECT_EQ(ini.getValue("DEFINE","tt")==NULL,true);
	
	//reopen
	
	fp = fopen(filepath,"w");
	strcpy(content," USER=root2 \r\n [COMMON] \n DB=sys2   	\n\n#commit   \n ;--------- \n\n\n");
	fwrite(content,sizeof(char),strlen(content),fp);
	fclose(fp);

	//test
	ini.open(filepath);

	EXPECT_NE(ini.getSection(""),(IniSection *)NULL);
	EXPECT_NE(ini.getSection("COMMON"),(IniSection *)NULL);
	EXPECT_EQ(ini.getSection("DEFINE"),(IniSection *)NULL);

	
	EXPECT_EQ(ini.getValue("","USER"),string("root2"));
	EXPECT_EQ(ini.getValue("COMMON","DB"),string("sys2"));
	EXPECT_EQ(ini.getValue("COMMON","PASSWD")==NULL,true);
	EXPECT_EQ(ini.getValue("DEFINE","name")==NULL,true);
	EXPECT_EQ(ini.getValue("DEFINE","tt")==NULL,true);

}

#ifdef GTEST_MAIN
#undef GTEST_MAIN

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif
