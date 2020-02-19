# Virtual kubernetes cluster

#### This is a testing infrastructure for [Kata Containers](https://katacontainers.io/) with [cri-o runtime](https://cri-o.io/) on top of [libvirt](https://libvirt.org/) and [vagrant](https://www.vagrantup.com) 

Require:
- Vagrant (>=2.2.6) [download](https://www.vagrantup.com/downloads.html)
- libvirt with nested virtualization

Installation:
- Plugin requirements [are here](https://github.com/vagrant-libvirt/vagrant-libvirt#installation)
- `vagrant plugin install vagrant-libvirt`
 
##### How does it works?

Run `vagrant up --provider=libvirt` to bring it up.

#### Connection
```bash
vagrant ssh v-00 #to get to the master node with kubectl
```