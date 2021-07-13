
ARG REGISTRY=""
ARG TAG=""
FROM ${REGISTRY}ignishpc/common-builder${TAG}
ARG RELPATH=""

RUN export DEBIAN_FRONTEND=noninteractive && \
	apt update && \
	apt -y --no-install-recommends install \
		rapidjson-dev \
		libevent-dev \
		zlib1g-dev  \
		libssl-dev \
		libtool  && \
	rm -rf /var/lib/apt/lists/*

ENV BOOST_VERSION=1.75.0
RUN mkdir /tmp/boost && \
	cd /tmp/boost && \
	wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_$(echo ${BOOST_VERSION} | tr . _).tar.gz -O src.tar.gz && \
	tar xvf src.tar.gz && \
	cd boost_$(echo ${BOOST_VERSION} | tr . _) && \
	./bootstrap.sh && \
	./b2 headers && \
	mv boost /usr/include/ && \
	rm -fR /tmp/boost

RUN mv ${IGNIS_DEVEL}/thrift /tmp/thrift && \
	cd /tmp/thrift && \
	./bootstrap.sh && \
	./configure --enable-tests=no --prefix /usr2 && \
	cd lib/cpp && \
	make -j && \
	make install && \
	rm -fR ${IGNIS_DEVEL}/thrift-src && \
	rm -f /usr2/lib/*.a && \
	mkdir ${IGNIS_DEVEL}/thrift && \
	mv /usr2/* ${IGNIS_DEVEL}/thrift

ENV SPDLOG_VERSION=1.8.5
RUN cd /tmp && \
	git clone https://github.com/gabime/spdlog --branch v${SPDLOG_VERSION} --single-branch spdlog && \
	cd spdlog && \
	cmake -DSPDLOG_BUILD_EXAMPLE=FALSE . && \
	make -j && \
	make install && \
	cd .. && \
	rm -fR spdlog

ENV GHC_FILESYSTEM_VERSION=1.5.4
RUN cd /usr/include && \
	mkdir ghc && \
	cd ghc && \
	wget https://github.com/gulrak/filesystem/releases/download/v${GHC_FILESYSTEM_VERSION}/filesystem.hpp

RUN cd ${IGNIS_HOME} && \
	mkdir -p core/cpp/include && \
	cp -R ${IGNIS_DEVEL}/thrift/include/* core/cpp/include && \
	cp -R /usr/include/rapidjson core/cpp/include/ && \
	cp -R ${IGNIS_DEVEL}/mpi/include/* core/cpp/include/ && \
	cp /usr/include/zlib.h core/cpp/include/ && \
	cp /usr/include/zconf.h core/cpp/include/ && \
	mkdir -p core/cpp/lib && \
	cp -P /usr/lib/x86_64-linux-gnu/libgomp.so* core/cpp/lib && \
	cp -R ${IGNIS_DEVEL}/thrift/lib/* core/cpp/lib && \
	cp -R ${IGNIS_DEVEL}/mpi/lib/* core/cpp/lib

COPY ${RELPATH}ignis-cpp-install.sh ${IGNIS_HOME}/bin
RUN chmod +x ${IGNIS_HOME}/bin/ignis-cpp-install.sh

COPY / /tmp/core-cpp
RUN cd /tmp/core-cpp && \
	export CC=${IGNIS_DEVEL}/mpi/bin/mpicc && \
	export CXX=${IGNIS_DEVEL}/mpi/bin/mpic++ && \
	export LIBRARY_PATH=${IGNIS_HOME}/core/cpp/lib && \
	export CPATH=${IGNIS_HOME}/core/cpp/include && \
	cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${IGNIS_HOME}/core/cpp/ . && \
	make -j && \
	make install && \
	rm -fr /tmp/core-cpp && \
	cd ${IGNIS_HOME} && \
	chmod +x core/cpp/bin/ignis-cpp 
