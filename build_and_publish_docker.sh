docker buildx create --use --platform=linux/arm64/v8 --name crossbuild-builder
docker buildx inspect --bootstrap
docker buildx build --platform=linux/arm64/v8 --push --tag ghcr.io/mathisloge/photobox/build:latest .
