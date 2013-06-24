#/bin/sh

LOG_FILE="initall.log"

echi " ">$LOG_FILE;
echo "Start at:[`date +'%Y-%m-%d %H:%M:%S'`]!"|tee -a $LOG_FILE;
echo "dsn:$DBNAME, dbuser:$DBUSER, dbpass:$DBPASS"|tee -a $LOG_FILE;

for initfile in `ls init_*.sql`
do
    table=`echo "$initfile" |sed "s/init_//g"| sed "s/\.sql//g"`;
	echo "  initializing table $table ..."|tee -a $LOG_FILE;

    isql -b $DBNAME $DBUSER $DBPASS < $initfile >> $LOG_FILE
	if [ $? -ne 0 ];then
    	echo "  failed!"|tee -a $LOG_FILE 2>&1
    	exit 1;
	fi	
	echo "  ok!"|tee -a $LOG_FILE 2>&1
done

echo "Finish at:[`date +'%Y-%m-%d %H:%M:%S'`]!"|tee -a $LOG_FILE;

