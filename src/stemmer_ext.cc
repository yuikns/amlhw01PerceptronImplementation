#include "stemmer.h"
#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;
vector<string> stemfile(const char * filename,struct stemmer * z)
{
	vector<string> pos;
	FILE * fp = fopen(filename,"r");
  	if (fp == NULL) { fprintf(stderr,"File %s not found\n",filename); return pos; }
  	size_t sz = 0;
	char *buff = NULL;
	char *cbuff = NULL;
	fseek(fp, 0L, SEEK_END);  
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buff = (char *)malloc(sizeof(char)*sz);
	cbuff = (char *)malloc(sizeof(char)*sz);
	fread(buff,sz,1,fp);
  	//printf("%s\n",buff);
  	memset(cbuff,0,sz);
  	int j = 0;
  	for(size_t i = 0 ; i < sz ; i ++)
  	{
  		char ch = buff[i];
  		if(isupper(ch) || islower(ch))
  		{
  			cbuff[j] = tolower(ch); // forces lower case
  			cbuff[stem(z,cbuff, j) + 1] = 0;
  			j++;
  		}else if(strlen(cbuff) > 0 )
  		{
  			pos.push_back(string(cbuff));
  			memset(cbuff,0,sz);
  			j = 0;
  		}
  	}
  	free(cbuff);
  	free(buff);
  	fclose(fp);
  	return pos;
}

vector<string> stemmail(const char * filename,struct stemmer * z)
{
	vector<string> pos;
	FILE * fp = fopen(filename,"r");
  	if (fp == NULL) { fprintf(stderr,"File %s not found\n",filename); return pos; }
  	size_t sz = 0;
	char *buff = NULL;
	char *cbuff = NULL;
	fseek(fp, 0L, SEEK_END);  
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buff = (char *)malloc(sizeof(char)*sz);
	cbuff = (char *)malloc(sizeof(char)*sz);
	fread(buff,sz,1,fp);
  	//printf("%s\n",buff);
  	memset(cbuff,0,sz);
  	
  	
  	int j = 0;
  	for(size_t i = 0 ; i < sz ; i ++)
  	{
  		char ch = buff[i];
  		if(isupper(ch) || islower(ch))
  		{
  			cbuff[j] = tolower(ch); // forces lower case
  			cbuff[stem(z,cbuff, j) + 1] = 0;
  			j++;
  		}else if(strlen(cbuff) > 0 )
  		{
  			pos.push_back(string(cbuff));
  			memset(cbuff,0,sz);
  			j = 0;
  		}
  	}
  	free(cbuff);
  	free(buff);
  	fclose(fp);
  	return pos;
}


