FROM ghcr.io/mathisloge/photobox/build-context:main as build

COPY . .

RUN cmake --workflow --preset release 

FROM scratch as artifact
COPY --from=build /build/photobox.rpm /photobox.rpm
