#!/bin/bash

CONTAINER_DIR=/media/mybook/backup/
USER_LIST=(erik-01 erik-02)


for((i=0;i<${#USER_LIST[*]};i++)); do 
    echo ${USER_LIST[$i]}
    losetup /dev/loop$i $CONTAINER_DIR${USER_LIST[$i]} -v;
    mount /dev/loop$i /home/${USER_LIST[$i]} -v
    chown ${USER_LIST[$i]} /home/${USER_LIST[$i]} -Rv
    
done



#losetup /dev/loop$i $CONTAINER_DIR${USER_LIST[$i]};
