## Name ##
 dob - an easy to use backup client for local and remote backups	

## Synopsis ##
 `dob --config=file [-d=debuglevel]`

## Description ##
 __Dob__ scans folders from several profiles for modifications and performs a backup of changed 
 files immediately.  
   
 + __`--config=file`__
    + Sets the configfile, where profiles are listed.
 + __`-d=[1-5]`__
    + 1 : print fatal errors
    + 2 : print errors (and 1)
    + 3 : print warnings (and 1 to 2)
    + 4 : print status information (and 1 to 3) is set as default
    + 5 : print debug information (and 1 to 4)
   
The profiles are defined in a configfile with the following syntax : 

 + __`[Name]`__
     + Start of a new profile with `Name` as profilename. This means you can setup up more than one profile in one configfile
 + __`syncFolder=folderpath`__  
     + Folder that will be scanned for modifications. This Folder should be mounted.  
 + __`destFolder=folderpath`__
     + Location backup will be stored. Can be a local or remote location.
 + __`syncType=syncronize`__
     + Direction of syncronisation
 + __`destType=local|remote`__
     + Set local or remote destination location
 + __`destProtocol=ssh|git`__
     + Set communication protocol in case of remote destination

## Usage ##
 It is possible to set symlinks into the syncFolder. These symlinks will be followd by the  
 scanprocess, thus all modification on this symlinked folder will be recognized.  

 With the help of a ssh-server and some storage on it, it is very easy to setup a cloud storage solution     
 for your linux devices (Netbook, Laptop, PC). You just need to install dob on all your devices, setup profiles  
 with the same destLocation and start dob. From now on, data will be syncronized between these  
 different devices.  

 For all paranoid people in the world it is possible to setup profiles with the same syncFolder, thus   
 your local data can be spreaded around the world to all servers you have acces to. Or you could mirror  
 your data to several local storage connected to your computer.  

## Examples ##
 + Make a local backup to an external harddrive 
   
     `[externalhdd]`  
     `syncType=syncronize`  
     `syncFolder=/home/jondo/important_data/`   
     `destFolder=/media/external_hdd`  
     `destType=local`  
  
 + Syncronize data with a remote host via ssh, also possible with multiple users
  
     `[privatecloud]`  
     `syncType=syncronize`  
     `syncFolder=/home/jondo/important_data/`   
     `destFolder=jondo@myprivatecloud.com:/home/jondo/`  
     `destType=remote`  
     `destProtocol=ssh`   
  
 + Automatically put data into a git repository
  
     `[githost]`  
     `syncType=syncronize`  
     `syncFolder=/home/jondo/important_data/`   
     `destFolder=jondo@githost.com:/gitlocation/`  
     `destType=remote`  
     `destProtocol=git`   

## Dependencies ##
 + rsync
 + boost-libs
 + gtkmm3
 + Linux Kernel 2.6.13 (inotify)

## Copyrigth
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.  
This is free software: you are free to change and redistribute it.  There is NO WARRANTY, to the extent permitted by law.

## Author ##
Written by Erik Zenker (erikzenker@hotmail.com)
