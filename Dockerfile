# Multi-arch Linux shared library builder for libgsf 03.11.
# Usage (from Windows, macOS, or Linux):
#   docker buildx build \
#     --platform linux/amd64,linux/arm64 \
#     --output type=local,dest=dist \
#     .
#
# Artifacts land under dist/linux_amd64 and dist/linux_arm64 with the
# consumer-compatible names libgsf-x86_64-03.11.so / libgsf-aarch64-03.11.so.

FROM gcc:12-bookworm AS build

WORKDIR /src
COPY Makefile ./
COPY cjson ./cjson
COPY GSF_03-11 ./GSF_03-11
COPY scripts ./scripts

RUN make clean && make all && make smoke

FROM scratch AS export
COPY --from=build /src/dist/*.so /
