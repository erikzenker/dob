#! /bin/bash

KEY=1o3430he5dxxgu8
SECRET=thbjxy0k1e4bau7\&
TIMESTAMP=1360531633
METHOD=PLAINTEXT
VERSION=1.0

# Set put parameters
PUT_URL=https://api-content.dropbox.com/1/files_put/
PUT_DEST=dropbox/
UPLOAD_FILE=/home/erik/test
FILE_LENGTH=4000

# Parse token and access token
TOKEN=$(cat token.txt | sed 's/oauth_token_secret=[0-9,a-z]*//' | sed 's/&oauth_token=//')
TOKEN_SECRET=$(cat token.txt | sed 's/oauth_token_secret=//' | sed 's/&oauth_token=[0-9,a-z]*//')

ACCESS_TOKEN=$(cat access_token.txt | sed 's/oauth_token_secret=[0-9,a-z]*//' | sed 's/&oauth_token=//' | sed 's/&uid=[0-9]*//')
ACCESS_TOKEN_SECRET=$(cat access_token.txt | sed 's/oauth_token_secret=//' | sed 's/&oauth_token=[0-9,a-z]*//' | sed 's/&uid=[0-9]*//')

# Put upload_file
curl --upload-file $UPLOAD_FILE --trace-ascii - --header "Authorization: OAuth oauth_version=$VERSION, oauth_signature_method=$METHOD ,oauth_consumer_key=$KEY ,oauth_token=$ACCESS_TOKEN , oauth_signature=$SECRET$ACCESS_TOKEN_SECRET, Content-Length=$FILE_LENGTH" $PUT_URL$PUT_DEST

