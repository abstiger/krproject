#include "kr_calc_dumper.h"

char *kr_calc_dump(T_KRCalc *krcalc, E_KRCalcFormat format)
{
    char *dump_str = NULL;
    switch(format) 
    {
        case KR_CALCFORMAT_FLEX:
            dump_str = kr_calc_dump_flex(krcalc);
            break;
        case KR_CALCFORMAT_JSON:
            dump_str = kr_calc_dump_json(krcalc);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupport format %c", format);
            break;
    }

    return dump_str;
}
