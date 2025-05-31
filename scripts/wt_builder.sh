#!/bin/bash

echo "Removing wt_builder Docker image..."
docker rmi wt_builder || true

docker build -t wt_builder -f dockerfiles/wt_builder .