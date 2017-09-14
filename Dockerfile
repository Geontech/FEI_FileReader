FROM geontech/redhawk-runtime:2.0.6

LABEL name="Geon's FEI File Reader Device" \
    license="GPLv3" \
    description="Deploys a node with the FEI_FileReader pre-installed" \
    maintainer="Thomas Goodwin <btgoodwin@geontech.com>"

ENV DOMAINNAME ""
ENV NODENAME   ""
ENV NODEFLAGS  ""

VOLUME /var/rf_snapshots

RUN yum install -y \
        redhawk-sdrroot-dev-mgr \
        redhawk-devel \
        autoconf \
        automake \
        git && \
    source /etc/profile.d/redhawk.sh && \
    source /etc/profile.d/redhawk-sdrroot.sh && \
    git clone git://github.com/GeonTech/FEI_FileReader.git FEI_FileReader && \
    pushd FEI_FileReader && \
    ./build.sh && \
    ./build.sh install && \
    popd && \
    rm -rf FEI_FileReader && \
    \
    yum autoremove -y \
        redhawk-devel \
        autoconf \
        automake \
        git && \
    yum install -y \
        boost-regex \
        boost-iostreams \
        boost-filesystem \
        boost-date-time \
        boost-serialization \
        boost-system \
        boost-thread && \
    yum clean all -y

# Node auto-configure
ADD DockerFiles/fei_filereader-node-init.sh /root/fei_filereader-node-init.sh
RUN chmod u+x /root/fei_filereader-node-init.sh && \
    echo "source /root/fei_filereader-node-init.sh" | tee -a /root/.bashrc

# Supervisord init and exit scripts
ADD DockerFiles/supervisord-fei_filereader.conf /etc/supervisor.d/fei_filereader.conf
ADD DockerFiles/kill_supervisor.py /usr/bin/kill_supervisor.py
RUN chmod u+x /usr/bin/kill_supervisor.py

CMD [ "supervisord" ]
