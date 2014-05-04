# Simple awk script to pull the device names out
# of the original devices.h
# I like to sort them
# so I just sort them afterwards and manually grab the
# old first and last line and put them back

BEGIN {
    print "const char *devnames[] = {";
}


/\.name/  {
    sub(/^[^"]*/,"");
    sub(/,[^"]*$/,"");
    print $0 ",";
    
    }

END {
    print "};"
}

   
