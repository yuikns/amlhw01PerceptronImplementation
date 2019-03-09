#include "dirtrav.h"
#include <vector>
#include <map>
#include <string>
#include <stdio.h>

#include <stdlib.h>      // for malloc, free 
#include <ctype.h>       // for isupper, islower, tolower 
#include <string.h>		// for memset

#include <queue>
#include <set>
#include <algorithm>

#include "stemmer.h"
#include "stemmer_ext.h"

#include "perceptron.h"

using namespace std;


int add_file(const char * filename, int is_dir,void *pfl){
	vector<string > * pfilelist = (vector<string > * ) pfl;
	//printf("[%c]%s--%d\n",is_dir?'D':'F',file_name,(*((int *)usr))++);
	pfilelist->push_back(string(filename));
	return 1;
}

class cmpviaval
{
	bool reverse;
public:
	cmpviaval(const bool& revparam=false){
		reverse=revparam;
	}
	bool operator() (const std::pair<string,int>& lhs, const std::pair<string,int>&rhs) const
	{
		if (reverse) return (lhs.second < rhs.second);
		else return (lhs.second > rhs.second);
	}
};



int model_train(vector<string> tfblist,vector<string> tfhlist,string mkname)
{
	// 1. calculate frequency
	printf("%s reading data ...",mkname.c_str());
	struct stemmer * z = create_stemmer();
	map<string,int> wordmap;
	// stopwords
	std::set<string> stopwords;
	vector<string> stopwordsvec = stemfile("stopwords.txt",z);
	for(vector<string>::size_type ix = 0; ix != stopwordsvec.size(); ix++){
		stopwords.insert(stopwordsvec[ix]);
	}
	for(vector<string>::size_type ix = 0; ix != tfblist.size(); ix++)
	{
			vector<string> baseballpos = stemmail(tfblist[ix].c_str(),z);
			for(vector<string>::size_type jx = 0; jx != baseballpos.size(); jx++)
			{
				map<string,int>::iterator it = wordmap.find(baseballpos[jx]);
				if(it != wordmap.end())
				{
					//printf("%s %d => %d \n",it->first.c_str(),it->second,it->second+1);
					it->second ++;
				}else {
					//printf("%s -- inserted \n",baseballpos[jx].c_str());
					wordmap.insert ( std::pair<string,int>(baseballpos[jx],1));
				}
			}
	}
	for(std::vector<string>::size_type ix = 0; ix != tfhlist.size(); ix++)
	{
			vector<string> hockeypos = stemmail(tfhlist[ix].c_str(),z);
			for(vector<string>::size_type jx = 0; jx != hockeypos.size(); jx++)
			{
				map<string,int>::iterator it = wordmap.find(hockeypos[jx]);
				if(it != wordmap.end())
				{
					//printf("%s %d => %d \n",it->first.c_str(),it->second,it->second+1);
					it->second ++;
				}else {
					//printf("%s -- inserted \n",hockeypos[jx].c_str());
					wordmap.insert ( std::pair<string,int>(hockeypos[jx],1));
				}
			}
	}
	printf("done\n");
	printf("all extracted word size : %d \n", (int)wordmap.size());
	
	printf("%s generate word frequency sequence ...",mkname.c_str());
	// 2. reread each file, calculate word frequency in one document
	priority_queue<std::pair<string,int>,vector<std::pair<string,int> >,cmpviaval > wordminheap;// baseball/ min heap
	
	for(map<string,int>::iterator it = wordmap.begin() ; it != wordmap.end() ; it++)
	{
		if(stopwords.find(it->first) != stopwords.end()) continue;
		if(wordminheap.size() < FEATURE_NUM)
		{
			wordminheap.push(*it);
		}else if(wordminheap.top().second < it->second)
		{
			wordminheap.push(*it);
			wordminheap.pop();
		}
	}
    printf("done\n");
    
    vector<string> wordseq; // baseball +1
    while(wordminheap.size() > 0)
    {
    	wordseq.push_back(wordminheap.top().first);
    	//printf("%s-%d-%d\n",wordminheap.top().first.c_str(),wordminheap.top().second,imh++);
    	wordminheap.pop();
    }
    reverse(wordseq.begin(),wordseq.end());
    
    perceptron lm;
    lm.set_word_seq(wordseq);
    
    printf("%s set predict dataset ...",mkname.c_str());
    // set train dataset
    for(vector<string>::size_type ix = 0; ix != tfblist.size(); ix++) // +1
	{
			vector<string> baseballpos = stemmail(tfblist[ix].c_str(),z);
			fsdoc doc ;
			doc.label = 1;
			int all_str_size = baseballpos.size();
			map<string,int> cdcmap;
			for(vector<string>::size_type jx = 0; jx != baseballpos.size(); jx++)
			{
				map<string,int>::iterator it = cdcmap.find(baseballpos[jx]);
				if(it != cdcmap.end())
				{
					it->second ++;
				}else {
					cdcmap.insert ( std::pair<string,int>(baseballpos[jx],1));
				}
			}
			for(vector<string>::size_type jx = 0 ; jx != wordseq.size(); jx ++)
			{
				map<string,int>::iterator it = cdcmap.find(wordseq[jx]);
				doc.x[jx] = (it == cdcmap.end()) ? 0 : (double)it->second / all_str_size;
			}
			
			lm.add_doc(doc);
	}
	printf("...");
	for(std::vector<string>::size_type ix = 0; ix != tfhlist.size(); ix++) // -1
	{
			vector<string> hockeypos = stemmail(tfhlist[ix].c_str(),z);
			fsdoc doc ;
			doc.label = -1;
			int all_str_size = hockeypos.size();
			map<string,int> cdcmap;
			for(vector<string>::size_type jx = 0; jx != hockeypos.size(); jx++)
			{
				map<string,int>::iterator it = cdcmap.find(hockeypos[jx]);
				if(it != cdcmap.end())
				{
					it->second ++;
				}else {
					cdcmap.insert ( std::pair<string,int>(hockeypos[jx],1));
				}
			}
			for(vector<string>::size_type jx = 0 ; jx != wordseq.size(); jx ++)
			{
				map<string,int>::iterator it = cdcmap.find(wordseq[jx]);
				doc.x[jx] = (it == cdcmap.end()) ? 0 : (double)it->second / all_str_size;
			}
			
			lm.add_doc(doc);			
	}
	printf("done\n");
	printf("%s start train ... ",mkname.c_str());
	lm.learn_doc();
    printf("done\n");
    printf("%s store model ... ",mkname.c_str());
	bool status = lm.store_model(mkname.c_str());
	printf("%s done\n",status ? "success" : "fail");
	free_stemmer(z);
	return 1;
}


int main(int argc , char * argv [])
{
	//Cross-validation Count
	int cvcount = 5;
	for(int i = 0; i < cvcount ; i ++)
	{
		vector<string> tfblist; //target file : baseball list
		vector<string> tfhlist; //target file : hockey list
		for(int j = 0 ; j < cvcount ; j ++)
		{
			if(i==j) continue;
			char dirbuff[1024];
			snprintf(dirbuff,1024,"output/s%d/baseball",j+1);
			dirTraversal(dirbuff,true,add_file,&tfblist);
			snprintf(dirbuff,1024,"output/s%d/hockey",j+1);
			dirTraversal(dirbuff,true,add_file,&tfhlist);
		}
		char mkbuff[40];
		//sprintf(mkbuff,"output/s%d.model",i+1);
		sprintf(mkbuff,"s%d",i+1);
		model_train(tfblist,tfhlist,string(mkbuff));
	}
	printf("all done\n");
	
	return 0;
}



