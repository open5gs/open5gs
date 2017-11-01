if [ 1 -ne $# ]
then
    echo "You must specify argument : ./systemctl.sh  {start|stop|force-stop|restart|force-reload|status}"

    exit;
fi

systemctl $1 nextepc-mmed
systemctl $1 nextepc-sgwd
systemctl $1 nextepc-pgwd
systemctl $1 nextepc-hssd
systemctl $1 nextepc-pcrfd
