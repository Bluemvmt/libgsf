# Overview

Source code for the GSF parser (libgsf) for version 3.10.  
This code was provided to us by Navocean.  Download the latest 
[GSF Source Code](https://www.leidos.com/products/ocean-marine).

The best way to update this for new versions will be to fork
this repo and copy the latest from the above site.

# Compiling

Bluemvmt only cares about supporting Ubuntu.  To aid in compiling
for that environment this directory has a [Cloud Config](cloud-config.yaml)
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
