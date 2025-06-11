#!/bin/bash

echo "Removing wt_code_server Docker image..."
docker rmi wt_code_server || true

docker build -t wt_code_server -f dockerfiles/wt_code_server .