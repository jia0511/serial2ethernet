
/*
 * 		 功能:        从字符串中截取数据 
 * 	 	 licensePlate:参---->值  返回车牌
 * 		 data:        从data参数中截取车牌
 * 		 appoint:     等于KEY_VALUE截取格式"key":"value",等于_VALUE截取格式value
 * 		 type: 		  等于STRING 要截取的value为string or char类型等于NUMERIC要截取的value为int or float 类型
 * 		 str:         要截取的字段 字符串
 * 		 返回值:      有找到返回Y 没有找到返回N
 * */
#include "readjs.h"

char dealwith_InterceptLicensePlate(char *licensePlate,const char *data,char appoint,char type,char *str){
	char *p = NULL,*np = NULL;
	unsigned char i;
	if(KEY_VALUE == appoint){
		if(NULL == (p = strstr(data,str))){
			printf("error:not found licensePlate return 'N'\n");
			return 'N';
		}else{
			for(i = 0; i < strlen(data) ;i ++){

				if(p[i] != ',' && p[i] != '}' && p[i] != ']')
					licensePlate[i] = p[i];	
				else if(p[i] == ',' || p[i] == '}' || p[i] == ']')
					break;
				else if(i >= MIN){
					printf("error:not found licensePlate is i value max\n\n");return 'N';
				}
			}/*end for*/
			return 'Y';
		}/*end else*/
	}/*end 1== appoint*/

	else if(_VALUE == appoint){
		if(NULL == (p = strstr(data,str))){
			printf("error:not found licensePlate return N\n");return 'N';

		}else if(STRING == type){ 
			if(NULL == (p =  strstr(p,":\""))){
				printf("error:input data error\n");return 'N';}

		}else if(NUMERIC == type){
			if(NULL == (p = strstr(p,":"))){
				printf("error:input data error\n");return 'N';}

		}
	}else{
		printf("error:appoint value error\n");
		return 'N';
	}

	if(NUMERIC == type){/*is ---->  _VALUE and NUMERIC*/

		p +=1;      /*截掉:所以要加1     */
		for(i = 0 ;i < MIN ;i++){
			if(p[i] != ',' && p[i] != ']' && p[i] != '}') 		licensePlate[i] = p[i];

			else if(p[i] == ',' || p[i] == ']' || p[i] == '}') 	return 'Y';

			else if(i >= MIN){ printf("error:not input data error\n"); return 'N';}
		}/*end for*/

	}else if(STRING == type){/*is -----> _VALUE and STRING*/

		p += 2; 	/*截掉:"所以要加2    */
		for(i = 0;i < MIN ;i++){

			if(p[i] != '\"')	 		licensePlate[i] = p[i];

			else if(p[i] == '\"') 		break;

			else if(i >= MIN){
				printf("error:not input data error\n");return 'N';}

		}/*end for*/

		return 'Y';

	}/*end else if*/
}

