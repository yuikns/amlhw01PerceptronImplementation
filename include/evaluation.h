#ifndef __EVALUATION_H__
#define __EVALUATION_H__

class evaluation
{
public:
	evaluation();
	virtual ~evaluation();
	void add(bool predicted,bool actual);
	double precision();
	double recall();
	double f1score();
	void reset();
	
protected:
private:
	int tp;
	int tn;
	int fp;
	int fn;
};

evaluation::evaluation()
{
	reset();
}

evaluation::~evaluation()
{
}

/**
 *
 */
void evaluation::add(bool predicted,bool actual)
{
	if(predicted && actual ) tp++;
	else if(!predicted && actual ) fn++;
	else if(predicted && !actual ) fp++;
	else if(!predicted && !actual ) tn++;
}

double evaluation::precision()
{
	if(tp+fp == 0 ) return 0;
	else return (double) tp / (tp + fp);
}

double evaluation::recall()
{
	if(tp+fn == 0 ) return 0;
	else return (double) tp / (tp + fn);
}

double evaluation::f1score()
{
	double p = precision();
	double r = recall();
	if(p + r == 0 ) return 0;
	return 2 * p * r / (p + r) ;
}

void evaluation::reset()
{
	tp = tn = fp = fn =  0;
}




#endif // __EVALUATION_H__


