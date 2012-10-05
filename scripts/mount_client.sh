#!/bin/bash

SERVERADRESS=localhost
USERNAME=erik
PORT=2222

sshfs $USERNAME@$SERVERADRESS:/home/$USERNAME /home/$(whoami)/OpenDropboxServer/ -p $PORT
