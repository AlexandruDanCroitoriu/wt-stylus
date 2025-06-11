#!/bin/bash

echo "Removing wt_builder_alpine Docker image..."
docker rmi wt_builder_alpine || true

docker build -t wt_builder_alpine -f dockerfiles/wt_builder_alpine .