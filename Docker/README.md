# FEI FileReader Compose

This Docker Compose definition runs an OmniORB server (Names and Events), Domain, and an instance of the FEI FileReader.  This is a stand-alone demo configuration.

## Setup

In in order to use this however, the FEI FileReader needs RF Snapshots mounted to `/var/rf_snapshots`.  

For the sake of this demo configuration, the YML file assumes you'll keep the files in the `snapshots` directory (i.e., relative to the YML definition file itself).  To bring up the demo configuration:

    docker-compose up -d

You can then use `docker exec` to join the running Domain or FEI FileReader device where you can then use the Python sandbox to setup allocations.  

If you're using [Docker-REDHAWK's](http://github.com/GeonTech/docker-redhawk) scripts, that's the `login` script.  You can also use the `rhide` script to start an IDE instance to setup allocations and stream data.

