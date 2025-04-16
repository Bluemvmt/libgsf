# Overview

Source code for the GSF parser (libgsf) for versions 3.09 and 3.10.  
Download the latest [GSF Source Code](https://www.leidos.com/products/ocean-marine).

The original source from above supports Windows and Linux.  It is 
important the purpose of this repository is to support deploying libgsf
in Kubernetes/Docker envionments and only supports compiling in Linux
environments.  That is, in fact, the main purpose for this repo.  If you
need a Windows binary please follow the link above.


# Compiling on Mac/Windows

Bluemvmt only cares about supporting Ubuntu.  To aid in compiling
for for Ubuntu from Windows and mac this directory has a [Cloud Config](cloud-config.yaml)
file to support provisioning a VM in which you can compile the
source on any host platform.  To start up a vm using [Multipass](https://multipass.run/)
you can use the following command from this directory:

```commandShell
multipass launch \
 --name libgsf-compile \
 --memory 2G \
 --cpus 2 \
 --disk 10G \
 --cloud-init cloud-config.yaml
```

Once the VM has started you can mount this directory into the VM with 
the following:
```commandShell
multipass mount $(pwd) libgsf-compile:/home/ubuntu/src -g 20:1000 -u 501:1000
```

The right side of the colon for the -g and -u options are the user/group
IDs of the VM.  The ids on the left are the host user/group IDs.  The latter
will likely vary between host machines so run **id** (this is unix like 
systems and will be different for Windows) to get the IDs for your particular
host.

# Using Docker

The other option is to do your development in a Docker container.  To do so:

```
docker run \
  -it \
  --rm \
  --name libgsf-dev \
  -v $(pwd):/home/ubuntu/src \
  vshefferbluemvmt/libgsf-dev \
  /bin/bash
```
