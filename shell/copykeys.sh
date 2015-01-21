# 1. ssh and run command on remote host

cat ~/.ssh/id_rsa.pub | ssh root@2.1.1.10 'mkdir .ssh && cat >.ssh/authorized_keys'
