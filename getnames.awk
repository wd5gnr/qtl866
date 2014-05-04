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

   
