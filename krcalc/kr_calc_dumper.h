#ifndef __KR_CALC_DUMPER_H__
#define __KR_CALC_DUMPER_H__

#include "kr_calc.h"

extern char *kr_calc_dump_flex(T_KRCalc *krcalc);
extern char *kr_calc_dump_json(T_KRCalc *krcalc);

extern char *kr_calc_dump(T_KRCalc *krcalc, E_KRCalcFormat format);

#endif /* __KR_CALC_DUMPER_H__ */
