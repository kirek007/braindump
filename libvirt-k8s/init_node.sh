##!/usr/bin/env bash
#
#KUBE_PORT=${KUBE_PORT:-6443}
#
## Wait for master
#retries=60
#until nc -w 2 "${MASTER_IP}" "${KUBE_PORT}"; do
#    retries=$((retries - 1))
#    echo "Waiting for it ${MASTER_IP}:${KUBE_PORT}... (${retries})"
#    if [[ ${retries} -le 0 ]]; then
#        break
#    fi
#    sleep 1
#done
#
#kubeadm join "${MASTER_IP}:${KUBE_PORT}" --token "${KUBERNETES_TOKEN}" --discovery-token-unsafe-skip-ca-verification
#
