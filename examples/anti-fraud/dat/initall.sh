#/bin/sh

dbms_name=
if [ $# -ne 1 ]; then
    echo 'please enter dbms name(db2/oracle):'
    read dbms_name 
else
    dbms_name=$1
fi
echo "dbms_name: $dbms_name"

LOG_FILE=initall.log
if [ $dbms_name == "db2" ]; then
    echo "connecting db2";
    db2 connect to $DBNAME >$LOG_FILE; 
fi

for initfile in `ls init_*.sql`
do
    table=`echo "init table $initfile" | sed "s/\.sql//g"`;
	echo "run $table ..."

    if [ $dbms_name == "db2" ]; then
        db2 -tf $initfile >> $LOG_FILE 2>&1
    elif [ $dbms_name == "oracle" ]; then
        sqlplus $DBUSER/$DBPASS < $initfile >> $LOG_FILE 2>&1
    fi
	if [ $? -ne 0 ];then
    	echo "[`date +'%Y-%m-%d %H:%M:%S'`] $table failed" >> $LOG_FILE 2>&1
    	exit 1;
	fi	
	echo "ok"
	echo "[`date +'%Y-%m-%d %H:%M:%S'`]  $table ok" >> $LOG_FILE 2>&1
done

if [ $dbms_name == "db2" ]; then
    db2 connect reset >> $LOG_FILE; 
fi

echo "Success!"

