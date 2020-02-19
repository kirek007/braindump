#!/usr/bin/env bash

POD_NETWORK="10.1.0.0/16"

kubeadm init --apiserver-advertise-address="${MASTER_IP}" --apiserver-cert-extra-sans="${MASTER_IP}" --token "${KUBERNETES_TOKEN}" --token-ttl 3600m --pod-network-cidr="${POD_NETWORK}"

export KUBECONFIG=/etc/kubernetes/admin.conf


#systemctl restart kubelet
kubectl taint nodes --all node-role.kubernetes.io/master-

# Prepare config
export HOME=/home/vagrant
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown "$(id vagrant -u)":"$(id vagrant -g)" $HOME/.kube/config
sudo cp $HOME/.kube/config /vagrant

#Wait for kubernetes to be Ready
echo "Waiting for kubernetes to become Ready"
until [ $(kubectl get po --field-selector status.phase!=Running -A 2>/dev/null | wc -l) -gt 0 ]; do
  echo -n "."
  sleep 5
done

kubectl apply -f https://docs.projectcalico.org/manifests/calico.yaml
kubectl set env daemonset/calico-node -n kube-system "IP_AUTODETECTION_METHOD=interface=eth1"
kubectl set env daemonset/calico-node -n kube-system "CALICO_IPV4POOL_CIDR=${POD_NETWORK}"

echo "Waiting for network to become Ready"
until [ $(kubectl get po --field-selector status.phase!=Running -A 2>/dev/null | wc -l) -gt 0 ]; do
  echo -n "."
  sleep 5
done

cat <<EOF | kubectl apply -f -
---
apiVersion: node.k8s.io/v1beta1
kind: RuntimeClass
metadata:
  name: kata-runtime
handler: kata-runtime
---
apiVersion: apps/v1
kind: Deployment
metadata:
  creationTimestamp: null
  labels:
    app: kata-nginx
  name: kata-nginx
spec:
  replicas: 5
  selector:
    matchLabels:
      app: kata-nginx
  template:
    metadata:
      labels:
        app: kata-nginx
    spec:
      runtimeClassName: kata-runtime
      containers:
      - image: nginx
        name: nginx
        resources: {}
EOF