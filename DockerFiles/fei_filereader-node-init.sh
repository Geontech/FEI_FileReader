#!/bin/bash
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of Docker REDHAWK.
#
# Docker REDHAWK is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# Docker REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
set -e

FEI_CONFIG="--filepath /var/rf_snapshots"
FEI_CONFIG="${FEI_CONFIG} --domainname=${DOMAINNAME}"
FEI_CONFIG="${FEI_CONFIG} --nodename=${NODENAME}"

if ! [ -d $SDRROOT/dev/nodes/${NODENAME} ]; then
    echo Configuring FEI_FileReader Node

    ${SDRROOT}/dev/devices/FEI_FileReader/nodeconfig.py ${FEI_CONFIG}
else
    echo FEI_FileReader Node already configured
fi

