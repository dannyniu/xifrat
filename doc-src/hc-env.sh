#!/bin/sh

: ${HC_SHARE_PATH:=/usr/share/hardcopy:/usr/local/share/hardcopy:..:.}

hc_dir_test()
(
    IFS=:
    for path in $HC_SHARE_PATH ; do
        if [ -d "$path" ] &&
               [ -d "$path/src-include" ] &&
               [ -f "$path/src-include/hardcopy.php" ] ;
        then
            echo "$path"
            break
        fi
    done
)

export HARDCOPY_DIR=$(hc_dir_test)

if ! [ "$HARDCOPY_DIR" ] ; then
    echo The include source folder for hardcopy is not found!
    exit 1
fi

if ! [ -d src-include ] &&
        ! ln -s "$HARDCOPY_DIR/src-include" src-include &&
        ! cp -R "$HARDCOPY_DIR/src-include" src-include ; then
    echo Failed to prepare assets for \"src-include\".
    exit 1
fi

export HARDCOPY_SRCINC="src-include"
export HARDCOPY_SRCINC_MAIN="$HARDCOPY_SRCINC/hardcopy.php"

hcBrowserPreview()
{
    bind="${2:-127.0.0.1}:${1:-8080}"
    echo
    echo Open "\"http://$bind/toc.php\"" in browser.
    echo
    php -S "$bind" -t .
}

hcBuildSinglepage()
{
    HARDCOPY_OUTPUT_CONTROL="" php toc.php > ../doc/xifrat-spec.html
}
