# runs hourly to do these things
# turns block.txt back on
# erases contents of pics folder
#
# This prevents us from forgetting and leaving block.txt off by accident
# after using it, allowing pictures to be accessed by bots crawling URLs, etc.
#
# The ONLY way to vew pics is to SSH in and set block.txt to 1 manually
# and that buys you only an hour of viewing before you have to SSH in again.

echo -n "1" > /home/jcr13/public_html/block.txt
rm /home/jcr13/public_html/pics/*.jpg