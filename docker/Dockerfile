FROM ubuntu
MAINTAINER ruslan.ovechkin@mixpanel.com 

RUN echo "0.1" > /version

VOLUME /myprojects 

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get -y install wget
RUN apt-get -y install vim
RUN apt-get -y install bzip2
RUN apt-get -y install libsm6
RUN apt-get -y install libglib2.0-0
RUN apt-get -y install libxext6 
RUN apt-get -y install libxrender1 
RUN apt-get -y install tmux 
RUN apt-get -y install bash-completion 
RUN apt-get -y install build-essential 
RUN apt-get -y install git
RUN apt-get -y install python3-pip
RUN apt-get -y install python
RUN apt-get -y install python-pip
RUN apt-get -y install libglib2.0-dev
WORKDIR /myprojects

