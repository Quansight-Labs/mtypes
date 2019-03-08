# Ubuntu Base with Miniconda installation

# Get Latest Stable Ubuntu release
FROM ubuntu:latest
LABEL author="Anirrudh Krishnan"
LABEL maintainer="akrishnan@quansight.com"

# Install Normal Tools
RUN \ 
apt-get -y update && \
apt-get -y upgrade && \
apt-get install -y build-essential && \
apt-get install -y software-properties-common && \
apt-get install -y vim htop curl git wget 


# Setting up environment for Miniconda
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8
ENV PATH /opt/conda/bin:$PATH

RUN apt-get update --fix-missing && \
    apt-get install -y wget bzip2 ca-certificates curl git && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN wget --quiet https://repo.continuum.io/miniconda/Miniconda3-4.5.12-Linux-x86_64.sh -O ~/miniconda.sh && \
    /bin/bash ~/miniconda.sh -b -p /opt/conda && \
    rm ~/miniconda.sh && \
    /opt/conda/bin/conda clean -tipsy && \
    ln -s /opt/conda/etc/profile.d/conda.sh /etc/profile.d/conda.sh && \
    echo ". /opt/conda/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "conda activate base" >> ~/.bashrc

# Allows to debug within the Docker Machine
RUN --cap-add=SYS_PTRACE --security-opt seccomp=unconfined
# Making a new Workspace so we aren't bogged down by Linux
RUN mkdir workspace
RUN cd workspace
CMD [ "/bin/bash" ]