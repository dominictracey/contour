#!/bin/sh

SERVER='-h 192.168.2.1'
DATABASE='contour'
PORT=''

rm build.log
echo "Building the contour RDBMS backend" >> build.log
echo "The first time you run this you can safely ignore any errors relating to failed DROPs" >> build.log
echo  "" >> build.log

psql $SERVER $PORT $DATABASE -f trackpoints.sql >> build.log
psql $SERVER $PORT $DATABASE -f crew.sql >> build.log
psql $SERVER $PORT $DATABASE -f net.sql >> build.log
psql $SERVER $PORT $DATABASE -f boat.sql >> build.log
psql $SERVER $PORT $DATABASE -f trip.sql >> build.log
psql $SERVER $PORT $DATABASE -f effort.sql >> build.log
psql $SERVER $PORT $DATABASE -f draw.sql >> build.log
psql $SERVER $PORT $DATABASE -f tow.sql >> build.log
psql $SERVER $PORT $DATABASE -f catch.sql >> build.log

