#ifndef __KR_CALC_PARSER_H__
#define __KR_CALC_PARSER_H__

#include "kr_calc.h"

extern int kr_calc_parse_json(T_KRCalc *krcalc);
extern int kr_calc_parse_flex(T_KRCalc *krcalc);

extern int kr_calc_parse(T_KRCalc *krcalc);

#endif /* __KR_CALC_PARSER_H__ */
