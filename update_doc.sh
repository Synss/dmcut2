#!/bin/sh -e
rsync --verbose --progress --stats --compress --delete -d \
Documentation/html/* mathias_laurin,dmcut2@web.sourceforge.net:htdocs
