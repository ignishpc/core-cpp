ARG REGISTRY=""
ARG NAMESPACE="ignishpc/"
ARG TAG=""
FROM ${REGISTRY}${NAMESPACE}builder${TAG}
ARG DOCK_DIR=""
ARG BUILD_CORES="4"

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
	wget https://archives.boost.io/release/${BOOST_VERSION}/source/boost_$(echo ${BOOST_VERSION} | tr . _).tar.gz -O src.tar.gz && \
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
	make -j${BUILD_CORES}  && \
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
	make -j${BUILD_CORES}  && \
	make install && \
	cd .. && \
	rm -fR spdlog

ENV GHC_FILESYSTEM_VERSION=1.5.4
RUN cd /usr/include && \
	mkdir ghc && \
	cd ghc && \
	wget https://github.com/gulrak/filesystem/releases/download/v${GHC_FILESYSTEM_VERSION}/filesystem.hpp

RUN cd ${IGNIS_HOME} && \
	mkdir -p core/cpp/core/include && \
	cp -R ${IGNIS_DEVEL}/thrift/include/* core/cpp/core/include && \
	cp -R /usr/include/rapidjson core/cpp/core/include/ && \
	cp -R ${IGNIS_DEVEL}/mpi/include/* core/cpp/core/include/ && \
	cp /usr/include/zlib.h core/cpp/core/include/ && \
	cp /usr/include/zconf.h core/cpp/core/include/ && \
	mkdir -p core/cpp/core/lib && \
	find /usr/lib/ -name 'libgomp.so*' -exec cp "{}" core/cpp/core/lib/  \; && \
	cp -R ${IGNIS_DEVEL}/thrift/lib/* core/cpp/core/lib && \
	cp -R ${IGNIS_DEVEL}/mpi/lib/* core/cpp/core/lib

COPY ${DOCK_DIR}bin/* ${IGNIS_HOME}/bin/
RUN chmod +x ${IGNIS_HOME}/bin/*

COPY / /tmp/core-cpp
RUN cd /tmp/core-cpp && \
	export CC=${IGNIS_DEVEL}/mpi/bin/mpicc && \
	export CXX=${IGNIS_DEVEL}/mpi/bin/mpic++ && \
	export LIBRARY_PATH=${IGNIS_HOME}/core/cpp/core/lib && \
	export CPATH=${IGNIS_HOME}/core/cpp/core/include && \
	cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${IGNIS_HOME}/core/cpp/core/ . && \
	make -j${BUILD_CORES} && \
	make install && \
	rm -fr /tmp/core-cpp && \
	cd ${IGNIS_HOME} && \
	chmod +x core/cpp/core/bin/ignis-cpp && \
	mv core/cpp/core/bin/ignis-cpp ${IGNIS_HOME}/bin/
