#include "kr_calc_parser.h"

int kr_calc_parse(T_KRCalc *krcalc)
{
    int ret = -1;
    switch(krcalc->calc_format)
    {
        case KR_CALCFORMAT_FLEX:
            ret = kr_calc_parse_flex(krcalc);
            break;
        case KR_CALCFORMAT_JSON:
            ret = kr_calc_parse_json(krcalc);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupport format %c", krcalc->calc_format);
            break;
    }

    return ret;
}
