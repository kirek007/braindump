# -*- mode: ruby -*-
# vi: set ft=ruby :

###
#
# VM configuration
#
###
$nodes_count = 2
$vm_memory = 2000
$vm_cpus = 2
$instance_name_prefix = "v"
$master_ip = "172.100.100.10"

# Token
$kubernetes_token = "wl6ndy.vyaag8hxsnfikwyb"

Vagrant.configure("2") do |config|
    config.vagrant.plugins = "vagrant-scp"
    config.vm.box = "generic/ubuntu1804"
    config.vm.box_check_update = false

    config.vm.provision "shell", path: "provision.sh"

    (0..$nodes_count).each do |i|
        config.vm.define vm_name = "%s-%02d" % [$instance_name_prefix, i] do |node|
            node.vm.hostname = "%s-%02d" % [$instance_name_prefix, i]

            node.vm.provider :libevrt do |l|
                l.memory = $vm_memory
                l.cpus = $vm_cpus
		l.nested = true
            end

            if i == 0 #1 isMaster
                node.vm.network :private_network, ip: $master_ip, libvirt__guest_ipv6: "no"
                node.vm.provision "shell", path: "init_master.sh", env: {:MASTER_IP => $master_ip, :KUBERNETES_TOKEN => $kubernetes_token}
            else
                ip = "172.100.100.#{i+20}"
                node.vm.network :private_network, ip: ip, libvirt__guest_ipv6: "no"
                    node.vm.provision "shell", path: "init_node.sh", env: {:MASTER_IP => $master_ip, :KUBERNETES_TOKEN => $kubernetes_token}
            end
        end
    end
end
