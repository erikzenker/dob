#!/bin/bash

SERVERADRESS=localhost
USERNAME=erik-01

sshfs $USERNAME@localhost:/home/$USERNAME /home/$(whoami)/OpenDropboxServer/
