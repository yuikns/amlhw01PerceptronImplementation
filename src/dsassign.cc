/**
 * step 1 : dataset assign
 */
#include "ini.h"
#include "dirtrav.h"
#include "filenode.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <string>



using namespace std;

int read_basedir(void * basedir, const char* section, const char* name, const char* value);
int get_file(const char * file_name, int is_dir,void * usr);
inline int copy_file(const char *dest,const char *source);

int read_basedir(void * basedir, const char* section, const char* name, const char* value)
{
	if(strcmp(section,"basic") == 0 && strcmp(name,"dataset") == 0 )
	{
		memccpy((char *)basedir,value,strlen(value),1024);
	}
	return 1;
}


/**
 * read files , set file list
 */
int get_file(const char * file_name, int is_dir,void *usr){
	vector<filenode > * pfilelist = (vector<filenode > * ) usr;
	//printf("[%c]%s--%d\n",is_dir?'D':'F',file_name,(*((int *)usr))++);
	int len = strlen(file_name);
	filenode fn;
	memccpy(fn.fullname,file_name,strlen(file_name),sizeof(fn.fullname) -1);
	bool name_setted = false;
	for(int i = len - 2 ; i >=0 ; i--) 
	// why i start from len - 2 ?  len - 1 may the last word
	{
		if( file_name[i] == '/' || file_name[i] == '\\')
		{
			name_setted = true;
			memccpy(fn.name,file_name+i+1,strlen(file_name + i + 1),sizeof(fn.name));
			break;
		}
	}
	if(!name_setted)
	{
		memccpy(fn.name,fn.fullname,strlen(fn.fullname),sizeof(fn.name) -1);
	}
	pfilelist->push_back(fn);
	return 1;
}

inline int copy_file(const char *dest,const char *source)
{
	FILE * fin = NULL;
	FILE * fout = NULL;
	if((fin = fopen(source,"rb")) == NULL){
		fprintf(stderr,"error: can not open %s ! \n", source);
		return 0;
	}
	if((fout = fopen(dest,"wb")) == NULL){
		fprintf(stderr,"error: can not open %s ! \n", dest);
		return 0;
	}
	size_t sz = 0;
	char *buff = NULL;
	fseek(fin, 0L, SEEK_END);  
	sz = ftell(fin);
	fseek(fin, 0L, SEEK_SET);
	buff = (char *)malloc(sizeof(char)*sz);
	fread(buff,sz,1,fin);
	fwrite(buff,sz,1,fout);
	free(buff);
	fclose(fin);
	fclose(fout);
	return 1;
}

int main(int argc, char * argv[])
{
	srand((unsigned int)time(NULL)); // for random
	const char * cfg_filename = "config.ini";
	char basedir[1024];
	memset(basedir,0,1024);
	if(ini_parse(cfg_filename,read_basedir,basedir) != 0){
		fprintf(stderr,"read config error\n");
		return -1;
	}
	// handler these files
	vector<filenode > baseballfl;// baseball file list
	vector<filenode > hockeyfl; // hockey file list
	char baseballbd[1040]; // baseball base dir
	char hockeybd[1040]; // hockey base dir
	sprintf(baseballbd,"%s/baseball",basedir);
	sprintf(hockeybd,"%s/hockey",basedir);
	dirTraversal(baseballbd,true,get_file,&baseballfl);
	dirTraversal(hockeybd,true,get_file,&hockeyfl);
	printf("baseball file count : %d , hockey file count : %d \n",(int)baseballfl.size(),(int)hockeyfl.size());
	//Cross-validation Count
	int cvcount = 5;
	mkdir("output",0775);
	for(int i = 0 ; i < cvcount ; i ++)
	{
		char subdir[30];
		sprintf(subdir,"output/s%d",i+1);
		// mkdir , sys/stat.h
		mkdir(subdir,0775);
		sprintf(subdir,"output/s%d/baseball",i+1);
		mkdir(subdir,0775);
		sprintf(subdir,"output/s%d/hockey",i+1);
		mkdir(subdir,0775);
	}
	
	printf("assign baseball ...");
	int cpcount = 0;
	for(std::vector<filenode>::size_type ix = 0; ix != baseballfl.size(); ix++)
	{
		char subdir[30];
		sprintf(subdir,"output/s%d/baseball/%s",rand()%cvcount+1,baseballfl[ix].name);
		//printf("%s => %s\n",baseballfl[ix].fullname,subdir);
		copy_file(subdir,baseballfl[ix].fullname);
		cpcount ++;
	}
	printf("all %d done \nassign hockey ...",cpcount);
	
	cpcount = 0;
	for(std::vector<filenode>::size_type ix = 0; ix != hockeyfl.size(); ix++)
	{
		char subdir[30];
		sprintf(subdir,"output/s%d/hockey/%s",rand()%cvcount+1,hockeyfl[ix].name);
		//printf("%s => %s\n",hockeyfl[ix].fullname,subdir);
		copy_file(subdir,hockeyfl[ix].fullname);
		cpcount ++;
	}
	printf("all %d done\n",cpcount);
	printf("all done \n");
	return 0;
}

