#include "krutils/kr_utils.h"
#include <time.h>
#include <math.h>

typedef struct _my_record_t 
{
    int     i;
    double  d;
    char    s[20];
}T_MyRecord;

T_MyRecord gstRecord[10] = {0};

static inline int record_compare(void *p1, void *p2, void *sort_field)
{
    T_MyRecord *ptRec1 = (T_MyRecord *)p1;
    T_MyRecord *ptRec2 = (T_MyRecord *)p2;
printf("    record_compare[%d] [%d]\n", ptRec1->i, ptRec2->i);
    return (int)(ptRec1->i - ptRec2->i);
}

static inline void print_record(void *value, void *user_data)
{
    T_MyRecord *ptRecord = (T_MyRecord *)value;
    printf("Record:i=[%d],d=[%lf],s=[%s]\n", ptRecord->i, ptRecord->d, ptRecord->s);
}

int main(int argc,char *argv[])
{
    int i = 0;
	T_KRList *ptList = kr_list_new();
	kr_list_set_compare(ptList, (KRCompareDataFunc )record_compare);
	
	srandom((unsigned int)time(NULL));
	
	for (i=0; i<10; i++) {
	    gstRecord[i].i=random()%9;
	    gstRecord[i].d = i;
	    snprintf(gstRecord[i].s, 20, "record_%d", i);

printf("kr_list_add_sorted[%d] [%d]\n", i, gstRecord[i].i);	  
        //kr_list_add_tail(ptList, &gstRecord[i]);
	    kr_list_add_sorted(ptList, &gstRecord[i], NULL);
	}
	
	kr_list_foreach(ptList, print_record, NULL);
	
	T_KRListNode *ptNode = kr_list_search(ptList, &gstRecord[7]);
	if (ptNode != NULL) {
	    print_record(kr_list_value(ptNode), NULL);
	}
        
    kr_list_destroy(ptList);

	return 0;
}


