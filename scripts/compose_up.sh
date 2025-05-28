#!/bin/bash

# if [ "$#" -ne 1 ]; then
#     echo "Usage: $0 <POSTGRES_PASSWORD>"  
#     exit 1
# fi

# POSTGRES_PASSWORD=$1

export DOCKER_SOCK_GROUP_ID=$(stat -c '%g' /var/run/docker.sock)
# export POSTGRES_PASSWORD=$POSTGRES_PASSWORD

docker compose down
docker compose up -d