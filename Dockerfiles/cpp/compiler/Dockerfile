
ARG REGISTRY=""
ARG NAMESPACE="ignishpc/"
ARG TAG=""
FROM ${REGISTRY}${NAMESPACE}cpp-builder${TAG} as cpp-builder
FROM ${REGISTRY}${NAMESPACE}common${TAG}
ARG RELPATH=""

COPY --from=cpp-builder ${IGNIS_HOME} ${IGNIS_HOME}


RUN export DEBIAN_FRONTEND=noninteractive && \
	apt update && \
	apt install -y \
		gcc \
		g++ \
		make \
		cmake && \
	rm -rf /var/lib/apt/lists/*

RUN cd ${IGNIS_HOME}/core/cpp && \
	for file in */*; do ln -s ${IGNIS_HOME}/core/cpp/$file /usr/local/$file ; done && \
	rm -f ${IGNIS_HOME}/bin/ignis-cpp-install.sh && \
	ldconfig
