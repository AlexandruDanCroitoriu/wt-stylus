#!/bin/bash
echo "Removing Jenkins Docker image..."
docker rmi jenkins-docker || true

echo "Building Jenkins Docker image..."
docker build -t jenkins-docker -f ./dockerfiles/jenkins .