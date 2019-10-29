# Virtual kubernetes cluster

Require:
- Vagrant (>=2.2.6)
- Virtualbox

##### How does it works?

Run `./bring_up.sh` to bring it up.

#### Customization

In Vagarnt file you can try to change kubernetes versions but be aware that not all packages are properly versioned
so there is filed to improve.

#### Connection

After setup there should be file in root dir `config.kubeconfig` so you can just type
`kubectl --kubeconfig=config.kubeconfig cluster-info` to check if it works


