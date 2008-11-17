#!/bin/bash

echo -n "Checking Code Coverage of unit tests"
DATE=$( date +%Y%m%d%H%M%S )

if [ -d .svn ]; then
        REV=`LANG=C svn info | grep Revision | cut -c 11-`
        echo " for SVN Revision: $REV"
        TITLE="ActivityDiary_SVN"$REV"_"$DATE
else
        echo ":"
        TITLE="ActivityDiary_"$DATE
fi

if ! [ -d coverage/html ]; then
        mkdir -p coverage/html
fi

echo $TITLE
lcov -t "$TITLE" -b ../ -d ../ -q -c -o coverage/$TITLE.info
genhtml --legend -t "$TITLE" -o coverage/html/$TITLE coverage/$TITLE.info &> /dev/null

cd coverage/html/
ln -sf $TITLE LATEST
cd ../../

# XXX: reset counters ( lcov -z seems to be outdated... tries to dlete .da files instead of .gcda ) 
find ../ -name "*.gcda" | xargs rm

