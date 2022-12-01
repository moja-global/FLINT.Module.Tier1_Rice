FROM ghcr.io/moja-global/flint.core:master

WORKDIR $ROOTDIR/src

ARG GITHUB_AT

RUN git clone -b main https://${GITHUB_AT}@github.com/MullionGroup/FLINT.Module.Tier1_Rice.git \
    && mkdir -p FLINT.Module.Tier1_Rice/source/build \
    && cd FLINT.Module.Tier1_Rice/source/build \
    && cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
            -DCMAKE_INSTALL_PREFIX=$ROOTDIR \
            -DENABLE_TESTS:BOOL=OFF .. \
    && make --quiet -s -j $NUM_CPU \
    && make --quiet install \
    && make clean \
    && cd $ROOTDIR/src