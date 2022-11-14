mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ..
get_char()
{
    SAVEDSTTY=`stty -g`
    stty -echo
    stty cbreak
    dd if=/dev/tty bs=1 count=1 2> /dev/null
    stty -raw
    stty echo
    stty $SAVEDSTTY
}
 
echo -e "\nPress any key to finish ..."
char=`get_char`