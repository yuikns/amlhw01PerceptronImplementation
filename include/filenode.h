#ifndef __FILE_NODE_H__
#define __FILE_NODE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
char fullname[1024]; // name with full path
char name[1024]; // name only
}filenode;

#ifdef __cplusplus
}
#endif


#endif // __FILE_NODE_H__
