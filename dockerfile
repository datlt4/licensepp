FROM ubuntu:focal
ENV DEBIAN_FRONTEND noninteractive

# Install necessary library
WORKDIR /root
RUN apt update && apt upgrade -y
RUN apt update && apt install --assume-yes software-properties-common
RUN apt install --assume-yes git tmux tree cmake

WORKDIR /root
RUN git clone https://github.com/amrayn/licensepp
RUN git clone https://github.com/weidai11/cryptopp.git
RUN git clone https://github.com/amraynweb/cryptopp-pem.git
RUN git clone https://github.com/amrayn/ripe.git
RUN cp cryptopp-pem/* cryptopp/

WORKDIR /root/cryptopp
RUN make -j$(nproc)
RUN make install

WORKDIR /root/licensepp
RUN mkdir -p build
WORKDIR /root/licensepp/build
RUN cmake ..
RUN make -j$(nproc)
RUN make install

WORKDIR /root/ripe
RUN mkdir -p build
WORKDIR /root/ripe/build
RUN cmake ..
RUN make -j$(nproc)
RUN make install

WORKDIR /root
RUN echo "export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH" | tee -a ~/.bashrc && source ~/.bashrc
RUN apt install libssl1* libssl-dev -y
RUN git clone https://github.com/LuongTanDat/licensepp.git -b licensepp-crow

WORKDIR /root/licensepp/build
RUN cmake ..
RUN make -j$(nproc)
