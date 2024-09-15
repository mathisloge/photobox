FROM ghcr.io/mathisloge/photobox/build-context:main as build


RUN dnf install -y cmake ninja-build gcc mesa-libGL-devel qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtmultimedia-devel libgphoto2-devel fedora-packager rpmdevtools && \
    dnf clean all && \
    rm -rf /var/cache/yum

COPY . .

RUN cmake --workflow --preset release 

FROM scratch as artifact
COPY --from=build /build/photobox.rpm /photobox.rpm
