#/bin/sh

LOG_FILE="initall.log"

dbms_name=
if [ $# -ne 1 ]; then
    echo 'please enter dbms name(db2/oracle/mysql/sqlite3):'
    read dbms_name 
else
    dbms_name=$1
fi
echo "dbms_name: $dbms_name" > $LOG_FILE;
echo "Start at:[`date +'%Y-%m-%d %H:%M:%S'`]!"|tee -a $LOG_FILE;

if [ $dbms_name == "db2" ]; then
    echo "connecting db2";
    db2 connect to $DBNAME >> $LOG_FILE; 
fi

for initfile in `ls init_*.sql`
do
    table=`echo "$initfile" |sed "s/init_//g"| sed "s/\.sql//g"`;
	echo "  initializing table $table ..."|tee -a $LOG_FILE;

    if [ $dbms_name == "db2" ]; then
        db2 -tf $initfile >> $LOG_FILE 2>&1
    elif [ $dbms_name == "oracle" ]; then
        sqlplus $DBUSER/$DBPASS < $initfile >> $LOG_FILE 2>&1
    elif [ $dbms_name == "mysql" ]; then
        mysql --user=$DBUSER --password=$DBPASS --database=$DBNAME < $initfile >> $LOG_FILE 2>&1
    elif [ $dbms_name == "sqlite3" ]; then
        sqlite3 $SQLITE3_HOME/krdb.db < $initfile >> $LOG_FILE 2>&1
    fi
	if [ $? -ne 0 ];then
    	echo "  failed!"|tee -a $LOG_FILE 2>&1
    	exit 1;
	fi	
	echo "  ok!"|tee -a $LOG_FILE 2>&1
done

if [ $dbms_name == "db2" ]; then
    db2 connect reset >> $LOG_FILE; 
fi

echo "Finish at:[`date +'%Y-%m-%d %H:%M:%S'`]!"|tee -a $LOG_FILE;

