#/bin/sh

LOG_FILE=initall.log
##Ω® ”Õº
for i in `ls -l init_*.sql|awk '{print $8}'`
do
	TABLE_NAME=`echo init table $i | sed "s/\.sql//g"`
	echo "init table ${TABLE_NAME} ...\c"
	sqlplus ${DBUSER}/${DBPASS} < ${i} >> ${LOG_FILE} 2>&1
	if [ $? -ne 0 ];then
    	echo "[`date +'%Y-%m-%d %H:%M:%S'`] ${VIEW_NAME} failed" >> ${LOG_FILE} 2>&1
    	exit 1;
	fi	
	echo "ok"
	echo "[`date +'%Y-%m-%d %H:%M:%S'`]  ${VIEW_NAME} ok" >> ${LOG_FILE} 2>&1
done