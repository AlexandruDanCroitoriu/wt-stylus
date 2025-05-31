#!/bin/bash

wait_for_apt_lock() {
    while fuser /var/lib/dpkg/lock >/dev/null 2>&1 \
       || fuser /var/lib/dpkg/lock-frontend >/dev/null 2>&1 \
       || fuser /var/lib/apt/lists/lock >/dev/null 2>&1; do
        echo "Waiting for other apt/dpkg/apt-lists processes to finish..."
        sleep 3
    done
}

# Add Docker's official GPG key:
wait_for_apt_lock
apt-get update

wait_for_apt_lock
apt-get install -y ca-certificates curl

install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
. /etc/os-release
if [ -z "$UBUNTU_CODENAME" ] && [ -n "$VERSION_CODENAME" ]; then
    UBUNTU_CODENAME="$VERSION_CODENAME"
fi
echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
    $UBUNTU_CODENAME stable" | \
tee /etc/apt/sources.list.d/docker.list > /dev/null

wait_for_apt_lock
apt-get update

wait_for_apt_lock
apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Ensure Docker is installed and running before proceeding
if ! command -v docker >/dev/null 2>&1; then
    echo "Docker installation failed or docker not found in PATH."
    exit 1
fi

systemctl start docker || service docker start

sh ./scripts/compose_up.sh