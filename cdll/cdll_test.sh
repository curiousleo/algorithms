#!/bin/sh

# Use this script and an image viewer with cdll_test to see a "live"
# update of the cdll, e.g.:
#
#        touch out.dot                        # make sure file exists
#        sh cdll_test.sh out.dot &            # start this script in bg
#        ./cdll_test -i out.dot               # start test session
#
# In a different terminal, start an image viewer, e.g.
#
#        feh -R 1 -Z -g 600x600 out.dot.png & # start image viewer in bg

while inotifywait -e modify -q $1; do
  dot -Tpng -O $1;
done
