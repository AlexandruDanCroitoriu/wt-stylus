#!/bin/bash

# Wait for apt/dpkg locks to be released
while fuser /var/lib/dpkg/lock >/dev/null 2>&1 || fuser /var/lib/dpkg/lock-frontend >/dev/null 2>&1; do
    echo "Waiting for other apt/dpkg processes to finish..."
    sleep 3
done

# Add Docker's official GPG key:
apt-get update
apt-get install -y ca-certificates curl
install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
    $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
tee /etc/apt/sources.list.d/docker.list > /dev/null
apt-get update

apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Ensure Docker is installed and running before proceeding
if ! command -v docker >/dev/null 2>&1; then
    echo "Docker installation failed or docker not found in PATH."
    exit 1
fi

systemctl start docker || service docker start

sh ./scripts/wt_builder.sh
sh ./scripts/wt_code_server.sh
sh ./scripts/jenkins.sh
sh ./scripts/compose_up.sh