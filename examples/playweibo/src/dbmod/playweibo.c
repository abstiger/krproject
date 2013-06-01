#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parson.h"


void *weibo_map_func_pre(void *msg)
{
    JSON_Value *jsonval = json_parse_string((char *)msg);
    return jsonval;
}


void weibo_map_func_post(void *data)
{
    json_value_free(data);
}


void weibo_map_func(void *fldval, int fldno, int fldlen, void *data)
{
    JSON_Object *status = json_value_get_object(data);
    JSON_Object *user = json_object_get_object(status, "user");
    JSON_Object *retweeted_status = json_object_get_object(status, "retweeted_status");
    JSON_Object *visible = json_object_get_object(status, "visible");
    memset(fldval, 0x00, fldlen);
    switch(fldno)
    {
        case 0:
            *(long *)fldval = (long )time(NULL);
            break;
        case 1:
            *(long *)fldval = (long )json_object_get_number(status, "created_at_time");
            break;
        case 2:
            printf("user_id=[%.0lf]\n", (double )json_object_get_number(user, "id"));
            *(double *)fldval = (double )json_object_get_number(user, "id");
            break;
        case 3:
            printf("screen_name=[%s], ",json_object_get_string(user, "screen_name"));
            memcpy(fldval, json_object_get_string(user, "screen_name"), fldlen);
            break;    
        case 4:
            memcpy(fldval, json_object_get_string(user, "name"), fldlen);
            break;
        case 5:
            printf("gender=[%s], ",json_object_get_string(user, "gender"));
            memcpy(fldval, json_object_get_string(user, "gender"), fldlen);
            break;
        case 6:
            printf("verified=[%d], ", json_object_get_boolean(user, "verified"));
            *(long *)fldval = (long )json_object_get_boolean(user, "verified");
            break;
        case 7:
            printf("followers_count=[%ld], ", (long )json_object_get_number(user, "followers_count"));
            *(long *)fldval = (long )json_object_get_number(user, "followers_count");
            break;
        case 8:
            printf("friends_count=[%ld], ", (long )json_object_get_number(user, "friends_count"));
            *(long *)fldval = (long )json_object_get_number(user, "friends_count");
            break;
        case 9:
            printf("statuses_count=[%ld], ", (long )json_object_get_number(user, "statuses_count"));
            *(long *)fldval = (long )json_object_get_number(user, "statuses_count");
            break;
        case 10:
            printf("favourites_count=[%ld], ", (long )json_object_get_number(user, "favourites_count"));
            *(long *)fldval = (long )json_object_get_number(user, "favourites_count");
            break;
        case 11:
            printf("online_status=[%ld]\n", (long )json_object_get_number(user, "online_status"));
            *(long *)fldval = (long )json_object_get_number(user, "online_status");
            break;
        case 12:
            memcpy(fldval, json_object_get_string(user, "avatar_large"), fldlen);
            break;
        case 13:
            printf("id=[%.0lf]\n", (double )json_object_get_number(status, "id"));
            *(double *)fldval = (double )json_object_get_number(status, "id");
            break;
        case 14:
            *(double *)fldval = (double )json_object_get_number(status, "mid");
            break;
        case 15:
            memcpy(fldval, json_object_get_string(status, "text"), fldlen);
            break;
        case 16:
            memcpy(fldval, json_object_get_string(status, "source"), fldlen);
            break;
        case 17:
            *(long *)fldval = json_object_get_number(status, "favorited");
            break;
        case 18:
            *(long *)fldval = json_object_get_number(status, "truncated");
            break;
        case 19:
            if (json_object_get_string(status, "thumbnail_pic") != NULL)
                memcpy(fldval, json_object_get_string(status, "thumbnail_pic"), fldlen);
            break;
        case 20:
            if (json_object_get_string(status, "bmiddle_pic") != NULL)
                memcpy(fldval, json_object_get_string(status, "bmiddle_pic"), fldlen);
            break;
        case 21:
            if (json_object_get_string(status, "original_pic") != NULL)
                memcpy(fldval, json_object_get_string(status, "original_pic"), fldlen);
            break;
        case 22:
            if (retweeted_status != NULL) 
                *(long *)fldval = (long )json_object_get_number(retweeted_status, "id");
            break;
        case 23:
            *(long *)fldval = json_object_get_number(status, "reposts_count");
            break;
        case 24:
            *(long *)fldval = json_object_get_number(status, "comments_count");
            break;
        case 25:
            *(long *)fldval = json_object_get_number(status, "attitudes_count");
            break;
        case 26:
            *(long *)fldval = json_object_get_number(visible, "type");
            break;
        case 27:
            *(long *)fldval = (long )json_object_get_number(visible, "list_id");
            break;
        default:
            break;
    }
}

