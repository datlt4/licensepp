FROM ubuntu:focal
ENV DEBIAN_FRONTEND noninteractive

# Install necessary library
WORKDIR /root
RUN apt update && apt upgrade -y
RUN apt update && apt install --assume-yes software-properties-common
RUN apt update && apt install --assume-yes git cmake g++ zlib* wget

WORKDIR /root
RUN git clone https://github.com/amrayn/licensepp.git
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

# install boost
WORKDIR /root
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.tar.gz -O /root/boost_1_77_0.tar.gz
RUN apt install --assume-yes build-essential autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev
RUN tar -xvzf boost_1_77_0.tar.gz
WORKDIR /root/boost_1_77_0
RUN ./bootstrap.sh --prefix=/usr/ --with-libraries=python && ./b2 --with=all -j$(nproc) install

WORKDIR /root
RUN rm -rf licensepp cryptopp pem ripe
RUN rm -rf /root/boost_1_77_0
RUN rm -rf /root/boost_1_77_0.tar.gz

WORKDIR /root
RUN echo "export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH" | tee -a ~/.bashrc
RUN apt install libssl1* libssl-dev -y
RUN git clone https://github.com/LuongTanDat/licensepp.git -b licensepp-crow licensepp-crow

WORKDIR /root/licensepp-crow/build
RUN export LD_LIBRARY_PATH=/usr/local/lib && cmake ..
RUN make -j$(nproc)

EXPOSE 6464
WORKDIR /root/licensepp-crow/build
CMD ["./licensepp-generator-server", "6464"]
