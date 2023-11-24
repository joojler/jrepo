
awk '{$1="";print $0}' | sed "/^$/d"
