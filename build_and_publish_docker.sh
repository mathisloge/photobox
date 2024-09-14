#docker buildx create --use --platform=linux/arm64/v8 --name crossbuild-builder
#docker buildx inspect --bootstrap
docker buildx build --output outdir --platform=linux/arm64/v8 --target=artifact .
