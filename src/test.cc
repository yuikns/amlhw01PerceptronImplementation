#include "evaluation.h"

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
	if(!is_dir) pfilelist->push_back(string(filename));
	return 1;
}


evaluation do_classify(vector<string> tfblist,vector<string> tfhlist,string mkname)
{
	struct stemmer * z = create_stemmer();
	evaluation e;
	perceptron m;
	m.load_model(mkname.c_str());
	vector<string> wordseq = m.get_word_seq();
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
			int label = m.predict(doc);
			//printf("predicted : %s , actual : %s \n",label>0?"baseball":"hockeypos","baseball");
			e.add(label>0,1);
	}
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
			int label = m.predict(doc);
			//printf("predicted : %s , actual : %s \n",label>0?"baseball":"hockeypos","hockeypos");
			e.add(label>0,0);
	}
	printf("%s \t precision : %0.4f%% , recall : %0.4f%% , f1 score : %0.4f%%\n",mkname.c_str(),e.precision() * 100,e.recall() * 100,e.f1score() * 100);
	free_stemmer(z);
	return e;
}


int main(int argc,char *argv[])
{
	int cvcount = 5;
	double psum = 0;
	double rsum = 0;
	double f1sum = 0;
	for(int i = 0; i < cvcount ; i ++)
	{
		vector<string> tfblist; //target file : baseball list
		vector<string> tfhlist; //target file : hockey list
		char dirbuff[1024];
		snprintf(dirbuff,1024,"output/s%d/baseball",i+1);
		dirTraversal(dirbuff,true,add_file,&tfblist);
		snprintf(dirbuff,1024,"output/s%d/hockey",i+1);
		dirTraversal(dirbuff,true,add_file,&tfhlist);
		char mkbuff[40];
		//sprintf(mkbuff,"output/s%d.model",i+1);
		sprintf(mkbuff,"s%d",i+1);
		evaluation e = do_classify(tfblist,tfhlist,string(mkbuff));
		
		psum += e.precision();
		rsum += e.recall();
		f1sum += e.f1score();
		
	}
	printf("final result : \nprecision : %0.4f%% , recall : %0.4f%% , f1 score : %0.4f%%\n",psum * 100 / cvcount,rsum * 100 / cvcount,f1sum * 100 / cvcount);
	printf("classified done\n");
	return 0;
}



