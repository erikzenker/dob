#! /bin/bash

KEY=1o3430he5dxxgu8
SECRET=thbjxy0k1e4bau7\&
TIMESTAMP=1360531633
METHOD=PLAINTEXT
VERSION=1.0
REQUEST_URL=https://api.dropbox.com/1/oauth/request_token
ACCESS_URL=https://api.dropbox.com/1/oauth/access_token

# Request secret token
curl --trace-ascii - --header "Authorization: OAuth oauth_version=$VERSION, oauth_signature_method=$METHOD ,oauth_consumer_key=$KEY, oauth_signature=$SECRET" $REQUEST_URL --data "" -o token.txt
TOKEN=$(cat token.txt | sed 's/oauth_token_secret=[0-9,a-z]*//' | sed 's/&oauth_token=//')
TOKEN_SECRET=$(cat token.txt | sed 's/oauth_token_secret=//' | sed 's/&oauth_token=[0-9,a-z]*//')

# Authorize token
#curl "https://www.dropbox.com/1/oauth/authorize?oauth_token=$TOKEN&oauth_callback=http://www.google.de"
chromium https://www.dropbox.com/1/oauth/authorize?oauth_token=$TOKEN&oauth_callback=http://www.google.de

# Wait for authorization
sleep 10s

# Request access token
curl --trace-ascii - --header "Authorization: OAuth oauth_version=$VERSION, oauth_signature_method=$METHOD ,oauth_consumer_key=$KEY ,oauth_token=$TOKEN , oauth_signature=$SECRET$TOKEN_SECRET" $ACCESS_URL --data "" -o access_token.txt
