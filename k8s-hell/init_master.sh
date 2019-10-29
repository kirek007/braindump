#!/usr/bin/env bash

kubeadm init --kubernetes-version="${KUBERNETES_VERSION}" --apiserver-advertise-address="${MASTER_IP}" --apiserver-cert-extra-sans="${MASTER_IP}" --token "${KUBERNETES_TOKEN}" --token-ttl 120m --pod-network-cidr=10.72.0.0/16

export KUBECONFIG=/etc/kubernetes/admin.conf
kubectl apply -f https://docs.projectcalico.org/v3.9/manifests/calico.yaml

systemctl restart kubelet

# Prepare config
export HOME=/home/vagrant
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown "$(id vagrant -u)":"$(id vagrant -g)" $HOME/.kube/config
sudo cp $HOME/.kube/config /vagrant
