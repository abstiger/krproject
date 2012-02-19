#ifndef __UTILS_H__
#define __UTILS_H__

typedef int boolean;

#ifndef KR_FALSE
#define KR_FALSE 0
#endif

#ifndef KR_TRUE
#define KR_TRUE 1
#endif

#ifndef KR_SUCCESS
#define KR_SUCCESS 0
#endif

#ifndef KR_FAILURE
#define KR_FAILURE 1
#endif

typedef int TokenType;

/* ExpKind is used for node describing */
typedef enum {
    OpK,FidK,SidK,DidK,AggrK,RegexK,NumK,FnumK,CharK
} ExpKind;

/* Type is used for type checking */
typedef enum {
    KR_INTEGER,KR_DOUBLE,KR_STRING,KR_BOOLEAN
}KRType;

/* Values is used for storing node value */
typedef union {
      int    i;
	  double d;
	  char  *s;
}KRValue;

#endif /* __UTILS_H__ */
